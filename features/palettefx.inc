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
 * @file palettefx.inc
 * @brief PaletteFx: Add more colors to your keyboard
 *
 * While most of QMK's built-in RGB matrix effects are based on a single color
 * hue, sampling from a palette of colors allows for more personality. PaletteFx
 * is a suite of custom effects for QMK's RGB Matrix in which the effect colors
 * are sampled from a palette, aka color gradient or color map.
 *
 * PaletteFx includes 16 palettes and 6 effects, with the possibility to define
 * additional palettes and effects.
 *
 *
 * For full documentation, see
 * <https://getreuer.info/posts/keyboards/palettefx>
 */

#if defined(PALETTEFX_ENABLE_ALL_EFFECTS) || defined(PALETTEFX_GRADIENT_ENABLE)
RGB_MATRIX_EFFECT(PALETTEFX_GRADIENT)
#endif
#if defined(PALETTEFX_ENABLE_ALL_EFFECTS) || defined(PALETTEFX_FLOW_ENABLE)
RGB_MATRIX_EFFECT(PALETTEFX_FLOW)
#endif
#if defined(PALETTEFX_ENABLE_ALL_EFFECTS) || defined(PALETTEFX_RIPPLE_ENABLE)
RGB_MATRIX_EFFECT(PALETTEFX_RIPPLE)
#endif
#if defined(PALETTEFX_ENABLE_ALL_EFFECTS) || defined(PALETTEFX_SPARKLE_ENABLE)
RGB_MATRIX_EFFECT(PALETTEFX_SPARKLE)
#endif
#if defined(PALETTEFX_ENABLE_ALL_EFFECTS) || defined(PALETTEFX_VORTEX_ENABLE)
RGB_MATRIX_EFFECT(PALETTEFX_VORTEX)
#endif
#if defined(RGB_MATRIX_KEYREACTIVE_ENABLED) && ( \
    defined(PALETTEFX_ENABLE_ALL_EFFECTS) || defined(PALETTEFX_REACTIVE_ENABLE))
RGB_MATRIX_EFFECT(PALETTEFX_REACTIVE)
#endif

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#if !(defined(PALETTEFX_ENABLE_ALL_EFFECTS) || \
      defined(PALETTEFX_GRADIENT_ENABLE) || \
      defined(PALETTEFX_FLOW_ENABLE) || \
      defined(PALETTEFX_RIPPLE_ENABLE) || \
      defined(PALETTEFX_SPARKLE_ENABLE) || \
      defined(PALETTEFX_VORTEX_ENABLE) || \
      (defined(RGB_MATRIX_KEYREACTIVE_ENABLED) && \
       defined(PALETTEFX_REACTIVE_ENABLE)))
#pragma message \
    "palettefx: No palettefx effects are enabled. Enable all effects by adding in config.h `#define PALETTEFX_ENABLE_ALL_EFFECTS`, or enable individual effects with `#define PALETTE_<name>_ENABLE`."
#endif

///////////////////////////////////////////////////////////////////////////////
// PaletteFx function definitions
///////////////////////////////////////////////////////////////////////////////

/** Gets the selected palette. */
static const uint16_t* palettefx_get_palette_data(void);

/**
 * @brief Computes the interpolated HSV palette color at 0 <= x < 256.
 *
 * Looks up and linearly interpolates `palette` at 0 <= x < 256. The color
 * saturation and value are scaled according to rgb_matrix_config.
 *
 * @param palette A table of size 16, each entry being a 16-bit HSV color.
 * @param x       Palette lookup position, a value in 0-255.
 * @return HSV color.
 */
static hsv_t palettefx_interp_color(const uint16_t* palette, uint8_t x);

/** Gets the color data for the selected palette. */
static const uint16_t* palettefx_get_palette_data(void);

/**
 * @brief Compute a scaled 16-bit time that wraps smoothly.
 *
 * Computes `timer` scaled by `scale`, returning the result as a uint16.
 * Generally, the scaled time would have a discontinuity every ~65 seconds when
 * `timer` wraps 16-bit range. This is corrected for to wrap smoothly mod 2^16.
 *
 * @param timer A 32-bit timer.
 * @param scale Scale value in 0-255.
 * @return Scaled time.
 */
inline static uint16_t palettefx_scaled_time(uint32_t timer, uint8_t scale);

///////////////////////////////////////////////////////////////////////////////
// PaletteFx effects
///////////////////////////////////////////////////////////////////////////////

