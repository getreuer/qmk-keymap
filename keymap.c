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
 * @mainpage Pascal Getreuer's QMK keymap
 *
 * This is my Quantum Mechanical Keyboard (QMK) keymap for the Dactyl Ergodox.
 * Who knew a keyboard could do so much?
 *
 * Feature libraries
 * -----------------
 *  * features/achordion.h: customize the tap-hold decision
 *  * features/autocorrection.h: run rudimentary autocorrection on your keyboard
 *  * features/caps_word.h: modern alternative to Caps Lock
 *  * features/custom_shift_keys.h: they're surprisingly tricky to get right;
 *                                  here is my approach
 *  * features/layer_lock.h: macro to stay in the current layer
 *  * features/mouse_turbo_click.h: macro that clicks the mouse rapidly
 *  * features/repeat_key.h: a "repeat last key" implementation
 *  * features/sentence_case.h: capitalize first letter of sentences
 *  * features/select_word.h: macro for convenient word or line selection
 *
 * License
 * -------
 * This code uses the Apache License 2.0. See LICENSE.txt for details.
 *
 * For further documentation of this keymap's features, see
 * <https://getreuer.info/posts/keyboards>
 */

#include QMK_KEYBOARD_H

#include "features/achordion.h"
#include "features/custom_shift_keys.h"
#include "features/repeat_key.h"
#include "features/select_word.h"
#include "features/sentence_case.h"
#include "layout.h"

enum layers {
  BASE,
  QWERTY,
  SYM,
  ADJUST,
};

enum custom_keycodes {
  UPDIR = SAFE_RANGE,
  EXIT,
  SCOPE,
  SELWORD,
  TMUXESC,
  SRCHSEL,
  USRNAME,
  DASH,
  ARROW,
  THMBUP,
  REPEAT,
  ALTREP,
  M_ION,
  M_NION,
  M_MENT,
  M_QUEN,
  M_TMENT,
  M_THE,
  M_UPDIR,
  M_INCLUDE,
  M_DOCSTR,
};

// This keymap uses home row mods. In addition to mods, I have home row
// layer-tap keys for the SYM layer. The key arrangement is a variation on
// "GASC-order" home row mods:
//
//             Left hand                          Right hand
// +-------+-------+-------+-------+   +-------+-------+-------+-------+
// |  Sym  |  Alt  | Shift | Ctrl  |   | Ctrl  | Shift |  Alt  |  Sym  |
// +-------+-------+-------+-------+   +-------+-------+-------+-------+
// |  Gui  |                                                   |  Gui  |
// +-------+                                                   +-------+

// Home row mods for Magic Sturdy layer.
#define HOME_S LT(SYM, KC_S)
#define HOME_T LALT_T(KC_T)
#define HOME_R LSFT_T(KC_R)
#define HOME_D LCTL_T(KC_D)
#define HOME_N RCTL_T(KC_N)
#define HOME_E RSFT_T(KC_E)
#define HOME_A LALT_T(KC_A)
#define HOME_I LT(SYM, KC_I)
#define HOME_X LGUI_T(KC_X)
#define HOME_SC RGUI_T(KC_SCLN)

// Home row mods for QWERTY layer.
#define QHOME_A LT(SYM, KC_A)
#define QHOME_S LALT_T(KC_S)
#define QHOME_D LSFT_T(KC_D)
#define QHOME_F LCTL_T(KC_F)
#define QHOME_J RCTL_T(KC_J)
#define QHOME_K RSFT_T(KC_K)
#define QHOME_L LALT_T(KC_L)
#define QHOME_SC LT(SYM, KC_SCLN)
#define QHOME_Z LGUI_T(KC_Z)
#define QHOME_SL RGUI_T(KC_SLSH)

// Alternate Repeat is the "magic" key.
#define MAGIC ALTREP

// I have F13 bound to toggle Redshift (http://jonls.dk/redshift/).
#define REDSHFT KC_F13

