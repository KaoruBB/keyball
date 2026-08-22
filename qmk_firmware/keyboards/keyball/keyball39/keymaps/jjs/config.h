/*
This is the c configuration file for the keymap

Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#ifdef RGB_MATRIX_ENABLE
#define RGB_MATRIX_KEYPRESSES
#define RGB_MATRIX_SLEEP
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 150
#define RGB_MATRIX_HUE_STEP 17
#define RGB_MATRIX_SAT_STEP 17
#define RGB_MATRIX_VAL_STEP 15
#define RGB_MATRIX_SPD_STEP 16
#define RGB_MATRIX_LED_PROCESS_LIMIT (RGB_MATRIX_LED_COUNT + 4) / 5
#define RGB_MATRIX_LED_FLUSH_LIMIT 16
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_SPLASH

#define ENABLE_RGB_MATRIX_BREATHING
#define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#define ENABLE_RGB_MATRIX_SOLID_SPLASH
#endif

#define TAP_CODE_DELAY 5

// Keyball39 handedness detection is inverted on newer QMK builds.
#define SPLIT_HAND_MATRIX_GRID_LOW_IS_LEFT

#define POINTING_DEVICE_AUTO_MOUSE_ENABLE
#define AUTO_MOUSE_DEFAULT_LAYER 4

#define POINTING_DEVICE_HIRES_SCROLL_ENABLE
#define POINTING_DEVICE_HIRES_SCROLL_MULTIPLIER 8
#define WHEEL_EXTENDED_REPORT

#define COMBO_TERM 35
#define COMBO_MUST_TAP_PER_COMBO
