// Copyright 2021-2022 Google LLC
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
//
//
// For full documentation, see
// https://getreuer.info/posts/keyboards/caps-word

#include "caps_word.h"

static bool caps_word_active = false;

bool process_caps_word(uint16_t keycode, keyrecord_t* record) {
#ifndef NO_ACTION_ONESHOT
  const uint8_t mods = get_mods() | get_oneshot_mods();
#else
  const uint8_t mods = get_mods();
#endif  // NO_ACTION_ONESHOT

  if (!caps_word_active) {
    // Pressing both shift keys at the same time enables caps word.
    if ((mods & MOD_MASK_SHIFT) == MOD_MASK_SHIFT) {
      caps_word_set(true);  // Activate Caps Word.
      return false;
    }
    return true;
  }

  if (!record->event.pressed) { return true; }

  if (!(mods & ~MOD_MASK_SHIFT)) {
    switch (keycode) {
      // Ignore MO, TO, TG, TT, and OSL layer switch keys.
      case QK_MOMENTARY ... QK_MOMENTARY + 255:
      case QK_TO ... QK_TO + 255:
      case QK_TOGGLE_LAYER ... QK_TOGGLE_LAYER + 255:
      case QK_LAYER_TAP_TOGGLE ... QK_LAYER_TAP_TOGGLE + 255:
      case QK_ONE_SHOT_LAYER ... QK_ONE_SHOT_LAYER + 255:
        return true;

#ifndef NO_ACTION_TAPPING
      case QK_MOD_TAP ... QK_MOD_TAP_MAX:
        if (record->tap.count == 0) {
          // Deactivate if a mod becomes active through holding a mod-tap key.
          caps_word_set(false);
          return true;
        }
        keycode &= 0xff;
        break;

#ifndef NO_ACTION_LAYER
      case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
#endif  // NO_ACTION_LAYER
        if (record->tap.count == 0) { return true; }
        keycode &= 0xff;
        break;
#endif  // NO_ACTION_TAPPING
    }

    switch (keycode) {
      // Letter keys should be shifted.
      case KC_A ... KC_Z:
        add_weak_mods(MOD_BIT(KC_LSFT));
        return true;

      // Keycodes that continue Caps Word but shouldn't get shifted.
      case KC_1 ... KC_0:
      case KC_BSPC:
      case KC_MINS:
      case KC_UNDS:
        return true;

      // Any other keycode deactivates Caps Word.
    }
  }

  caps_word_set(false);  // Deactivate Caps Word.
  return true;
}

void caps_word_set(bool active) {
  if (active != caps_word_active) {
    if (active) {
      clear_mods();
#ifndef NO_ACTION_ONESHOT
      clear_oneshot_mods();
#endif  // NO_ACTION_ONESHOT
    }

    caps_word_active = active;
    caps_word_set_user(active);
  }
}

bool caps_word_get(void) { return caps_word_active; }

__attribute__((weak)) void caps_word_set_user(bool active) {}

