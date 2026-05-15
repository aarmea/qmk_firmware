#pragma once

// Hold-tap timing
#define TAPPING_TERM 200

// Hold-side fires as soon as another key is pressed during the hold.
// Important for thumb hold-taps to feel responsive.
#define HOLD_ON_OTHER_KEY_PRESS

// Allows tap-then-hold to repeat the tap.
// This is what makes Command-held-alone produce repeated backspaces:
//   1st press → tap (TAPPING_TERM expires) → BSPC tapped on release
//   Hold again within QUICK_TAP_TERM → tap behavior repeats while held
// In practice, hold Command for ~200ms, release briefly, hold again → repeat.
// For continuous repeat-on-single-hold, see note in process_record_user.
#define QUICK_TAP_TERM 200

// Reserve one byte of EEPROM for the Mac/Win flag
#define EECONFIG_USER_DATA_SIZE 1
