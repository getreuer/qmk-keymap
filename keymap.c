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
  M_ION,
  M_NION,
  M_MENT,
  M_QUEN,
  M_TMENT,
  M_THE,
  M_UPDIR,
  M_INCLUDE,
  M_DOCSTR,
  M_MKGRVS,
  M_EQEQ,
};

// This keymap uses Ikcelaks' Magic Sturdy layout for the base layer (see
// https://github.com/Ikcelaks/keyboard_layouts). I've also made some twists of
// my own. The "magic" is a key whose function depends on the last pressed key,
// implemented using the Alternate Repeat Key. This key is used to remove the
// top SFBs and type common n-grams.
//
// The following describes the functionality, where * is the magic key and @ the
// repeat key. For example, tapping A and then the magic key types "ao".
//
// SFB removal and common n-grams:
//
//     A * -> AO     L * -> LK      S * -> SK
//     C * -> CY     M * -> MENT    T * -> TMENT
//     D * -> DY     O * -> OA      U * -> UE
//     E * -> EU     P * -> PY      Y * -> YP
//     G * -> GY     Q * -> QUEN    spc * -> THE
//     I * -> ION    R * -> RL
//
// When the magic key types a letter, following it with the repeat key produces
// "n". This is useful to type certain patterns without SFBs.
//
//     A * @ -> AON             (like "kaon")
//     D * @ -> DYN             (like "dynamic")
//     E * @ -> EUN             (like "reunite")
//     O * @ -> OAN             (like "loan")
//
// Other patterns:
//
//     spc * @ -> THEN
//     I * @ -> IONS            (like "nations")
//     M * @ -> MENTS           (like "moments")
//     Q * @ -> QUENC           (like "frequency")
//     T * @ -> TMENTS          (lite "adjustments")
//     = *   -> ===             (JS code)
//     ! *   -> !==             (JS code)
//     " *   -> """<cursor>"""  (Python code)
//     ` *   -> ```<cursor>```  (Markdown code)
//     # *   -> #include        (C code)
//     < -   -> <-              (Haskell code)
//     . *   -> ../             (shell)
//     . * @ -> ../../
#define MAGIC QK_AREP  // The "magic" key is Alternate Repeat.

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

// I have F13 bound to toggle Redshift (http://jonls.dk/redshift/).
#define REDSHFT KC_F13

