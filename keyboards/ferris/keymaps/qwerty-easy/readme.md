qwerty-easy
===========

A 34-key layout that maximizes *existing* muscle memory by keeping keys in familiar locations.


Motivation
----------

Most low-key-count layouts (including the bundled `default`) lean heavily on tap-hold and home-row modifiers to fit a full keyboard onto 34 keys. That power comes at a cost: every letter you type is also a potential modifier, and a moment of unintentional lingering on a home-row key produces typos that are easy to make and hard to predict.

`qwerty-easy` takes the opposite approach. The base layer is pure QWERTY with no hold behavior on any alpha key. Everything you already know about a normal keyboard still applies — only the *missing* keys (symbols, function keys, navigation, modifiers other than Shift) move to layers reached via the thumbs. Typing speed and accuracy on prose are unaffected by the small-keyboard format.

The trade-off is that this layout has fewer layers and slightly less density than `default`. That suits people who:

- Are coming from a normal keyboard and don't want to retrain their alphas.
- Type prose / chat / code comments more often than they type dense symbol-heavy code.
- Want a "stable floor" while they explore the keyboard, without committing up front to a hold-heavy scheme.

It also includes a few opinionated extras that aren't present in `default`:

- **Runtime OS mode.** A persistent Mac/Windows flag stored in EEPROM changes how the modifier keys behave, so the same firmware works correctly on either OS.
- **Media controls** on the SYM layer.
- **Window and desktop switching** on the SYM layer, with the underlying shortcut chosen by OS mode.


How it differs from `default`
-----------------------------

| | `default` | `qwerty-easy` |
|---|---|---|
| Layers | 8 (Mouse, Nav, RSym, LSym, F-keys, Numbers, Always-accessible, Base) | 3 (Base, SYM, FN) |
| Home-row mods on alphas | Yes (Shift on pinkies, Ctrl/Alt on bottom-row) | No |
| Layer access | Spread across home-row alphas | Two thumb keys only |
| Number row | On the Numbers layer (right ring) | On the SYM layer (top row) |
| OS awareness | None | Mac/Win toggle persisted to EEPROM |
| Media / window / desktop | Not built-in | On SYM layer |
| Mouse layer | Yes | No |
| Learning curve | Steep — accepts re-training to gain density | Shallow — built for "drop in and type" |


Layout
------

### Base layer

```
┌────┬────┬────┬────┬────┐                ┌────┬────┬────┬────┬────┐
│ Q  │ W  │ E  │ R  │ T  │                │ Y  │ U  │ I  │ O  │ P  │
├────┼────┼────┼────┼────┤                ├────┼────┼────┼────┼────┤
│ A  │ S  │ D  │ F  │ G  │                │ H  │ J  │ K  │ L  │ ;  │
├────┼────┼────┼────┼────┤                ├────┼────┼────┼────┼────┤
│ Z  │ X  │ C  │ V  │ B  │                │ N  │ M  │ ,  │ .  │ /  │
└────┴────┴────┴────┴────┘                └────┴────┴────┴────┴────┘
             ┌────┬────┐                  ┌────┬────┐
             │CMD │SPC │                  │ENT │ESC │
             └────┴────┘                  └────┴────┘
```

Pure QWERTY. No hold-tap on any alpha key.


### SYM layer — hold Return

