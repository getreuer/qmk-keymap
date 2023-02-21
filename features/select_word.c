// Copyright 2021-2023 Google LLC
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
 * @file select_word.c
 * @brief Select word implementation
 *
 * For full documentation, see
 * <https://getreuer.info/posts/keyboards/select-word>
 */

#include "select_word.h"

// Mac users, uncomment this line:
// #define MAC_HOTKEYS

// clang-format off
enum {
    STATE_NONE,        // No selection.
    STATE_SELECTED,    // Macro released with something selected.
    STATE_WORD,        // Macro held with word(s) selected.
    STATE_FIRST_LINE,  // Macro held with one line selected.
    STATE_LINE         // Macro held with multiple lines selected.
};
// clang-format on
static uint8_t state = STATE_NONE;

// Idle timeout timer to disable Select Word after a period of inactivity.
#if SELECT_WORD_TIMEOUT > 0
static uint16_t idle_timer = 0;

void select_word_task(void) {
  if (state && timer_expired(timer_read(), idle_timer)) {
    state = STATE_NONE;
  }
}
#endif  // SELECT_WORD_TIMEOUT > 0

bool process_select_word(uint16_t keycode, keyrecord_t* record,
                         uint16_t sel_keycode) {
  if (keycode == KC_LSFT || keycode == KC_RSFT) {
    return true;
  }

#if SELECT_WORD_TIMEOUT > 0
  idle_timer = record->event.time + SELECT_WORD_TIMEOUT;
#endif  // SELECT_WORD_TIMEOUT > 0

  if (keycode == sel_keycode && record->event.pressed) {  // On key press.
    const uint8_t mods = get_mods();
#ifndef NO_ACTION_ONESHOT
    const bool shifted = (mods | get_oneshot_mods()) & MOD_MASK_SHIFT;
    clear_oneshot_mods();
#else
    const bool shifted = mods & MOD_MASK_SHIFT;
#endif  // NO_ACTION_ONESHOT

    if (!shifted) {  // Select word.
#ifdef MAC_HOTKEYS
      set_mods(MOD_BIT(KC_LALT));  // Hold Left Alt (Option).
#else
      set_mods(MOD_BIT(KC_LCTL));  // Hold Left Ctrl.
#endif  // MAC_HOTKEYS
      if (state == STATE_NONE) {
        // On first use, tap Ctrl+Right then Ctrl+Left (or with Alt on Mac) to
        // ensure the cursor is positioned at the beginning of the word.
        send_keyboard_report();
        tap_code(KC_RGHT);
        tap_code(KC_LEFT);
      }
      register_mods(MOD_BIT(KC_LSFT));
      register_code(KC_RGHT);
      state = STATE_WORD;
    } else {  // Select line.
      if (state == STATE_NONE) {
#ifdef MAC_HOTKEYS
        // Tap GUI (Command) + Left, then Shift + GUI + Right.
        set_mods(MOD_BIT(KC_LGUI));
        send_keyboard_report();
        tap_code(KC_LEFT);
        register_mods(MOD_BIT(KC_LSFT));
        tap_code(KC_RGHT);
#else
        // Tap Home, then Shift + End.
        clear_mods();
        send_keyboard_report();
        tap_code(KC_HOME);
        register_mods(MOD_BIT(KC_LSFT));
        tap_code(KC_END);
#endif  // MAC_HOTKEYS
        set_mods(mods);
        state = STATE_FIRST_LINE;
      } else {
        register_code(KC_DOWN);
        state = STATE_LINE;
      }
    }
    return false;
  }

  // `sel_keycode` was released, or another key was pressed.
  switch (state) {
    case STATE_WORD:
      unregister_code(KC_RGHT);
#ifdef MAC_HOTKEYS
      unregister_mods(MOD_BIT(KC_LSFT) | MOD_BIT(KC_LALT));
#else
      unregister_mods(MOD_BIT(KC_LSFT) | MOD_BIT(KC_LCTL));
#endif  // MAC_HOTKEYS
      state = STATE_SELECTED;
      break;

    case STATE_FIRST_LINE:
      state = STATE_SELECTED;
      break;

    case STATE_LINE:
      unregister_code(KC_DOWN);
      state = STATE_SELECTED;
      break;

    case STATE_SELECTED:
      if (keycode == KC_ESC) {
        tap_code(KC_RGHT);
        state = STATE_NONE;
        return false;
      }
      // Fallthrough intended.
    default:
      state = STATE_NONE;
  }

  return true;
}
