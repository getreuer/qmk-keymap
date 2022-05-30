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
// Pascal Getreuer's QMK keymap.
//
// For documentation of this keymap's features, see
// https://getreuer.info/posts/keyboards

#include QMK_KEYBOARD_H

#include "features/achordion.h"
#include "features/autocorrection.h"
#include "features/custom_shift_keys.h"
#include "features/select_word.h"

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
  JOINLN,
  TMUXESC,
  SRCHSEL,
  DASH,
  ARROW,
  THMBUP,
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

// Home row mods for Dvorak layer.
#define HOME_A  LT(SYM, KC_A)
#define HOME_O  LALT_T(KC_O)
#define HOME_E  LSFT_T(KC_E)
#define HOME_U  LCTL_T(KC_U)
#define HOME_H  RCTL_T(KC_H)
#define HOME_T  RSFT_T(KC_T)
#define HOME_N  LALT_T(KC_N)
#define HOME_S  LT(SYM, KC_S)
#define HOME_SC LGUI_T(KC_SCLN)
#define HOME_Z  RGUI_T(KC_Z)

// Home row mods for QWERTY layer.
#define QHOME_A LT(SYM, KC_A)
#define QHOME_S LALT_T(KC_S)
#define QHOME_D LSFT_T(KC_D)
#define QHOME_F LCTL_T(KC_F)
#define QHOME_J RCTL_T(KC_J)
#define QHOME_K RSFT_T(KC_K)
#define QHOME_L LALT_T(KC_L)
#define QHOME_SC LT(SYM, KC_SCLN)
#define QHOME_Z  LGUI_T(KC_Z)
#define QHOME_SL RGUI_T(KC_SLSH)

// I have F13 bound to toggle Redshift (http://jonls.dk/redshift/).
#define REDSHFT KC_F13

const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] PROGMEM = {
  [BASE] = LAYOUT_LR(  // Base layer: Dvorak.
    KC_GRV , KC_7   , KC_8   , KC_9   , KC_0   , KC_5   ,
    KC_TAB , KC_QUOT, KC_COMM, KC_DOT , KC_P   , KC_Y   ,
    KC_ESC , HOME_A , HOME_O , HOME_E , HOME_U , KC_I   ,
    KC_LSFT, HOME_SC, KC_Q   , KC_J   , KC_K   , KC_X   ,
    KC_LCTL, KC_PGUP, KC_PGDN, KC_DOWN, KC_UP  ,
                                                          MO(SYM), KC_UNDS,
                                                                   KC_BTN2,
                                                 KC_DEL , KC_SPC , KC_BTN1,

                      KC_6   , KC_1   , KC_2   , KC_3   , KC_4   , KC_BSLS,
                      KC_F   , KC_G   , KC_C   , KC_R   , KC_L   , KC_SLSH,
                      KC_D   , HOME_H , HOME_T , HOME_N , HOME_S , KC_MINS,
                      KC_B   , KC_M   , KC_W   , KC_V   , HOME_Z , KC_RSFT,
                               KC_LEFT, KC_RGHT, DASH   , ARROW  , THMBUP ,
    KC_RGUI, TMUXESC,
    JOINLN ,
    SELWORD, KC_BSPC, KC_ENT
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
                      KC_H   , QHOME_J, QHOME_K, QHOME_L, QHOME_SC, KC_QUOT,
                      KC_N   , KC_M   , KC_COMM, KC_DOT , QHOME_SL, _______,
                               _______, _______, _______, _______, _______,
    _______, _______,
    _______,
    _______, _______, _______
  ),

  [SYM] = LAYOUT_LR(  // Symbol layer.
    _______, KC_F7  , KC_F8  , KC_F9  , KC_F10 , KC_F5  ,
    _______, KC_QUOT, KC_LABK, KC_RABK, KC_DQUO, KC_DOT ,
    _______, KC_EXLM, KC_MINS, KC_PLUS, KC_EQL , KC_HASH,
    _______, KC_CIRC, KC_SLSH, KC_ASTR, KC_BSLS, UPDIR,
    _______, _______, _______, C(KC_END), C(KC_HOME),
                                                          _______, _______,
                                                                   _______,
                                                 SRCHSEL, _______, _______,

                      KC_F6  , KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F11 ,
                      KC_AMPR, SCOPE  , KC_LBRC, KC_RBRC, KC_PERC, KC_F12 ,
                      KC_PIPE, KC_COLN, KC_LPRN, KC_RPRN, KC_QUES, TO(ADJUST),
                      KC_TILD, KC_DLR , KC_LCBR, KC_RCBR, KC_AT  , _______,
                               KC_HOME, KC_END , _______, _______, _______,
    _______, _______,
    _______,
    _______, _______, _______
  ),

  [ADJUST] = LAYOUT_LR(  // Adjust layer.
    RESET  , XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    EXIT   , XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                                          XXXXXXX, XXXXXXX,
                                                                   _______,
                                                 XXXXXXX, XXXXXXX, _______,

                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RESET  ,
                      XXXXXXX, KC_BRID, KC_BRIU, REDSHFT, XXXXXXX, XXXXXXX ,
                      XXXXXXX, DF(BASE), DF(QWERTY), XXXXXXX, XXXXXXX, EXIT  ,
                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                               XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX,
    XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX
  ),
};

