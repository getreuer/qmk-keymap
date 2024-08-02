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
 * @mainpage Pascal Getreuer's QMK keymap
 *
 * This is my Quantum Mechanical Keyboard (QMK) keymap for the Dactyl Ergodox.
 * Who knew a keyboard could do so much?
 *
 * Feature libraries
 * -----------------
 *  * features/achordion.h: customize the tap-hold decision
 *  * features/autocorrection.h: run rudimentary autocorrection on your keyboard
 *  * features/caps_word.h: modern alternative to Caps Lock
 *  * features/custom_shift_keys.h: they're surprisingly tricky to get right;
 *                                  here is my approach
 *  * features/layer_lock.h: macro to stay in the current layer
 *  * features/mouse_turbo_click.h: macro that clicks the mouse rapidly
 *  * features/orbital_mouse.h: a polar approach to mouse key control
 *  * features/repeat_key.h: a "repeat last key" implementation
 *  * features/sentence_case.h: capitalize first letter of sentences
 *  * features/select_word.h: macro for convenient word or line selection
 *  * features/socd_cleaner.h: enhance WASD for fast inputs for gaming
 *
 * License
 * -------
 * This repo uses the Apache License 2.0 except where otherwise indicated. See
 * LICENSE.txt for details.
 *
 * For further documentation of this keymap's features, see
 * <https://getreuer.info/posts/keyboards>
 */

#ifdef ACHORDION_ENABLE
#include "features/achordion.h"
#endif  // ACHORDION_ENABLE
#ifdef CUSTOM_SHIFT_KEYS_ENABLE
#include "features/custom_shift_keys.h"
#endif  // CUSTOM_SHIFT_KEYS_ENABLE
#ifdef LAYER_LOCK_ENABLE
#include "features/layer_lock.h"
#endif  // LAYER_LOCK_ENABLE
#ifdef ORBITAL_MOUSE_ENABLE
#include "features/orbital_mouse.h"
#endif  // ORBITAL_MOUSE_ENABLE
#ifdef SENTENCE_CASE_ENABLE
#include "features/sentence_case.h"
#endif  // SENTENCE_CASE_ENABLE
#if __has_include("user_song_list.h")
#include "user_song_list.h"
#endif

enum layers {
  BASE,
  SYM,
  NUM,
  WIN,
  FUN,
};

enum custom_keycodes {
  ARROW = SAFE_RANGE,
  DASH,
  LLOCK,
  RGB_DEF,
  SELLINE,
  SRCHSEL,
  TMUXESC,
  UPDIR,
  USRNAME,
  // Macros invoked through the Magic key.
  M_DOCSTR,
  M_EQEQ,
  M_INCLUDE,
  M_ION,
  M_MENT,
  M_MKGRVS,
  M_QUEN,
  M_THE,
  M_TMENT,
  M_UPDIR,
  M_NOOP,
};

// This keymap uses Ikcelaks' Magic Sturdy layout for the base layer (see
// https://github.com/Ikcelaks/keyboard_layouts). I've also made some twists of
// my own. The "magic" is a key whose function depends on the last pressed key,
// implemented using the Alternate Repeat Key. This key is used to remove the
// top SFBs and type common n-grams.
//
// The following describes the magic key functionality, where * represents the
// magic key and @ the repeat key. For example, tapping A and then the magic key
// types "ao". Most of this is coded in my `get_alt_repeat_key_keycode_user()`
// definition below.
//
// SFB removal and common n-grams:
//
//     A * -> AO     L * -> LK      S * -> SK
//     C * -> CY     M * -> MENT    T * -> TMENT
//     D * -> DY     O * -> OA      U * -> UE
//     E * -> EU     P * -> PY      Y * -> YP
//     G * -> GY     Q * -> QUEN    spc * -> THE
//     I * -> ION    R * -> RL
//
// When the magic key types a letter, following it with the repeat key produces
// "n". This is useful to type certain patterns without SFBs.
//
//     A * @ -> AON             (like "kaon")
//     D * @ -> DYN             (like "dynamic")
//     E * @ -> EUN             (like "reunite")
//     O * @ -> OAN             (like "loan")
//
// Other patterns:
//
//     spc * @ -> THEN
//     I * @ -> IONS            (like "nations")
//     M * @ -> MENTS           (like "moments")
//     Q * @ -> QUENC           (like "frequency")
//     T * @ -> TMENTS          (lite "adjustments")
//     = *   -> ===             (JS code)
//     ! *   -> !==             (JS code)
//     " *   -> """<cursor>"""  (Python code)
//     ` *   -> ```<cursor>```  (Markdown code)
//     # *   -> #include        (C code)
//     . *   -> ../             (shell)
//     . * @ -> ../../
#define MAGIC QK_AREP

