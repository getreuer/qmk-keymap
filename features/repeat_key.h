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
 * Also included is a Reverse Repeat Key, performing the "reverse" if there is
 * one for the last key. If Page Down was the last key, the Reverse Repeat
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

/** Handler function for Repeat Key and Reverse Repeat Key. */
bool process_repeat_key_with_rev(uint16_t keycode, keyrecord_t* record,
                                 uint16_t repeat_keycode,
                                 uint16_t rev_repeat_keycode);

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
 * The number of times the last key has been repeated. When a key is pressed
 * normally, the count is 0. When the Repeat Key is used to repeat a key, the
 * count is 1 on the first repeat, 2 on the second repeat, an so on.
 */
uint8_t repeat_key_count(void);

/** Keycode of the last pressed key to be repeated. */
uint16_t repeat_key_keycode(void);

/** Modifiers to be applied when repeating. */
uint8_t repeat_key_mods(void);

/**
 * Optional callback defining which keys are eligible for repeating. The
 * callback is called on every key press. Returning true means the key may be
 * repeated, and returning false means the key is ignored.
 *
 * Here is the default implementation, which ignores modifier and layer switch
 * keys so that it is possible to set some mods and change layers between
 * pressing a key and repeating it:
 *
 *     bool repeat_key_press_user(uint16_t keycode, keyrecord_t* record) {
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
 *             return true;
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
bool repeat_key_press_user(uint16_t keycode, keyrecord_t* record);

/**
 * Registers (presses down) the Reverse Repeat Key, performing the reverse, if
 * there is one, for the last pressed key. If no reverse is found, the function
 * takes no action and returns false.
 *
 * @return True if a reverse key was found.
 */
bool rev_repeat_key_register(void);

/**
 * Unregisters (releases) the Reverse Repeat Key.
 *
 * @return True if a reverse key was found.
 */
bool rev_repeat_key_unregister(void);

/**
 * Taps the Reverse Repeat Key with a delay of TAP_CODE_DELAY.
 *
 * @return True if a reverse key was found.
 */
bool rev_repeat_key_tap(void);

/**
 * Finds the keycode to be performed by Reverse Repeat Key.
 *
 * The Reverse Repeat keycode is found by searching for `repeat_key_keycode()`
 * in the `rev_repeat_key_pairs` table, and returning the other element in the
 * pair. If no match is found, `KC_NO` is returned.
 *
 * When `repeat_key_keycode()` is a basic keycode, `repeat_key_mods()` is
 * considered in the lookup as well so that for instance "B" is distinct from
 * "Ctrl + B." Handness of mods is ignored except for Right Alt (AltGr).
 */
uint16_t rev_repeat_key_keycode(void);

/**
 * Table of opposing keycode pairs for Reverse Repeat. To use the Reverse Repeat
 * Key, define this table in your keymap and define `NUM_REV_REPEAT_KEY_PAIRS`
 * as the number of pairs in the table. A suggested definition:
 *
 *     const uint16_t rev_repeat_key_pairs[][2] PROGMEM = {
 *       {KC_LEFT, KC_RGHT},             // Left / Right Arrow.
 *       {KC_UP  , KC_DOWN},             // Up / Down Arrow.
 *       {S(KC_LEFT), S(KC_RGHT)},       // Shift + Left / Right Arrow.
 *       {S(KC_UP), S(KC_DOWN)},         // Shift + Up / Down Arrow.
 *       {C(KC_LEFT), C(KC_RGHT)},       // Ctrl + Left / Right Arrow.
 *       {C(S(KC_LEFT)), C(S(KC_RGHT))}, // Ctrl + Shift + Left / Right Arrow.
 *       {KC_HOME, KC_END },             // Home / End.
 *       {KC_PGUP, KC_PGDN},             // Page Up / Page Down.
 *       {C(KC_PGUP), C(KC_PGDN)},       // Ctrl + Page Up / Page Down.
 *       {KC_TAB , S(KC_TAB)},           // Tab / Shift + Tab.
 *       {KC_WBAK, KC_WFWD},             // Browser Back / Forward.
 *       {KC_MNXT, KC_MPRV},             // Next / Previous Media Track.
 *       {KC_MFFD, KC_MRWD},             // Fast Forward / Rewind Media.
 *       {KC_VOLU, KC_VOLD},             // Volume Up / Down.
 *       {KC_BRIU, KC_BRID},             // Brightness Up / Down.
 *
 *       // Navigation hotkeys in Vim, Emacs, and other programs.
 *       {KC_H   , KC_L   },             // Left / Right.
 *       {KC_J   , KC_K   },             // Down / Up.
 *       {KC_W   , KC_B   },             // Forward / Backward by word.
 *       {C(KC_N), C(KC_P)},             // Next / Previous.
 *       {C(KC_D), C(KC_U)},             // Down / Up.
 *       {C(KC_F), C(KC_B)},             // Forward / Backward.
 *       {A(KC_F), A(KC_B)},             // Forward / Backward by word.
 *
 *       // Mouse keys (if enabled).
 *     #ifdef MOUSEKEY_ENABLE
 *       {KC_MS_L, KC_MS_R},             // Mouse Cursor Left / Right.
 *       {KC_MS_U, KC_MS_D},             // Mouse Cursor Up / Down.
 *       {KC_WH_L, KC_WH_R},             // Mouse Wheel Left / Right.
 *       {KC_WH_U, KC_WH_D},             // Mouse Wheel Up / Down.
 *       {S(KC_WH_U), S(KC_WH_D)},       // Shift + Mouse Wheel Up / Down.
 *       {C(KC_WH_U), C(KC_WH_D)},       // Ctrl + Mouse Wheel Up / Down.
 *     #endif  // MOUSEKEY_ENABLE
 *
 *       // Lighting keys (if enabled).
 *     #ifdef BACKLIGHT_ENABLE
 *       {BL_UP  , BL_DOWN},             // Increase / decrease backlight.
 *     #endif  // BACKLIGHT_ENABLE
 *     #if defined(RGBLIGHT_ENABLE) || defined(RGB_MATRIX_ENABLE)
 *       {RGB_MOD, RGB_RMOD},            // Effect mode forward / backward.
 *       {RGB_HUI, RGB_HUD},             // Increase / decrease hue.
 *       {RGB_SAI, RGB_SAD},             // Increase / decrease saturation.
 *       {RGB_VAI, RGB_VAD},             // Increase / decrease value.
 *       {RGB_SPI, RGB_SPD},             // Increase / decrease effect speed.
 *     #endif  // defined(RGBLIGHT_ENABLE) || defined(RGB_MATRIX_ENABLE)
 *     };
 *     const uint16_t NUM_REV_REPEAT_KEY_PAIRS =
 *       sizeof(rev_repeat_key_pairs) / sizeof(*rev_repeat_key_pairs);
 *
 * The Reverse Repeat Key searches this table for an entry matching the keycode
 * of the last key pressed and uses the other key in pair, as documented above
 * for `rev_repeat_key_keycode()`.
 */
extern const uint16_t rev_repeat_key_pairs[][2] PROGMEM;
extern const uint16_t NUM_REV_REPEAT_KEY_PAIRS;

#ifdef __cplusplus
}
#endif
