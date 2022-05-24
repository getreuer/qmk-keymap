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
// Sentence case: automatically capitalize the first letter of sentences.
//
// This library automatically capitalizes the first letter of sentences,
// reducing the need to explicitly use shift. A simple finite state machine
// is used that matches things like
//
//   "a. a"
//   "a.  a"
//   "a? a"
//
// but not
//
//   "a... a"
//   "a.a. a"
//
// Additionally by default, abbreviations "vs." and "etc." are exceptionally
// detected as not real sentence endings. You can use the callback
// `sentence_case_check_ending()` to define other exceptions.
//
// For full documentation, see
// https://getreuer.info/posts/keyboards/sentence-case

#pragma once

#include "quantum.h"

#ifdef __cplusplus
extern "C" {
#endif

// Call this function from `process_record_user()` to implement Sentence Case.
bool process_sentence_case(uint16_t keycode, keyrecord_t* record);

void sentence_case_task(void);
#if SENTENCE_CASE_TIMEOUT > 0
void sentence_case_task(void);
#else
static inline void sentence_case_task(void) {}
#endif

// Callback that gets called when Sentence Case changes to or from a "primed"
// state, useful to indicate with an LED or otherwise that the next letter typed
// will be capitalized.
void sentence_case_primed(bool primed);

// Callback defining which keys are "letter" keys. Typically KC_A to KC_Z. If a
// letter key occurs at the beginning of a sentence, Sentence Case applies
// shift to capitalize it.
bool sentence_case_is_letter(uint16_t keycode, keyrecord_t* record);

// Callback defining which keys punctuate the end of a sentence. Typically
// KC_DOT (.), Shift + KC_SLSH (?), and Shift + KC_1 (!).
bool sentence_case_is_punct(uint16_t keycode, keyrecord_t* record);

// This callback is called when a punctuating key is typed to decide whether it
// is a real sentence ending, meaning the first letter of the following word
// should be capitalized. For instance, abbreviations like "vs." are usually not
// real sentence endings. The input argument is a buffer of the last
// SENTENCE_CASE_BUFFER_SIZE keycodes. Returning true means it is a real
// sentence ending; returning false means it is not.
//
// The default implementation checks for the abbreviations "vs." and "etc.":
//
//   bool sentence_case_check_ending(const uint16_t* buffer) {
//     // Don't consider the abbreviations "vs." and "etc." to end the sentence.
//     if (SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_V, KC_S, KC_DOT) ||
//         SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_E, KC_T, KC_C, KC_DOT)) {
//       return false;  // Not a real sentence ending.
//     }
//     return true;  // Real sentence ending; capitalize next letter.
//   }
//
bool sentence_case_check_ending(const uint16_t* buffer);

// The size of the keycode buffer for `sentence_case_check_ending()`. It must be
// at least as large as the longest pattern checked. If less than 2, buffering
// is disabled and the callback is not called.
#ifndef SENTENCE_CASE_BUFFER_SIZE
#define SENTENCE_CASE_BUFFER_SIZE 8
#endif  // SENTENCE_CASE_BUFFER_SIZE

// Returns true if a given pattern of keys was just typed by comparing with the
// keycode buffer. This is useful for defining exceptions in
// `sentence_case_check_ending()`.
//
// For example, `SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_V, KC_S, KC_DOT)` returns
// true if " vs." were the last four keys typed.
//
// NOTE: The pattern must be no longer than SENTENCE_CASE_BUFFER_SIZE.
#define SENTENCE_CASE_JUST_TYPED(...) \
  ({ \
    static const uint16_t PROGMEM pattern[] = {__VA_ARGS__}; \
    sentence_case_just_typed( \
        buffer, pattern, sizeof(pattern) / sizeof(uint16_t)); \
  })
bool sentence_case_just_typed(
    const uint16_t* buffer, const uint16_t* pattern, int8_t pattern_len);

#ifdef __cplusplus
}
#endif