// Short aliases for home row mods and other tap-hold keys.
#define HOME_S LT(SYM, KC_S)
#define HOME_T LALT_T(KC_T)
#define HOME_R LSFT_T(KC_R)
#define HOME_D LCTL_T(KC_D)
#define HOME_N RCTL_T(KC_N)
#define HOME_E RSFT_T(KC_E)
#define HOME_A LALT_T(KC_A)
#define HOME_I LT(SYM, KC_I)
#define HOME_X LGUI_T(KC_X)
#define HOME_SC RGUI_T(KC_SCLN)

#define NUM_G LT(NUM, KC_G)
#define WIN_COL LT(WIN, KC_SCLN)


///////////////////////////////////////////////////////////////////////////////
// Combos (https://docs.qmk.fm/features/combo)
///////////////////////////////////////////////////////////////////////////////
const uint16_t caps_combo[] PROGMEM = {KC_J, KC_COMM, COMBO_END};
const uint16_t j_k_combo[] PROGMEM = {KC_J, KC_K, COMBO_END};
const uint16_t j_g_combo[] PROGMEM = {KC_J, NUM_G, COMBO_END};
const uint16_t d_y_combo[] PROGMEM = {HOME_D, KC_Y, COMBO_END};
// clang-format off
combo_t key_combos[] = {
    COMBO(caps_combo, CW_TOGG),          // J and , => activate Caps Word.
    COMBO(j_k_combo, KC_BSLS),           // J and K => backslash
    COMBO(j_g_combo, OSL(NUM)),          // J and G => one-shot NUM layer
    COMBO(d_y_combo, OSL(FUN)),          // D and Y => one-shot FUN layer
};
// clang-format on

///////////////////////////////////////////////////////////////////////////////
// Custom shift keys (https://getreuer.info/posts/keyboards/custom-shift-keys)
///////////////////////////////////////////////////////////////////////////////
#ifdef CUSTOM_SHIFT_KEYS_ENABLE
const custom_shift_key_t custom_shift_keys[] = {
    {KC_DOT, KC_QUES},
    {KC_COMM, KC_EXLM},
    {HOME_SC, KC_AT  },
    {KC_MPLY, KC_MNXT},
    {KC_EQL , KC_EQL },  // Don't shift =
    {KC_SLSH, KC_SLSH},  // Don't shift /
};
uint8_t NUM_CUSTOM_SHIFT_KEYS =
    sizeof(custom_shift_keys) / sizeof(custom_shift_key_t);
#endif  // CUSTOM_SHIFT_KEYS_ENABLE

///////////////////////////////////////////////////////////////////////////////
// Tap-hold configuration (https://docs.qmk.fm/tap_hold)
///////////////////////////////////////////////////////////////////////////////
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t* record) {
  switch (keycode) {
    case HOME_T:
    case HOME_A:
      return TAPPING_TERM + 15;
    default:
      return TAPPING_TERM;
  }
}

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t* record) {
  // If you quickly hold a tap-hold key after tapping it, the tap action is
  // repeated. Key repeating is useful e.g. for Vim navigation keys, but can
  // lead to missed triggers in fast typing. Here, returning 0 means we
  // instead want to "force hold" and disable key repeating.
  switch (keycode) {
    case HOME_N:
      return QUICK_TAP_TERM;  // Enable key repeating.
    default:
      return 0;  // Otherwise, force hold and disable key repeating.
  }
}