// clang-format off
const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] PROGMEM = {
  [BASE] = LAYOUT_LR(  // Base layer: Magic Sturdy.
    KC_GRV , KC_7   , KC_8   , KC_9   , KC_0   , KC_5   ,
    KC_TAB , KC_V   , KC_M   , KC_L   , KC_C   , KC_P   ,
    KC_BSPC, HOME_S , HOME_T , HOME_R , HOME_D , KC_Y   ,
    KC_LSFT, HOME_X , KC_K   , KC_J   , KC_G   , KC_W   ,
    KC_LCTL, KC_PGUP, KC_PGDN, KC_DOWN, KC_UP  ,
                                                          MO(SYM), KC_UNDS,
                                                                   KC_BSLS,
                                                 KC_DEL , KC_SPC , KC_BTN1,

                      KC_6   , KC_1   , KC_2   , KC_3   , KC_4   , KC_BSLS,
                      KC_B   , MAGIC  , KC_U   , KC_O   , KC_Q   , KC_SLSH,
                      KC_F   , HOME_N , HOME_E , HOME_A , HOME_I , KC_MINS,
                      KC_Z   , KC_H   , KC_COMM, KC_DOT , HOME_SC, KC_ENT ,
                               KC_LEFT, KC_RGHT, DASH   , ARROW  , THMBUP ,
    KC_QUOT, TMUXESC,
    SCOPE  ,
    SELWORD, QK_REP , KC_ESC
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
                      KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , _______,
                      KC_H   , QHOME_J, QHOME_K, QHOME_L, QHOME_SC, _______,
                      KC_N   , KC_M   , KC_COMM, KC_DOT , QHOME_SL, _______,
                               _______, _______, _______, _______, _______,
    _______, _______,
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
    TMUXESC, _______,
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

#ifdef AUTOCORRECT_ENABLE
bool apply_autocorrect(uint8_t backspaces, const char* str,
                       char* typo, char* correct) {
  for (uint8_t i = 0; i < backspaces; ++i) {
    tap_code(KC_BSPC);
  }
  send_string_with_delay_P(str, TAP_CODE_DELAY);
  return false;
}
#endif  // AUTOCORRECT_ENABLE

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
    // These magic patterns work with Caps Word.
    case M_ION:
    case M_MENT:
    case M_QUEN:
    case M_TMENT:
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
    case HOME_X:  // X + D and X + G.
      if (other_keycode == HOME_D || other_keycode == KC_G) {
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
  return 800;  // Use a timeout of 800 ms.
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

// clang-format off
uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {
  if ((mods & ~MOD_MASK_SHIFT) == 0) {
    switch (keycode) {
      // For navigating next/previous search results in Vim:
      // N -> Shift + N, Shift + N -> N.
      case HOME_N:
        if ((mods & MOD_MASK_SHIFT) == 0) {
          return S(KC_N);
        }
        // Fall through intended.
      case KC_N: return KC_N;

      // Behavior for Magic Sturdy's "magic" key.
      case HOME_A: return KC_O;       // A -> O
      case KC_C: return KC_Y;         // C -> Y
      case HOME_D: return KC_Y;       // D -> Y
      case HOME_E: return KC_U;       // E -> U
      case KC_G: return KC_Y;         // G -> Y
      case HOME_I: return M_ION;      // I -> ON
      case KC_L: return KC_K;         // L -> K
      case KC_M: return M_MENT;       // M -> ENT
      case KC_O: return KC_A;         // O -> A
      case KC_P: return KC_Y;         // P -> Y
      case KC_Q: return M_QUEN;       // Q -> UEN
      case HOME_R: return KC_L;       // R -> L
      case HOME_S: return KC_K;       // S -> K
      case HOME_T: return M_TMENT;    // T -> TMENT
      case KC_U: return KC_E;         // U -> E
      case KC_Y: return KC_P;         // Y -> P
      case KC_SPC: return M_THE;      // spc -> THE

      case KC_DOT: return M_UPDIR;    // . -> ./
      case KC_COMM:                   // ! -> ==
        if ((mods & MOD_MASK_SHIFT) == 0) {
          return KC_NO;
        }
        // Fall through intended.
      case KC_EQL: return M_EQEQ;     // = -> ==
      case KC_HASH: return M_INCLUDE; // # -> include
      case KC_QUOT:
        if ((mods & MOD_MASK_SHIFT) != 0) {
          return M_DOCSTR;  // " -> ""<cursor>"""
        }
        return KC_NO;
      case KC_GRV:  // ` -> ``<cursor>``` (for Markdown code)
        return M_MKGRVS;
      case KC_LABK: return KC_MINS;   // < -> - (for Haskell)
    }
  } else if ((mods & MOD_MASK_CTRL)) {
    switch (keycode) {
      case HOME_A:  // Ctrl+A -> Ctrl+C
        return C(KC_C);
    }
  }
  return KC_TRNS;
}
// clang-format on

bool remember_last_key_user(uint16_t keycode, keyrecord_t* record,
                            uint8_t* remembered_mods) {
  // Unpack tapping keycode for tap-hold keys.
  switch (keycode) {
#ifndef NO_ACTION_TAPPING
    case QK_MOD_TAP ... QK_MOD_TAP_MAX:
      keycode = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
      break;
#ifndef NO_ACTION_LAYER
    case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
      keycode = QK_LAYER_TAP_GET_TAP_KEYCODE(keycode);
      break;
#endif  // NO_ACTION_LAYER
#endif  // NO_ACTION_TAPPING
  }

  // Forget Shift on letters when Shift or AltGr are the only mods.
  // Exceptionally, I want Shift remembered on N and Z for "NN" and "ZZ" in Vim.
  switch (keycode) {
    case KC_A ... KC_M:
    case KC_O ... KC_Y:
      if ((*remembered_mods & ~(MOD_MASK_SHIFT | MOD_BIT(KC_RALT))) == 0) {
        *remembered_mods &= ~MOD_MASK_SHIFT;
      }
      break;
  }

  return true;
}

// An enhanced version of SEND_STRING: if Caps Word is active, the Shift key is
// held while sending the string. Additionally, the last key is set such that if
// the Repeat Key is pressed next, it produces `repeat_keycode`.
#define MAGIC_STRING(str, repeat_keycode) \
  magic_send_string_P(PSTR(str), (repeat_keycode))
static void magic_send_string_P(const char* str, uint16_t repeat_keycode) {
  uint8_t saved_mods = 0;
  // If Caps Word is on, save the mods and hold Shift.
  if (is_caps_word_on()) {
    saved_mods = get_mods();
    register_mods(MOD_BIT(KC_LSFT));
  }

  send_string_with_delay_P(str, TAP_CODE_DELAY);  // Send the string.
  set_last_keycode(repeat_keycode);

  // If Caps Word is on, restore the mods.
  if (is_caps_word_on()) {
    set_mods(saved_mods);
  }
}

// clang-format off
bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  if (!process_achordion(keycode, record)) { return false; }
  if (!process_sentence_case(keycode, record)) { return false; }
  if (!process_select_word(keycode, record, SELWORD)) { return false; }
  if (!process_custom_shift_keys(keycode, record)) { return false; }

  const uint8_t mods = get_mods();
  const bool shifted = (mods | get_weak_mods()
#ifndef NO_ACTION_ONESHOT
                        | get_oneshot_mods()
#endif  // NO_ACTION_ONESHOT
                       ) & MOD_MASK_SHIFT;

  // If alt repeating a key A-Z with no mods other than Shift, set the last key
  // to KC_N. Above, alternate repeat of KC_N is defined to be again KC_N. This
  // way, either tapping alt repeat and then repeat (or double tapping alt
  // repeat) is useful to type certain patterns without SFBs:
  //
  //   O <altrep> <rep> -> OAN (as in "loan")
  //   D <altrep> <rep> -> DYN (as in "dynamic")
  if (get_repeat_key_count() < 0 &&
      KC_A <= keycode && keycode <= KC_Z && keycode != KC_N &&
      (get_last_mods() & ~MOD_MASK_SHIFT) == 0) {
    set_last_keycode(KC_N);
    set_last_mods(0);
  }

  if (record->event.pressed) {
    switch (keycode) {
      case EXIT:
        layer_off(ADJUST);
        return false;

      case SCOPE:
        SEND_STRING_DELAY("::", TAP_CODE_DELAY);
        return false;

      case UPDIR:
        SEND_STRING_DELAY("../", TAP_CODE_DELAY);
        return false;

      case TMUXESC:  // Enter copy mode in Tmux.
        SEND_STRING(SS_LCTL("a") SS_TAP(X_ESC));
        return false;

      case SRCHSEL:  // Searches the current selection in a new tab.
        // Mac users, change LCTL to LGUI.
        SEND_STRING_DELAY(
            SS_LCTL("ct") SS_DELAY(100) SS_LCTL("v") SS_TAP(X_ENTER),
            TAP_CODE_DELAY);
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
      case M_ION:     MAGIC_STRING(/*i*/"on", KC_S); break;
      case M_MENT:    MAGIC_STRING(/*m*/"ent", KC_S); break;
      case M_QUEN:    MAGIC_STRING(/*q*/"uen", KC_C); break;
      case M_TMENT:   MAGIC_STRING(/*t*/"ment", KC_S); break;
      case M_THE:     MAGIC_STRING(/* */"the", KC_N); break;
      case M_UPDIR:   MAGIC_STRING(/*.*/"./", UPDIR); break;
      case M_INCLUDE: SEND_STRING_DELAY(/*#*/"include ", TAP_CODE_DELAY); break;
      case M_EQEQ:    SEND_STRING_DELAY(/*=*/"==", TAP_CODE_DELAY); break;
      case M_DOCSTR:
        SEND_STRING_DELAY(/*"*/"\"\"\"\"\""
            SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT), TAP_CODE_DELAY);
        break;
      case M_MKGRVS:
        SEND_STRING_DELAY(/*`*/"``\n\n```" SS_TAP(X_UP), TAP_CODE_DELAY);
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
