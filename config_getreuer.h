// Copyright 2021-2025 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

// Unfortunately, some applications drop or misorder fast key events. This is a
// partial fix to slow down the rate at which macros are sent.
#define TAP_CODE_DELAY 5

// Tap-hold configuration for home row mods.
#define TAPPING_TERM 240
#define TAPPING_TERM_PER_KEY
#define FLOW_TAP_TERM 100
#define CHORDAL_HOLD
#define PERMISSIVE_HOLD
#define QUICK_TAP_TERM_PER_KEY

// Uncomment this line for verbose QMK core tap-hold logging.
// #define ACTION_DEBUG

#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION

#define UNICODE_SELECTED_MODES UNICODE_MODE_LINUX

// Use Left Ctrl + Right Ctrl to activate the Command feature.
#define IS_COMMAND() (get_mods() == MOD_MASK_CTRL)

// Holding Shift while Caps Word is active inverts the shift state.
#define CAPS_WORD_INVERT_ON_SHIFT
// When idle, turn off Caps Word after 5 seconds.
#define CAPS_WORD_IDLE_TIMEOUT 5000

// Don't apply custom shift keys with mods other than Shift.
#define CUSTOM_SHIFT_KEYS_NEGMODS ~MOD_MASK_SHIFT

// Only apply custom shift keys on layers 0 and 4.
#define CUSTOM_SHIFT_KEYS_LAYER_MASK (1 << 0) | (1 << 4)

// When idle, turn off Layer Lock after 60 seconds.
#define LAYER_LOCK_IDLE_TIMEOUT 60000

// When idle, turn off Sentence Case after 2 seconds.
#define SENTENCE_CASE_TIMEOUT 2000

// Define speed curve for Orbital Mouse.
#define ORBITAL_MOUSE_SPEED_CURVE \
      {24, 24, 24, 32, 62, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72}
//     |               |               |               |           |
// t = 0.000           1.024           2.048           3.072       3.840 s

#define LUMINO_TRANSITION 850
#define LUMINO_BOOT_COLOR RGB_WHITE
#define LUMINO_NO_EEPROM

#define PALETTEFX_ENABLE_ALL_EFFECTS
#define PALETTEFX_ENABLE_ALL_PALETTES

#ifdef RGB_MATRIX_ENABLE
// Since we have PaletteFx, disable most built-in RGB Matrix effects.
#  undef ENABLE_RGB_MATRIX_ALPHAS_MODS
#  undef ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN
#  undef ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
#  undef ENABLE_RGB_MATRIX_BREATHING
#  undef ENABLE_RGB_MATRIX_BAND_SAT
#  undef ENABLE_RGB_MATRIX_BAND_VAL
#  undef ENABLE_RGB_MATRIX_BAND_PINWHEEL_SAT
#  undef ENABLE_RGB_MATRIX_BAND_PINWHEEL_VAL
#  undef ENABLE_RGB_MATRIX_BAND_SPIRAL_SAT
#  undef ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL
#  undef ENABLE_RGB_MATRIX_CYCLE_ALL
#  undef ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
#  undef ENABLE_RGB_MATRIX_CYCLE_UP_DOWN
#  undef ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
#  undef ENABLE_RGB_MATRIX_CYCLE_OUT_IN
#  undef ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
#  undef ENABLE_RGB_MATRIX_CYCLE_PINWHEEL
#  undef ENABLE_RGB_MATRIX_CYCLE_SPIRAL
#  undef ENABLE_RGB_MATRIX_DUAL_BEACON
#  undef ENABLE_RGB_MATRIX_RAINBOW_BEACON
#  undef ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS
#  undef ENABLE_RGB_MATRIX_FLOWER_BLOOMING
#  undef ENABLE_RGB_MATRIX_RAINDROPS
#  undef ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
#  undef ENABLE_RGB_MATRIX_HUE_BREATHING
#  undef ENABLE_RGB_MATRIX_HUE_PENDULUM
#  undef ENABLE_RGB_MATRIX_HUE_WAVE
#  undef ENABLE_RGB_MATRIX_PIXEL_FRACTAL
#  undef ENABLE_RGB_MATRIX_PIXEL_FLOW
#  undef ENABLE_RGB_MATRIX_PIXEL_RAIN
#  undef ENABLE_RGB_MATRIX_STARLIGHT
#  undef ENABLE_RGB_MATRIX_STARLIGHT_DUAL_SAT
#  undef ENABLE_RGB_MATRIX_RIVERFLOW

#  undef ENABLE_RGB_MATRIX_TYPING_HEATMAP
#  undef ENABLE_RGB_MATRIX_DIGITAL_RAIN

#  undef ENABLE_RGB_MATRIX_SOLID_REACTIVE
#  undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
#  undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
#  undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
#  undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
#  undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
#  undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
#  undef ENABLE_RGB_MATRIX_SPLASH
#  undef ENABLE_RGB_MATRIX_MULTISPLASH
#  undef ENABLE_RGB_MATRIX_SOLID_SPLASH
#  undef ENABLE_RGB_MATRIX_SOLID_MULTISPLASH
#endif  // RGB_MATRIX_ENABLE

#if defined(AUDIO_ENABLE)
#  define STARTUP_SONG  SONG(NO_SOUND)
#  define GOODBYE_SONG  SONG(NO_SOUND)
#endif  // AUDIO_ENABLE

