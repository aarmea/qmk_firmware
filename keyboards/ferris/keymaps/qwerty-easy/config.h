#pragma once

// Hold-tap timing
#define TAPPING_TERM 200

// Hold-side fires as soon as another key is pressed during the hold.
// Important for thumb hold-taps to feel responsive.
#define HOLD_ON_OTHER_KEY_PRESS

// Window after a Command tap during which a re-press becomes a held
// KC_BSPC (so OS auto-repeat produces repeated backspaces). Honored
// manually by CMD_BSPC in keymap.c — QMK's built-in QUICK_TAP_TERM
// only applies to LT()/MT(), not to custom keycodes.
#define QUICK_TAP_TERM 200

// Reserve one byte of EEPROM for the Mac/Win flag
#define EECONFIG_USER_DATA_SIZE 1