```
┌────┬────┬────┬────┬────┐                ┌────┬────┬────┬────┬────┐
│ 1  │ 2  │ 3  │ 4  │ 5  │                │ 6  │ 7  │ 8  │ 9  │ 0  │
├────┼────┼────┼────┼────┤                ├────┼────┼────┼────┼────┤
│ -  │ =  │ [  │ ]  │ \  │                │ ←  │ ↓  │ ↑  │ →  │ '  │
├────┼────┼────┼────┼────┤                ├────┼────┼────┼────┼────┤
│⏮  │⏯ │⏭  │vol-│vol+│                │dsk-│win-│win+│dsk+│ `  │
└────┴────┴────┴────┴────┘                └────┴────┴────┴────┴────┘
```

- **Top row:** number row, exactly where it sits on a normal keyboard.
- **Home row left (A → G):** common programming symbols `- = [ ] \\`.
- **Home row right (H → L):** Vim-style arrow keys, with `'` on `;`.
- **Bottom row left (Z → B):** media transport — prev, play/pause, next, volume down, volume up.
- **Bottom row right (N → .):** window and desktop switching, OS-aware. Outer pair (`N`/`.`) cycles desktops, inner pair (`M`/`,`) cycles windows. Left key is "previous", right key is "next".
- **Bottom-right (`/` position):** backtick.

Hold the Space thumb (Triangle) at the same time for shifted variants of any of these.


### FN layer — hold Escape

```
┌────┬────┬────┬────┬────┐                ┌────┬────┬────┬────┬────┐
│F1  │F2  │F3  │F4  │F5  │                │F6  │F7  │F8  │F9  │F10 │
├────┼────┼────┼────┼────┤                ├────┼────┼────┼────┼────┤
│Tab │⇧ os│Del │    │Ins │                │Home│PgDn│PgUp│End │F11 │
├────┼────┼────┼────┼────┤                ├────┼────┼────┼────┼────┤
│⌃ os│ X* │ C* │    │    │                │    │M⇄  │    │    │F12 │
└────┴────┴────┴────┴────┘                └────┴────┴────┴────┴────┘
```

- **Top row:** F1–F10. F11 sits on `;`, F12 on `/`.
- **`A`:** Tab.
- **`S`:** Sticky (one-shot) Shift.
- **`D`:** Delete.
- **`G`:** Insert.
- **`Z` / `X` / `C`:** sticky modifiers, arranged left-to-right to match the bottom-row mod order of a real keyboard for the active OS:
  - **Windows mode:** Ctrl · Win · Alt
  - **Mac mode:** Ctrl · Option · Cmd
- **`H` `J` `K` `L`:** Home, PgDn, PgUp, End (a spatial mirror of the SYM-layer arrow keys).
- **`M`:** toggle Mac/Win mode and persist to EEPROM.


Thumb cluster
-------------

Left to right:

| Thumb | Tap | Hold |
|---|---|---|
| Outer L (Command) | Backspace | Ctrl (Win) / Cmd (Mac) |
| Inner L (Triangle) | Space | Shift |
| Inner R (Return) | Enter | SYM layer |
| Outer R (Escape) | Escape | FN layer |

A few non-obvious behaviors of the Command thumb:

- **Tap-then-hold within 200ms** registers `Backspace` and lets the OS auto-repeat fire — so a quick double-press-and-hold deletes continuously.
- **Hold + another key** registers the OS modifier (Ctrl on Windows, Cmd on Mac), so e.g. `Command + S` is Save on both systems.
- **Cmd + Space (Spotlight on Mac)** is the two-thumb gesture: hold Command, tap Space.


OS mode
-------

The keymap stores a single bit (`mac_mode`) in EEPROM. Two ways to toggle it:

1. **At runtime:** `FN + M`. Effect is immediate and persists.
2. **At boot:** hold the Command (left outer) thumb while plugging the keyboard in. The bit is flipped during `keyboard_post_init_user`. Useful as a recovery path if you've reassigned `FN + M`.

The flag affects:

- `Command` thumb modifier identity (Ctrl vs. Cmd).
- FN-layer sticky modifiers (`X` and `C` swap to match each OS's physical bottom-row order).
- SYM-layer window/desktop shortcuts (Alt+Tab vs. Cmd+Tab; Ctrl+Win+Arrow vs. Ctrl+Arrow).


Generative AI disclosure
------------------------

This keyboard layout was written with the assistance of generative AI-based coding agents, particularly Claude Code.