// clang-format off
const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] PROGMEM = {
  // The base layer is Ikcelaks' Magic Sturdy layout, a variation of Oxey's
  // Sturdy, plus a few tweaks of my own.
  // https://github.com/Ikcelaks/keyboard_layouts/blob/main/magic_sturdy/magic_sturdy.md
  [BASE] = LAYOUT_LR(
    KC_GRV , KC_7   , KC_8   , KC_9   , KC_0   , KC_5   ,
    KC_TAB , KC_V   , KC_M   , KC_L   , KC_C   , KC_P   ,
    KC_ESC , HOME_S , HOME_T , HOME_R , HOME_D , KC_Y   ,
    KC_LSFT, HOME_X , KC_K   , KC_J   , KC_G   , KC_W   ,
    KC_LCTL, KC_PGUP, KC_PGDN, KC_DOWN, KC_UP  ,
                                                          MO(SYM), KC_UNDS,
                                                                   KC_BSLS,
                                                 KC_DEL , KC_SPC , KC_BTN1,

                      KC_6   , KC_1   , KC_2   , KC_3   , KC_4   , KC_SLSH,
                      KC_B   , MAGIC  , KC_U   , KC_O   , KC_Q   , KC_MINS,
                      KC_F   , HOME_N , HOME_E , HOME_A , HOME_I , KC_BSPC,
                      KC_Z   , KC_H   , KC_COMM, KC_DOT , HOME_SC, KC_ENT ,
                               KC_LEFT, KC_RGHT, DASH   , ARROW  , THMBUP ,
    KC_QUOT, TMUXESC,
    SCOPE  ,
    SELWORD, REPEAT , KC_ENT
  ),

  [QWERTY] = LAYOUT_LR(  // Alternative base layer: QWERTY.
    _______, _______, _______, _______, _______, _______,
    _______, KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,
    _______, QHOME_A, QHOME_S, QHOME_D, QHOME_F, KC_G   ,
    _______, QHOME_Z, KC_X   , KC_C   , KC_V   , KC_B   ,
    _______, _______, _______, _______, _______,
                                                          _______, _______,
                                                                   _______,
                                                 _______, _______, _______,

                      _______, _______, _______, _______, _______, _______,
                      KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_MINS,
                      KC_H   , QHOME_J, QHOME_K, QHOME_L, QHOME_SC, _______,
                      KC_N   , KC_M   , KC_COMM, KC_DOT , QHOME_SL, _______,
                               _______, _______, _______, _______, _______,
    KC_QUOT, _______,
    _______,
    _______, _______, _______
  ),

  [SYM] = LAYOUT_LR(  // Symbol layer.
    _______, KC_F7  , KC_F8  , KC_F9  , KC_F10 , KC_F5  ,
    TMUXESC, _______, KC_LABK, KC_RABK, KC_AT  , KC_DOT ,
    _______, KC_EXLM, KC_MINS, KC_PLUS, KC_EQL , KC_HASH,
    _______, _______, KC_SLSH, KC_ASTR, KC_CIRC, UPDIR,
    _______, _______, _______, C(KC_END), C(KC_HOME),
                                                          _______, _______,
                                                                   _______,
                                                 SRCHSEL, _______, _______,

                      KC_F6  , KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F11 ,
                      KC_AMPR, _______, KC_LBRC, KC_RBRC, _______, KC_F12 ,
                      KC_PIPE, KC_COLN, KC_LPRN, KC_RPRN, KC_PERC, TO(ADJUST),
                      KC_TILD, KC_DLR , KC_LCBR, KC_RCBR, _______, _______,
                               KC_HOME, KC_END , _______, _______, _______,
    _______, _______,
    _______,
    _______, _______, _______
  ),

  [ADJUST] = LAYOUT_LR(  // Adjust layer.
    QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    EXIT   , XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                                          XXXXXXX, XXXXXXX,
                                                                   _______,
                                                 XXXXXXX, XXXXXXX, _______,

                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,
                      XXXXXXX, KC_BRID, KC_BRIU, REDSHFT, XXXXXXX, XXXXXXX ,
                      XXXXXXX, DF(BASE), DF(QWERTY), XXXXXXX, XXXXXXX, EXIT  ,
                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                               XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX,
    XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX
  ),
};
// clang-format on

const uint32_t unicode_map[] PROGMEM = {};

const custom_shift_key_t custom_shift_keys[] = {
    {KC_DOT, KC_QUES},  // Shift . is ?
    {KC_COMM, KC_EXLM},
    {KC_EQL, KC_EQL},  // Don't shift =
    {KC_SLSH, KC_SLSH},  // Don't shift /
};
uint8_t NUM_CUSTOM_SHIFT_KEYS =
    sizeof(custom_shift_keys) / sizeof(*custom_shift_keys);

const uint16_t caps_combo[] PROGMEM = {KC_J, KC_COMM, COMBO_END};
combo_t key_combos[] = {
    // J and , => activate Caps Word.
    COMBO(caps_combo, CW_TOGG),
};
uint16_t COMBO_LEN = sizeof(key_combos) / sizeof(*key_combos);

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t* record) {
  switch (keycode) {
    // Increase the tapping term a little for slower ring and pinky fingers.
    case HOME_S:
    case HOME_T:
    case HOME_A:
    case HOME_I:
    case QHOME_A:
    case QHOME_S:
    case QHOME_L:
    case QHOME_SC:
      return TAPPING_TERM + 15;

    default:
      return TAPPING_TERM;
  }
}

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t* record) {
  // If you quickly hold a tap-hold key after tapping it, the tap action is
  // repeated. Key repeating is useful e.g. for Vim navigation keys, but can
  // lead to missed triggers in fast typing. Here, returning 0 means we
  // instead want to "force hold" and disable key repeating.
  switch (keycode) {
    case HOME_N:
    // Repeating is useful for Vim navigation keys.
    case QHOME_J:
    case QHOME_K:
    case QHOME_L:
      return QUICK_TAP_TERM;  // Enable key repeating.
    default:
      return 0;  // Otherwise, force hold and disable key repeating.
  }
}

