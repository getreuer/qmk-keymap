// Copyright 2024 Google LLC
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
 * @file keycode_string.c
 * @brief keycode_string implementation
 *
 * @note When parsing keycodes, avoid hardcoded numerical codes or twiddling
 * bits directly, use QMK's APIs instead. Keycode encoding is internal to QMK
 * and may change between versions.
 *
 * For full documentation, see
 * <https://getreuer.info/posts/keyboards/keycode-string>
 */

#include "keycode_string.h"

#include "quantum.h"

/** Names of some common keycodes. */
static const keycode_string_name_t keycode_names[] = {
    {KC_ENT, "KC_ENT"},   {KC_ESC, "KC_ESC"},
    {KC_BSPC, "KC_BSPC"}, {KC_TAB, "KC_TAB"},
    {KC_SPC, "KC_SPC"},   {KC_MINS, "KC_MINS"},
    {KC_EQL, "KC_EQL"},   {KC_LBRC, "KC_LBRC"},
    {KC_RBRC, "KC_RBRC"}, {KC_BSLS, "KC_BSLS"},
    {KC_SCLN, "KC_SCLN"}, {KC_QUOT, "KC_QUOT"},
    {KC_GRV, "KC_GRV"},   {KC_COMM, "KC_COMM"},
    {KC_DOT, "KC_DOT"},   {KC_SLSH, "KC_SLSH"},
    {KC_INS, "KC_INS"},   {KC_HOME, "KC_HOME"},
    {KC_PGUP, "KC_PGUP"}, {KC_DEL, "KC_DEL"},
    {KC_END, "KC_END"},   {KC_PGDN, "KC_PGDN"},
    {KC_RGHT, "KC_RGHT"}, {KC_LEFT, "KC_LEFT"},
    {KC_DOWN, "KC_DOWN"}, {KC_UP, "KC_UP"},
#ifdef TRI_LAYER_ENABLE
    {TL_LOWR, "TL_LOWR"}, {TL_UPPR, "TL_UPPR"},
#endif  // TRI_LAYER_ENABLE
#ifdef GRAVE_ESC_ENABLE
    {QK_GESC, "QK_GESC"},
#endif  // GRAVE_ESC_ENABLE
#ifdef CAPS_WORD_ENABLE
    {CW_TOGG, "CW_TOGG"},
#endif  // CAPS_WORD_ENABLE
#ifdef LAYER_LOCK_ENABLE
    {QK_LLCK, "QK_LLCK"},
#endif  // LAYER_LOCK_ENABLE
    {DB_TOGG, "DB_TOGG"},

    // End of table sentinel.
    {0, NULL},
};
/** Users can override this to define names of additional keycodes. */
__attribute__((weak)) const keycode_string_name_t custom_keycode_names[] = {
    {0, NULL},
};
/** Names of the 4 mods on each hand. */
static const char* mod_names[4] = {"CTL", "SFT", "ALT", "GUI"};

/** Internal buffer for holding a stringified keycode. */
static char buffer[32];

/**
 * @brief Finds the name of a keycode in `table` or returns NULL.
 *
 * The last entry of the table must be {0, NULL}.
 *
 * @param table   A table of keycode_string_name_t to be searched.
 * @return Name string for the keycode, or NULL if not found.
 */
static const char* find_keycode_name(const keycode_string_name_t* table,
                                     uint16_t keycode) {
  for (; table->keycode; ++table) {
    if (table->keycode == keycode) {
      return table->name;
    }
  }
  return NULL;
}

/**
 * @brief Copies null-terminated str to dest of up to 16 chars.
 *
 * @note Null terminator is *not* copied.
 *
 * @param dest  Destination start pointer.
 * @param str   Null-terminated string of up to 16 chars.
 * @return Pointer to one char past the end of the copied string.
 */
