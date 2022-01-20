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
// https://getreuer.info/posts/keyboards/custom-shift-keys

#include "custom_shift_keys.h"

bool process_custom_shift_keys(uint16_t keycode, keyrecord_t *record) {
  static uint16_t registered_keycode = KC_NO;

  // If a custom shift key is registered, then this event is either releasing
  // it or manipulating another key at the same time. Either way, we release
  // the currently registered key.
  if (registered_keycode != KC_NO) {
    unregister_code16(registered_keycode);
    registered_keycode = KC_NO;
  }

  // Search for a custom key with keycode equal to `keycode`.
  for (int i = 0; i < NUM_CUSTOM_SHIFT_KEYS; ++i) {
    if (keycode == custom_shift_keys[i].keycode) {
      if (record->event.pressed) {
        const uint8_t mods = get_mods();
#ifndef NO_ACTION_ONESHOT
        if ((mods | get_weak_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT) {
          del_oneshot_mods(MOD_MASK_SHIFT);
#else
        if ((mods | get_weak_mods()) & MOD_MASK_SHIFT) {
#endif  // NO_ACTION_ONESHOT
          del_mods(MOD_MASK_SHIFT);
          del_weak_mods(MOD_MASK_SHIFT);
          registered_keycode = custom_shift_keys[i].shifted_keycode;
        } else {
          registered_keycode = custom_shift_keys[i].keycode;
        }

        register_code16(registered_keycode);
        set_mods(mods);  // Restore the mods.
      }

      return false;
    }
  }

  return true;
}

