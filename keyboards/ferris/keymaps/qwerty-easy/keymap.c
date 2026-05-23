#include QMK_KEYBOARD_H

enum layers {
    BASE,
    SYM,
    FN,
};

// ────────────────────────────────────────────────────────────────
// OS mode: stored in EEPROM, toggled by holding Command on boot
// ────────────────────────────────────────────────────────────────
typedef union {
    uint8_t raw;
    struct {
        bool mac_mode : 1;
    };
} user_config_t;

user_config_t user_config;

// Custom keycodes whose mod identity depends on OS mode
enum custom_keycodes {
    CMD_BSPC = SAFE_RANGE,  // Command thumb: Ctrl (Win/Linux) or Cmd (Mac)
    OSM_X,                  // FN-layer sticky: Gui-Win (Win) / Alt-Opt (Mac)
    OSM_C,                  // FN-layer sticky: Alt (Win) / Gui-Cmd (Mac)
    MAC_TOG,                // FN+M: toggle Mac/Win mode at runtime
    WIN_PREV,               // SYM+M: prev window (Cmd+Shift+Tab / Alt+Shift+Tab)
    WIN_NEXT,               // SYM+,: next window (Cmd+Tab / Alt+Tab)
    DSK_PREV,               // SYM+N: prev desktop (Ctrl+Left / Ctrl+Win+Left)
    DSK_NEXT,               // SYM+.: next desktop (Ctrl+Right / Ctrl+Win+Right)
    SCRN_LCK,               // FN+N: lock screen (Ctrl+Cmd+Q / Win+L)
};

// ────────────────────────────────────────────────────────────────
// Boot-time mode toggle: hold Command (left thumb outer) at power-on
// ────────────────────────────────────────────────────────────────
void keyboard_post_init_user(void) {
    eeconfig_read_user_datablock(&user_config, 0, sizeof(user_config));

    // Scan matrix to check whether Command is pressed at boot.
    // Adjust (row, col) if your Sweep variant places the left thumb
    // outer key elsewhere — check info.json for your board.
    matrix_scan();
    wait_ms(30);
    matrix_scan();

    if (matrix_is_on(4, 0)) {
        user_config.mac_mode = !user_config.mac_mode;
        eeconfig_update_user_datablock(&user_config, 0, sizeof(user_config));
    }
}

void eeconfig_init_user_datablock(void) {
    user_config.raw = 0;
    user_config.mac_mode = false;
    eeconfig_update_user_datablock(&user_config, 0, sizeof(user_config));
}

// ────────────────────────────────────────────────────────────────
// Manual mod-tap for Command, plus OS-aware sticky mods on FN layer.
//
// Why manual: standard LCTL_T can't change its mod identity at runtime.
// We track Command state and register the right mod when another key
// is pressed during the hold.
// ────────────────────────────────────────────────────────────────
static bool     cmd_pressed      = false;
static uint16_t cmd_timer        = 0;
static bool     cmd_mod_active   = false;
static uint16_t cmd_last_tap     = 0;
static bool     cmd_had_tap      = false;
static bool     cmd_quick_hold   = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uint16_t cmd_mod = user_config.mac_mode ? KC_LGUI : KC_LCTL;

    // If any non-Command key is pressed while Command is held,
    // promote Command to its modifier role. Skipped while in
    // quick-tap-hold (we're holding KC_BSPC for OS auto-repeat).
    if (cmd_pressed && !cmd_mod_active && !cmd_quick_hold && keycode != CMD_BSPC && record->event.pressed) {
        register_code(cmd_mod);
        cmd_mod_active = true;
    }

    switch (keycode) {
        case CMD_BSPC:
            if (record->event.pressed) {
                // Quick-tap-then-hold: if this press follows a pure tap
                // within QUICK_TAP_TERM, register KC_BSPC and let the OS
                // auto-repeat drive the repeated backspaces until release.
                if (cmd_had_tap && timer_elapsed(cmd_last_tap) < QUICK_TAP_TERM) {
                    register_code(KC_BSPC);
                    cmd_quick_hold = true;
                }
                cmd_pressed = true;
                cmd_timer = timer_read();
                cmd_mod_active = false;
            } else {
                cmd_pressed = false;
                if (cmd_quick_hold) {
                    unregister_code(KC_BSPC);
                    cmd_quick_hold = false;
                    cmd_had_tap = false;
                } else if (cmd_mod_active) {
                    unregister_code(cmd_mod);
                    cmd_mod_active = false;
                    cmd_had_tap = false;
                } else {
                    // No other key was pressed during the hold → backspace tap.
                    // Record so a quick re-press within QUICK_TAP_TERM
                    // becomes a held repeat.
                    tap_code(KC_BSPC);
                    cmd_last_tap = timer_read();
                    cmd_had_tap = true;
                }
            }
            return false;

        case OSM_X:
            if (record->event.pressed) {
                add_oneshot_mods(user_config.mac_mode ? MOD_BIT(KC_LALT) : MOD_BIT(KC_LGUI));
            }
            return false;

        case OSM_C:
            if (record->event.pressed) {
                add_oneshot_mods(user_config.mac_mode ? MOD_BIT(KC_LGUI) : MOD_BIT(KC_LALT));
            }
            return false;

        case MAC_TOG:
            if (record->event.pressed) {
                user_config.mac_mode = !user_config.mac_mode;
                eeconfig_update_user_datablock(&user_config, 0, sizeof(user_config));
            }
            return false;

        case WIN_NEXT:
        case WIN_PREV:
            if (record->event.pressed) {
                uint16_t base = user_config.mac_mode ? KC_LGUI : KC_LALT;
                register_code(base);
                if (keycode == WIN_PREV) register_code(KC_LSFT);
                tap_code(KC_TAB);
                if (keycode == WIN_PREV) unregister_code(KC_LSFT);
                unregister_code(base);
            }
            return false;

        case DSK_NEXT:
        case DSK_PREV:
            if (record->event.pressed) {
                register_code(KC_LCTL);
                if (!user_config.mac_mode) register_code(KC_LGUI);
                tap_code(keycode == DSK_NEXT ? KC_RGHT : KC_LEFT);
                if (!user_config.mac_mode) unregister_code(KC_LGUI);
                unregister_code(KC_LCTL);
            }
            return false;

        case SCRN_LCK:
            if (record->event.pressed) {
                if (user_config.mac_mode) {
                    register_code(KC_LCTL);
                    register_code(KC_LGUI);
                    tap_code(KC_Q);
                    unregister_code(KC_LGUI);
                    unregister_code(KC_LCTL);
                } else {
                    register_code(KC_LGUI);
                    tap_code(KC_L);
                    unregister_code(KC_LGUI);
                }
            }
            return false;
    }
    return true;
}

