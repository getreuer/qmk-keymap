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

/**
 * @file caps_word.h
 * @brief Caps Word, a modern alternative to Caps Lock
 *
 * Overview
 * --------
 *
 * @note Caps Word is now a core QMK feature! See
 * <https://docs.qmk.fm/features/caps_word>
 *
 * This library implements "Caps Word", which is like conventional Caps Lock,
 * but automatically disables itself at the end of the word. This is useful for
 * typing all-caps identifiers like `MOD_MASK_ALT`.
 *
 * Caps Word is activated by pressing the left and right shift keys at the same
 * time. This way you don't need a dedicated key for using Caps Word. I've
 * tested that this works as expected with one-shot mods and Space Cadet Shift.
 * If your shift keys are mod-taps, activate Caps Word by holding both shift
 * mod-tap keys until the tapping term, release them, then begin typing.
 * Alternatively, you can call `caps_word_on()` to activate Caps Word from a
 * combo, tap dance, or other means.
 *
 * Configuration
 * -------------
 *
 * Word-breaking keys:
 * Use the `caps_word_press_user()` callback to define whether a key should
 * continue Caps Word or "break the word" and stop Caps Word.
 *
 * Representing state:
 * Use `caps_word_set_user()` callback to know when Caps Word turns on and off,
 * for instance to use an LED to indicate when Caps Word is active.
 *
 * Idle timout:
 * Optionally, Caps Word may be configured to deactivate if the keyboard is idle
 * for some time. This is useful to mitigate unintended shifting when you get
 * interrupted or switch to the mouse while Caps Word is active. In your
 * config.h, define `CAPS_WORD_IDLE_TIMEOUT` with a time in milliseconds:
 *
 *     #define CAPS_WORD_IDLE_TIMEOUT 5000  // Turn off after 5 seconds.
 *
 * and in your keymap.c, define (or add to) `housekeeping_task_user()` as
 *
 *     void housekeeping_task_user(void) {
 *       caps_word_task();
 *       // Other tasks...
 *     }
 *
 * For full documentation, see
 * <https://getreuer.info/posts/keyboards/caps-word>
 */

#pragma once

#include "quantum.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Handler function for Caps Word.
 *
 * Call this function from `process_record_user()` to implement Caps Word.
 */
bool process_caps_word(uint16_t keycode, keyrecord_t* record);

// If CAPS_WORD_IDLE_TIMEOUT is set, call `caps_word_task()` from
// `housekeeping_task_user()` as described above.
//
// If CAPS_WORD_IDLE_TIMEOUT isn't set, calling this function has no effect (but
// will still compile).
#if CAPS_WORD_IDLE_TIMEOUT > 0
void caps_word_task(void);
#else
static inline void caps_word_task(void) {}
#endif

/**
 * Turns on Caps Word.
 *
 * For instance activate Caps Word with a combo by defining a `COMBO_ACTION`
 * that calls `caps_word_on()`:
 *
 * void process_combo_event(uint16_t combo_index, bool pressed) {
 *   switch(combo_index) {
 *     case CAPS_COMBO:
 *       if (pressed) {
 *         caps_word_on();
 *       }
 *       break;
 *
 *     // Other combos...
 *   }
 * }
 */
void caps_word_on(void);

/** Turns off Caps Word. */
void caps_word_off(void);

/** Toggles Caps Word. */
void caps_word_toggle(void);

/** Returns true if Caps Word is currently on. */
bool is_caps_word_on(void);

/**
 * Optional callback that gets called when Caps Word turns on or off.
 *
 * This callback is useful to represent the current Caps Word state, e.g. by
 * setting an LED or playing a sound. In your keymap, define
 *
 *     void caps_word_set_user(bool active) {
 *       if (active) {
 *         // Do something when Caps Word activates.
 *       } else {
 *         // Do something when Caps Word deactivates.
 *       }
 *     }
 */
void caps_word_set_user(bool active);

/**
 * Optional callback, called on each key press while Caps Word is active.
 *
 * When the key should be shifted (that is, a letter key), the callback should
 * call `add_weak_mods(MOD_BIT(KC_LSFT))` to shift the key. The callback also
 * determines whether the key should continue Caps Word. Returning true
 * continues the current "word", while returning false is "word breaking" and
 * deactivates Caps Word. The default callback is
 *
 *     bool caps_word_press_user(uint16_t keycode) {
 *       switch (keycode) {
 *         // Keycodes that continue Caps Word, with shift applied.
 *         case KC_A ... KC_Z:
 *         case KC_MINS:
 *           add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to the next key.
 *           return true;
 *
 *         // Keycodes that continue Caps Word, without shifting.
 *         case KC_1 ... KC_0:
 *         case KC_BSPC:
 *         case KC_DEL:
 *         case KC_UNDS:
 *           return true;
 *
 *         default:
 *           return false;  // Deactivate Caps Word.
 *       }
 *     }
 *
 * To customize, copy the above function into your keymap and add/remove
 * keycodes to the above cases.
 *
 * @note Outside of this callback, you can use `caps_word_off()` to deactivate
 * Caps Word.
 */
bool caps_word_press_user(uint16_t keycode);

// Deprecated APIs.

/** @deprecated Use `caps_word_on()` and `caps_word_off()` instead. */
static inline void caps_word_set(bool active) {
  if (active) {
    caps_word_on();
  } else {
    caps_word_off();
  }
}

/** @deprecated Use `is_caps_word_on()` instead. */
static inline bool caps_word_get(void) { return is_caps_word_on(); }

#ifdef __cplusplus
}
#endif
