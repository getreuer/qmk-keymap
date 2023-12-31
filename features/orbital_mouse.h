// Copyright 2023 Google LLC
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
 * @file orbital_mouse.h
 * @brief Orbital Mouse - a polar approach to mouse key control.
 *
 * Orbital Mouse is a userspace library that replaces QMK Mouse Keys. The
 * pointer moves according to a heading direction. Two keys move forward and
 * backward along that direction while another two keys steer.
 *
 * To add this library to your keymap, call process_orbital_mouse() from
 * process_record_user() and orbital_mouse_task() from matrix_scan_user() in
 * keymap.c as described below, and in rules.mk, add
 *
 *     SRC += features/orbital_mouse.c
 *     MOUSE_ENABLE = yes
 *
 * Then use the "OM_*" Orbital Mouse keycodes in your layout. A suggested
 * right-handed layout for Orbital Mouse control is
 *
 *     OM_DBLS, OM_BTNS, OM_U   , OM_BTN2, OM_SEL1,
 *     OM_HLDS, OM_L   , OM_D   , OM_R   , OM_SEL2,
 *     OM_RELS, OM_W_D , OM_W_U , OM_BTN3, OM_SEL3,
 *
 * For full documentation, see
 * <https://getreuer.info/posts/keyboards/orbital-mouse>
 */

#pragma once

#include "quantum.h"

/**
 * Handler function for Orbital Mouse.
 *
 * In your keymap.c, handle Orbital Mouse from your `process_record_user()`
 * function by calling this function as:
 *
 *     #include "features/orbital_mouse.h"
 *
 *     bool process_record_user(uint16_t keycode, keyrecord_t* record) {
 *       if (!process_orbital_mouse(keycode, record)) { return false; }
 *
 *       // Your macros ...
 *       return true;
 *     }
 */
bool process_orbital_mouse(uint16_t keycode, keyrecord_t* record);

/**
 * Matrix task function for Orbital Mouse.
 *
 * Call this function from your `matrix_scan_user()` function in keymap.c:
 *
 *     void matrix_scan_user(void) {
 *       orbital_mouse_task();
 *
 *       // Other tasks ...
 *     }
 */
void orbital_mouse_task(void);

/**
 * Sets the pointer movement speed curve at run time.
 *
 * This function enables dynamically switching between multiple speed curves.
 *
 * @param speed_curve Pointer to an array of size 16. If NULL, the speed curve
 *                    defined by ORBITAL_MOUSE_SPEED_CURVE is set.
 */
void set_orbital_mouse_speed_curve(const uint8_t* speed_curve);

/**
 * Gets the heading direction as a value in 0-63.
 *
 * Value 0 is up, and values increase in the counter-clockwise direction.
 *
 *      0 = up            32 = down
 *      8 = up-left       40 = down-right
 *     16 = left          48 = right
 *     24 = down-left     56 = up-right
 */
uint8_t get_orbital_mouse_angle(void);

/** Sets the heading direction. */
void set_orbital_mouse_angle(uint8_t angle);

// The following defines the keycodes for Orbital Mouse. 28 keycodes are needed.
// While keycodes for userspace features are conventionally allocated in the
// user-defined keycode range, that range is limited (32 keycodes). It would be
// unreasonable to allocate Orbital Mouse's keys there. Being a Mouse Keys
// replacement, we repurpose the Mouse Keys keycodes (`KC_MS_U`, `KC_BTN1`,
// etc.) for the analogous functions in Orbital Mouse. We also repurpose the
// block of keycodes `UC(0x41)` to `UC(0x49)`. These keycode represent Unicode
// input of ASCII characters, which seems unlikely to be missed.
enum {
  /** Move forward. */
  OM_U = KC_MS_U,
  /** Move backward. */
  OM_D = KC_MS_D,
  /** Steer left (counter-clockwise). */
  OM_L = KC_MS_L,
  /** Steer right (clockwise). */
  OM_R = KC_MS_R,
  /** Mouse wheel up. */
  OM_W_U = KC_WH_U,
  /** Mouse wheel down. */
  OM_W_D = KC_WH_D,
  /** Mouse wheel left. */
  OM_W_L = KC_WH_L,
  /** Mouse wheel right. */
  OM_W_R = KC_WH_R,
  /** Press mouse button 1. */
  OM_BTN1 = KC_BTN1,
  /** Press mouse button 2. */
  OM_BTN2 = KC_BTN2,
  /** Press mouse button 3. */
  OM_BTN3 = KC_BTN3,
  /** Press mouse button 4. */
  OM_BTN4 = KC_BTN4,
  /** Press mouse button 5. */
  OM_BTN5 = KC_BTN5,
  /** Press mouse button 6. */
  OM_BTN6 = KC_BTN6,
  /** Press mouse button 7. */
  OM_BTN7 = KC_BTN7,
  /** Press mouse button 8. */
  OM_BTN8 = KC_BTN8,
  /** Press the selected mouse button. */
  OM_BTNS = KC_ACL0,
  /** Hold the selected mouse button. */
  OM_HLDS = KC_ACL1,
  /** Release the selected mouse button. */
  OM_RELS = KC_ACL2,
  /** Double click the selected mouse button. */
  OM_DBLS = UC(0x41),
  /** Select mouse button 1. */
  OM_SEL1 = OM_DBLS + 1,
  /** Select mouse button 2. */
  OM_SEL2 = OM_DBLS + 2,
  /** Select mouse button 3. */
  OM_SEL3 = OM_DBLS + 3,
  /** Select mouse button 4. */
  OM_SEL4 = OM_DBLS + 4,
  /** Select mouse button 5. */
  OM_SEL5 = OM_DBLS + 5,
  /** Select mouse button 6. */
  OM_SEL6 = OM_DBLS + 6,
  /** Select mouse button 7. */
  OM_SEL7 = OM_DBLS + 7,
  /** Select mouse button 8. */
  OM_SEL8 = OM_DBLS + 8,
};

