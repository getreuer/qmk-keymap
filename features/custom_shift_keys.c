// Copyright 2021 Google LLC
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

#include "custom_shift_keys.h"

static uint16_t custom_shift_keycode(int active_key, bool shifted) {
  const custom_shift_key_t* p = &custom_shift_keys[active_key];
  return shifted ? p->shifted_keycode : p->keycode;
}

bool process_custom_shift_keys(uint16_t keycode, keyrecord_t *record) {
  const uint8_t kNone = 255;
  static uint8_t saved_mods = 0;
  static uint8_t active_key = kNone;
  static bool shifted = false;

  // If a custom key is active, then this event is either releasing it or
  // manipulating another key at the same time. Either way, we release the
  // currently active key.
  if (active_key != kNone) {
    unregister_code16(custom_shift_keycode(active_key, shifted));
    add_mods(saved_mods);  // Restore shift mods that were active on press.
    active_key = kNone;
    saved_mods = 0;
    shifted = false;
  }

  // Search for a custom key with keycode equal to `keycode`.
  for (int i = 0; i < NUM_CUSTOM_SHIFT_KEYS; ++i) {
    if (keycode == custom_shift_keys[i].keycode) {
      if (record->event.pressed) {
        if ((get_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT) {
          // The key is being pressed with shift held. We save the shift mods
          // in `saved_mods`, then delete shift from the mod states.
          saved_mods = get_mods() & MOD_MASK_SHIFT;
          del_mods(MOD_MASK_SHIFT);
          del_oneshot_mods(MOD_MASK_SHIFT);
          shifted = true;
        } else {
          shifted = false;
        }

        active_key = i;  // Remember which custom key is active.
        register_code16(custom_shift_keycode(active_key, shifted));
      }

      return false;
    }
  }

  return true;
}

