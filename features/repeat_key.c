// Copyright 2022-2023 Google LLC
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
 * @file repeat_key.c
 * @brief Repeat Key implementation
 */

#include "repeat_key.h"

static keyrecord_t last_record = {0};
static layer_state_t last_layer_state = 0;
static uint16_t last_keycode = KC_NO;
static uint8_t last_mods = 0;
static uint8_t repeat_counter = 0;
static bool recursing = false;

static void recursively_process_repeat_key(bool pressed, uint16_t time) {
  if (recursing || IS_NOEVENT(last_record.event)) {
    return;
  }

  if (pressed && repeat_counter < 255) {
    ++repeat_counter;
  }

  const uint8_t saved_mods = get_mods();
  // Apply the last mods state, stacking on top of current mods.
  register_mods(last_mods);

  const layer_state_t saved_layer_state = layer_state;
  // `process_record()` below simulates a key event at a given (row, column)
  // matrix position. We recall the last layer state in case the intended
  // key is on another layer.
  layer_state_set(last_layer_state);

  // Generate an event and plumb it into the event pipeline. The pipeline may
  // end up calling this function again, so we set `recursing = true` to prevent
  // an infinite recursion.
  last_record.event.pressed = pressed;
  last_record.event.time = time | 1;
  recursing = true;
  process_record(&last_record);
  recursing = false;

  // Restore the layer state.
  layer_state_set(saved_layer_state);

  // Restore the mods state.
  if (saved_mods != get_mods()) {
    set_mods(saved_mods);
    send_keyboard_report();
  }
}

static void remember_key_press(uint16_t keycode, keyrecord_t* record) {
  last_record = *record;
  last_layer_state = layer_state;
  last_keycode = keycode;
  last_mods = get_mods() | get_weak_mods();
#ifndef NO_ACTION_ONESHOT
  last_mods |= get_oneshot_mods();
#endif  // NO_ACTION_ONESHOT
  repeat_counter = 0;
}

bool process_repeat_key(uint16_t keycode, keyrecord_t* record,
                        uint16_t repeat_keycode) {
  if (recursing) {
    return true;
  }

  if (keycode == repeat_keycode) {
    recursively_process_repeat_key(record->event.pressed, record->event.time);
    return false;
  } else if (record->event.pressed && repeat_key_press_user(keycode, record)) {
    remember_key_press(keycode, record);
  }

  return true;
}

bool process_repeat_key_with_rev(uint16_t keycode, keyrecord_t* record,
                                 uint16_t repeat_keycode,
                                 uint16_t rev_repeat_keycode) {
  if (keycode == rev_repeat_keycode) {
    return !(record->event.pressed ? rev_repeat_key_register()
                                   : rev_repeat_key_unregister());
  }
  return process_repeat_key(keycode, record, repeat_keycode);
}

void repeat_key_register(void) {
  recursively_process_repeat_key(true, timer_read());
}

void repeat_key_unregister(void) {
  recursively_process_repeat_key(false, timer_read());
}

void repeat_key_tap(void) {
  repeat_key_register();
  wait_ms(TAP_CODE_DELAY);
  repeat_key_unregister();
}

uint8_t repeat_key_count(void) { return repeat_counter; }

uint16_t repeat_key_keycode(void) { return last_keycode; }

uint8_t repeat_key_mods(void) { return last_mods; }

bool rev_repeat_key_register(void) {
  const uint16_t keycode = rev_repeat_key_keycode();
  if (keycode) {
    register_code16(keycode);
    return true;
  }
  return false;
}

bool rev_repeat_key_unregister(void) {
  const uint16_t keycode = rev_repeat_key_keycode();
  if (keycode) {
    unregister_code16(keycode);
    return true;
  }
  return false;
}

bool rev_repeat_key_tap(void) {
  const uint16_t keycode = rev_repeat_key_keycode();
  if (keycode) {
    tap_code16(keycode);
    return true;
  }
  return false;
}

uint16_t rev_repeat_key_keycode(void) {
  // When possible, incorporate any mods into the keycode if so that lookup
  // considers for instance "B" distinctly from "Ctrl + B." Mod handedness is
  // ignored and represented as left handed, except for Right Alt (AltGr).
  uint16_t target = last_keycode;
  if (IS_QK_BASIC(target)) {
    target |= (uint16_t)((last_mods >> 4) | (last_mods & 0xf)) << 8;
    if ((last_mods & QK_RALT) == QK_RALT) {
      target |= MOD_BIT(KC_RALT);
    }
  }

  // Search for `target` in the rev_repeat_key_pairs table.
  const uint16_t* keycodes = (const uint16_t*)rev_repeat_key_pairs;
  const uint16_t num_keycodes = 2 * NUM_REV_REPEAT_KEY_PAIRS;
  for (uint16_t i = 0; i < num_keycodes; ++i) {
    if (target == pgm_read_word(keycodes + i)) {
      // Xor (i ^ 1) to get the other element in the pair.
      return pgm_read_word(keycodes + (i ^ 1));
    }
  }
  return KC_NO;  // Not found.
}

// Default implementation of repeat_key_press_user().
__attribute__((weak)) bool repeat_key_press_user(uint16_t keycode,
                                                 keyrecord_t* record) {
  switch (keycode) {
    // Ignore MO, TO, TG, and TT layer switch keys.
    case QK_MOMENTARY ... QK_MOMENTARY_MAX:
    case QK_TO ... QK_TO_MAX:
    case QK_TOGGLE_LAYER ... QK_TOGGLE_LAYER_MAX:
    case QK_LAYER_TAP_TOGGLE ... QK_LAYER_TAP_TOGGLE_MAX:
    // Ignore mod keys.
    case KC_LCTL ... KC_RGUI:
    case KC_HYPR:
    case KC_MEH:
      // Ignore one-shot keys.
#ifndef NO_ACTION_ONESHOT
    case QK_ONE_SHOT_LAYER ... QK_ONE_SHOT_LAYER_MAX:
    case QK_ONE_SHOT_MOD ... QK_ONE_SHOT_MOD_MAX:
#endif  // NO_ACTION_ONESHOT
      return false;

      // Ignore hold events on tap-hold keys.
#ifndef NO_ACTION_TAPPING
    case QK_MOD_TAP ... QK_MOD_TAP_MAX:
#ifndef NO_ACTION_LAYER
    case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
#endif  // NO_ACTION_LAYER
      if (record->tap.count == 0) {
        return false;
      }
      break;
#endif  // NO_ACTION_TAPPING

#ifdef SWAP_HANDS_ENABLE
    case QK_SWAP_HANDS ... QK_SWAP_HANDS_MAX:
      if (IS_SWAP_HANDS_KEYCODE(keycode) || record->tap.count == 0) {
        return false;
      }
      break;
#endif  // SWAP_HANDS_ENABLE
  }

  return true;
}
