// Copyright 2022 Google LLC
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
// For full documentation, see
// https://getreuer.info/posts/keyboards/sentence-case

#include "sentence_case.h"

#include <string.h>

// State in matching the beginning of a sentence.
enum {
  STATE_INIT    = 0,
  STATE_WORD    = 1,
  STATE_MATCHED = 2,
  STATE_ABBREV  = 3,
  STATE_ENDING  = 4,
  STATE_PRIMED  = 5,
};
static uint8_t sentence_state = STATE_INIT;

static void set_sentence_state(uint8_t new_state) {
  const bool primed = (new_state == STATE_PRIMED);
  if (primed != (sentence_state == STATE_PRIMED)) {
    sentence_case_primed(primed);
  }
  sentence_state = new_state;
}

#if SENTENCE_CASE_TIMEOUT > 0
#if SENTENCE_CASE_TIMEOUT < 100 || SENTENCE_CASE_TIMEOUT > 30000
// Constrain timeout to a sensible range. With the 16-bit timer, the longest
// representable timeout is 32768 ms, rounded here to 30000 ms = half a minute.
#error "sentence_case: SENTENCE_CASE_TIMEOUT must be between 100 and 30000 ms"
#endif

static uint16_t idle_timer = 0;

void sentence_case_task(void) {
  if (sentence_state && timer_expired(timer_read(), idle_timer)) {
    set_sentence_state(STATE_INIT);
  }
}
#endif  // SENTENCE_CASE_TIMEOUT > 0

// "Letter" keys, typically A-Z, for keys within words. If occurring at the
// beginning of a sentence, Sentence Case applies shift to capitalize.
__attribute__((weak)) bool sentence_case_is_letter(
    uint16_t keycode, keyrecord_t* record) {
  return KC_A <= keycode && keycode <= KC_Z;
}

// Keys that punctuate the end of a sentence.
__attribute__((weak)) bool sentence_case_is_punct(
    uint16_t keycode, keyrecord_t* record) {
#ifndef NO_ACTION_ONESHOT
  const uint8_t mods = get_mods() | get_oneshot_mods();
#else
  const uint8_t mods = get_mods();
#endif  // NO_ACTION_ONESHOT
  const bool shifted = mods & MOD_MASK_SHIFT;
  switch (keycode) {
    case KC_DOT:
      return !shifted;
    case KC_1:
    case KC_SLSH:
      return shifted;
    case KC_QUES:
    case KC_EXLM:
      return true;
  }
  return false;
}

