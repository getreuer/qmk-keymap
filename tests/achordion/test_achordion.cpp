// Copyright 2026 Google LLC
// SPDX-License-Identifier: Apache-2.0

#include "keyboard_report_util.hpp"
#include "keycode.h"
#include "test_common.hpp"
#include "test_fixture.hpp"
#include "test_keymap_key.hpp"

using testing::_;
using testing::InSequence;

class Achordion : public TestFixture {
   protected:
    // Left hand (cols 0-4) holds the layer-tap key and a regular key.
    KeymapKey key_lt{0, 0, 0, LT(2, KC_TAB)};
    KeymapKey key_lt_l2{2, 0, 0, KC_TRANSPARENT};
    KeymapKey key_left{0, 1, 0, KC_A};
    KeymapKey key_left_l2{2, 1, 0, KC_1};
    // A mod-tap key with a non-eager mod, also on the left hand.
    KeymapKey key_mt{0, 2, 0, LALT_T(KC_C)};
    KeymapKey key_mt_l2{2, 2, 0, KC_TRANSPARENT};
    // Two more left hand keys, forming a combo on each layer.
    KeymapKey key_l0{0, 3, 0, KC_Q};
    KeymapKey key_l0_l2{2, 3, 0, KC_7};
    KeymapKey key_l1{0, 4, 0, KC_W};
    KeymapKey key_l1_l2{2, 4, 0, KC_8};
    // Right hand (cols 5-9) holds the two combo keys.
    KeymapKey key_r0{0, 5, 0, KC_T};
    KeymapKey key_r0_l2{2, 5, 0, KC_4};
    KeymapKey key_r1{0, 6, 0, KC_N};
    KeymapKey key_r1_l2{2, 6, 0, KC_5};

    void SetUp() override {
        set_keymap({key_lt, key_lt_l2, key_left, key_left_l2, key_mt, key_mt_l2, key_l0, key_l0_l2, key_l1, key_l1_l2, key_r0, key_r0_l2, key_r1, key_r1_l2});
    }
};

// Sanity check: the base layer combo works on its own.
TEST_F(Achordion, combo_on_base_layer) {
    TestDriver driver;

    EXPECT_REPORT(driver, (KC_B));
    EXPECT_EMPTY_REPORT(driver);
    tap_combo({key_r0, key_r1});
    VERIFY_AND_CLEAR(driver);
}

// Regression test for https://github.com/getreuer/qmk-keymap/issues/93:
// while a layer-tap key is held, a combo defined on that layer must fire,
// not the base layer combo in the same positions.
TEST_F(Achordion, combo_on_layer_of_held_layer_tap) {
    TestDriver driver;

    // Hold the layer-tap key long enough that QMK settles it as held and hands
    // it to Achordion, which holds the event pending the next key press.
    EXPECT_NO_REPORT(driver);
    key_lt.press();
    idle_for(TAPPING_TERM + 1);
    VERIFY_AND_CLEAR(driver);

    // Chord the two right hand keys. On layer 2 they are KC_4 + KC_5, which is
    // the combo for KC_RIGHT_BRACKET.
    EXPECT_REPORT(driver, (KC_RIGHT_BRACKET));
    EXPECT_EMPTY_REPORT(driver);
    tap_combo({key_r0, key_r1});
    VERIFY_AND_CLEAR(driver);

    EXPECT_NO_REPORT(driver);
    key_lt.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}

// Tapping the layer-tap key still sends its tap keycode.
TEST_F(Achordion, layer_tap_tapped) {
    TestDriver driver;

    EXPECT_REPORT(driver, (KC_TAB));
    EXPECT_EMPTY_REPORT(driver);
    tap_key(key_lt);
    VERIFY_AND_CLEAR(driver);
}

// A single (non-combo) key on the opposite hand settles the layer-tap as held.
TEST_F(Achordion, layer_tap_held_with_opposite_hand_key) {
    TestDriver driver;

    EXPECT_NO_REPORT(driver);
    key_lt.press();
    idle_for(TAPPING_TERM + 1);
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_4));
    EXPECT_EMPTY_REPORT(driver);
    tap_key(key_r0);
    VERIFY_AND_CLEAR(driver);

    EXPECT_NO_REPORT(driver);
    key_lt.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}

// A key on the same hand settles the layer-tap as tapped (Achordion's whole
// point), so the other key comes from the base layer.
TEST_F(Achordion, layer_tap_tapped_with_same_hand_key) {
    TestDriver driver;

    EXPECT_NO_REPORT(driver);
    key_lt.press();
    idle_for(TAPPING_TERM + 1);
    VERIFY_AND_CLEAR(driver);

    {
        InSequence s;
        EXPECT_REPORT(driver, (KC_TAB));
        EXPECT_EMPTY_REPORT(driver);
        EXPECT_REPORT(driver, (KC_A));
        EXPECT_EMPTY_REPORT(driver);
    }
    tap_key(key_left);
    key_lt.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}

// The issue reported that firing the combo twice gave the base layer combo
// first and the layer 2 combo second. Both must be the layer 2 combo.
TEST_F(Achordion, combo_on_held_layer_tap_fires_twice) {
    TestDriver driver;

    EXPECT_NO_REPORT(driver);
    key_lt.press();
    idle_for(TAPPING_TERM + 1);
    VERIFY_AND_CLEAR(driver);

    for (int i = 0; i < 2; ++i) {
        EXPECT_REPORT(driver, (KC_RIGHT_BRACKET));
        EXPECT_EMPTY_REPORT(driver);
        tap_combo({key_r0, key_r1});
        VERIFY_AND_CLEAR(driver);
    }

    EXPECT_NO_REPORT(driver);
    key_lt.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}

// A combo on the same hand as the layer-tap key settles it as tapped, so the
// base layer combo is the one that fires.
TEST_F(Achordion, same_hand_combo_settles_layer_tap_as_tap) {
    TestDriver driver;

    EXPECT_NO_REPORT(driver);
    key_lt.press();
    idle_for(TAPPING_TERM + 1);
    VERIFY_AND_CLEAR(driver);

    {
        InSequence s;
        EXPECT_REPORT(driver, (KC_TAB));
        EXPECT_EMPTY_REPORT(driver);
        EXPECT_REPORT(driver, (KC_Z));
        EXPECT_EMPTY_REPORT(driver);
    }
    tap_combo({key_l0, key_l1});
    key_lt.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
    EXPECT_TRUE(layer_state_is(0));
}

// A non-eager mod-tap key still applies its mod to a combo fired while it is
// unsettled.
TEST_F(Achordion, combo_while_holding_non_eager_mod_tap) {
    TestDriver driver;

    EXPECT_NO_REPORT(driver);
    key_mt.press();
    idle_for(TAPPING_TERM + 1);
    VERIFY_AND_CLEAR(driver);

    {
        InSequence s;
        // Achordion applies the Alt when it settles the mod-tap as held.
        EXPECT_REPORT(driver, (KC_LEFT_ALT));
        EXPECT_REPORT(driver, (KC_B, KC_LEFT_ALT));
        EXPECT_REPORT(driver, (KC_LEFT_ALT));
    }
    tap_combo({key_r0, key_r1});
    VERIFY_AND_CLEAR(driver);

    EXPECT_EMPTY_REPORT(driver);
    key_mt.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}