///////////////////////////////////////////////////////////////////////////////
// Achordion (https://getreuer.info/posts/keyboards/achordion)
///////////////////////////////////////////////////////////////////////////////
#ifdef ACHORDION_ENABLE
bool achordion_chord(uint16_t tap_hold_keycode,
                     keyrecord_t* tap_hold_record,
                     uint16_t other_keycode,
                     keyrecord_t* other_record) {
  // Also allow same-hand holds when the other key is in the rows outside the
  // alphas. I need the `% (MATRIX_ROWS / 2)` because my keyboards are split.
  uint8_t row = other_record->event.key.row % (MATRIX_ROWS / 2);
  if (!(1 <= row && row <= 3)) { return true; }

  switch (tap_hold_keycode) {
    // Exceptionally allow symbol layer LTs + row 0 in same-hand chords.
    case HOME_S:
    case HOME_I:
      if (row == 0) { return true; }
      break;
    // Exceptionally allow G + J as a same-hand chord.
    case NUM_G:
      if (other_keycode == KC_J) { return true; }
      break;
  }

  return achordion_opposite_hands(tap_hold_record, other_record);
}

uint16_t achordion_timeout(uint16_t tap_hold_keycode) {
  switch (tap_hold_keycode) {
    default:
      return 800;  // Use a timeout of 800 ms.
  }
}

uint16_t achordion_streak_chord_timeout(
    uint16_t tap_hold_keycode, uint16_t next_keycode) {
  // Disable streak detection on LT keys.
  if (IS_QK_LAYER_TAP(tap_hold_keycode)) {
    return 0;
  }

  // Exceptions so that certain hotkeys don't get blocked as streaks.
  switch (tap_hold_keycode) {
    case HOME_N:
      if (next_keycode == KC_C || next_keycode == KC_V) {
        return 0;
      }
      break;
    case HOME_D:
      if (next_keycode == HOME_N) {
        return 0;
      }
      break;
  }

  // Otherwise, tap_hold_keycode is a mod-tap key.
  const uint8_t mod = mod_config(QK_MOD_TAP_GET_MODS(tap_hold_keycode));
  if ((mod & MOD_LSFT) != 0) {
    return 100;  // A short streak timeout for Shift mod-tap keys.
  } else {
    return 220;  // A longer timeout otherwise.
  }
}
#endif  // ACHORDION_ENABLE

///////////////////////////////////////////////////////////////////////////////
// Autocorrect (https://docs.qmk.fm/features/autocorrect)
///////////////////////////////////////////////////////////////////////////////
#ifdef AUTOCORRECT_ENABLE
bool apply_autocorrect(uint8_t backspaces, const char* str,
                       char* typo, char* correct) {
  for (uint8_t i = 0; i < backspaces; ++i) {
    tap_code(KC_BSPC);
  }
  send_string_with_delay_P(str, TAP_CODE_DELAY);
  return false;
}
#endif  // AUTOCORRECT_ENABLE

///////////////////////////////////////////////////////////////////////////////
// Caps word (https://docs.qmk.fm/features/caps_word)
///////////////////////////////////////////////////////////////////////////////
#ifdef CAPS_WORD_ENABLE
bool caps_word_press_user(uint16_t keycode) {
  switch (keycode) {
    // Keycodes that continue Caps Word, with shift applied.
    case KC_A ... KC_Z:
      add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to the next key.
      return true;

    // Keycodes that continue Caps Word, without shifting.
    case KC_1 ... KC_0:
    case KC_BSPC:
    case KC_DEL:
    case KC_UNDS:
    case M_THE:
    case M_ION:
    case M_MENT:
    case M_QUEN:
    case M_TMENT:
      return true;

    default:
      return false;  // Deactivate Caps Word.
  }
}
#endif  // CAPS_WORD_ENABLE

