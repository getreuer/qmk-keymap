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
//
//
// Pascal Getreuer's QMK keymap.
//
// For documentation of this keymap's features, see
// https://getreuer.info/posts/keyboards

#include QMK_KEYBOARD_H

#include "features/autocorrection.h"
#include "features/caps_word.h"
#include "features/custom_shift_keys.h"
#include "features/select_word.h"

#include "layout.h"

enum layers {
  BASE,
  QWERTY,
  SYM,
  UNI,
};

enum custom_keycodes {
  UPDIR = SAFE_RANGE,
  SCOPE,
  SELWORD,
  TMUXESC,
  SRCHSEL,
  // Unicode symbols.
  U_EN,
  U_EM,
  U_MIN,
  U_LEFT,
  U_DOWN,
  U_UP,
  U_RGHT,
  U_LR,
  U_PLMN,
  U_NE,
  U_EYEH,
  U_THUP,
  U_GRIN,
  U_COOL,
  U_WERY,
};

#define SCL_GUI  LGUI_T(KC_SCLN)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [BASE] = LAYOUT_LR(  // Base layer: Dvorak.
    KC_GRV , KC_7   , KC_8   , KC_9   , KC_0   , KC_5   ,
    KC_TAB , KC_QUOT, KC_COMM, KC_DOT , KC_P   , KC_Y   ,
    MO(SYM), KC_A   , KC_O   , KC_E   , KC_U   , KC_I   ,
    KC_LSFT, SCL_GUI, KC_Q   , KC_J   , KC_K   , KC_X   ,
    KC_LCTL, KC_PGUP, KC_PGDN, KC_DOWN, KC_UP  ,
                                                          OSL(UNI), KC_UNDS,
                                                                   KC_ESC ,
                                                 KC_DEL , KC_SPC , KC_LCTL,

                      KC_6   , KC_1   , KC_2   , KC_3   , KC_4   , KC_SLSH,
                      KC_F   , KC_G   , KC_C   , KC_R   , KC_L   , KC_MINS,
                      KC_D   , KC_H   , KC_T   , KC_N   , KC_S   , MO(SYM),
                      KC_B   , KC_M   , KC_W   , KC_V   , KC_Z   , KC_RSFT,
                               KC_LEFT, KC_RGHT, SELWORD, TMUXESC, KC_RCTL,
    KC_RGUI, KC_LALT,
    OSM(MOD_RSFT),
    KC_RCTL, KC_BSPC, KC_ENT
  ),

  [QWERTY] = LAYOUT_LR(  // Alternative base layer: QWERTY.
    _______, _______, _______, _______, _______, _______,
    _______, KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,
    _______, KC_A   , KC_S   , KC_D   , KC_F   , KC_G   ,
    _______, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   ,
    _______, _______, _______, _______, _______,
                                                          _______, _______,
                                                                   _______,
                                                 _______, _______, _______,

                      _______, _______, _______, _______, _______, KC_MINS,
                      KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_QUOT,
                      KC_H   , KC_J   , KC_K   , KC_L   , SCL_GUI, _______,
                      KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, _______,
                               _______, _______, _______, _______, _______,
    _______, _______,
    _______,
    _______, _______, _______
  ),

  [SYM] = LAYOUT_LR(  // Symbol layer.
    RESET  , KC_F7  , KC_F8  , KC_F9  , KC_F10 , KC_F5  ,
    _______, KC_QUOT, KC_LABK, KC_RABK, KC_DQUO, KC_DOT ,
    _______, KC_EXLM, KC_MINS, KC_PLUS, KC_EQL , KC_HASH,
    _______, KC_CIRC, KC_SLSH, KC_ASTR, KC_BSLS, UPDIR,
    TO(BASE), _______, _______, C(KC_HOME), C(KC_END),
                                                          _______, C(KC_A),
                                                                   C(KC_C),
                                                 SRCHSEL, _______, C(KC_V),

                      KC_F6  , KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F11 ,
                      KC_AMPR, SCOPE  , KC_LBRC, KC_RBRC, KC_PERC, KC_F12 ,
                      KC_PIPE, KC_COLN, KC_LPRN, KC_RPRN, KC_QUES, _______,
                      KC_TILD, KC_DLR , KC_LCBR, KC_RCBR, KC_AT  , _______,
                               KC_HOME, KC_END , _______, _______, TO(QWERTY),
    _______, _______,
    _______,
    _______, _______, _______
  ),

  [UNI] = LAYOUT_LR(  // Unicode layer.
    _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______,
                                                          _______, _______,
                                                                   _______,
                                                 _______, _______, _______,

                      _______, _______, _______, _______, _______, _______,
                      U_MIN  , U_EN   , U_UP   , U_EM   , U_PLMN , _______,
                      U_NE   , U_LEFT , U_DOWN , U_RGHT , U_LR   , _______,
                      U_EYEH , U_THUP , U_GRIN , U_COOL , U_WERY , _______,
                               _______, _______, _______, _______, _______,
    _______, _______,
    _______,
    _______, _______, _______
  ),
};

