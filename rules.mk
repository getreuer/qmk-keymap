# Copyright 2021-2022 Google LLC
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

SRC += features/achordion.c
SRC += features/autocorrection.c
SRC += features/custom_shift_keys.c
SRC += features/select_word.c

MCU = atmega32u4

# Bootloader selection
#   Teensy       halfkay
#   Pro Micro    caterina
#   Atmel DFU    atmel-dfu
#   LUFA DFU     lufa-dfu
#   QMK DFU      qmk-dfu
#   ATmega32A    bootloadHID
#   ATmega328P   USBasp
BOOTLOADER = atmel-dfu

BOOTMAGIC_ENABLE = no      # Virtual DIP switch configuration(+1000)
MOUSEKEY_ENABLE = yes      # Mouse keys(+4700)
EXTRAKEY_ENABLE = yes      # Audio control and System control(+450)
CONSOLE_ENABLE = no        # Console for debug(+400)
COMMAND_ENABLE = no        # Commands for debug and configuration
NKRO_ENABLE = no           # Nkey Rollover - if this doesn't work, see here: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work
BACKLIGHT_ENABLE = no      # Enable keyboard backlight functionality
MIDI_ENABLE = no           # MIDI controls
AUDIO_ENABLE = no          # Audio output on port C6
BLUETOOTH_ENABLE = no      # Enable Bluetooth with the Adafruit EZ-Key HID
RGBLIGHT_ENABLE = no       # Enable WS2812 RGB underlight.
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no
CAPS_WORD_ENABLE = yes

UNICODEMAP_ENABLE = yes
COMBO_ENABLE = yes
TAP_DANCE_ENABLE = no
LTO_ENABLE = yes

# Do not enable SLEEP_LED_ENABLE. it uses the same timer as BACKLIGHT_ENABLE
SLEEP_LED_ENABLE = no    # Breathing sleep LED during USB suspend

SPLIT_KEYBOARD = yes