// ────────────────────────────────────────────────────────────────
// Layout
//
// Thumbs (L→R):
//   Command (outer L) | Triangle (inner L) | Return (inner R) | Escape (outer R)
//
//   Command  tap=BSPC,  hold=Ctrl/Cmd (OS-aware), hold-alone=repeat BSPC
//   Triangle tap=SPC,   hold-with-other=Shift
//   Return   tap=ENT,   hold=SYM layer
//   Escape   tap=ESC,   hold=FN  layer
// ────────────────────────────────────────────────────────────────
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    // BASE — pure QWERTY
    [BASE] = LAYOUT_split_3x5_2(
        KC_Q, KC_W, KC_E, KC_R, KC_T,                     KC_Y, KC_U, KC_I,    KC_O,   KC_P,
        KC_A, KC_S, KC_D, KC_F, KC_G,                     KC_H, KC_J, KC_K,    KC_L,   KC_SCLN,
        KC_Z, KC_X, KC_C, KC_V, KC_B,                     KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH,

              CMD_BSPC, LSFT_T(KC_SPC),         LT(SYM, KC_ENT), LT(FN, KC_ESC)
    ),

    // SYM — held via Return
    //   Top row Q→P:  1 2 3 4 5 / 6 7 8 9 0
    //   Home  A→G:    - = [ ] backslash
    //   Home  H→L:    ← ↓ ↑ →  (Vim arrows)
    //   Home  ;:      '
    //   Bottom Z X C V B: media — prev, play/pause, next, vol-, vol+
    //   Bottom N M , .: window/desktop nav (outer = desktops, inner = windows)
    //     N: prev desktop  M: prev window  ,: next window  .: next desktop
    //   Bottom /: `
    //   Hold Triangle simultaneously for shifted variants
    [SYM] = LAYOUT_split_3x5_2(
        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,         KC_6,     KC_7,     KC_8,     KC_9,     KC_0,
        KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS,      KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  KC_QUOT,
        KC_MPRV, KC_MPLY, KC_MNXT, KC_VOLD, KC_VOLU,      DSK_PREV, WIN_PREV, WIN_NEXT, DSK_NEXT, KC_GRAVE,

                       _______, _______,                _______, _______
    ),

    // FN — held via Escape
    //   Top row Q→P:   F1–F10
    //   ;: F11, /: F12
    //   A: Tab
    //   S: Sticky Shift
    //   D: Delete
    //   G: Insert
    //   Z: Sticky Ctrl                  (both modes)
    //   X: Sticky Gui  (Win)  / Alt-Opt (Mac)
    //   C: Sticky Alt  (Win)  / Gui-Cmd (Mac)
    //   Z/X/C positions mirror a real keyboard's bottom-row mod order
    //   for the active OS (Win: Ctrl-Win-Alt; Mac: Ctrl-Opt-Cmd).
    //   H J K L: Home, PgDn, PgUp, End  (arrow-key spatial mapping)
    //   N: lock screen (Ctrl+Cmd+Q on Mac, Win+L on Windows)
    //   M: toggle Mac/Win mode (persists to EEPROM)
    [FN] = LAYOUT_split_3x5_2(
        KC_F1,        KC_F2,         KC_F3,   KC_F4,   KC_F5,       KC_F6,    KC_F7,   KC_F8,   KC_F9,   KC_F10,
        KC_TAB,       OSM(MOD_LSFT), KC_DEL,  _______, KC_INS,      KC_HOME,  KC_PGDN, KC_PGUP, KC_END,  KC_F11,
        OSM(MOD_LCTL),OSM_X,         OSM_C,   _______, _______,     SCRN_LCK, MAC_TOG, _______, _______, KC_F12,

                       _______, _______,                _______, _______
    ),
};