const uint32_t unicode_map[] PROGMEM = {};

const custom_shift_key_t custom_shift_keys[] = {
  {KC_DOT , KC_QUES}, // Shift . is ?
  {KC_COMM, KC_EXLM},
  {KC_EQL , KC_EQL }, // Don't shift =
  {KC_SLSH, KC_SLSH}, // Don't shift /
};
uint8_t NUM_CUSTOM_SHIFT_KEYS =
    sizeof(custom_shift_keys) / sizeof(custom_shift_key_t);

enum combo_events {
  // . and C => activate Caps Word.
  CAPS_COMBO,
  // , and . => types a period, space, and sets one-shot mod for shift.
  // This combo is useful to flow between sentences.
  END_SENTENCE_COMBO,
  COMBO_LENGTH
};
uint16_t COMBO_LEN = COMBO_LENGTH;

const uint16_t caps_combo[] PROGMEM = {KC_DOT, KC_C, COMBO_END};
const uint16_t end_sentence_combo[] PROGMEM = {KC_COMM, KC_DOT, COMBO_END};

combo_t key_combos[] = {
  [CAPS_COMBO] = COMBO_ACTION(caps_combo),
  [END_SENTENCE_COMBO] = COMBO_ACTION(end_sentence_combo),
};

void process_combo_event(uint16_t combo_index, bool pressed) {
  if (pressed) {
    switch(combo_index) {
      case CAPS_COMBO:
        caps_word_on();
        break;

      case END_SENTENCE_COMBO:
        SEND_STRING(". ");
        add_oneshot_mods(MOD_BIT(KC_LSFT));  // Set one-shot mod for shift.
        break;
    }
  }
}

bool get_tapping_force_hold(uint16_t keycode, keyrecord_t* record) {
  // If you quickly hold a tap-hold key after tapping it, the tap action is
  // repeated. Key repeating is useful e.g. for Vim navigation keys, but can
  // lead to missed triggers in fast typing. Here, returning true means we
  // instead want to "force hold" and disable key repeating.
  switch (keycode) {
    // Repeating is useful for Vim navigation keys.
    case HOME_U:
    case HOME_H:
    case QHOME_J:
    case QHOME_K:
    case QHOME_L:
    // Repeating Z is useful for spamming undo.
    case HOME_Z:
    case QHOME_Z:
      return false;  // Enable key repeating.
    default:
      return true;  // Otherwise, force hold and disable key repeating.
  }
}

bool achordion_chord(uint16_t tap_hold_keycode,
                     keyrecord_t* tap_hold_record,
                     uint16_t other_keycode,
                     keyrecord_t* other_record) {
  // Exceptionally consider the following chords as holds, even though they
  // are on the same hand in Dvorak.
  switch (tap_hold_keycode) {
    case HOME_A:  // A + U.
      if (other_keycode == HOME_U) { return true; }
      break;

    case HOME_S:  // S + H and S + G.
      if (other_keycode == HOME_H || other_keycode == KC_G) { return true; }
      break;
  }

  // Also allow same-hand holds when the other key is in the rows below the
  // alphas. I need the `% (MATRIX_ROWS / 2)` because my keyboard is split.
  if (other_record->event.key.row % (MATRIX_ROWS / 2) >= 4) { return true; }

  // Otherwise, follow the opposite hands rule.
  return achordion_opposite_hands(tap_hold_record, other_record);
}

uint16_t achordion_timeout(uint16_t tap_hold_keycode) {
  switch (tap_hold_keycode) {
    case HOME_SC:
    case HOME_Z:
      return 0;  // Bypass Achordion for these keys.
  }

  return 800;  // Otherwise use a timeout of 800 ms.
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  if (!process_achordion(keycode, record)) { return false; }
  if (!process_autocorrection(keycode, record)) { return false; }
  if (!process_custom_shift_keys(keycode, record)) { return false; }
  if (!process_select_word(keycode, record, SELWORD)) { return false; }

#ifndef NO_ACTION_ONESHOT
  const bool shifted = (get_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT;
#else
  const bool shifted = get_mods() & MOD_MASK_SHIFT;
#endif  // NO_ACTION_ONESHOT

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

      case JOINLN:  // Join lines like Vim's `J` command.
        SEND_STRING( // Go to the end of the line and tap delete.
            SS_TAP(X_END) SS_TAP(X_DEL)
            // In case this has joined two words together, insert one space.
            SS_TAP(X_SPC)
            SS_LCTL(
              // Go to the beginning of the next word.
              SS_TAP(X_RGHT) SS_TAP(X_LEFT)
              // Select back to the end of the previous word. This should select
              // all spaces and tabs between the joined lines from indentation
              // or trailing whitespace, including the space inserted earlier.
              SS_LSFT(SS_TAP(X_LEFT) SS_TAP(X_RGHT)))
            // Replace the selection with a single space.
            SS_TAP(X_SPC));
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
    }
  }

  return true;
}

void matrix_scan_user(void) {
  achordion_task();
}