#if defined(PALETTEFX_ENABLE_ALL_EFFECTS) || defined(PALETTEFX_GRADIENT_ENABLE)
// "Gradient" static effect. This is essentially a palette-colored version of
// RGB_MATRIX_GRADIENT_UP_DOWN. A vertically-sloping gradient is made, with the
// highest color on the top keys of keyboard and the lowest color at the bottom.
static bool PALETTEFX_GRADIENT(effect_params_t* params) {
  // On first call, compute and cache the slope of the gradient.
  static uint8_t gradient_slope = 0;
  if (!gradient_slope) {
    uint8_t y_max = 64;  // To avoid overflow below, x_max must be at least 64.
    for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; ++i) {
      if (g_led_config.point[i].y > y_max) {
        y_max = g_led_config.point[i].y;
      }
    }
    // Compute the quotient `255 / y_max` with 6 fractional bits and rounding.
    gradient_slope = (64 * 255 + y_max / 2) / y_max;
  }

  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  const uint16_t* palette = palettefx_get_palette_data();

  for (uint8_t i = led_min; i < led_max; ++i) {
    RGB_MATRIX_TEST_LED_FLAGS();
    const uint8_t y = g_led_config.point[i].y;
    const uint8_t value = 255 - (((uint16_t)y * (uint16_t)gradient_slope) >> 6);
    rgb_t rgb = rgb_matrix_hsv_to_rgb(palettefx_interp_color(palette, value));
    rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
  }

  return rgb_matrix_check_finished_leds(led_max);
}
#endif

#if defined(PALETTEFX_ENABLE_ALL_EFFECTS) || defined(PALETTEFX_FLOW_ENABLE)
// "Flow" animated effect. Draws moving wave patterns mimicking the appearance
// of flowing liquid. For interesting variety of patterns, space coordinates are
// slowly rotated and a function of several sine waves is evaluated.
static bool PALETTEFX_FLOW(effect_params_t* params) {
  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  const uint16_t* palette = palettefx_get_palette_data();
  const uint16_t time =
      palettefx_scaled_time(g_rgb_timer, 1 + rgb_matrix_config.speed / 8);
  // Compute rotation coefficients with 7 fractional bits.
  const int8_t rot_c = cos8(time / 4) - 128;
  const int8_t rot_s = sin8(time / 4) - 128;
  const uint8_t omega = 32 + sin8(time) / 4;

  for (uint8_t i = led_min; i < led_max; ++i) {
    RGB_MATRIX_TEST_LED_FLAGS();
    const uint8_t x = g_led_config.point[i].x;
    const uint8_t y = g_led_config.point[i].y;

    // Rotate (x, y) by the 2x2 rotation matrix described by rot_c, rot_s.
    const uint8_t x1 = (uint8_t)((((int16_t)rot_c) * ((int16_t)x)) / 128)
        - (uint8_t)((((int16_t)rot_s) * ((int16_t)y)) / 128);
    const uint8_t y1 = (uint8_t)((((int16_t)rot_s) * ((int16_t)x)) / 128)
        + (uint8_t)((((int16_t)rot_c) * ((int16_t)y)) / 128);

    uint8_t value = scale8(sin8(x1 - 2 * time), omega) + y1 + time / 4;
    // Evaluate `sawtooth(value)`.
    value = 2 * ((value <= 127) ? value : (255 - value));

    rgb_t rgb = rgb_matrix_hsv_to_rgb(palettefx_interp_color(palette, value));
    rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
  }

  return rgb_matrix_check_finished_leds(led_max);
}
#endif

