// Copyright 2024-2025 Google LLC
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
 * @brief Pascal's keymap for the ZSA Voyager.
 *
 * This file makes definitions specific to the Voyager. See getreuer.c for the
 * keymap itself.
 */

#include QMK_KEYBOARD_H

#include "voyager.h"
#include "layout.h"
#include "getreuer.c"

// (See getreuer.c for keymaps definition.)

#ifdef CHORDAL_HOLD
// Handedness for Chordal Hold (https://github.com/qmk/qmk_firmware/pull/24560)
const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM =
  LAYOUT_LR(
  '*'    , '*'    , '*'    , '*'    , '*'    , '*'    ,
  '*'    , 'L'    , 'L'    , 'L'    , 'L'    , 'L'    ,
  '*'    , 'L'    , 'L'    , 'L'    , 'L'    , 'L'    ,
  '*'    , 'L'    , 'L'    , 'L'    , 'L'    , 'L'    ,
                                               'L'    , 'L'    ,

                    '*'    , '*'    , '*'    , '*'    , '*'    , '*'    ,
                    'R'    , 'R'    , 'R'    , 'R'    , 'R'    , '*'    ,
                    'R'    , 'R'    , 'R'    , 'R'    , 'R'    , '*'    ,
                    'R'    , 'R'    , 'R'    , 'R'    , 'R'    , '*'    ,
           'R'    , 'R'
);
#endif  // CHORDAL_HOLD

