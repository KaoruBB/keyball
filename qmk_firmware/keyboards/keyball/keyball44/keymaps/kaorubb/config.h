#pragma once

#undef LAYER_STATE_8BIT
#define LAYER_STATE_16BIT

// Keyball44's left-side identification grid reads low on QMK 0.29.0.
#define SPLIT_HAND_MATRIX_GRID_LOW_IS_LEFT

#define TAP_CODE_DELAY 5

#define POINTING_DEVICE_AUTO_MOUSE_ENABLE
#define AUTO_MOUSE_DEFAULT_LAYER 12
