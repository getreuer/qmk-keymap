// Copyright 2022 Google LLC
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
// https://getreuer.info/posts/keyboards/achordion

#include "achordion.h"

// Copy of the `record` and `keycode` args for the current active tap-hold key.
static keyrecord_t tap_hold_record;
static uint16_t tap_hold_keycode = KC_NO;
// Timeout timer. When it expires, the key is considered held.
static uint16_t hold_timer = 0;
// Mods applied if holding a mod-tap.
static uint8_t hold_mods = 0;
// Layer activated if holding a layer-tap.
static uint8_t hold_layer = 0;
// Whether the tap-hold decision has been "settled" yet.
static bool settled = false;

// Applies the mods or layer switch for the tap-hold key's hold action.
static void apply_hold_action(void) {
  if (QK_MOD_TAP <= tap_hold_keycode && tap_hold_keycode <= QK_MOD_TAP_MAX) {
    // Extract mods from mod-tap keycode.
    hold_mods = (tap_hold_keycode >> 8) & 0xf;
    if ((tap_hold_keycode & 0x1000) != 0) { hold_mods <<= 4; }
    register_mods(hold_mods);
  } else {
    // Extract layer from layer-tap keycode.
    hold_layer = (tap_hold_keycode >> 8) & 0xf;
    layer_on(hold_layer);
  }
}

// Clears the mods or layer set by `apply_hold_action()`.
static void clear_hold_action(void) {
  if (hold_mods) {
    unregister_mods(hold_mods);
    hold_mods = 0;
  } else if (hold_layer) {
    layer_off(hold_layer);
    hold_layer = 0;
  }
}

bool process_achordion(uint16_t keycode, keyrecord_t* record) {
  // Determine whether the current event is for a mod-tap or layer-tap key.
  const bool is_tap_hold = (QK_MOD_TAP <= keycode && keycode <= QK_MOD_TAP_MAX)
      || (QK_LAYER_TAP <= keycode && keycode <= QK_LAYER_TAP_MAX);
  // Check key position to avoid acting on combos.
  const bool is_physical_pos = (record->event.key.row < 254
                             && record->event.key.col < 254);

  if (tap_hold_keycode == KC_NO) {
    if (record->event.pressed && is_physical_pos
        && is_tap_hold && record->tap.count == 0) {
      // A tap-hold key is pressed and considered by QMK as "held".
      const uint16_t timeout = achordion_timeout(keycode);
      if (timeout > 0) {
        settled = false;
        // Save info about this key.
        tap_hold_keycode = keycode;
        tap_hold_record = *record;
        hold_timer = record->event.time + timeout;
        return false;  // Skip default handling.
      }
    }

    return true;  // Otherwise, continue with default handling.
  }

  if (keycode == tap_hold_keycode && !record->event.pressed) {
    // The active tap-hold key is being released.
    tap_hold_keycode = KC_NO;
    clear_hold_action();
    return false;
  }

  if (!settled && record->event.pressed) {
    // Press event occurred on a key other than the active tap-hold key.
    settled = true;

    // If the other key is *also* a tap-hold key and considered by QMK to be
    // held, then we settle the active key as held. This way, things like
    // chording multiple home row modifiers will work, but let's our logic
    // consider simply a single tap-hold key as "active" at a time.
    //
    // Otherwise, we call `achordion_chord()` to determine whether to settle the
    // tap-hold key as tapped vs. held. We implement the tap or hold by plumbing
    // events back into the handling pipeline so that QMK features and other
    // user code can see them. This is done by calling `process_record()`, which
    // in turn calls most handlers including `process_record_user()`. Note that
    // this makes this function recursive, as it is called by
    // `process_record_user()`, so care is needed. We set `settled = true` above
    // to prevent infinite loops.
    if (!is_physical_pos || (is_tap_hold && record->tap.count == 0) ||
        achordion_chord(tap_hold_keycode, &tap_hold_record, keycode, record)) {
      apply_hold_action();
    } else {
      tap_hold_record.tap.count = 1;  // Revise event as a tap.
      process_record(&tap_hold_record);  // Create tap press event.
#if TAP_CODE_DELAY > 0
      wait_ms(TAP_CODE_DELAY);
#endif  // TAP_CODE_DELAY > 0
      tap_hold_record.event.pressed = false;
      process_record(&tap_hold_record);  // Create tap release event.
    }
    process_record(record);  // Re-process the event.
    return false;  // Block the original event.
  }
  return true;
}

void achordion_task(void) {
  if (!settled && timer_expired(timer_read(), hold_timer)) {
    // Timeout expired, settle the key as held.
    settled = true;
    apply_hold_action();
  }
}

// Returns true if `pos` on the left hand of the keyboard, false if right.
static bool on_left_hand(keypos_t pos) {
#ifdef SPLIT_KEYBOARD
  return pos.row < MATRIX_ROWS / 2;
#else
  return (MATRIX_COLS > MATRIX_ROWS)
      ? pos.col < MATRIX_COLS / 2 : pos.row < MATRIX_ROWS / 2;
#endif
}

bool achordion_opposite_hands(const keyrecord_t* tap_hold_record,
                              const keyrecord_t* other_record) {
  return on_left_hand(tap_hold_record->event.key)
      != on_left_hand(other_record->event.key);
}

// By default, use the BILATERAL_COMBINATIONS rule to consider the tap-hold key
// "held" only when it and the other key are on opposite hands.
__attribute__((weak)) bool achordion_chord(uint16_t tap_hold_keycode,
                                           keyrecord_t* tap_hold_record,
                                           uint16_t other_keycode,
                                           keyrecord_t* other_record) {
  return achordion_opposite_hands(tap_hold_record, other_record);
}

// By default, the timeout is 1000 ms for all keys.
__attribute__((weak)) uint16_t achordion_timeout(uint16_t tap_hold_keycode) {
  return 1000;
}