///////////////////////////////////////////////////////////////////////////////
// Sentence case (https://getreuer.info/posts/keyboards/sentence-case)
///////////////////////////////////////////////////////////////////////////////
#ifdef SENTENCE_CASE_ENABLE
char sentence_case_press_user(uint16_t keycode, keyrecord_t* record,
                              uint8_t mods) {
  if ((mods & ~(MOD_MASK_SHIFT | MOD_BIT(KC_RALT))) == 0) {
    const bool shifted = mods & MOD_MASK_SHIFT;
    switch (keycode) {
      case KC_A ... KC_Z:
      case M_THE:
        return 'a';  // Letter key.

      case KC_DOT:  // Both . and Shift . (?) punctuate sentence endings.
      case KC_EXLM:
      case KC_QUES:
        return '.';

      case KC_COMM:
        return shifted ? '.' : '#';

      case KC_2 ... KC_0:  // 2 3 4 5 6 7 8 9 0
      case KC_AT ... KC_RPRN:  // @ # $ % ^ & * ( )
      case KC_MINS ... KC_SCLN:  // - = [ ] backslash ;
      case KC_UNDS ... KC_COLN:  // _ + { } | :
      case KC_GRV:
        return '#';  // Symbol key.

      case KC_SPC:
        return ' ';  // Space key.

      case KC_QUOT:
      case KC_DQUO:
        return '\'';  // Quote key.
    }
  }

  // Otherwise clear Sentence Case to initial state.
  sentence_case_clear();
  return '\0';
}
#endif  // SENTENCE_CASE_ENABLE

///////////////////////////////////////////////////////////////////////////////
// Repeat key (https://docs.qmk.fm/features/repeat_key)
///////////////////////////////////////////////////////////////////////////////
bool remember_last_key_user(uint16_t keycode, keyrecord_t* record,
                            uint8_t* remembered_mods) {
  // Unpack tapping keycode for tap-hold keys.
  switch (keycode) {
#ifndef NO_ACTION_TAPPING
    case QK_MOD_TAP ... QK_MOD_TAP_MAX:
      keycode = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
      break;
#ifndef NO_ACTION_LAYER
    case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
      keycode = QK_LAYER_TAP_GET_TAP_KEYCODE(keycode);
      break;
#endif  // NO_ACTION_LAYER
#endif  // NO_ACTION_TAPPING
  }

  // Forget Shift on most letters when Shift or AltGr are the only mods. Some
  // letters are excluded, e.g. for "NN" and "ZZ" in Vim.
  switch (keycode) {
    case KC_A ... KC_H:
    case KC_K ... KC_M:
    case KC_O ... KC_U:
      if ((*remembered_mods & ~(MOD_MASK_SHIFT | MOD_BIT(KC_RALT))) == 0) {
        *remembered_mods &= ~MOD_MASK_SHIFT;
      }
      break;
  }

  return true;
}

uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {
  if ((mods & MOD_MASK_CTRL)) {
    switch (keycode) {
      case HOME_A: return C(KC_C);  // Ctrl+A -> Ctrl+C
      case KC_C: return C(KC_V);    // Ctrl+C -> Ctrl+V
    }
  } else if ((mods & ~MOD_MASK_SHIFT) == 0) {
    // This is where most of the "magic" for the MAGIC key is implemented.
    switch (keycode) {
      case KC_SPC:  // spc -> THE
      case KC_ENT:
      case KC_TAB:
        return M_THE;

      // For navigating next/previous search results in Vim:
      // N -> Shift + N, Shift + N -> N.
      case HOME_N:
        if ((mods & MOD_MASK_SHIFT) == 0) {
          return S(KC_N);
        }
        // Fall through intended.
      case KC_N: return KC_N;

      // Fix SFBs and awkward strokes.
      case HOME_A: return KC_O;       // A -> O
      case KC_O: return KC_A;         // O -> A
      case HOME_E: return KC_U;       // E -> U
      case KC_U: return KC_E;         // U -> E
      case HOME_I:
        if ((mods & MOD_MASK_SHIFT) == 0) {
          return M_ION;  // I -> ON
        } else {
          return KC_QUOT;  // Shift I -> '
        }
      case KC_M: return M_MENT;       // M -> ENT
      case KC_Q: return M_QUEN;       // Q -> UEN
      case HOME_T: return M_TMENT;    // T -> TMENT

      case KC_C: return KC_Y;         // C -> Y
      case HOME_D: return KC_Y;       // D -> Y
      case NUM_G: return KC_Y;        // G -> Y
      case KC_P: return KC_Y;         // P -> Y
      case KC_Y: return KC_P;         // Y -> P

      case KC_L: return KC_K;         // L -> K
      case HOME_S: return KC_K;       // S -> K

      case HOME_R: return KC_L;       // R -> L
      case KC_DOT:
        if ((mods & MOD_MASK_SHIFT) == 0) {
          return M_UPDIR;  // . -> ./
        }
        return M_NOOP;
      case KC_HASH: return M_INCLUDE; // # -> include
      case KC_EQL: return M_EQEQ;     // = -> ==

      case KC_COMM:
        if ((mods & MOD_MASK_SHIFT) != 0) {
          return KC_EQL;  // ! -> =
        }
        return M_NOOP;
      case KC_QUOT:
        if ((mods & MOD_MASK_SHIFT) != 0) {
          return M_DOCSTR;  // " -> ""<cursor>"""
        }
        return M_NOOP;
      case KC_GRV:  // ` -> ``<cursor>``` (for Markdown code)
        return M_MKGRVS;
      case KC_LABK:  // < -> - (for Haskell)
        return KC_MINS;

      case KC_SLSH:
        return KC_SLSH;  // / -> / (easier reach than Repeat)

      case KC_F:
      case KC_V:
      case HOME_X:
      case HOME_SC:
        return M_NOOP;

      case KC_PLUS:
      case KC_MINS:
      case KC_ASTR:
      case KC_PERC:
      case KC_PIPE:
      case KC_AMPR:
      case KC_CIRC:
      case KC_TILD:
      case KC_EXLM:
      case KC_RABK:
        return KC_EQL;

      case C(KC_A): return C(KC_C);  // Ctrl+A -> Ctrl+C
    }
  }
  return KC_TRNS;
}

