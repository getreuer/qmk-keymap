// Copyright 2021-2025 Google LLC
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
 * This is my Quantum Mechanical Keyboard (QMK) keymap. Who knew a keyboard
 * could do so much?
 *
 * This repo uses the Apache License 2.0 except where otherwise indicated. See
 * LICENSE.txt for details.
 *
 * For further documentation of this keymap's features, see
 * <https://getreuer.info/posts/keyboards>
 */

enum layers {
  BASE,
  SYM,
  NAV,
  NUM,
  WIN,
  FUN,
  EXT,
};

enum custom_keycodes {
  ARROW = SAFE_RANGE,
  UPDIR,
  STDCC,
  USRNAME,
  TMUXESC,
  SRCHSEL,
  RGBNEXT,
  RGBHUP,
  RGBHRND,
  RGBDEF1,
  RGBDEF2,
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
  M_NBSP,
  M_NOOP,
};

// The "magic" key is the Alternate Repeat Key.
#define MAGIC QK_AREP
// F20 mutes the mic on my system.
#define MUTEMIC KC_F20

// Short aliases for home row mods and other tap-hold keys.
#define HRM_S LALT_T(KC_S)
#define HRM_T LT(SYM, KC_T)
#define HRM_R LSFT_T(KC_R)
#define HRM_D LT(NAV, KC_D)
#define HRM_G LCTL_T(KC_G)
#define HRM_X LGUI_T(KC_X)

#define HRM_N LT(NUM, KC_N)
#define HRM_E RSFT_T(KC_E)
#define HRM_A LT(SYM, KC_A)
#define HRM_I LALT_T(KC_I)
#define HRM_H RCTL_T(KC_H)
#define HRM_DOT LT(WIN, KC_DOT)
#define HRM_QUO RGUI_T(KC_QUOT)