#if defined(PALETTEFX_ENABLE_ALL_EFFECTS) || defined(PALETTEFX_RIPPLE_ENABLE)
// "Ripple" animated effect. Draws circular rings emanating from random points,
// simulating water drops falling in a quiet pool.
static bool PALETTEFX_RIPPLE(effect_params_t* params) {
  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  const uint16_t* palette = palettefx_get_palette_data();

  // Each instance of this struct represents one water drop. For efficiency, at
  // most 3 drops are active at any time.
  static struct {
    uint16_t time;
    uint8_t x;
    uint8_t y;
    uint8_t amplitude;
    uint8_t scale;
    uint8_t phase;
  } drops[3];
  static uint32_t drop_timer = 0;
  static uint8_t drops_tail = 0;

  if (params->iter == 0) {
    if (params->init) {
      for (uint8_t j = 0; j < 3; ++j) {
        drops[j].amplitude = 0;
      }
      drop_timer = g_rgb_timer;
    }

    if (drops[drops_tail].amplitude == 0 &&
        timer_expired32(g_rgb_timer, drop_timer)) {
      // Spawn a new drop, located at a random LED.
      const uint8_t i = random8_max(RGB_MATRIX_LED_COUNT);
      drops[drops_tail].time = (uint16_t)g_rgb_timer;
      drops[drops_tail].x = g_led_config.point[i].x;
      drops[drops_tail].y = g_led_config.point[i].y;
      drops[drops_tail].amplitude = 1;
      ++drops_tail;
      if (drops_tail == 3) { drops_tail = 0; }
      drop_timer = g_rgb_timer + 1000;
    }

    uint8_t amplitude(uint8_t t) {  // Drop amplitude as a function of time.
      if (t <= 55) {
        return (t < 32) ? (3 + 5 * t) : 192;
      } else {
        t = (((uint16_t)(255 - t)) * UINT16_C(123)) >> 7;
        return scale8(t, t);
      }
    }

    for (uint8_t j = 0; j < 3; ++j) {
      if (drops[j].amplitude == 0) { continue; }
      const uint16_t tick = scale16by8(g_rgb_timer - drops[j].time,
          1 + rgb_matrix_config.speed / 4);
      if (tick < 4 * 255) {
        const uint8_t t = (uint8_t)(tick / 4);
        drops[j].amplitude = amplitude(t);
        drops[j].scale = 255 / (1 + t / 2);
        drops[j].phase = (uint8_t)tick;
      } else {
        drops[j].amplitude = 0;  // Animation for this drop is complete.
      }
    }
  }

  for (uint8_t i = led_min; i < led_max; ++i) {
    RGB_MATRIX_TEST_LED_FLAGS();
    int16_t value = 128;

    for (uint8_t j = 0; j < 3; ++j) {
      if (drops[j].amplitude == 0) { continue; }

      const uint8_t x = abs8((g_led_config.point[i].x - drops[j].x) / 2);
      const uint8_t y = abs8((g_led_config.point[i].y - drops[j].y) / 2);
      const uint8_t r = sqrt16(x * x + y * y);
      const uint16_t r_scaled = (uint16_t)r * (uint16_t)drops[j].scale;

      if (r_scaled < 255) {
        // The drop is made from a radial sine wave modulated by a smooth bump
        // to localize its spatial extent.
        const uint8_t bump = scale8(ease8InOutApprox(255 - (uint8_t)r_scaled),
                                    drops[j].amplitude);
        const int8_t wave = (int16_t)cos8(8 * r - drops[j].phase) - 128;
        value += ((int16_t)wave * (int16_t)bump) / 128;
      }
    }

    // Clip `value` to 0-255 range.
    if (value < 0) { value = 0; }
    if (value > 255) { value = 255; }
    rgb_t rgb =
        rgb_matrix_hsv_to_rgb(palettefx_interp_color(palette, (uint8_t)value));
    rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
  }

  return rgb_matrix_check_finished_leds(led_max);
}
#endif

#if defined(PALETTEFX_ENABLE_ALL_EFFECTS) || defined(PALETTEFX_SPARKLE_ENABLE)
// "Sparkle" effect. Each LED is animated by a sine wave with pseudorandom
// phase, so that the matrix "sparkles." All the LED sines are modulated by a
// global amplitude factor, which varies by a slower sine wave, so that the
// matrix as a whole periodically brightens and dims.
static bool PALETTEFX_SPARKLE(effect_params_t* params) {
  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  const uint16_t* palette = palettefx_get_palette_data();
  const uint8_t time =
      palettefx_scaled_time(g_rgb_timer, 1 + rgb_matrix_config.speed / 8);
  const uint8_t amplitude = 128 + sin8(time) / 2;
  uint16_t rand_state = 1 + params->iter;

  for (uint8_t i = led_min; i < led_max; ++i) {
    RGB_MATRIX_TEST_LED_FLAGS();
    // Multiplicative congruential generator for a random phase for each LED.
    rand_state *= UINT16_C(36563);
    const uint8_t phase = (uint8_t)(rand_state >> 8);

    const uint8_t value = scale8(sin8(2 * time + phase), amplitude);

    rgb_t rgb = rgb_matrix_hsv_to_rgb(palettefx_interp_color(palette, value));
    rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
  }

  return rgb_matrix_check_finished_leds(led_max);
}
#endif

#if defined(PALETTEFX_ENABLE_ALL_EFFECTS) || defined(PALETTEFX_SPARKLE_ENABLE)
// "Vortex" animated effect. LEDs are animated according to a polar function
// with the appearance of a spinning vortex centered on k_rgb_matrix_center.
static bool PALETTEFX_VORTEX(effect_params_t* params) {
  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  const uint16_t* palette = palettefx_get_palette_data();
  const uint16_t time =
      palettefx_scaled_time(g_rgb_timer, 1 + rgb_matrix_config.speed / 4);

  for (uint8_t i = led_min; i < led_max; ++i) {
    RGB_MATRIX_TEST_LED_FLAGS();
    const int16_t x = g_led_config.point[i].x - k_rgb_matrix_center.x;
    const int16_t y = g_led_config.point[i].y - k_rgb_matrix_center.y;
    uint8_t value = sin8(atan2_8(y, x) + time - sqrt16(x * x + y * y) / 2);

    rgb_t rgb = rgb_matrix_hsv_to_rgb(palettefx_interp_color(palette, value));
    rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
  }

  return rgb_matrix_check_finished_leds(led_max);
}
#endif