// An enhanced version of SEND_STRING: if Caps Word is active, the Shift key is
// held while sending the string. Additionally, the last key is set such that if
// the Repeat Key is pressed next, it produces `repeat_keycode`. This helper is
// used for several macros below in my process_record_user() function.
#define MAGIC_STRING(str, repeat_keycode) \
  magic_send_string_P(PSTR(str), (repeat_keycode))
static void magic_send_string_P(const char* str, uint16_t repeat_keycode) {
  uint8_t saved_mods = 0;
  // If Caps Word is on, save the mods and hold Shift.
  if (is_caps_word_on()) {
    saved_mods = get_mods();
    register_mods(MOD_BIT(KC_LSFT));
  }

  send_string_P(str);  // Send the string.
  set_last_keycode(repeat_keycode);

  // If Caps Word is on, restore the mods.
  if (is_caps_word_on()) {
    set_mods(saved_mods);
  }
}

///////////////////////////////////////////////////////////////////////////////
// Status LEDs
///////////////////////////////////////////////////////////////////////////////
#ifdef STATUS_LED_1
// LED 1 indicates when any layer above the SYM layer is active.
layer_state_t layer_state_set_user(layer_state_t state) {
  STATUS_LED_1(get_highest_layer(layer_state) > SYM);
  return state;
}
#endif  // STATUS_LED_1

#ifdef STATUS_LED_2
// LED 2 indicates when Sentence case is in primed to capitalize the next key.
void sentence_case_primed(bool primed) {
  STATUS_LED_2(primed);
}
#endif  // STATUS_LED_2

#ifdef STATUS_LED_3
// LED 3 indicates when Caps word is active.
void caps_word_set_user(bool active) {
  STATUS_LED_3(active);
}
#endif  // STATUS_LED_3

///////////////////////////////////////////////////////////////////////////////
// User macro callbacks (https://docs.qmk.fm/feature_macros)
///////////////////////////////////////////////////////////////////////////////

