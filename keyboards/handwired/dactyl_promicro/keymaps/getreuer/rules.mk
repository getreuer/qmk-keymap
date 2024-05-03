# Copyright 2021-2024 Google LLC
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

BOOTLOADER = atmel-dfu

SRC += features/achordion.c
SRC += features/custom_shift_keys.c
SRC += features/layer_lock.c
SRC += features/orbital_mouse.c
SRC += features/sentence_case.c

MOUSEKEY_ENABLE = no
EXTRAKEY_ENABLE = yes
CONSOLE_ENABLE = no
COMMAND_ENABLE = no
NKRO_ENABLE = no
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no
AUTOCORRECT_ENABLE = yes
CAPS_WORD_ENABLE = yes

MOUSE_ENABLE = yes
UNICODEMAP_ENABLE = yes
COMBO_ENABLE = yes
REPEAT_KEY_ENABLE = yes
TAP_DANCE_ENABLE = no
LTO_ENABLE = yes