static char* write_string(char* dest, const char* str) {
  for (int8_t i = 0; i < 16 && str[i]; ++i) {
    *dest++ = str[i];
  }
  return dest;
}

/** Converts lowest 4 bits of value to a hex digit char. */
static char hex_digit(uint8_t value) { return "0123456789ABCDEF"[value & 15]; }

/**
 * @brief Writes a uint16 value as hexadecimal to dest.
 *
 * @note Null terminator is *not* written.
 *
 * @param dest   Destination start pointer.
 * @param value  Integer value.
 * @return Pointer to one char past the end of the written string.
 */
static char* write_hex_uint16(char* dest, uint16_t value) {
  dest = write_string(dest, "0x");
  if (value > 255) {
    *dest++ = hex_digit(value >> 12);
    *dest++ = hex_digit(value >> 8);
  }
  *dest++ = hex_digit(value >> 4);
  *dest++ = hex_digit(value);
  return dest;
}

/**
 * @brief Writes a uint8 value as a decimal to dest.
 *
 * @note Null terminator is *not* written.
 *
 * @param dest   Destination start pointer.
 * @param value  Integer value in 0-255.
 * @return Pointer to one char past the end of the written string.
 */
static char* write_decimal_uint8(char* dest, uint8_t value) {
  const int8_t num_digits = (value < 10) ? 1 : ((value < 100) ? 2 : 3);
  dest += num_digits;
  for (int8_t i = 1; i <= num_digits; ++i) {
    uint8_t digit = value % 10;
    value /= 10;
    dest[-i] = '0' + digit;
  }
  return dest;
}

/**
 * @brief Writes a keycode of the format `name + param + ')'`.
 *
 * @note Null terminator is *not* written.
 *
 * @param dest   Destination start pointer.
 * @param name   Name string to be written, including a '('.
 * @param param  Unary parameter, formatted as a decimal integer 0-255.
 * @return Pointer to one char past the end of the written string.
 */
static char* write_unary_keycode(char* dest, const char* name, uint8_t param) {
  dest = write_string(dest, name);
  dest = write_decimal_uint8(dest, param);
  *dest++ = ')';
  return dest;
}

/**
 * @brief Writes 5-bit mods in the format "MOD_*".
 *
 * This implementation only covers the case where one mod is applied. Otherwise,
 * the mod is written numerically as a hex value.
 *
 * @note Null terminator is *not* written.
 *
 * @param dest   Destination start pointer.
 * @param mods   Modifier in 5-bit encoding.
 * @return Pointer to one char past the end of the written string.
 */
static char* write_mods(char* dest, uint8_t mods) {
  const bool is_rhs = mods > 15;
  mods &= 15;
  if (mods != 0 && (mods & (mods - 1)) == 0) {  // One mod is set.
    dest = write_string(dest, "MOD_");
    *dest++ = is_rhs ? 'R' : 'L';
    dest = write_string(dest, mod_names[biton(mods)]);
  } else {  // Fallback: write the mod as a hex value.
    dest = write_hex_uint16(dest, mods);
  }
  return dest;
}

/**
 * @brief Writes a keycode to dest.
 *
 * @note Null terminator is *not* written.
 *
 * @param dest     Destination start pointer.
 * @param keycode  QMK keycode.
 * @return Pointer to one char past the end of the written string.
 */