void keyboard_post_init_user(void) {
  // Play MUSHROOM_SOUND two seconds after init, if defined and audio enabled.
#if defined(AUDIO_ENABLE) && defined(MUSHROOM_SOUND)
  uint32_t play_init_song_callback(uint32_t trigger_time, void* cb_arg) {
    static float init_song[][2] = SONG(MUSHROOM_SOUND);
    PLAY_SONG(init_song);
    return 0;
  }
  defer_exec(2000, play_init_song_callback, NULL);
#endif // defined(AUDIO_ENABLE) && defined(MUSHROOM_SOUND)
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
#ifdef ACHORDION_ENABLE
  if (!process_achordion(keycode, record)) { return false; }
#endif  // ACHORDION_ENABLE
#ifdef ORBITAL_MOUSE_ENABLE
  if (!process_orbital_mouse(keycode, record)) { return false; }
#endif  // ORBITAL_MOUSE_ENABLE
#ifdef LAYER_LOCK_ENABLE
  if (!process_layer_lock(keycode, record, LLOCK)) { return false; }
#endif  // LAYER_LOCK_ENABLE
#ifdef SENTENCE_CASE_ENABLE
  if (!process_sentence_case(keycode, record)) { return false; }
#endif  // SENTENCE_CASE_ENABLE
#ifdef CUSTOM_SHIFT_KEYS_ENABLE
  if (!process_custom_shift_keys(keycode, record)) { return false; }
#endif  // CUSTOM_SHIFT_KEYS_ENABLE

  const uint8_t mods = get_mods();
  const uint8_t all_mods = (mods | get_weak_mods()
#ifndef NO_ACTION_ONESHOT
                        | get_oneshot_mods()
#endif  // NO_ACTION_ONESHOT
  );
  const uint8_t shift_mods = all_mods & MOD_MASK_SHIFT;
  const bool alt = all_mods & MOD_BIT(KC_LALT);

  // If alt repeating key A, E, I, O, U, Y with no mods other than Shift, set
  // the last key to KC_N. Above, alternate repeat of KC_N is defined to be
  // again KC_N. This way, either tapping alt repeat and then repeat (or
  // equivalently double tapping alt repeat) is useful to type certain patterns
  // without SFBs:
  //
  //   D <altrep> <rep> -> DYN (as in "dynamic")
  //   O <altrep> <rep> -> OAN (as in "loan")
  if (get_repeat_key_count() < 0 && !shift_mods &&
      (keycode == KC_A || keycode == KC_E || keycode == KC_I ||
       keycode == KC_O || keycode == KC_U || keycode == KC_Y)) {
    set_last_keycode(KC_N);
    set_last_mods(0);
  }

  switch (keycode) {
    // Behavior:
    //  * Unmodified:       _ (KC_UNDS)
    //  * With Shift:       - (KC_MINS)
    //  * With Alt:         Unicode en dash
    //  * With Shift + Alt: Unicode em dash
    case KC_UNDS: {
        static uint16_t registered_keycode = KC_NO;

        if (record->event.pressed) {
          if (alt) {
            send_unicode_string(shift_mods ? "\xe2\x80\x94" : "\xe2\x80\x93");
          } else {
            process_caps_word(keycode, record);
            const bool shifted = (mods | get_weak_mods()) & MOD_MASK_SHIFT;
            clear_weak_mods();
            clear_mods();

            if (registered_keycode) {  // Invoked through Repeat key.
              unregister_code16(registered_keycode);
            } else {
              registered_keycode = shifted ? KC_MINS : KC_UNDS;
            }

            register_code16(registered_keycode);
            set_mods(mods);
          }
        } else if (registered_keycode) {
          unregister_code16(registered_keycode);
          registered_keycode = KC_NO;
        }
      } return false;

    // Hold behavior: switches to WIN layer.
    // Tap behavior:
    //  * Unmodified:       :
    //  * With Shift:       std:: (C++, Rust)
    case WIN_COL:
      if (record->tap.count) {
        static bool registered = false;

        if (record->event.pressed) {
          if (registered) {  // Invoked through Repeat key.
            unregister_code16(KC_COLN);
          } else if (shift_mods) {
            clear_mods();
            SEND_STRING_DELAY("std:", TAP_CODE_DELAY);
            set_mods(mods);
          }
          register_code16(KC_COLN);
          registered = true;
        } else {
          unregister_code16(KC_COLN);
          registered = false;
        }
        return false;
      }
      return true;
  }

  if (record->event.pressed) {
    switch (keycode) {
      case UPDIR:
        SEND_STRING_DELAY("../", TAP_CODE_DELAY);
        return false;

      case TMUXESC:  // Enter copy mode in Tmux.
        SEND_STRING_DELAY(SS_LCTL("a") SS_TAP(X_ESC), TAP_CODE_DELAY);
        return false;

      case SRCHSEL:  // Searches the current selection in a new tab.
        // Mac users, change LCTL to LGUI.
        SEND_STRING_DELAY(
            SS_LCTL("ct") SS_DELAY(100) SS_LCTL("v") SS_TAP(X_ENTER),
            TAP_CODE_DELAY);
        return false;

      case SELLINE:  // Selects the current line.
        SEND_STRING_DELAY(
            SS_TAP(X_HOME) SS_LSFT(SS_TAP(X_END)), TAP_CODE_DELAY);
        return false;

      case USRNAME:
        SEND_STRING_DELAY("getreuer", TAP_CODE_DELAY);
        return false;

      case ARROW:  // Unicode arrows -> => <-> <=> through Shift and Alt.
        send_unicode_string(alt ? (shift_mods
                                    ? "\xe2\x87\x94"     // <=>
                                    : "\xe2\x86\x94")    // <->
                                 : (shift_mods
                                    ? "\xe2\x87\x92"     // =>
                                    : "\xe2\x86\x92"));  // ->
        return false;

      case KC_COLN:
        if (shift_mods) {  // Shift + : types a happy emoji.
          static const char* emojis[] = {
              "\xf0\x9f\xa5\xb3",  // Party hat.
              "\xf0\x9f\x91\x8d",  // Thumbs up.
              "\xe2\x9c\x8c",      // Victory hand.
              "\xf0\x9f\xa4\xa9",  // Star eyes.
              "\xf0\x9f\x94\xa5",  // Fire.
              "\xf0\x9f\x8e\x89",  // Party popper.
              "\xf0\x9f\x91\xbe",  // Purple alien.
              "\xf0\x9f\x98\x81",  // Grin.
          };
          const int NUM_EMOJIS = sizeof(emojis) / sizeof(*emojis);

          // Pick an index between 0 and NUM_EMOJIS - 2.
          uint8_t index = (UINT16_C(36563) * record->event.time) >> 8;
          index = ((NUM_EMOJIS - 1) * index) >> 8;
          // Don't pick the same emoji twice in a row.
          static uint8_t last_index = 0;
          if (index >= last_index) { ++index; }
          last_index = index;

          // Produce the emoji.
          send_unicode_string(emojis[index]);
          return false;
        }
        return true;

#ifdef RGB_MATRIX_ENABLE
      case RGB_DEF:  // Set RGB matrix to some nice defaults.
        rgb_matrix_enable_noeeprom();
        rgb_matrix_mode_noeeprom(RGB_MATRIX_ALPHAS_MODS);
        rgb_matrix_sethsv_noeeprom(17, 255, 255);  // Amber color.
        return false;
#endif  // RGB_MATRIX_ENABLE

      // Macros invoked through the MAGIC key.
      case M_THE:     MAGIC_STRING(/* */"the", KC_N); break;
      case M_ION:     MAGIC_STRING(/*i*/"on", KC_S); break;
      case M_MENT:    MAGIC_STRING(/*m*/"ent", KC_S); break;
      case M_QUEN:    MAGIC_STRING(/*q*/"uen", KC_C); break;
      case M_TMENT:   MAGIC_STRING(/*t*/"ment", KC_S); break;
      case M_UPDIR:   MAGIC_STRING(/*.*/"./", UPDIR); break;
      case M_INCLUDE: SEND_STRING(/*#*/"include "); break;
      case M_EQEQ:    SEND_STRING(/*=*/"=="); break;
      case M_DOCSTR:
        SEND_STRING(/*"*/"\"\"\"\"\""
            SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT));
        break;
      case M_MKGRVS:
        SEND_STRING(/*`*/"``\n\n```" SS_TAP(X_UP));
        break;
    }
  }

  return true;
}

void matrix_scan_user(void) {
#ifdef ACHORDION_ENABLE
  achordion_task();
#endif  // ACHORDION_ENABLE
#ifdef LAYER_LOCK_ENABLE
  layer_lock_task();
#endif  // LAYER_LOCK_ENABLE
#ifdef ORBITAL_MOUSE_ENABLE
  orbital_mouse_task();
#endif  // ORBITAL_MOUSE_ENABLE
#ifdef SENTENCE_CASE_ENABLE
  sentence_case_task();
#endif  // SENTENCE_CASE_ENABLE
}

