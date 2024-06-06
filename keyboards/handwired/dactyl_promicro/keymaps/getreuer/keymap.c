// Copyright 2021-2024 Google LLC
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
 * @brief Pascal's keymap for the Dactyl Ergodox.
 *
 * This file defines what keycode is performed by each key position. See also
 * getreuer.c for definitions of macros, etc. used in my keymap.
 */

#include QMK_KEYBOARD_H

#include "layout.h"
#include "getreuer.c"

// clang-format off
const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] PROGMEM = {
  [BASE] = LAYOUT_LR(  // Base layer: Magic Sturdy.
    KC_GRV , G(KC_TAB), SELLINE, KC_DOWN, KC_UP  , KC_BTN1,
    KC_TAB , KC_V   , KC_M   , KC_L   , KC_C   , KC_P   ,
    KC_BSPC, HOME_S , HOME_T , HOME_R , HOME_D , KC_Y   ,
    WIN_COL, HOME_X , KC_K   , KC_J   , NUM_G  , KC_W   ,
    KC_LCTL, KC_PGUP, KC_PGDN, KC_DOWN, KC_UP  ,
                                                          MO(FUN), KC_UNDS,
                                                                   KC_BSLS,
                                                 KC_UNDS, KC_SPC , KC_BTN1,

                      KC_HOME, KC_LEFT, KC_RGHT , KC_END , KC_DEL , KC_MPLY,
                      KC_B   , MAGIC  , KC_U   , KC_O   , KC_Q   , KC_SLSH,
                      KC_F   , HOME_N , HOME_E , HOME_A , HOME_I , KC_QUOT,
                      KC_Z   , KC_H   , KC_COMM, KC_DOT , HOME_SC, KC_ENT ,
                               KC_LEFT, KC_RGHT, DASH   , ARROW  , KC_RCTL,
    KC_MINS, MO(SYM),
    KC_MUTE,
    KC_DEL , QK_REP , KC_ESC
  ),

  [SYM] = LAYOUT_LR(  // Symbol layer.
    _______, C(KC_Z), C(KC_V), C(KC_A), C(KC_C), C(KC_X),
    TMUXESC, MO(FUN), KC_LABK, KC_RABK, KC_BSLS, KC_GRV ,
    _______, KC_EXLM, KC_MINS, KC_PLUS, KC_EQL , KC_HASH,
    _______, _______, KC_SLSH, KC_ASTR, KC_CIRC, USRNAME,
    _______, _______, _______, C(KC_END), C(KC_HOME),
                                                          _______, _______,
                                                                   _______,
                                                 _______, _______, _______,

                      C(KC_PGUP), KC_PGDN, KC_PGUP, C(KC_PGDN),  _______, KC_MUTE,
                      KC_AMPR, ARROW  , KC_LBRC, KC_RBRC, _______, KC_F12 ,
                      KC_PIPE, KC_COLN, KC_LPRN, KC_RPRN, KC_PERC, _______,
                      KC_TILD, KC_DLR , KC_LCBR, KC_RCBR, _______, _______,
                               KC_HOME, KC_END , SELLINE, SRCHSEL, _______,
    _______, _______,
    _______,
    _______, _______, _______
  ),

  [NUM] = LAYOUT_LR(  // Number layer.
    _______, _______, _______, _______, _______, _______,
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    _______, XXXXXXX, KC_LALT, KC_LSFT, KC_LCTL, XXXXXXX,
    _______, KC_LGUI, XXXXXXX, KC_LCTL, XXXXXXX, XXXXXXX,
    _______, _______, _______, _______, _______,
                                                          _______, _______,
                                                                   _______,
                                                 TO(BASE), _______, _______,

                      _______, _______, _______, _______, _______, _______,
                      KC_TAB , KC_8   , KC_9   , KC_4   , KC_PLUS, KC_SLSH,
                      KC_COLN, KC_1   , KC_2   , KC_3   , KC_MINS, KC_ASTR,
                      KC_COMM, KC_7   , KC_6   , KC_5   , KC_DOT , _______,
                               _______, _______, _______, _______, _______,
    _______, _______,
    _______,
    _______, KC_0   , LLOCK
  ),

  [WIN] = LAYOUT_LR(  // Window management layer.
    RGB_TOG, RGB_DEF, RGB_MOD, RGB_HUI, RGB_SAI, RGB_VAI,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, KC_LALT, KC_LSFT, KC_LCTL, XXXXXXX,
    XXXXXXX, KC_LGUI, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    _______, _______, _______, _______, _______,
                                                          _______, _______,
                                                                   _______,
                                                 TO(BASE), G(KC_SPC), _______,

                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,
                      G(KC_TAB), G(KC_8), G(KC_9), G(KC_4), XXXXXXX, XXXXXXX,
                      G(S(KC_LEFT)), G(KC_1), G(KC_2), G(KC_3), G(S(KC_RGHT)), XXXXXXX,
                      XXXXXXX, G(KC_7), G(KC_6), G(KC_5), KC_VOLD , KC_VOLU,
                               _______, _______, _______, _______, _______,
    _______, _______,
    _______,
    _______, QK_REP , LLOCK
  ),

  [FUN] = LAYOUT_LR(  // Funky fun layer.
    _______, _______, _______, _______, _______, _______,
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    _______, XXXXXXX, KC_LALT, KC_LSFT, KC_LCTL, XXXXXXX,
    _______, KC_LGUI, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    _______, _______, _______, _______, _______,
                                                          _______, _______,
                                                                   _______,
                                                 TO(BASE), _______, _______,


                      _______, _______, _______, _______, _______, QK_BOOT,
                      XXXXXXX, KC_F8  , KC_F9  , KC_F4  , KC_F10 , XXXXXXX,
                      XXXXXXX, KC_F1  , KC_F2  , KC_F3  , KC_F11 , XXXXXXX,
                      XXXXXXX, KC_F7  , KC_F6  , KC_F5  , KC_F12 , _______,
                               _______, _______, _______, _______, _______,
    _______, _______,
    _______,
    _______, XXXXXXX, LLOCK
  ),
};

