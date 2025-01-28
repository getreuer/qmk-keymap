# Copyright 2024-2025 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

COMBO_ENABLE = yes
EXTRAKEY_ENABLE = yes
LTO_ENABLE = yes
MOUSEKEY_ENABLE = no
REPEAT_KEY_ENABLE = yes
UNICODE_ENABLE = no
UNICODEMAP_ENABLE = no
UCIS_ENABLE = no
UNICODE_COMMON = yes

AUTOCORRECT_ENABLE ?= yes
CAPS_WORD_ENABLE ?= yes
CONSOLE_ENABLE ?= no
GRAVE_ESC_ENABLE ?= no
LAYER_LOCK_ENABLE ?= yes
NKRO_ENABLE ?= no
RGB_MATRIX_CUSTOM_USER ?= yes
SPACE_CADET_ENABLE ?= no
TAP_DANCE_ENABLE ?= no

ACHORDION_ENABLE ?= no
ifeq ($(strip $(ACHORDION_ENABLE)), yes)
	OPT_DEFS += -DACHORDION_ENABLE
	SRC += features/achordion.c
endif

CUSTOM_SHIFT_KEYS_ENABLE ?= yes
ifeq ($(strip $(CUSTOM_SHIFT_KEYS_ENABLE)), yes)
	OPT_DEFS += -DCUSTOM_SHIFT_KEYS_ENABLE
	SRC += features/custom_shift_keys.c
endif

KEYCODE_STRING_ENABLE ?= yes
ifeq ($(strip $(KEYCODE_STRING_ENABLE)), yes)
	OPT_DEFS += -DKEYCODE_STRING_ENABLE
	SRC += features/keycode_string.c
endif

ORBITAL_MOUSE_ENABLE ?= yes
ifeq ($(strip $(ORBITAL_MOUSE_ENABLE)), yes)
	MOUSE_ENABLE = yes
	OPT_DEFS += -DORBITAL_MOUSE_ENABLE
	SRC += features/orbital_mouse.c
endif

SENTENCE_CASE_ENABLE ?= yes
ifeq ($(strip $(SENTENCE_CASE_ENABLE)), yes)
	OPT_DEFS += -DSENTENCE_CASE_ENABLE
	SRC += features/sentence_case.c
endif

SELECT_WORD_ENABLE ?= yes
ifeq ($(strip $(SELECT_WORD_ENABLE)), yes)
	OPT_DEFS += -DSELECT_WORD_ENABLE
	SRC += features/select_word.c
endif