static char* write_keycode(char* dest, uint16_t keycode) {
  // Search the `custom_keycode_names` table first so that it is possible to
  // override how any keycode would be formatted otherwise.
  const char* keycode_name = find_keycode_name(custom_keycode_names, keycode);
  if (keycode_name) {
    return write_string(dest, keycode_name);
  }
  // Search the `keycode_names` table.
  keycode_name = find_keycode_name(keycode_names, keycode);
  if (keycode_name) {
    return write_string(dest, keycode_name);
  }

  if (keycode <= 255) {  // Basic keycodes.
    switch (keycode) {
      // Modifiers KC_LSFT, KC_RCTL, etc.
      case MODIFIER_KEYCODE_RANGE: {
        const uint8_t i = keycode - KC_LCTL;
        const bool is_rhs = i > 3;
        dest = write_string(dest, "KC_");
        *dest++ = is_rhs ? 'R' : 'L';
        dest = write_string(dest, mod_names[i & 3]);
      }
        return dest;

      // Letters A-Z.
      case KC_A ... KC_Z:
        dest = write_string(dest, "KC_");
        *dest++ = (char)(keycode + (UINT8_C('A') - KC_A));
        return dest;

      // Digits 0-9 (NOTE: Unlike the ASCII order, KC_0 comes *after* KC_9.)
      case KC_1 ... KC_0:
        dest = write_string(dest, "KC_");
        *dest++ = '0' + (char)((keycode - KC_1 + 1) % 10);
        return dest;

      // Keypad digits.
      case KC_KP_1 ... KC_KP_0:
        dest = write_string(dest, "KC_KP_");
        *dest++ = '0' + (char)((keycode - KC_KP_1 + 1) % 10);
        return dest;

      // Function keys. F1-F12 and F13-F24 are coded in separate ranges.
      case KC_F1 ... KC_F12:
        dest = write_string(dest, "KC_F");
        dest = write_decimal_uint8(dest, keycode - KC_F1 + 1);
        return dest;

      case KC_F13 ... KC_F24:
        dest = write_string(dest, "KC_F");
        dest = write_decimal_uint8(dest, keycode - KC_F13 + 13);
        return dest;
    }
  }

  switch (keycode) {
    // A modified keycode, like S(KC_1) for Shift + 1 = !. This implementation
    // only covers modified keycodes where one modifier is applied, e.g. a
    // Ctrl + Shift + kc or Hyper + kc keycode is not formatted.
    case QK_MODS ... QK_MODS_MAX: {
      uint8_t mods = QK_MODS_GET_MODS(keycode);
      const bool is_rhs = mods > 15;
      mods &= 15;
      if (mods != 0 && (mods & (mods - 1)) == 0) {  // One mod is set.
        const char* name = mod_names[biton(mods)];
        if (is_rhs) {
          *dest++ = 'R';
          dest = write_string(dest, name);
        } else {
          *dest++ = name[0];
        }
        *dest++ = '(';
        dest = write_keycode(dest, QK_MODS_GET_BASIC_KEYCODE(keycode));
        *dest++ = ')';
        return dest;
      }
    } break;

    // One-shot mod OSM(mod) key.
    case QK_ONE_SHOT_MOD ... QK_ONE_SHOT_MOD_MAX:
      dest = write_string(dest, "OSM(");
      dest = write_mods(dest, QK_ONE_SHOT_MOD_GET_MODS(keycode));
      *dest++ = ')';
      return dest;

    // Various layer switch keys.
    case QK_MOMENTARY ... QK_MOMENTARY_MAX:  // MO(layer) key.
      return write_unary_keycode(dest, "MO(", QK_MOMENTARY_GET_LAYER(keycode));

    case QK_TO ... QK_TO_MAX:  // TO(layer) key.
      return write_unary_keycode(dest, "TO(", QK_TO_GET_LAYER(keycode));

    case QK_TOGGLE_LAYER ... QK_TOGGLE_LAYER_MAX:  // TG(layer) key.
      return write_unary_keycode(dest, "TG(",
                                 QK_TOGGLE_LAYER_GET_LAYER(keycode));

    case QK_ONE_SHOT_LAYER ... QK_ONE_SHOT_LAYER_MAX:  // OSL(layer) key.
      return write_unary_keycode(dest, "OSL(",
                                 QK_ONE_SHOT_LAYER_GET_LAYER(keycode));

    case QK_LAYER_MOD ... QK_LAYER_MOD_MAX:  // LM(layer,mod) key.
      dest = write_string(dest, "LM(");
      dest = write_decimal_uint8(dest, QK_LAYER_MOD_GET_LAYER(keycode));
      *dest++ = ',';
      dest = write_mods(dest, QK_LAYER_MOD_GET_MODS(keycode));
      *dest++ = ')';
      return dest;

    case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:  // Layer-tap LT(layer,kc) key.
      dest = write_string(dest, "LT(");
      dest = write_decimal_uint8(dest, QK_LAYER_TAP_GET_LAYER(keycode));
      *dest++ = ',';
      dest = write_keycode(dest, QK_LAYER_TAP_GET_TAP_KEYCODE(keycode));
      *dest++ = ')';
      return dest;

    // Mod-tap MT(mod,kc) key. This implementation formats the MT keys where one
    // modifier is applied. For MT keys with multiple modifiers, the mod arg is
    // written numerically as a hex code.
    case QK_MOD_TAP ... QK_MOD_TAP_MAX: {
      uint8_t mods = mod_config(QK_MOD_TAP_GET_MODS(keycode));
      const bool is_rhs = mods > 15;
      mods &= 15;
      if (mods != 0 && (mods & (mods - 1)) == 0) {  // One mod is set.
        *dest++ = is_rhs ? 'R' : 'L';
        dest = write_string(dest, mod_names[biton(mods)]);
        dest = write_string(dest, "_T(");
      } else {  // Fallback: write the mod as a hex value.
        dest = write_string(dest, "MT(");
        dest = write_hex_uint16(dest, mods);
        *dest++ = ',';
      }
      dest = write_keycode(dest, QK_MOD_TAP_GET_TAP_KEYCODE(keycode));
      *dest++ = ')';
    }
      return dest;

#ifdef TAP_DANCE_ENABLE
    case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:  // Tap dance TD(i) key.
      return write_unary_keycode(dest, "TD(", QK_TAP_DANCE_GET_INDEX(keycode));
#endif  // TAP_DANCE_ENABLE

#ifdef UNICODE_ENABLE
    case QK_UNICODE ... QK_UNICODE_MAX:  // Unicode UC(codepoint) key.
      dest = write_string(dest, "UC(");
      dest = write_hex_uint16(dest, QK_UNICODE_GET_CODE_POINT(keycode));
      *dest++ = ')';
      return dest;
#elif defined(UNICODEMAP_ENABLE)
    case QK_UNICODEMAP ... QK_UNICODEMAP_MAX:  // Unicode Map UM(i) key.
      return write_unary_keycode(dest, "UM(", QK_UNICODEMAP_GET_INDEX(keycode));

    case QK_UNICODEMAP_PAIR ... QK_UNICODEMAP_PAIR_MAX: {  // UP(i,j) key.
      const uint8_t i = QK_UNICODEMAP_PAIR_GET_UNSHIFTED_INDEX(keycode);
      const uint8_t j = QK_UNICODEMAP_PAIR_GET_SHIFTED_INDEX(keycode);
      dest = write_string(dest, "UP(");
      dest = write_decimal_uint8(dest, i);
      *dest++ = ',';
      dest = write_decimal_uint8(dest, j);
      *dest++ = ')';
    }
      return dest;
#endif

    case KB_KEYCODE_RANGE:  // Keyboard range keycode.
      dest = write_string(dest, "QK_KB_");
      dest = write_decimal_uint8(dest, keycode - QK_KB_0);
      return dest;

    case USER_KEYCODE_RANGE:  // User range keycode.
      dest = write_string(dest, "QK_USER_");
      dest = write_decimal_uint8(dest, keycode - QK_USER_0);
      return dest;
  }

  // Fallback: write the keycode as a hex value.
  dest = write_hex_uint16(dest, keycode);
  return dest;
}

char* keycode_string(uint16_t keycode) {
  char* dest = write_keycode(buffer, keycode);
  *dest = '\0';
  return buffer;
}