const uint32_t PROGMEM unicode_map[] = {};

const custom_shift_key_t custom_shift_keys[] = {
  {KC_DOT , KC_QUES}, // Shift . is ?
  {KC_COMM, KC_EXLM},
  {KC_MINS, KC_EQL },
  {KC_SLSH, KC_BSLS},
};
uint8_t NUM_CUSTOM_SHIFT_KEYS =
    sizeof(custom_shift_keys) / sizeof(custom_shift_key_t);

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  if (!process_autocorrection(keycode, record)) { return false; }
  if (!process_caps_word(keycode, record)) { return false; }
  if (!process_custom_shift_keys(keycode, record)) { return false; }
  if (!process_select_word(keycode, record, SELWORD)) { return false; }

  const uint8_t mods = get_mods();
  const uint8_t oneshot_mods = get_oneshot_mods();
  const bool shifted = (mods | oneshot_mods) & MOD_MASK_SHIFT;

  if (record->event.pressed) {
    switch (keycode) {
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

      case U_EN:  // En dash.
        send_unicode_hex_string("2013");
        return false;

      case U_EM:  // Em dash.
        send_unicode_hex_string("2014");
        return false;

      case U_MIN:  // Minus sign, or when shifted, infinity.
        send_unicode_hex_string(shifted ? "221e" : "2212");
        return false;

      case U_LEFT:  // Left arrow.
        send_unicode_hex_string(shifted ? "21d0" : "2190");
        return false;

      case U_DOWN:  // Down arrow.
        send_unicode_hex_string(shifted ? "21d3" : "2193");
        return false;

      case U_UP:  // Up arrow.
        send_unicode_hex_string(shifted ? "21d1" : "2191");
        return false;

      case U_RGHT:  // Right arrow.
        send_unicode_hex_string(shifted ? "21d2" : "2192");
        return false;

      case U_LR:  // Left-right arrow.
        send_unicode_hex_string(shifted ? "21d4" : "2194");
        return false;

      case U_PLMN:  // Plus-minus sign.
        send_unicode_hex_string(shifted ? "2213" : "b1");
        return false;

      case U_NE:  // Not-equal to.
        send_unicode_hex_string("2260");
        return false;

      case U_EYEH:  // Eye hearts emoji, or when shifted, party emoji.
        send_unicode_hex_string(shifted ? "1f973" : "1f60d");
        return false;

      case U_THUP:  // Thumbs up emoji, or when shifted, victory emoji.
        send_unicode_hex_string(shifted ? "270c" : "1f44d");
        return false;

      case U_GRIN:  // Grin emoji.
        send_unicode_hex_string("1f601");
        return false;

      case U_COOL:  // Sunglasses emoji.
        send_unicode_hex_string("1f60e");
        return false;

      case U_WERY:  // Weary emoji, or when shifted, loudly crying emoji.
        send_unicode_hex_string(shifted ? "1f62d" : "1f629");
        return false;
    }
  }

  return true;
}

