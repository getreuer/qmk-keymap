// Copyright 2024 Google LLC
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
 * @file keymap.c
 * @brief Pascal's keymap for the ZSA Moonlander.
 *
 * This file defines what keycode is performed by each key position. See also
 * getreuer.c for definitions of macros, etc. used in my keymap.
 */

#include QMK_KEYBOARD_H

#include "moonlander.h"
#include "layout.h"
#include "getreuer.c"

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [BASE] = LAYOUT_LR(  // Base layer: Magic Sturdy.
    OM_BTNS, G(KC_TAB), SELLINE, KC_DOWN, KC_UP  , KC_DEL, RGB_TOG,
    KC_TAB , KC_V   , KC_M   , KC_L   , KC_C   , KC_P   , TG(MOUSE),
    KC_BSPC, HOME_S , HOME_T , HOME_R , HOME_D , KC_Y   , KC_BSLS,
    WIN_COL, HOME_X , KC_K   , KC_J   , NUM_G  , KC_W   ,
    KC_LCTL, C(KC_PGUP), C(KC_PGDN), KC_PGUP, KC_PGDN,
                                                                   NAV_UND,
                                                 NAV_UND, KC_SPC , KC_BTN1,

             KC_VOLU, KC_HOME, KC_LEFT, KC_RGHT, KC_END , KC_MUTE, KC_MPLY,
             KC_VOLD, KC_B   , MAGIC  , KC_U   , KC_O   , KC_Q   , KC_SLSH,
             KC_BSLS, KC_F   , HOME_N , HOME_E , HOME_A , HOME_I , KC_QUOT,
                      KC_Z   , KC_H   , KC_COMM, KC_DOT , HOME_SC, KC_ENT ,
                               C(KC_LEFT), C(KC_RGHT), DASH   , ARROW  , USRNAME,
    MOU_ESC,
    MO(WIN), QK_REP , MOU_ESC
  ),

  [SYM] = LAYOUT_LR(  // Symbol layer.
    _______, KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  , _______,
    TMUXESC, _______, KC_LABK, KC_RABK, KC_BSLS, KC_GRV , _______,
    _______, KC_EXLM, KC_MINS, KC_PLUS, KC_EQL , KC_HASH, _______,
    _______, _______, KC_SLSH, KC_ASTR, KC_CIRC, USRNAME,
    _______, _______, _______, C(KC_END), C(KC_HOME),
                                                                   _______,
                                                 KC_UNDS, _______, _______,

             _______, KC_F6  , KC_F7  , KC_F8  , KC_F9  , KC_F10 , KC_F11 ,
             _______, KC_AMPR, ARROW  , KC_LBRC, KC_RBRC, _______, KC_F12 ,
             _______, KC_PIPE, KC_COLN, KC_LPRN, KC_RPRN, KC_PERC, _______,
                      KC_TILD, KC_DLR , KC_LCBR, KC_RCBR, _______, _______,
                               _______, _______, _______, _______, _______,
    _______,
    _______, _______, _______
  ),

  [MOUSE] = LAYOUT_LR(  // Mouse keys layer.
    _______, _______, _______, _______, _______, _______, _______,
    _______, G(KC_TAB), OM_BTN2, OM_U   , OM_BTNS, OM_DBLS, _______,
    TO(BASE), KC_WBAK, OM_L   , OM_D   , OM_R   , OM_HLDS, _______,
    _______, KC_LGUI, OM_BTN3, OM_W_D , OM_W_U , OM_RELS,
    _______, _______, _______, _______, _______,
                                                                   _______,
                                                 LLOCK  , OM_BTNS, _______,

             _______, _______, _______, _______, _______, _______, _______,
             _______, OM_DBLS, OM_BTNS, OM_U   , OM_BTN2, G(KC_TAB), _______,
             _______, OM_HLDS, OM_L   , OM_D   , OM_R   , KC_WBAK, TO(BASE),
                      OM_RELS, OM_W_D , OM_W_U , OM_BTN3, KC_RGUI, _______,
                               _______, _______, _______, _______, _______,
    _______,
    _______, OM_BTNS, _______
  ),

  [NAV] = LAYOUT_LR(  // Nav layer.
    _______, _______, _______, _______, _______, _______, _______,
    _______, C(KC_Z), C(KC_V), C(KC_A), C(KC_C), C(KC_X), _______,
    _______, XXXXXXX, KC_LALT, KC_LSFT, KC_LCTL, XXXXXXX, _______,
    _______, KC_LGUI, C(KC_PGUP), C(KC_PGDN), XXXXXXX, XXXXXXX,
    _______, _______, _______, _______, _______,
                                                                   _______,
                                                 _______, _______, _______,

             _______, _______, _______, _______, _______, _______, _______,
             _______, KC_PGUP, KC_HOME, KC_UP  , KC_END , SELLINE, SRCHSEL,
             _______, KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, KC_DEL , _______,
                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_APP , _______,
                               _______, _______, _______, _______, _______,
    _______,
    _______, QK_REP , LLOCK
  ),

  [NUM] = LAYOUT_LR(  // Number layer.
    _______, _______, _______, _______, _______, _______, _______,
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
    _______, XXXXXXX, KC_LALT, KC_LSFT, KC_LCTL, XXXXXXX, _______,
    _______, KC_LGUI, XXXXXXX, KC_LCTL, XXXXXXX, XXXXXXX,
    _______, _______, _______, _______, _______,
                                                                    _______,
                                                 TO(BASE), _______, _______,

             _______, _______, _______, _______, _______, _______, _______,
             _______, KC_TAB , KC_8   , KC_9   , KC_4   , KC_PLUS, KC_SLSH,
             _______, KC_COLN, KC_1   , KC_2   , KC_3   , KC_MINS, KC_ASTR,
                      KC_COMM, KC_7   , KC_6   , KC_5   , KC_DOT , _______,
                               _______, _______, _______, _______, _______,
    _______,
    _______, KC_0   , LLOCK
  ),

  [WIN] = LAYOUT_LR(  // Window management layer.
    RGB_TOG, RGB_DEF, RGB_MOD, RGB_HUI, RGB_SAI, RGB_VAI, _______,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
    XXXXXXX, XXXXXXX, KC_LALT, KC_LSFT, KC_LCTL, XXXXXXX, _______,
    XXXXXXX, KC_LGUI, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    _______, _______, _______, _______, _______,
                                                                      _______,
                                                 TO(BASE), G(KC_SPC), _______,

             _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,
             _______, G(KC_TAB), G(KC_8), G(KC_9), G(KC_4), XXXXXXX, XXXXXXX,
             _______, G(S(KC_LEFT)), G(KC_1), G(KC_2), G(KC_3), G(S(KC_RGHT)), XXXXXXX,
                      XXXXXXX, G(KC_7), G(KC_6), G(KC_5), KC_VOLD , KC_VOLU,
                               _______, _______, _______, _______, _______,
    _______,
    _______, QK_REP , LLOCK
  ),
};