bool caps_word_press_user(uint16_t keycode) {
  switch (keycode) {
    // Keycodes that continue Caps Word, with shift applied.
    case KC_A ... KC_Z:
      add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to the next key.
      return true;

    // Keycodes that continue Caps Word, without shifting.
    case KC_1 ... KC_0:
    case KC_BSPC:
    case KC_DEL:
    // I have a dedicated underscore key, so no need to shift KC_MINS.
    case KC_MINS:
    case KC_UNDS:
      return true;

    default:
      return false;  // Deactivate Caps Word.
  }
}

bool achordion_chord(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record,
                     uint16_t other_keycode, keyrecord_t* other_record) {
  // Exceptionally consider the following chords as holds, even though they
  // are on the same hand in Magic Sturdy.
  switch (tap_hold_keycode) {
    case HOME_S:  // S + D.
      if (other_keycode == HOME_D) {
        return true;
      }
      break;
  }

  // Also allow same-hand holds when the other key is in the rows below the
  // alphas. I need the `% (MATRIX_ROWS / 2)` because my keyboard is split.
  if (other_record->event.key.row % (MATRIX_ROWS / 2) >= 4) {
    return true;
  }

  // Otherwise, follow the opposite hands rule.
  return achordion_opposite_hands(tap_hold_record, other_record);
}

uint16_t achordion_timeout(uint16_t tap_hold_keycode) {
  switch (tap_hold_keycode) {
    case HOME_X:
    case HOME_SC:
    case QHOME_Z:
    case QHOME_SL:
      return 0;  // Bypass Achordion for these keys.
  }

  return 800;  // Otherwise use a timeout of 800 ms.
}

char sentence_case_press_user(uint16_t keycode, keyrecord_t* record,
                              uint8_t mods) {
  if ((mods & ~(MOD_MASK_SHIFT | MOD_BIT(KC_RALT))) == 0) {
    const bool shifted = mods & MOD_MASK_SHIFT;
    switch (keycode) {
      case KC_LCTL ... KC_RGUI:  // Mod keys.
        return '\0';  // These keys are ignored.

      case KC_A ... KC_Z:
        return 'a';  // Letter key.

      case KC_DOT:  // Both . and Shift . (?) punctuate sentence endings.
        return '.';
      case KC_COMM:  // Shift , (!) is a sentence ending.
        return shifted ? '.' : '#';

      case KC_1 ... KC_0:  // 1 2 3 4 5 6 7 8 9 0
      case KC_MINS ... KC_SCLN:  // - = [ ] ; ` backslash
      case KC_GRV:
      case KC_SLSH:
        return '#';  // Symbol key.

      case KC_SPC:
        return ' ';  // Space key.

      case KC_QUOT:
        return '\'';  // Quote key.
    }
  }

  // Otherwise clear Sentence Case to initial state.
  sentence_case_clear();
  return '\0';
}

uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {
  if ((mods & ~MOD_MASK_SHIFT) == 0) {
    switch (keycode) {
      // Behavior for Magic Sturdy's "magic" key.
      case HOME_A:  // A -> O
        return KC_O;
      case KC_C:  // C -> Y
        return KC_Y;
      case HOME_D:  // D -> Y
        return KC_Y;
      case HOME_E:  // E -> U
        return KC_U;
      case KC_G:  // G -> Y
        return KC_Y;
      case HOME_I:  // I -> ON
        return M_ION;
      case KC_L:  // L -> K
        return KC_K;
      case KC_M:  // M -> ENT
        return M_MENT;
      case HOME_N:  // N -> ION
        return M_NION;
      case KC_N:
        return KC_N;
      case KC_O:  // O -> A
        return KC_A;
      case KC_P:  // P -> Y
        return KC_Y;
      case KC_Q:  // Q -> UEN
        return M_QUEN;
      case HOME_R:  // R -> L
        return KC_L;
      case HOME_S:  // S -> K
        return KC_K;
      case HOME_T:  // T -> MENT
        return M_TMENT;
      case KC_U:  // U -> E
        return KC_E;
      case KC_Y:  // Y -> P
        return KC_P;

      case KC_SPC:  // spc -> THE
        return M_THE;
      case KC_DOT:  // . -> ./
        return M_UPDIR;
      case KC_HASH:  // # -> include
        return M_INCLUDE;
      case KC_QUOT:
        if ((mods & MOD_MASK_SHIFT) != 0) {
          // " -> ""<cursor>""" (for Python doc strings)
          return M_DOCSTR;
        }
        return KC_NO;
    }
  } else if ((mods & MOD_MASK_CTRL)) {
    switch (keycode) {
      case HOME_A:  // Ctrl+A -> Ctrl+C
        return C(KC_C);
      case KC_C:  // Ctrl+C -> Ctrl+V
        return C(KC_V);
    }
  }
  return KC_TRNS;
}