#if defined(RGB_MATRIX_KEYREACTIVE_ENABLED) && ( \
    defined(PALETTEFX_ENABLE_ALL_EFFECTS) || defined(PALETTEFX_REACTIVE_ENABLE))
// Reactive animated effect. This effect is "reactive," it responds to key
// presses. For each key press, LEDs near the key change momentarily.
static bool PALETTEFX_REACTIVE(effect_params_t* params) {
  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  const uint16_t* palette = palettefx_get_palette_data();
  const uint8_t count = g_last_hit_tracker.count;

  uint8_t amplitude(uint8_t t) {  // Bump amplitude as a function of time.
    if (t <= 55) {
      return (t < 32) ? (4 + 8 * t) : 255;
    } else {
      t = (((uint16_t)(255 - t)) * UINT16_C(164)) >> 7;
      return scale8(t, t);
    }
  }

  uint8_t hit_amplitude[LED_HITS_TO_REMEMBER] = {0};
  for (uint8_t j = 0; j < count; ++j) {
    const uint16_t tick = scale16by8(g_last_hit_tracker.tick[j],
        1 + rgb_matrix_config.speed / 4);
    if (tick <= 255) {
      hit_amplitude[j] = amplitude((uint8_t)tick);
    }
  }

  for (uint8_t i = led_min; i < led_max; ++i) {
    RGB_MATRIX_TEST_LED_FLAGS();
    uint8_t value = 0;

    for (uint8_t j = 0; j < count; ++j) {
      if (hit_amplitude[j] == 0) { continue; }

      uint8_t dx = abs8((g_led_config.point[i].x - g_last_hit_tracker.x[j]) / 2);
      uint8_t dy = abs8((g_led_config.point[i].y - g_last_hit_tracker.y[j]) / 2);
      if (dx < 21 && dy < 21) {
        const uint16_t dist_sqr = dx * dx + dy * dy;
        if (dist_sqr < 21 * 21) {  // Accumulate a radial bump for each hit.
          const uint8_t dist = sqrt16(dist_sqr);
          value = qadd8(value, scale8(255 - 12 * dist, hit_amplitude[j]));
          // Early loop exit where the value has saturated.
          if (value == 255) { break; }
        }
      }
    }

    hsv_t hsv = palettefx_interp_color(palette, value);
    if (value < 32) {  // Make the background dark regardless of palette.
      hsv.v = scale8(hsv.v, 64 + 6 * value);
    }

    const rgb_t rgb = rgb_matrix_hsv_to_rgb(hsv);
    rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
  }
  return rgb_matrix_check_finished_leds(led_max);
}
#endif


///////////////////////////////////////////////////////////////////////////////
// PaletteFx implementation
///////////////////////////////////////////////////////////////////////////////

/**
 * @brief 16-bit HSV color.
 *
 * Represents a color with a 16-bit value in hue-saturation-value (HSV) space.
 * Components are packed as:
 *
 *  - Hue: lowest 8 bits.
 *  - Saturation: middle 4 bits.
 *  - Value: highest 4 bits.
 */
#define HSV16(h, s, v) ((((v) >> 4) << 12) | (((s) >> 4) << 8) | ((h) & 0xff))

/** Unpacks 16-bit HSV color to hsv_t. */
static hsv_t unpack_hsv16(uint16_t hsv16) {
  return (hsv_t){
    .h = (uint8_t)hsv16,
    .s = ((uint8_t)(hsv16 >> 8) & 0x0f) * 17,
    .v = ((uint8_t)(hsv16 >> 12) & 0x0f) * 17,
  };
}