bool process_sentence_case(uint16_t keycode, keyrecord_t* record) {
#if SENTENCE_CASE_BUFFER_SIZE > 1
  static uint16_t buffer[SENTENCE_CASE_BUFFER_SIZE] = {0};
#endif  // SENTENCE_CASE_BUFFER_SIZE > 1

  // Only process press events.
  if (!record->event.pressed) { return true; }

#ifndef NO_ACTION_ONESHOT
  const uint8_t mods = get_mods() | get_oneshot_mods();
#else
  const uint8_t mods = get_mods();
#endif  // NO_ACTION_ONESHOT
  uint8_t new_state = STATE_INIT;

  if ((mods & ~(MOD_MASK_SHIFT | MOD_BIT(KC_RALT))) != 0) {
    keycode = KC_NO;
  } else {
    switch (keycode) {
      // Ignore MO, TO, TG, TT, and OSL layer switch keys.
      case QK_MOMENTARY ... QK_MOMENTARY_MAX:
      case QK_TO ... QK_TO_MAX:
      case QK_TOGGLE_LAYER ... QK_TOGGLE_LAYER_MAX:
      case QK_LAYER_TAP_TOGGLE ... QK_LAYER_TAP_TOGGLE_MAX:
      case QK_ONE_SHOT_LAYER ... QK_ONE_SHOT_LAYER_MAX:
      // Ignore shift keys.
      case KC_LSFT:
      case KC_RSFT:
      case OSM(MOD_LSFT):
      case OSM(MOD_RSFT):
        return true;

#ifndef NO_ACTION_TAPPING
      case QK_MOD_TAP ... QK_MOD_TAP_MAX:
#ifndef NO_ACTION_LAYER
      case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
#endif  // NO_ACTION_LAYER
        if (record->tap.count == 0) { return true; }
        keycode &= 0xff;
        break;
#endif  // NO_ACTION_TAPPING

#ifdef SWAP_HANDS_ENABLE
      case QK_SWAP_HANDS ... QK_SWAP_HANDS_MAX:
        if (keycode > 0x56f0 || record->tap.count == 0) { return true; }
        keycode &= 0xff;
        break;
#endif  // SWAP_HANDS_ENABLE
    }

    // We search for sentence beginnings using a simple finite state machine. It
    // matches things like "a. a" and "a.  a" but not "a.. a" or "a.a. a". The
    // state transition matrix is:
    //
    //            PUNCT     LETTER    SPACE
    //           +----------------------------
    //   INIT    | INIT     WORD      INIT
    //   WORD    | ENDING   WORD      INIT
    //   MATCHED | ENDING   WORD      INIT
    //   ABBREV  | ABBREV   ABBREV    INIT
    //   ENDING  | INIT     ABBREV    PRIMED
    //   PRIMED  | INIT     MATCHED   PRIMED
    if (keycode == KC_SPC) {  // Space key.
      if (sentence_state == STATE_PRIMED
          || (sentence_state == STATE_ENDING
#if SENTENCE_CASE_BUFFER_SIZE > 1
              && sentence_case_check_ending(buffer)
#endif  // SENTENCE_CASE_BUFFER_SIZE > 1
      )) {
        new_state = STATE_PRIMED;
      }
    } else if (sentence_case_is_letter(keycode, record)) {  // Letter key.
      if (sentence_state <= STATE_MATCHED) {
        new_state = STATE_WORD;
      } else if (sentence_state == STATE_PRIMED) {
        // This is the start of a sentence. Apply weak shift mod to capitalize.
        new_state = STATE_MATCHED;
        if ((mods & MOD_MASK_SHIFT) == 0) {
          register_weak_mods(MOD_BIT(KC_LSFT));
        }
      } else {
        new_state = STATE_ABBREV;
      }
    } else if (sentence_case_is_punct(keycode, record)) {  // Punctuating key.
      if (sentence_state == STATE_WORD || sentence_state == STATE_MATCHED) {
#if SENTENCE_CASE_BUFFER_SIZE > 1
        if (sentence_case_check_ending(buffer)) {
#else
        {
#endif  // SENTENCE_CASE_BUFFER_SIZE > 1
          new_state = STATE_ENDING;
        }
      } else if (sentence_state == STATE_ABBREV) {
        new_state = STATE_ABBREV;
      }
    }

#if SENTENCE_CASE_BUFFER_SIZE > 1
    memmove(buffer, buffer + 1,
            (SENTENCE_CASE_BUFFER_SIZE - 1) * sizeof(uint16_t));
    buffer[SENTENCE_CASE_BUFFER_SIZE - 1] = keycode;
#endif  // SENTENCE_CASE_BUFFER_SIZE > 1
#if SENTENCE_CASE_TIMEOUT > 0
    idle_timer = record->event.time + SENTENCE_CASE_TIMEOUT;
#endif  // SENTENCE_CASE_TIMEOUT > 0
  }

  set_sentence_state(new_state);
  return true;
}

bool sentence_case_just_typed(const uint16_t* buffer,
                              const uint16_t* pattern, int8_t pattern_len) {
#if SENTENCE_CASE_BUFFER_SIZE > 1
  buffer += SENTENCE_CASE_BUFFER_SIZE - pattern_len;
  for (int8_t i = 0; i < pattern_len; ++i) {
    if (buffer[i] != pgm_read_word(pattern + i)) {
      return false;
    }
  }
  return true;
#else
  return false;
#endif  // SENTENCE_CASE_BUFFER_SIZE > 1
}

__attribute__((weak)) bool sentence_case_check_ending(const uint16_t* buffer) {
#if SENTENCE_CASE_BUFFER_SIZE >= 5
  // Don't consider the abbreviations "vs." and "etc." to end the sentence.
  if (SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_V, KC_S, KC_DOT) ||
      SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_E, KC_T, KC_C, KC_DOT)) {
    return false;  // Not a real sentence ending.
  }
#endif  // SENTENCE_CASE_BUFFER_SIZE >= 5
  return true;  // Real sentence ending; capitalize next letter.
}

__attribute__((weak)) void sentence_case_primed(bool primed) {}

