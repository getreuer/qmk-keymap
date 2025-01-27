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

// clang-format off

// Macro for conveniently defining Moonlander layouts.
#define LAYOUT_LR_MOONLANDER(                              \
    k00, k01, k02, k03, k04, k05, k06,                     \
    k10, k11, k12, k13, k14, k15, k16,                     \
    k20, k21, k22, k23, k24, k25, k26,                     \
    k30, k31, k32, k33, k34, k35,                          \
    k40, k41, k42, k43, k44,                               \
                                  k53,                     \
                        k50, k51, k52,                     \
                                                           \
                        k60, k61, k62, k63, k64, k65, k66, \
                        k70, k71, k72, k73, k74, k75, k76, \
                        k80, k81, k82, k83, k84, k85, k86, \
                             k91, k92, k93, k94, k95, k96, \
                                  ka2, ka3, ka4, ka5, ka6, \
                        kb3,                               \
                        kb4, kb5, kb6)                     \
{                                                          \
    { k00, k01, k02, k03, k04, k05, k06 },                 \
    { k10, k11, k12, k13, k14, k15, k16 },                 \
    { k20, k21, k22, k23, k24, k25, k26 },                 \
    { k30, k31, k32, k33, k34, k35, KC_NO },               \
    { k40, k41, k42, k43, k44, KC_NO, KC_NO },             \
    { k50, k51, k52, k53, KC_NO, KC_NO, KC_NO },           \
    { k60, k61, k62, k63, k64, k65, k66 },                 \
    { k70, k71, k72, k73, k74, k75, k76 },                 \
    { k80, k81, k82, k83, k84, k85, k86 },                 \
    { KC_NO,k91, k92, k93, k94, k95, k96 },                \
    { KC_NO, KC_NO, ka2, ka3, ka4, ka5, ka6 },             \
    { KC_NO, KC_NO, KC_NO, kb3, kb4, kb5, kb6 }            \
}

// Adapt Voyager layout to the Moonlander.
#define LAYOUT_LR(                                                          \
    k00, k01, k02, k03, k04, k05,                                           \
    k10, k11, k12, k13, k14, k15,                                           \
    k20, k21, k22, k23, k24, k25,                                           \
    k30, k31, k32, k33, k34, k35,                                           \
                             k40, k41,                                      \
                                                                            \
                         k50, k51, k52, k53, k54, k55,                      \
                         k60, k61, k62, k63, k64, k65,                      \
                         k70, k71, k72, k73, k74, k75,                      \
                         k80, k81, k82, k83, k84, k85,                      \
                    k90, k91)                                               \
    LAYOUT_LR_MOONLANDER(                                                   \
    k00    , k01    , k02    , k03    , k04    , k05    , KC_BTN1,          \
    k10    , k11    , k12    , k13    , k14    , k15    , KC_PGUP,          \
    k20    , k21    , k22    , k23    , k24    , k25    , KC_PGDN,          \
    k30    , k31    , k32    , k33    , k34    , k35    ,                   \
    XXXXXXX, XXXXXXX, XXXXXXX, KC_DOWN, KC_UP  ,                            \
                                                                   XXXXXXX, \
                                                 k40    ,     k41, KC_BTN1, \
                                                                            \
             RGBDEF1, k50    , k51    , k52    , k53    , k54    , k55    , \
             RGBHRND, k60    , k61    , k62    , k63    , k64    , k65    , \
             RGBBRI , k70    , k71    , k72    , k73    , k74    , k75    , \
                      k80    , k81    , k82    , k83    , k84    , k85    , \
                               KC_LEFT, KC_RGHT, XXXXXXX, XXXXXXX, XXXXXXX, \
    XXXXXXX,                                                                \
    XXXXXXX, k90    , k91)

// Matrix positions of the left home row keys.
#define LEFT_HOME_ROW 2
#define LEFT_HOME_PINKY_COL 1
#define LEFT_HOME_RING_COL 2
#define LEFT_HOME_MIDDLE_COL 3
#define LEFT_HOME_INDEX_COL 4

// Wrapper macros to use the Moonlander's LEDs.
#define STATUS_LED_1(status)  \
  do {                        \
    ML_LED_1((status));       \
    ML_LED_4((status));       \
  } while (false)
#define STATUS_LED_2(status)  \
  do {                        \
    ML_LED_2((status));       \
    ML_LED_5((status));       \
  } while (false)
#define STATUS_LED_3(status)  \
  do {                        \
    ML_LED_3((status));       \
    ML_LED_6((status));       \
  } while (false)

