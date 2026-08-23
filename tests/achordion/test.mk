# Copyright 2026 Google LLC
# SPDX-License-Identifier: Apache-2.0

# Absolute path to this repo's `features` directory, holding the Achordion
# sources under test. See README.md.
ifeq ($(ACHORDION_PATH),)
$(error Set ACHORDION_PATH to the `features` directory of the qmk-keymap repo, e.g. make test:achordion ACHORDION_PATH=/path/to/qmk-keymap/features)
endif

COMBO_ENABLE = yes

INTROSPECTION_KEYMAP_C = test_achordion_keymap.c

SRC += $(ACHORDION_PATH)/achordion.c
VPATH += $(ACHORDION_PATH)

# achordion.h emits a deprecation #warning; don't let -Werror trip on it.
EXTRAFLAGS += -Wno-error
