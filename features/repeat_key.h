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
 * @file repeat_key.h
 * @brief Repeat Key - an extensible "repeat last key" implementation.
 *
 * Repeat Key performs the action of last pressed key. Tapping the Repeat Key
 * after tapping the Z key types another "z." Repeat Key remembers the modifiers
 * that were active with the last key press. These modifiers are combined with
 * any additional active modifiers while pressing the Repeat Key. For instance,
 * if the last press key was Ctrl + Z, then Shift + Repeat Key performs Ctrl +
 * Shift + Z.
 *
 * Also included is an Alternate Repeat Key, performing the "alternate" if there
 * is one for the last key. By default it is defined for navigation keys to act
 * in the reverse direction. If Page Down was the last key, the Alternate Repeat
 * performs Page Up.
 *
 * The implementation is a generic event-plumbing strategy that interoperates
 * predictably with most QMK features, including tap-hold keys, Auto Shift,
 * Combos, and userspace macros.
 *
 * For full documentation, see
 * <https://getreuer.info/posts/keyboards/repeat-key>
 */

#pragma once

#include "quantum.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Handler function for Repeat Key. Call either this function or
 * `process_repeat_key_with_rev()` (but not both) from `process_record_user()`
 * to implement Repeat Key in your keymap.
 *
 * If your `process_record_user()` has other handlers or macros, Repeat Key’s
 * handler `process_repeat_key()` should preferably be called before anything
 * else. (If you also use Achordion, then call Achordion's handler first, Repeat
 * Key's handler second, and then other handlers.)
 */
bool process_repeat_key(uint16_t keycode, keyrecord_t* record,
                        uint16_t repeat_keycode);

/** Handler function for Repeat Key and Alternate Repeat Key. */
bool process_repeat_key_with_alt(uint16_t keycode, keyrecord_t* record,
                                 uint16_t repeat_keycode,
                                 uint16_t alt_repeat_keycode);

/**
 * @brief Signed count of times the key has been repeated or alternate repeated.
 *
 * @note The count is nonzero only while a repeated or alternate-repeated key is
 * being processed.
 *
 * When a key is pressed normally, the count is 0. When the Repeat Key is used
 * to repeat a key, the count is 1 on the first repeat, 2 on the second repeat,
 * and continuing up to 127.
 *
 * Negative counts are used similarly for alternate repeating. When the
 * Alternate Repeat Key is used, the count is -1 on the first alternate repeat,
 * -2 on the second, continuing down to -127.
 */
int8_t get_repeat_key_count(void);

/** @brief Keycode of the key to be repeated. */
uint16_t get_repeat_key_keycode(void);
/** @brief Mods to be applied when repeating. */
uint8_t get_repeat_key_mods(void);
/** @brief Sets the keycode to repeat. */
void set_repeat_key_keycode(uint16_t keycode);
/** @brief Sets the mods to repeat. */
void set_repeat_key_mods(uint8_t mods);

/**
 * @brief Callback defining which keys are eligible for repeating.
 *
 * The callback is called on every key press. Returning true means the key may
 * be repeated, and returning false means the key is ignored.
 *
 * Here is the default implementation, which ignores modifier and layer switch
 * keys so that it is possible to set some mods and change layers between
 * pressing a key and repeating it:
 *
 *     bool get_repeat_key_eligible(uint16_t keycode, keyrecord_t* record) {
 *       switch (keycode) {
 *         // Ignore MO, TO, TG, and TT layer switch keys.
 *         case QK_MOMENTARY ... QK_MOMENTARY_MAX:
 *         case QK_TO ... QK_TO_MAX:
 *         case QK_TOGGLE_LAYER ... QK_TOGGLE_LAYER_MAX:
 *         case QK_LAYER_TAP_TOGGLE ... QK_LAYER_TAP_TOGGLE_MAX:
 *         // Ignore mod keys.
 *         case KC_LCTL ... KC_RGUI:
 *         case KC_HYPR:
 *         case KC_MEH:
 *         // Ignore one-shot keys.
 *     #ifndef NO_ACTION_ONESHOT
 *         case QK_ONE_SHOT_LAYER ... QK_ONE_SHOT_LAYER_MAX:
 *         case QK_ONE_SHOT_MOD ... QK_ONE_SHOT_MOD_MAX:
 *     #endif  // NO_ACTION_ONESHOT
 *           return false;
 *
 *         // Ignore hold events on tap-hold keys.
 *     #ifndef NO_ACTION_TAPPING
 *         case QK_MOD_TAP ... QK_MOD_TAP_MAX:
 *     #ifndef NO_ACTION_LAYER
 *         case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
 *     #endif  // NO_ACTION_LAYER
 *           if (record->tap.count == 0) {
 *             return false;
 *           }
 *           break;
 *     #endif  // NO_ACTION_TAPPING
 *
 *     #ifdef SWAP_HANDS_ENABLE
 *         case QK_SWAP_HANDS ... QK_SWAP_HANDS_MAX:
 *           if (IS_SWAP_HANDS_KEYCODE(keycode) || record->tap.count == 0) {
 *             return false;
 *           }
 *           break;
 *     #endif  // SWAP_HANDS_ENABLE
 *       }
 *
 *       return true;
 *     }
 *
 * To customize, copy the above function into your keymap and edit.
 */
bool get_repeat_key_eligible(uint16_t keycode, keyrecord_t* record);

/**
 * @brief Keycode to be used for alternate repeating.
 *
 * Alternate Repeat performs this keycode based on the last eligible pressed key
 * and mods, get_repeat_key_keycode() and get_repeat_key_mods(). For example,
 * when the last key was KC_UP, this function returns KC_DOWN. The function
 * returns KC_NO if the last key doesn't have a defined alternate.
 */
uint16_t get_alt_repeat_key_keycode(void);

/**
 * @brief Optional user callback to define additional alternate keys.
 *
 * When `get_alt_repeat_key_keycode()` is called, it first calls this callback.
 * It should return a keycode representing the "alternate" of the given keycode
 * and mods. Returning KC_NO defers to the default definitions in
 * `get_alt_repeat_key_keycode()`.
 *
 * This callback can be used to define additional pairs of keys that "reverse"
 * each other. More generally, Alternate Repeat can be configured to perform an
 * action that "complements" the last key---Alternate Repeat not limited to
 * reverse repeating, and it need not be symmetric. For instance, you can use it
 * to eliminate the worst same-finger bigrams in your layout.
 */
uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods);