#define EXT_COL LT(EXT, KC_SCLN)
#define NAV_SLS LSFT_T(KC_SLSH)
#define NAV_EQL LT(0, KC_EQL)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // This keymap uses Ikcelaks' Magic Sturdy layout for the base layer (see
  // https://github.com/Ikcelaks/keyboard_layouts). I've also made some twists
  // of my own. The "magic" is a key whose function depends on the last pressed
  // key, implemented using the Alternate Repeat Key. This key is used to remove
  // the top SFBs and type common n-grams.
  [BASE] = LAYOUT_LR(  // Base layer: Magic Sturdy.
    KC_GRV , SELLINE, C(KC_V), C(KC_A), C(KC_C), MS_BTN1,
    KC_TAB , KC_V   , KC_M   , KC_L   , KC_C   , KC_P   ,
    KC_BSPC, HRM_S  , HRM_T  , HRM_R  , HRM_D  , KC_Y   ,
    EXT_COL, HRM_X  , KC_K   , KC_J   , HRM_G  , KC_W   ,
                                                 KC_UNDS, KC_SPC ,

                      KC_HOME, KC_LEFT, KC_RGHT, KC_END , G(KC_TAB), KC_MPLY,
                      KC_B   , MAGIC  , KC_U   , KC_O   , KC_Q   , KC_SLSH,
                      KC_F   , HRM_N  , HRM_E  , HRM_A  , HRM_I  , KC_MINS,
                      KC_Z   , HRM_H  , KC_COMM, HRM_DOT, HRM_QUO, KC_ENT ,
             QK_REP , KC_ESC
  ),

  [SYM] = LAYOUT_LR(  // Symbol layer.
    _______, _______, _______, _______, _______, _______,
    TMUXESC, KC_GRV , KC_LABK, KC_RABK, KC_MINS, KC_PIPE,
    _______, KC_EXLM, KC_ASTR, NAV_SLS, NAV_EQL, KC_AMPR,
    STDCC  , KC_TILD, KC_PLUS, KC_LBRC, KC_RBRC, KC_PERC,
                                                 USRNAME, _______,

                      _______, _______, _______, _______, _______, _______,
                      KC_CIRC, KC_LCBR, KC_RCBR, KC_DLR , ARROW  , _______,
                      KC_HASH, KC_LPRN, KC_RPRN, KC_SCLN, KC_DQUO, _______,
                      KC_AT  , KC_COLN, KC_COMM, KC_DOT , KC_QUOT, _______,
             _______, _______
  ),

  [NAV] = LAYOUT_LR(  // Navigation layer.
    _______, _______, _______, _______, _______, _______,
    _______, KC_WREF, C(KC_PGUP), C(KC_PGDN), XXXXXXX, XXXXXXX,
    _______, KC_LALT, KC_LCTL, KC_LSFT, SELLINE, MS_BTN1,
    _______, KC_LGUI, KC_PGUP, KC_PGDN, XXXXXXX, XXXXXXX,
                                                 KC_WBAK, G(KC_TAB),

                      _______, _______, _______, _______, _______, _______,
                      KC_PGUP, KC_HOME, KC_UP  , KC_END , SRCHSEL, _______,
                      KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, KC_DEL , _______,
                      C(KC_Z), SELWBAK, SELWORD, KC_APP , XXXXXXX, _______,
             _______, QK_LLCK
  ),

  [NUM] = LAYOUT_LR(  // Number layer.
    _______, _______, _______, _______, _______, _______,
    _______, KC_SLSH, KC_9   , KC_8   , KC_7   , KC_ASTR,
    _______, KC_MINS, KC_3   , KC_2   , KC_1   , KC_PLUS,
    _______, KC_X   , KC_6   , KC_5   , KC_4   , KC_PERC,
                                                 _______, _______,

                      _______, _______, _______, _______, _______, _______,
                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
                      XXXXXXX, XXXXXXX, KC_E   , KC_RCTL, KC_LALT, _______,
                      XXXXXXX, XXXXXXX, KC_COMM, KC_DOT , KC_LGUI, _______,
             KC_0   , QK_LLCK
  ),

  [WIN] = LAYOUT_LR(  // Window management layer.
    _______, _______, _______, _______, _______, _______,
    RGBDEF1, RGBDEF2, KC_MUTE, KC_VOLD, KC_VOLU, MUTEMIC,
    RGBHRND, RGBHUP , G(KC_3), G(KC_2), G(KC_1), G(KC_D),
    LUMINO , RGBNEXT, G(KC_6), G(KC_5), G(KC_4), G(KC_W),
                                                 KC_MPLY, G(KC_SPC),

                      _______, _______, _______, _______, _______, _______,
                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                      XXXXXXX, XXXXXXX, KC_RSFT, XXXXXXX, KC_LALT, XXXXXXX,
                      XXXXXXX, S(A(KC_TAB)), A(KC_TAB), XXXXXXX, XXXXXXX, G(KC_ENT),
             _______, QK_LLCK
  ),

  [FUN] = LAYOUT_LR(  // Funky fun layer.
    _______, _______, _______, _______, _______, _______,
    XXXXXXX, KC_F12 , KC_F9  , KC_F8  , KC_F7  , XXXXXXX,
    XXXXXXX, KC_F10 , KC_F3  , KC_F2  , KC_F1  , XXXXXXX,
    XXXXXXX, KC_F11 , KC_F6  , KC_F5  , KC_F4  , XXXXXXX,
                                                 XXXXXXX, DB_TOGG,

                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,
                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                      XXXXXXX, XXXXXXX, KC_RSFT, KC_RCTL, KC_LALT, XXXXXXX,
                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_RGUI, QK_RBT ,
             XXXXXXX, QK_LLCK
  ),

  [EXT] = LAYOUT_LR(  // Mouse and extras.
    _______, _______, _______, _______, _______, _______,
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    OM_SLOW, KC_LALT, KC_LCTL, KC_LSFT, SELLINE, XXXXXXX,
    _______, KC_LGUI, C(KC_V), C(KC_A), C(KC_C), C(KC_X),
                                                 KC_WBAK, OM_BTN1,

                      _______, _______, _______, _______, _______, _______,
                      OM_W_U , OM_BTN1, OM_U   , OM_BTN2, SRCHSEL, _______,
                      OM_W_D , OM_L   , OM_D   , OM_R   , OM_SLOW, _______,
                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
             OM_BTN1, QK_LLCK
  ),
};

