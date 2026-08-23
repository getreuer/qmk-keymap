// Copyright 2026 Google LLC
// SPDX-License-Identifier: Apache-2.0
#include "quantum.h"

#include "achordion.h"

enum combos { base_combo, num_combo, left_base_combo, left_num_combo };

const uint16_t base_combo_keys[]      = {KC_T, KC_N, COMBO_END};
const uint16_t num_combo_keys[]       = {KC_4, KC_5, COMBO_END};
const uint16_t left_base_combo_keys[] = {KC_Q, KC_W, COMBO_END};
const uint16_t left_num_combo_keys[]  = {KC_7, KC_8, COMBO_END};

// clang-format off
combo_t key_combos[] = {
    [base_combo]      = COMBO(base_combo_keys, KC_B),
    [num_combo]       = COMBO(num_combo_keys, KC_RIGHT_BRACKET),
    [left_base_combo] = COMBO(left_base_combo_keys, KC_Z),
    [left_num_combo]  = COMBO(left_num_combo_keys, KC_MINUS),
};
// clang-format on

bool pre_process_record_user(uint16_t keycode, keyrecord_t* record) {
    if (!pre_process_achordion(keycode, record)) {
        return false;
    }
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    if (!process_achordion(keycode, record)) {
        return false;
    }
    return true;
}

void housekeeping_task_user(void) {
    achordion_task();
}