/**
 * Registers (presses down) the Repeat Key. This is useful for invoking Repeat
 * as part of a tap dance or other custom handler. Note that if doing so, you
 * likely want to define `repeat_key_press_user()` to ignore the key associated
 * with that handler so that the Repeat Key does not attempt to repeat itself.
 */
void repeat_key_register(void);

/** Unregisters (releases) the Repeat Key. */
void repeat_key_unregister(void);

/** Taps the Repeat Key with a delay of `TAP_CODE_DELAY`. */
void repeat_key_tap(void);

/**
 * Registers (presses down) the Alternate Repeat Key, performing the alternate,
 * if there is one, for the last pressed key. If no alternate is found, the
 * function takes no action and returns false.
 *
 * @return True if an alternate key was found.
 */
bool alt_repeat_key_register(void);

/**
 * Unregisters (releases) the Alternate Repeat Key.
 *
 * @return True if an alternate key was found.
 */
bool alt_repeat_key_unregister(void);

/**
 * Taps the Alternate Repeat Key with a delay of TAP_CODE_DELAY.
 *
 * @return True if an alternate key was found.
 */
bool alt_repeat_key_tap(void);

// Deprecated APIs.

/** @deprecated Use `process_repeat_key_with_alt()` instead. */
static inline bool process_repeat_key_with_rev(
    uint16_t keycode, keyrecord_t* record,
    uint16_t repeat_keycode,
    uint16_t rev_repeat_keycode) {
  return process_repeat_key_with_alt(keycode, record,
      repeat_keycode, rev_repeat_keycode);
}

/** @deprecated Use `get_repeat_key_count()` instead. */
static inline int8_t repeat_key_count(void) { return get_repeat_key_count(); }

/** @deprecated Use `get_repeat_key_keycode()` instead. */
static inline uint16_t repeat_key_keycode(void) {
  return get_repeat_key_keycode();
}

/** @deprecated Use `get_repeat_key_mods()` instead. */
static inline uint8_t repeat_key_mods(void) { return get_repeat_key_mods(); }

/** @deprecated Use `get_alt_repeat_key_keycode()` instead. */
static inline uint16_t rev_repeat_key_keycode(void) {
  return get_alt_repeat_key_keycode();
}

/** @deprecated Use `get_alt_repeat_key_keycode()` instead. */
static inline uint16_t get_rev_repeat_key_keycode(void) {
  return get_alt_repeat_key_keycode();
}

/** @deprecated Use `get_alt_repeat_key_keycode_user()` instead. */
uint16_t get_rev_repeat_key_keycode_user(uint16_t keycode, uint8_t mods);

/** @deprecated Use `alt_repeat_key_register()` instead. */
static inline bool rev_repeat_key_register(void) {
  return alt_repeat_key_register();
}

/** @deprecated Use `alt_repeat_key_unregister()` instead. */
static inline bool rev_repeat_key_unregister(void) {
  return alt_repeat_key_unregister();
}

/** @deprecated Use `alt_repeat_key_tap()` instead. */
static inline bool rev_repeat_key_tap(void) {
  return alt_repeat_key_tap();
}

#ifdef __cplusplus
}
#endif