/** PaletteFx palette color data. */
static const uint16_t palettefx_palettes[][16] PROGMEM = {
#if defined(PALETTEFX_ENABLE_ALL_PALETTES) || defined(PALETTEFX_AFTERBURN_ENABLE)
  // "Afterburn" palette.
  {
    HSV16(139, 255,  85),
    HSV16(134, 255,  85),
    HSV16(131, 255, 102),
    HSV16(128, 255, 102),
    HSV16(127, 187, 102),
    HSV16(125, 119, 102),
    HSV16(124,  51, 102),
    HSV16(125,   0, 119),
    HSV16( 15,  17, 119),
    HSV16( 17,  51, 153),
    HSV16( 18, 102, 170),
    HSV16( 19, 153, 204),
    HSV16( 21, 187, 238),
    HSV16( 23, 238, 255),
    HSV16( 26, 255, 255),
    HSV16( 30, 255, 255),
  },
#endif
#if defined(PALETTEFX_ENABLE_ALL_PALETTES) || defined(PALETTEFX_AMBER_ENABLE)
  // "Amber" palette.
  {
    HSV16(  5, 187, 170),
    HSV16( 15, 221, 170),
    HSV16( 22, 238, 187),
    HSV16( 23, 238, 187),
    HSV16( 25, 255, 204),
    HSV16( 27, 255, 221),
    HSV16( 29, 255, 238),
    HSV16( 29, 255, 255),
    HSV16( 28, 187, 255),
    HSV16( 30,  68, 255),
    HSV16( 32,  34, 255),
    HSV16( 32,  34, 255),
    HSV16( 26, 119, 255),
    HSV16( 24, 238, 255),
    HSV16( 21, 221, 255),
    HSV16( 16, 204, 255),
  },
#endif
#if defined(PALETTEFX_ENABLE_ALL_PALETTES) || defined(PALETTEFX_BADWOLF_ENABLE)
  // "Bad Wolf" palette. Inspired by the Bad Wolf theme by Steve Losh, which is
  // distributed under MIT/X11 license.
  // https://github.com/sjl/badwolf/tree/61d75affa51d40213d671edc9c8ff83992d7fd6f
  {
    HSV16( 14,  34,   0),
    HSV16(245, 255, 255),
    HSV16(245, 255, 255),
    HSV16(245, 255, 255),
    HSV16( 14,  34,   0),
    HSV16( 14,  34,   0),
    HSV16( 14,  34,   0),
    HSV16( 14,  34,   0),
    HSV16( 14,  34,   0),
    HSV16( 14,  34,   0),
    HSV16( 24,  17, 136),
    HSV16( 28,   0, 255),
    HSV16( 24,  17, 136),
    HSV16( 14,  34,   0),
    HSV16( 14,  34,   0),
    HSV16( 14,  34,   0),
  },
#endif
#if defined(PALETTEFX_ENABLE_ALL_PALETTES) || defined(PALETTEFX_CARNIVAL_ENABLE)
  // "Carnival" palette.
  {
    HSV16(132, 255,  85),
    HSV16(121, 187,  85),
    HSV16(108, 170, 102),
    HSV16( 90, 153, 119),
    HSV16( 70, 187, 136),
    HSV16( 57, 255, 153),
    HSV16( 50, 255, 187),
    HSV16( 44, 255, 204),
    HSV16( 39, 255, 238),
    HSV16( 32, 255, 255),
    HSV16( 27, 255, 255),
    HSV16( 22, 255, 255),
    HSV16( 18, 255, 255),
    HSV16(  8, 221, 238),
    HSV16(252, 204, 238),
    HSV16(241, 255, 221),
  },
#endif
#if defined(PALETTEFX_ENABLE_ALL_PALETTES) || defined(PALETTEFX_CLASSIC_ENABLE)
  // "Classic" palette.
  {
    HSV16(150, 255,  85),
    HSV16(151, 238, 119),
    HSV16(160, 136, 119),
    HSV16(176, 102, 136),
    HSV16(193,  85, 136),
    HSV16(216,  85, 136),
    HSV16(234, 102, 153),
    HSV16(247, 119, 187),
    HSV16(  3, 153, 204),
    HSV16( 10, 204, 221),
    HSV16( 15, 255, 255),
    HSV16( 18, 255, 255),
    HSV16( 23, 255, 255),
    HSV16( 24, 204, 255),
    HSV16( 25, 136, 255),
    HSV16( 26,  85, 255),
  },
#endif
#if defined(PALETTEFX_ENABLE_ALL_PALETTES) || defined(PALETTEFX_DRACULA_ENABLE)
  // "Dracula" palette. Inspired by the Dracula theme by Zeno Rocha, which is
  // distributed under MIT license.
  // https://github.com/dracula/dracula-theme/tree/ac4dc82dab2a3c35e5cac0cd80c97fbf4c2ca986
  {
    HSV16(165, 153, 119),
    HSV16(167, 153, 136),
    HSV16(170, 170, 187),
    HSV16(173, 170, 221),
    HSV16(177, 170, 238),
    HSV16(183, 170, 255),
    HSV16(190, 170, 255),
    HSV16(200, 153, 255),
    HSV16(216, 153, 255),
    HSV16(230, 170, 255),
    HSV16(  2, 153, 255),
    HSV16( 29, 153, 238),
    HSV16( 44, 255, 255),
    HSV16( 53, 238, 255),
    HSV16( 63, 238, 238),
    HSV16( 85, 238, 238),
  },
#endif
#if defined(PALETTEFX_ENABLE_ALL_PALETTES) || defined(PALETTEFX_GROOVY_ENABLE)
  // "Groovy" palette. Inspired by the Gruvbox theme by Pavel Pertsev, which is
  // distributed under MIT/X11 license.
  // https://github.com/morhetz/gruvbox/tree/f1ecde848f0cdba877acb0c740320568252cc482
  {
    HSV16( 26, 136, 187),
    HSV16( 23,  85, 102),
    HSV16( 21,  85,  85),
    HSV16( 21,  85,  85),
    HSV16( 21,  85,  85),
    HSV16( 17, 102, 102),
    HSV16(  5, 204, 255),
    HSV16(  4, 255, 255),
    HSV16(  4, 255, 255),
    HSV16(  7, 204, 255),
    HSV16( 23, 136, 187),
    HSV16( 26, 136, 187),
    HSV16( 28, 136, 187),
    HSV16( 50, 238, 238),
    HSV16( 54, 255, 238),
    HSV16( 54, 255, 238),
  },
#endif
#if defined(PALETTEFX_ENABLE_ALL_PALETTES) || defined(PALETTEFX_NOTPINK_ENABLE)
  // "Not Pink" palette.
  {
    HSV16(  9, 255, 153),
    HSV16(  3, 221, 187),
    HSV16(252, 204, 187),
    HSV16(248, 204, 187),
    HSV16(247, 187, 204),
    HSV16(245, 187, 238),
    HSV16(244, 170, 255),
    HSV16(245, 153, 255),
    HSV16(252, 102, 255),
    HSV16( 16,  68, 255),
    HSV16( 40,  34, 255),
    HSV16( 32,  51, 255),
    HSV16(  6,  85, 255),
    HSV16(248, 136, 255),
    HSV16(247, 187, 255),
    HSV16(249, 221, 255),
  },
#endif
#if defined(PALETTEFX_ENABLE_ALL_PALETTES) || defined(PALETTEFX_PHOSPHOR_ENABLE)
  // "Phosphor" palette.
  {
    HSV16(116, 102,  34),
    HSV16(113, 170,  51),
    HSV16(113, 255,  68),
    HSV16(110, 255,  68),
    HSV16(109, 255,  85),
    HSV16(105, 255, 102),
    HSV16( 95, 238, 102),
    HSV16( 85, 238, 119),
    HSV16( 84, 255, 136),
    HSV16( 84, 255, 153),
    HSV16( 83, 255, 187),
    HSV16( 80, 238, 204),
    HSV16( 69, 221, 238),
    HSV16( 46, 204, 255),
    HSV16( 42, 153, 255),
    HSV16( 40, 102, 255),
  },
#endif
#if defined(PALETTEFX_ENABLE_ALL_PALETTES) || defined(PALETTEFX_POLARIZED_ENABLE)
  // "Polarized" palette. Inspired by the Solarized theme by Ethan Schoonover,
  // which is distributed under MIT license.
  // https://github.com/altercation/solarized/tree/62f656a02f93c5190a8753159e34b385588d5ff3
  {
    HSV16(139, 255,  68),
    HSV16(139, 221,  85),
    HSV16(139, 204, 102),
    HSV16(138, 204, 119),
    HSV16(137, 204, 136),
    HSV16(137, 187, 170),
    HSV16(137, 153, 187),
    HSV16(132, 170, 187),
    HSV16(126, 187, 238),
    HSV16(124, 102, 255),
    HSV16(124, 102, 255),
    HSV16(124, 187, 255),
    HSV16(130, 170, 204),
    HSV16(137, 153, 187),
    HSV16(137, 153, 204),
    HSV16(137, 136, 221),
  },
#endif
#if defined(PALETTEFX_ENABLE_ALL_PALETTES) || defined(PALETTEFX_ROSEGOLD_ENABLE)
  // "Rose Gold" palette.
  {
    HSV16(246, 255, 204),
    HSV16(250, 238, 221),
    HSV16(253, 221, 238),
    HSV16(  1, 221, 255),
    HSV16(  6, 221, 255),
    HSV16( 12, 221, 255),
    HSV16( 18, 204, 255),
    HSV16( 22, 170, 255),
    HSV16( 20, 204, 255),
    HSV16( 17, 221, 255),
    HSV16( 12, 221, 255),
    HSV16(  7, 221, 255),
    HSV16(255, 204, 255),
    HSV16(248, 204, 255),
    HSV16(241, 238, 255),
    HSV16(235, 255, 255),
  },
#endif
#if defined(PALETTEFX_ENABLE_ALL_PALETTES) || defined(PALETTEFX_SPORT_ENABLE)
  // "Sport" palette.
  {
    HSV16(156, 102,  51),
    HSV16(155, 102,  68),
    HSV16(155, 102,  68),
    HSV16(154, 102,  68),
    HSV16(155, 102,  85),
    HSV16(156,  85, 102),
    HSV16(158,  68, 119),
    HSV16(159,  51, 136),
    HSV16(158,  17, 153),
    HSV16(130,   0, 170),
    HSV16( 49,  17, 204),
    HSV16( 42,  51, 238),
    HSV16( 39,  85, 255),
    HSV16( 37, 119, 255),
    HSV16( 36, 170, 255),
    HSV16( 36, 255, 255),
  },
#endif
#if defined(PALETTEFX_ENABLE_ALL_PALETTES) || defined(PALETTEFX_SYNTHWAVE_ENABLE)
  // "Synthwave" palette.
  {
    HSV16(170, 221, 119),
    HSV16(180, 221, 153),
    HSV16(196, 238, 153),
    HSV16(209, 255, 153),
    HSV16(220, 255, 170),
    HSV16(227, 255, 204),
    HSV16(233, 255, 238),
    HSV16(245, 204, 255),
    HSV16(  3, 170, 255),
    HSV16( 13, 187, 255),
    HSV16( 21, 204, 255),
    HSV16( 28, 221, 255),
    HSV16( 43, 255, 255),
    HSV16( 42, 255, 255),
    HSV16( 68,  68, 255),
    HSV16(132, 255, 255),
  },
#endif
#if defined(PALETTEFX_ENABLE_ALL_PALETTES) || defined(PALETTEFX_THERMAL_ENABLE)
  // "Thermal" palette.
  {
    HSV16(  7,   0,  17),
    HSV16(  8,   0,  17),
    HSV16( 12,   0,  34),
    HSV16( 13,   0,  51),
    HSV16( 15,  17,  51),
    HSV16( 16,  17,  68),
    HSV16( 20,  34,  85),
    HSV16( 95,   0, 102),
    HSV16(126,  85, 119),
    HSV16(112,  85, 153),
    HSV16( 54, 136, 187),
    HSV16( 39, 238, 221),
    HSV16( 33, 255, 255),
    HSV16( 25, 238, 255),
    HSV16( 13, 238, 255),
    HSV16(  5, 238, 255),
  },
#endif
#if defined(PALETTEFX_ENABLE_ALL_PALETTES) || defined(PALETTEFX_VIRIDIS_ENABLE)
  // "Viridis" palette. Inspired by the Viridis colormap by Stefan van der Walt
  // and Nathaniel Smith, which is distributed under CC0 license.
  // https://github.com/BIDS/colormap/blob/bc549477db0c12b54a5928087552ad2cf274980f/colormaps.py
  {
    HSV16(204, 221, 102),
    HSV16(194, 187, 119),
    HSV16(183, 153, 136),
    HSV16(169, 119, 153),
    HSV16(155, 153, 153),
    HSV16(145, 170, 153),
    HSV16(137, 187, 153),
    HSV16(130, 204, 153),
    HSV16(123, 221, 153),
    HSV16(117, 221, 170),
    HSV16(108, 187, 187),
    HSV16( 93, 153, 204),
    HSV16( 71, 170, 221),
    HSV16( 56, 221, 221),
    HSV16( 46, 255, 238),
    HSV16( 39, 255, 255),
  },
#endif
#if defined(PALETTEFX_ENABLE_ALL_PALETTES) || defined(PALETTEFX_WATERMELON_ENABLE)
  // "Watermelon" palette.
  {
    HSV16( 67, 255, 102),
    HSV16( 55, 255, 153),
    HSV16( 47, 255, 204),
    HSV16( 48, 136, 238),
    HSV16( 48,  68, 255),
    HSV16( 45,  34, 255),
    HSV16( 39,  34, 255),
    HSV16( 17,  85, 255),
    HSV16(  8, 187, 255),
    HSV16(  1, 255, 255),
    HSV16(  2, 238, 255),
    HSV16(  2, 238, 255),
    HSV16(  0, 238, 255),
    HSV16(253, 255, 238),
    HSV16(254, 255, 238),
    HSV16(255, 255, 221),
  },
#endif
#if __has_include("palettefx_user.inc")  // Include user palettes if present.
#include "palettefx_user.inc"
#endif
};
/** Number of palettes. User palettes, if any, are included in the count. */
#define NUM_PALETTEFX_PALETTES \
    (sizeof(palettefx_palettes) / sizeof(*palettefx_palettes))