bool get_repeat_key_eligible_user(uint16_t keycode, keyrecord_t* record,
                                  uint8_t* remembered_mods) {
  // Forget Shift on letter keys A-Y when Shift or AltGr are the only mods.
  // Exceptionally, I want to remember Shift on Z for "ZZ" in Vim.
  switch (keycode) {
    case KC_A ... KC_Y:
      if ((*remembered_mods & ~(MOD_MASK_SHIFT | MOD_BIT(KC_RALT))) == 0) {
        *remembered_mods &= ~MOD_MASK_SHIFT;
      }
      break;
  }

  return true;
}

// clang-format off
bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  if (!process_achordion(keycode, record)) { return false; }
  if (!process_repeat_key_with_alt(keycode, record, REPEAT, ALTREP)) {
    return false;
  }
  if (!process_custom_shift_keys(keycode, record)) { return false; }
  if (!process_select_word(keycode, record, SELWORD)) { return false; }
  if (!process_sentence_case(keycode, record)) { return false; }

  const uint8_t mods = get_mods();
  const bool shifted = (mods | get_weak_mods()
#ifndef NO_ACTION_ONESHOT
                        | get_oneshot_mods()
#endif  // NO_ACTION_ONESHOT
                       ) & MOD_MASK_SHIFT;

  if (record->event.pressed) {
    switch (keycode) {
      case EXIT:
        layer_off(ADJUST);
        return false;

      case SCOPE:
        SEND_STRING("::");
        return false;

      case UPDIR:
        SEND_STRING("../");
        return false;

      case TMUXESC:  // Enter copy mode in Tmux.
        SEND_STRING(SS_LCTL("a") SS_TAP(X_ESC));
        return false;

      case SRCHSEL:  // Searches the current selection in a new tab.
        // Mac users, change LCTL to LGUI.
        SEND_STRING(SS_LCTL("ct") SS_DELAY(100) SS_LCTL("v") SS_TAP(X_ENTER));
        return false;

      // The following cases type a few Unicode symbols.
      //
      // `send_unicode_hex_string()` is deprecated. The docs suggest to ensure
      // keymap.c is UTF-8 encoded and write literal Unicode characters in the
      // string passed to `send_unicode_string()`. Unfortunately, terminals can
      // have problems displaying Unicode correctly with monospaced width (or
      // at all). So we take another approach: write escape codes `\xhh` for the
      // UTF-8 encoding.

      case DASH:  // En dash, or em dash when shifted.
        send_unicode_string(shifted ? "\xe2\x80\x94" : "\xe2\x80\x93");
        return false;

      case ARROW:  // -> Unicode arrow, or => when shifted.
        send_unicode_string(shifted ? "\xe2\x87\x92" : "\xe2\x86\x92");
        return false;

      case THMBUP:  // Thumbs up emoji, or party emoji when shifted.
        send_unicode_string(shifted ? "\xf0\x9f\xa5\xb3" : "\xf0\x9f\x91\x8d");
        return false;

      // Macros invoked through the MAGIC key.
      case M_ION:     SEND_STRING(/*i*/"on"); break;
      case M_NION:    SEND_STRING(/*n*/"ion"); break;
      case M_MENT:    SEND_STRING(/*m*/"ent"); break;
      case M_QUEN:    SEND_STRING(/*q*/"uen"); break;
      case M_TMENT:   SEND_STRING(/*t*/"ment"); break;
      case M_THE:     SEND_STRING(/* */"the"); break;
      case M_UPDIR:   SEND_STRING(/*.*/"./"); break;
      case M_INCLUDE: SEND_STRING(/*#*/"include"); break;
      case M_DOCSTR:
        SEND_STRING(/*"*/"\"\"\"\"\""
            SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT));
        break;
    }
  }

  return true;
}

void matrix_scan_user(void) {
  achordion_task();
  select_word_task();
  sentence_case_task();
}