// A cheap pseudorandom generator.
static uint8_t myrand(void) {
  static uint16_t state = 1;
#ifdef __CHIBIOS__  // Use high-res timer on ChibiOS.
  state += (uint16_t)chVTGetSystemTimeX();
#else
  state += timer_read();
#endif
  state *= UINT16_C(36563);
  return state >> 8;
}

static uint16_t get_tap_keycode(uint16_t keycode) {
  switch (keycode) {
    case QK_MOD_TAP ... QK_MOD_TAP_MAX:
      return QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
#ifndef NO_ACTION_LAYER
    case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
      return QK_LAYER_TAP_GET_TAP_KEYCODE(keycode);
#endif  // NO_ACTION_LAYER
  }
  return keycode;
}

///////////////////////////////////////////////////////////////////////////////
// Combos (https://docs.qmk.fm/features/combo)
///////////////////////////////////////////////////////////////////////////////
const uint16_t caps_combo[] PROGMEM = {KC_J, KC_COMM, COMBO_END};
const uint16_t j_k_combo[] PROGMEM = {KC_J, KC_K, COMBO_END};
const uint16_t comm_dot_combo[] PROGMEM = {KC_COMM, HRM_DOT, COMBO_END};
const uint16_t f_n_combo[] PROGMEM = {KC_F, HRM_N, COMBO_END};
// clang-format off
combo_t key_combos[] = {
    COMBO(caps_combo, CW_TOGG),          // J and , => activate Caps Word.
    COMBO(j_k_combo, KC_BSLS),           // J and K => backslash
    COMBO(comm_dot_combo, KC_SCLN),      // , and . => ;
    COMBO(f_n_combo, OSL(FUN)),          // F and N => FUN layer
};
// clang-format on

///////////////////////////////////////////////////////////////////////////////
// Custom shift keys (https://getreuer.info/posts/keyboards/custom-shift-keys)
///////////////////////////////////////////////////////////////////////////////
#ifdef COMMUNITY_MODULE_CUSTOM_SHIFT_KEYS_ENABLE
const custom_shift_key_t custom_shift_keys[] = {
    {HRM_DOT, KC_QUES},
    {KC_DOT, KC_QUES},
    {KC_COMM, KC_EXLM},
    {KC_MINS, KC_SCLN},
    {KC_SLSH, KC_BSLS},
    {KC_MPLY, KC_MNXT},
};
#endif  // COMMUNITY_MODULE_CUSTOM_SHIFT_KEYS_ENABLE

///////////////////////////////////////////////////////////////////////////////
// Tap-hold configuration (https://docs.qmk.fm/tap_hold)
///////////////////////////////////////////////////////////////////////////////
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t* record) {
  switch (keycode) {
    case HRM_R:
    case HRM_E:
      return TAPPING_TERM - 45;
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
    case HRM_N:
    case HRM_H:
      return QUICK_TAP_TERM;  // Enable key repeating.
    default:
      return 0;  // Otherwise, force hold and disable key repeating.
  }
}

#ifdef CHORDAL_HOLD
bool get_chordal_hold(
        uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record,
        uint16_t other_keycode, keyrecord_t* other_record) {
  switch (tap_hold_keycode) {
    case NAV_SLS:
      return true;

    case HRM_D:
      if (other_keycode == KC_M ||
          other_keycode == KC_L ||
          other_keycode == KC_Y ||
          other_keycode == KC_K ||
          other_keycode == KC_J) { return true; }
      break;

    case HRM_N:  // Allow one-handed N + Repeat chord to type "0" on num layer.
      if (other_keycode == QK_REP) { return true; }
      break;

    case HRM_DOT:
      if (other_keycode == HRM_H ||
          other_keycode == KC_COMM) { return true; }
  }
  return get_chordal_hold_default(tap_hold_record, other_record);
}
#endif  // CHORDAL_HOLD

