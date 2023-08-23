// Copyright 2021-2023 Google LLC
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

#pragma once

#define USE_SERIAL
#define MASTER_RIGHT

// Tap-hold configuration for home row mods.
#define TAPPING_TERM 175
#define TAPPING_TERM_PER_KEY
#define PERMISSIVE_HOLD
#define QUICK_TAP_TERM_PER_KEY

#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION

#define UNICODE_SELECTED_MODES UNICODE_MODE_LINUX

// Use Left Ctrl + Right Ctrl to activate the Command feature.
#define IS_COMMAND() (get_mods() == MOD_MASK_CTRL)

// Holding Shift while Caps Word is active inverts the shift state.
#define CAPS_WORD_INVERT_ON_SHIFT
// When idle, turn off Caps Word after 5 seconds.
#define CAPS_WORD_IDLE_TIMEOUT 5000

// When idle, turn off Select Word after 2 seconds.
#define SELECT_WORD_TIMEOUT 2000

// When idle, turn off Sentence Case after 2 seconds.
#define SENTENCE_CASE_TIMEOUT 2000