// Validate at compile time that `1 <= NUM_PALETTEFX_PALETTES <= 32`. The upper
// limit is due to using RGB Matrix's hue config to select palettes.
_Static_assert(
    NUM_PALETTEFX_PALETTES >= 1,
    "palettefx: No palettes are enabled. To fix: enable all built-in palettes by adding in config.h `#define PALETTE_ENABLE_ALL_PALETTES`, or enable at least one with `#define PALETTE_<name>_ENABLE`, or define at least one custom palette in palettefx_user.inc.");
_Static_assert(
    NUM_PALETTEFX_PALETTES <= 256 / RGB_MATRIX_HUE_STEP,
    "palettefx: Too many palettes. Up to 32 (= 256 / RGB_MATRIX_HUE_STEP) palettes are supported. Otherwise, some palettes would be unreachable.");

/** Gets the index of the selected palette. */
static uint8_t palettefx_get_palette(void) {
  uint8_t i =
      (rgb_matrix_get_hue() / RGB_MATRIX_HUE_STEP) % NUM_PALETTEFX_PALETTES;

  if (256 % (RGB_MATRIX_HUE_STEP * NUM_PALETTEFX_PALETTES) != 0) {
    // Hue wraps mod 256. If NUM_PALETTEFX_PALETTES is not a power of 2, modulo
    // 256 wraps would jump, and adjustment is needed to cycle as desired:
    //
    // * If the hue is a step or less from 256, assume it has wrapped down
    //   from 0 and the last palette is selected.
    // * Otherwise, i = ((hue / step) % NUM_PALETTEFX_PALETTES) is used. If
    //   256 - 2 * step <= hue < 256 - step, hue is set to (step * i).
    hsv_t hsv = rgb_matrix_get_hsv();
    if (hsv.h >= 256 - 2 * RGB_MATRIX_HUE_STEP) {
      if (hsv.h >= 256 - RGB_MATRIX_HUE_STEP) {
        i = NUM_PALETTEFX_PALETTES - 1;
        hsv.h = RGB_MATRIX_HUE_STEP * (
            (256 / (RGB_MATRIX_HUE_STEP * NUM_PALETTEFX_PALETTES)) *
                NUM_PALETTEFX_PALETTES
            - 1);
      } else {
        i %= NUM_PALETTEFX_PALETTES;
        hsv.h = RGB_MATRIX_HUE_STEP * i;
      }
      rgb_matrix_sethsv_noeeprom(hsv.h, hsv.s, hsv.v);
    }
  }

  return i;
}