#ifdef COMMUNITY_MODULE_TAP_FLOW_ENABLE
uint16_t get_tap_flow_term(uint16_t keycode, keyrecord_t* record,
                           uint16_t prev_keycode) {
  // Only apply Tap Flow when following a letter key.
  if (get_tap_keycode(prev_keycode) <= KC_Z) {
    switch (keycode) {
      case HRM_S:
      case HRM_X:
      case HRM_I:
      case HRM_QUO:
      case HRM_DOT:
        return g_tap_flow_term;

      case HRM_T:
      case HRM_D:
      case HRM_G:
      case HRM_N:
      case HRM_H:
      case HRM_A:
        return g_tap_flow_term - 25;
    }
  }

  return 0;
}
#endif  // COMMUNITY_MODULE_TAP_FLOW_ENABLE

///////////////////////////////////////////////////////////////////////////////
// Achordion (https://getreuer.info/posts/keyboards/achordion)
///////////////////////////////////////////////////////////////////////////////
#ifdef COMMUNITY_MODULE_ACHORDION_ENABLE
bool achordion_chord(uint16_t tap_hold_keycode,
                     keyrecord_t* tap_hold_record,
                     uint16_t other_keycode,
                     keyrecord_t* other_record) {
  // Also allow same-hand holds when the other key is in the rows outside the
  // alphas. I need the `% (MATRIX_ROWS / 2)` because my keyboards are split.
  const uint8_t row = other_record->event.key.row % (MATRIX_ROWS / 2);
  if (!(1 <= row && row <= 3)) { return true; }

  switch (tap_hold_keycode) {
    case HRM_D:
      if (other_keycode == KC_M ||
          other_keycode == KC_L ||
          other_keycode == KC_Y ||
          other_keycode == KC_K ||
          other_keycode == KC_J) { return true; }
      break;
    case HRM_DOT:
      if (other_keycode == KC_ENT) { return true; }
      break;
  }

  return achordion_opposite_hands(tap_hold_record, other_record);
}

