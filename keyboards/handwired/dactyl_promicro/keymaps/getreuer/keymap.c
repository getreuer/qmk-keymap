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
 */

#include QMK_KEYBOARD_H

#include "layout.h"
#include "getreuer.c"

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

                      KC_6   , KC_1   , KC_2   , KC_3   , KC_4   , USRNAME,
                      KC_B   , MAGIC  , KC_U   , KC_O   , KC_Q   , KC_SLSH,
                      KC_F   , HOME_N , HOME_E , HOME_A , HOME_I , KC_MINS,
                      KC_Z   , KC_H   , KC_COMM, KC_DOT , HOME_SC, KC_ENT ,
                               KC_LEFT, KC_RGHT, DASH   , ARROW  , HAPPY  ,
    KC_QUOT, TG(MOUSE),
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
    _______, _______, KC_SLSH, KC_ASTR, KC_CIRC, SCOPE,
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

  [MOUSE] = LAYOUT_LR(  // Mouse keys layer.
    G(KC_TAB), _______, _______, _______, _______, _______,
    _______, KC_MPLY, OM_BTN2, OM_U   , OM_BTNS, OM_DBLS,
    EXIT   , KC_WBAK, OM_L   , OM_D   , OM_R   , OM_HLDS,
    _______, KC_LGUI, OM_BTN3, OM_W_D , OM_W_U , OM_RELS,
    _______, _______, _______, C(KC_C), C(KC_V),
                                                          _______, _______,
                                                                   _______,
                                                 _______, OM_BTNS, _______,

                      _______, _______, _______, _______, _______, G(KC_TAB),
                      OM_DBLS, OM_BTNS, OM_U   , OM_BTN2, KC_MPLY, _______,
                      OM_HLDS, OM_L   , OM_D   , OM_R   , KC_WBAK, EXIT   ,
                      OM_RELS, OM_W_D , OM_W_U , OM_BTN3, KC_RGUI, _______,
                               _______, _______, C(KC_PGUP), C(KC_PGDN),  _______,
    _______, _______,
    _______,
    _______, OM_BTNS, _______
  ),

  [ADJUST] = LAYOUT_LR(  // Adjust layer.
    QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    EXIT   , XXXXXXX, XXXXXXX, DF(BASE), DF(QWERTY), XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                                          XXXXXXX, XXXXXXX,
                                                                   XXXXXXX,
                                                 XXXXXXX, XXXXXXX, _______,

                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,
                      XXXXXXX, KC_BRID, KC_BRIU, XXXXXXX, XXXXXXX, XXXXXXX ,
                      XXXXXXX, KC_MPRV, KC_MNXT, KC_MPLY, XXXXXXX, EXIT  ,
                      XXXXXXX, KC_VOLU, KC_VOLD, KC_MUTE, XXXXXXX, XXXXXXX,
                               XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX,
    XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX
  ),
};
// clang-format on