static const uint16_t* palettefx_get_palette_data(void) {
  return palettefx_palettes[palettefx_get_palette()];
}

static hsv_t palettefx_interp_color(const uint16_t* palette, uint8_t x) {
  // Clamp `x` to [8, 247] and subtract 8, mapping to the range [0, 239].
  x = (x <= 8) ? 0 : ((x < 247) ? (x - 8) : 239);
  // Get index into the palette, 0 <= i <= 14.
  const uint8_t i = x >> 4;
  // Fractional position between i and (i + 1).
  const uint8_t frac = x << 4;

  // Look up palette colors at i and (i + 1).
  hsv_t a = unpack_hsv16(pgm_read_word(&palette[i]));
  hsv_t b = unpack_hsv16(pgm_read_word(&palette[i + 1]));

  // Linearly interpolate in HSV, accounting for wrapping in hue.
  const uint8_t hue_wrap = 128 & (a.h >= b.h ? (a.h - b.h) : (b.h - a.h));
  return (hsv_t){
    .h = lerp8by8(a.h ^ hue_wrap, b.h ^ hue_wrap, frac) ^ hue_wrap,
    .s = scale8(lerp8by8(a.s, b.s, frac), rgb_matrix_config.hsv.s),
    .v = scale8(lerp8by8(a.v, b.v, frac), rgb_matrix_config.hsv.v),
  };
}

static uint16_t palettefx_scaled_time(uint32_t timer, uint8_t scale) {
  static uint16_t wrap_correction = 0;
  static uint8_t last_high_byte = 0;
  const uint8_t high_byte = (uint8_t)(timer >> 16);

  if (last_high_byte != high_byte) {
    last_high_byte = high_byte;
    wrap_correction += ((uint16_t)scale) << 8;
  }

  return scale16by8(timer, scale) + wrap_correction;
}

#endif  // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