uint16_t achordion_timeout(uint16_t tap_hold_keycode) {
  switch (tap_hold_keycode) {
    case EXT_COL:
      return 0;  // Disable Achordion for this key.
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
    case HRM_N:
      if (next_keycode == KC_C || next_keycode == KC_V) {
        return 0;
      }
      break;
    case HRM_D:
      if (next_keycode == HRM_N) {
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
#endif  // COMMUNITY_MODULE_ACHORDION_ENABLE

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
      add_weak_mods(MOD_BIT_LSHIFT);  // Apply shift to the next key.
      return true;

    // Keycodes that continue Caps Word, without shifting.
    case KC_1 ... KC_0:
    case KC_BSPC:
    case KC_DEL:
    case KC_UNDS:
    case KC_COLN:
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
#ifdef COMMUNITY_MODULE_SENTENCE_CASE_ENABLE
char sentence_case_press_user(uint16_t keycode, keyrecord_t* record,
                              uint8_t mods) {
  if ((mods & ~(MOD_MASK_SHIFT | MOD_BIT_RALT)) == 0) {
    const bool shifted = mods & MOD_MASK_SHIFT;
    switch (keycode) {
      case KC_A ... KC_Z:
      case M_THE:
      case M_ION:
      case M_MENT:
      case M_TMENT:
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
#endif  // COMMUNITY_MODULE_SENTENCE_CASE_ENABLE

///////////////////////////////////////////////////////////////////////////////
// Repeat key (https://docs.qmk.fm/features/repeat_key)
///////////////////////////////////////////////////////////////////////////////
bool remember_last_key_user(uint16_t keycode, keyrecord_t* record,
                            uint8_t* remembered_mods) {
  // Unpack tapping keycode for tap-hold keys.
  keycode = get_tap_keycode(keycode);

#ifdef COMMUNITY_MODULE_SENTENCE_CASE_ENABLE
  if (is_sentence_case_primed() &&
      sentence_case_press_user(keycode, record, *remembered_mods) == 'a') {
    *remembered_mods |= MOD_BIT_LSHIFT;
  }
#endif  // COMMUNITY_MODULE_SENTENCE_CASE_ENABLE

  // Forget Shift on most letters when Shift or AltGr are the only mods. Some
  // letters are excluded, e.g. for "NN" and "ZZ" in Vim.
  switch (keycode) {
    case KC_A ... KC_H:
    case KC_K ... KC_M:
    case KC_O ... KC_U:
      if ((*remembered_mods & ~(MOD_MASK_SHIFT | MOD_BIT_RALT)) == 0) {
        *remembered_mods &= ~MOD_MASK_SHIFT;
      }
      break;
  }

  return true;
}

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
//     T * @ -> TMENTS          (like "adjustments")
//     = *   -> ===             (JS code)
//     ! *   -> !==             (JS code)
//     " *   -> """<cursor>"""  (Python code)
//     ` *   -> ```<cursor>```  (Markdown code)
//     # *   -> #include        (C code)
//     & *   -> &nbsp;          (HTML code)
//     . *   -> ../             (shell)
//     . * @ -> ../../
uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {
  keycode = get_tap_keycode(keycode);

  if (mods == MOD_BIT_LALT) {
    switch (keycode) {
      case KC_U: return A(KC_O);
      case KC_O: return A(KC_U);
      case KC_N: return A(KC_I);
      case KC_I: return A(KC_N);
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
      case KC_N:
        if ((mods & MOD_MASK_SHIFT) == 0) {
          return S(KC_N);
        }
        return KC_N;

      // Fix SFBs and awkward strokes.
      case KC_A: return KC_O;         // A -> O
      case KC_O: return KC_A;         // O -> A
      case KC_E: return KC_U;         // E -> U
      case KC_U: return KC_E;         // U -> E
      case KC_I:
        if ((mods & MOD_MASK_SHIFT) == 0) {
          return M_ION;  // I -> ON
        } else {
          return KC_QUOT;  // Shift I -> '
        }
      case KC_M: return M_MENT;       // M -> ENT
      case KC_Q: return M_QUEN;       // Q -> UEN
      case KC_T: return M_TMENT;      // T -> TMENT

      case KC_C: return KC_Y;         // C -> Y
      case KC_D: return KC_Y;         // D -> Y
      case KC_G: return KC_Y;         // G -> Y
      case KC_P: return KC_Y;         // P -> Y
      case KC_Y: return KC_P;         // Y -> P

      case KC_L: return KC_K;         // L -> K
      case KC_S: return KC_K;         // S -> K

      case KC_R: return KC_L;         // R -> L
      case KC_DOT:
        if ((mods & MOD_MASK_SHIFT) == 0) {
          return M_UPDIR;  // . -> ./
        }
        return M_NOOP;
      case KC_HASH: return M_INCLUDE;  // # -> include
      case KC_AMPR: return M_NBSP;     // & -> nbsp;
      case KC_EQL: return M_EQEQ;      // = -> ==
      case KC_RBRC: return KC_SCLN;    // ] -> ;
      case KC_AT: return USRNAME;      // @ -> <username>

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

      case KC_PLUS:
      case KC_MINS:
      case KC_ASTR:
      case KC_PERC:
      case KC_PIPE:
      case KC_CIRC:
      case KC_TILD:
      case KC_EXLM:
      case KC_DLR:
      case KC_RABK:
      case KC_LPRN:
      case KC_RPRN:
      case KC_UNDS:
      case KC_COLN:
        return KC_EQL;

      case KC_F:
      case KC_V:
      case KC_X:
      case KC_SCLN:
      case KC_1 ... KC_0:
        return M_NOOP;
    }
  }

  switch (keycode) {
    case KC_WH_U: return KC_WH_D;
    case KC_WH_D: return KC_WH_U;
    case SELWBAK: return SELWORD;
    case SELWORD: return SELWBAK;
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
    register_mods(MOD_BIT_LSHIFT);
  }

  send_string_P(str);  // Send the string.
  set_last_keycode(repeat_keycode);

  // If Caps Word is on, restore the mods.
  if (is_caps_word_on()) {
    set_mods(saved_mods);
  }
}

///////////////////////////////////////////////////////////////////////////////
// RGB Matrix Lighting (https://docs.qmk.fm/features/rgb_matrix)
///////////////////////////////////////////////////////////////////////////////
#if RGB_MATRIX_ENABLE
static void lighting_set_palette(uint8_t palette) {
  if (lumino_get_value() == 0) {
    lumino_cycle_3_state();
  }
  rgb_matrix_enable_noeeprom();
  rgb_matrix_sethsv_noeeprom(
      RGB_MATRIX_HUE_STEP * palette, 255, rgb_matrix_get_val());
}

static void lighting_preset(uint8_t effect, uint8_t palette) {
  lighting_set_palette(palette);
  rgb_matrix_mode_noeeprom(effect);
  rgb_matrix_set_speed_noeeprom(100);
}
#endif  // RGB_MATRIX_ENABLE

///////////////////////////////////////////////////////////////////////////////
// Debug logging
///////////////////////////////////////////////////////////////////////////////
#if !defined(NO_DEBUG) && defined(COMMUNITY_MODULE_KEYCODE_STRING_ENABLE)
#pragma message "dlog_record: enabled"

KEYCODE_STRING_NAMES_USER(
  KEYCODE_STRING_NAME(ARROW),
  KEYCODE_STRING_NAME(UPDIR),
  KEYCODE_STRING_NAME(STDCC),
  KEYCODE_STRING_NAME(USRNAME),
  KEYCODE_STRING_NAME(TMUXESC),
  KEYCODE_STRING_NAME(SRCHSEL),
  KEYCODE_STRING_NAME(SELWORD),
  KEYCODE_STRING_NAME(SELWBAK),
  KEYCODE_STRING_NAME(SELLINE),
  KEYCODE_STRING_NAME(RGBNEXT),
  KEYCODE_STRING_NAME(RGBHUP),
  KEYCODE_STRING_NAME(RGBHRND),
  KEYCODE_STRING_NAME(RGBDEF1),
  KEYCODE_STRING_NAME(RGBDEF2),
);

static void dlog_record(uint16_t keycode, keyrecord_t* record) {
  if (!debug_enable) { return; }
  uint8_t layer = read_source_layers_cache(record->event.key);
  bool is_tap_hold = IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode);
  xprintf("L%-2u ", layer);  // Log the layer.
  if (IS_COMBOEVENT(record->event)) {  // Combos don't have a position.
    xprintf("combo   ");
  } else {  // Log the "(row,col)" position.
    xprintf("(%2u,%2u) ", record->event.key.row, record->event.key.col);
  }
  xprintf("%-4s %-7s %s\n",  // "(tap|hold) (press|release) <keycode>".
      is_tap_hold ? (record->tap.count ? "tap" : "hold") : "",
      record->event.pressed ? "press" : "release",
      get_keycode_string(keycode));
}
#else
#pragma message "dlog_record: disabled"
#define dlog_record(keycode, record)
#endif  // !defined(NO_DEBUG) && defined(COMMUNITY_MODULE_KEYCODE_STRING_ENABLE)

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
#if RGB_MATRIX_ENABLE
  lighting_preset(RGB_MATRIX_CUSTOM_PALETTEFX_FLOW + (myrand() % 4), myrand());
#endif // RGB_MATRIX_ENABLE

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
  dlog_record(keycode, record);

  // Track whether the left home ring and index keys are held, ignoring layer.
  static bool left_home_ring_held = false;
  static bool left_home_index_held = false;
  if (record->event.key.row == LEFT_HOME_ROW) {
    switch (record->event.key.col) {
      case LEFT_HOME_RING_COL:
        left_home_ring_held = record->event.pressed;
        break;
      case LEFT_HOME_INDEX_COL:
        left_home_index_held = record->event.pressed;
        break;
    }

    // NAV stays on while layer locked or while either ring or index is held.
    if (!(is_layer_locked(NAV) ||
          left_home_ring_held || left_home_index_held)) {
      layer_off(NAV);
    }
  }

  // Logic for Alt mod when using alt-tabbing keys.
  if (keycode == HRM_DOT && record->tap.count == 0 && !record->event.pressed) {
    unregister_mods(MOD_BIT_LALT);
  } else if (record->event.pressed &&
      (keycode == S(A(KC_TAB)) || keycode == A(KC_TAB))) {
    register_mods(MOD_BIT_LALT);
  }

  const uint8_t mods = get_mods();
  const uint8_t all_mods = (mods | get_weak_mods()
#ifndef NO_ACTION_ONESHOT
                        | get_oneshot_mods()
#endif  // NO_ACTION_ONESHOT
  );
  const uint8_t shift_mods = all_mods & MOD_MASK_SHIFT;
  const bool alt = all_mods & MOD_BIT_LALT;
  const uint8_t layer = read_source_layers_cache(record->event.key);

  if (layer == SYM && record->event.pressed) {
    clear_weak_mods();
    send_keyboard_report();
  }

  // If alt repeating key A, E, I, O, U, Y with no mods other than Shift, set
  // the last key to KC_N. Above, alternate repeat of KC_N is defined to be
  // again KC_N. This way, either tapping alt repeat and then repeat (or
  // equivalently double tapping alt repeat) is useful to type certain patterns
  // without SFBs:
  //
  //   D <altrep> <rep> -> DYN (as in "dynamic")
  //   O <altrep> <rep> -> OAN (as in "loan")
  if (get_repeat_key_count() < 0 && (all_mods & ~MOD_MASK_SHIFT) == 0 &&
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

    // Hold behavior: switches to EXT layer.
    // Tap behavior:
    //  * Unmodified:       :
    //  * With Shift:       ;
    case EXT_COL:
      if (record->tap.count) {
        if (record->event.pressed) {
          if (shift_mods) {
            del_weak_mods(MOD_MASK_SHIFT);
            unregister_mods(MOD_MASK_SHIFT);
            tap_code_delay(KC_SCLN, TAP_CODE_DELAY);
            set_mods(mods);
          } else {
            tap_code16_delay(KC_COLN, TAP_CODE_DELAY);
          }
        }
        return false;
      }
      return true;

    case HRM_T:
      if (!record->tap.count) {
        if (record->event.pressed) {
          if ((mods & MOD_BIT_LSHIFT) != 0) {
            register_mods(MOD_BIT_LCTRL);
            layer_on(NAV);
          } else {
            layer_on(SYM);
          }
        } else {
          unregister_mods(MOD_BIT_LCTRL);
          layer_off(SYM);
        }
        return false;
      }
      return true;

    case NAV_SLS:
      if (!record->tap.count) {
        if (!record->event.pressed) {
          unregister_mods(MOD_BIT_LSHIFT);
        } else if (left_home_ring_held) {
          register_mods(MOD_BIT_LCTRL | MOD_BIT_LSHIFT);
          layer_on(NAV);
        }
        return false;
      }
      return true;  // Default handling taps /.

    case NAV_EQL:
      if (!record->tap.count) {
        if (left_home_ring_held && record->event.pressed) {
          register_mods(MOD_BIT_LCTRL);
          layer_on(NAV);
        }
        return false;
      }
      return true;

    case HRM_D:  // NAV switch.
      if (!record->tap.count) {
        if (record->event.pressed) {
          layer_on(NAV);
        }
        return false;
      }
      return true;

    case G(KC_TAB):
      lumino_sleep_soon();
      return true;
  }

  if (record->event.pressed) {
    switch (keycode) {
      case UPDIR:
        SEND_STRING_DELAY("../", TAP_CODE_DELAY);
        return false;

      case STDCC:
        SEND_STRING_DELAY("std::", TAP_CODE_DELAY);
        return false;

      case TMUXESC:  // Enter copy mode in Tmux.
        SEND_STRING_DELAY(SS_LCTL("a") SS_TAP(X_ESC), TAP_CODE_DELAY);
        set_last_keycode(C(KC_U));
        return false;

      case SRCHSEL:  // Searches the current selection in a new tab.
        // Mac users, change LCTL to LGUI.
        SEND_STRING_DELAY(
            SS_LCTL("ct") SS_DELAY(100) SS_LCTL("v") SS_TAP(X_ENTER),
            TAP_CODE_DELAY);
        return false;

      case USRNAME:
        add_oneshot_mods(shift_mods);
        clear_weak_mods();
        MAGIC_STRING("getreuer", KC_AT);
        return false;

      case ARROW:  // Unicode arrows -> => <-> <=> through Shift and Alt.
        send_unicode_string(alt ? (shift_mods
                                    ? "\xe2\x87\x94"     // <=>
                                    : "\xe2\x86\x94")    // <->
                                 : (shift_mods
                                    ? "\xe2\x87\x92"     // =>
                                    : "\xe2\x86\x92"));  // ->
        return false;

      case KC_RABK:
        if (shift_mods) {  // Shift + > types a happy emoji.
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
          uint8_t index = ((NUM_EMOJIS - 1) * myrand()) >> 8;
          // Don't pick the same emoji twice in a row.
          static uint8_t last_index = 0;
          if (index >= last_index) { ++index; }
          last_index = index;

          // Produce the emoji.
          send_unicode_string(emojis[index]);
          return false;
        }
        return true;

      // Macros invoked through the MAGIC key.
      case M_THE:     MAGIC_STRING(/* */"the", KC_N); break;
      case M_ION:     MAGIC_STRING(/*i*/"on", KC_S); break;
      case M_MENT:    MAGIC_STRING(/*m*/"ent", KC_S); break;
      case M_QUEN:    MAGIC_STRING(/*q*/"uen", KC_C); break;
      case M_TMENT:   MAGIC_STRING(/*t*/"ment", KC_S); break;
      case M_UPDIR:   MAGIC_STRING(/*.*/"./", UPDIR); break;
      case M_INCLUDE: SEND_STRING_DELAY(/*#*/"include ", TAP_CODE_DELAY); break;
      case M_EQEQ:    SEND_STRING_DELAY(/*=*/"==", TAP_CODE_DELAY); break;
      case M_NBSP:    SEND_STRING_DELAY(/*&*/"nbsp;", TAP_CODE_DELAY); break;

      case M_DOCSTR:
        SEND_STRING_DELAY(/*"*/"\"\"\"\"\""
            SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT), TAP_CODE_DELAY);
        break;
      case M_MKGRVS:
        SEND_STRING_DELAY(/*`*/"``\n\n```" SS_TAP(X_UP), TAP_CODE_DELAY);
        break;

#if RGB_MATRIX_ENABLE
      case RGBNEXT:
        if (shift_mods) {
          rgb_matrix_step_reverse_noeeprom();
        } else {
          rgb_matrix_step_noeeprom();
        }
        break;

      case RGBHUP:
        if (shift_mods) {
          rgb_matrix_decrease_hue_noeeprom();
        } else {
          rgb_matrix_increase_hue_noeeprom();
        }
        break;

      case RGBHRND:
        lighting_set_palette(myrand());
        break;

      case RGBDEF1:
        lighting_preset(RGB_MATRIX_CUSTOM_PALETTEFX_RIPPLE, PALETTEFX_CARNIVAL);
        break;

      case RGBDEF2:
        lighting_preset(RGB_MATRIX_CUSTOM_PALETTEFX_FLOW, PALETTEFX_POLARIZED);
        break;
#endif  // RGB_MATRIX_ENABLE
    }
  }

  return true;
}

