/*
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

#include QMK_KEYBOARD_H

#include "quantum.h"

enum custom_keycodes {
    BTN3_SCRL = KEYBALL_SAFE_RANGE,
};

static bool     btn3_scroll_held;
static bool     btn3_scroll_used;
static uint16_t btn3_scroll_timer;

const uint16_t PROGMEM fd_escape_combo[] = {LGUI_T(KC_F), LALT_T(KC_D),
                                            COMBO_END};
const uint16_t PROGMEM qw_tab_combo[] = {KC_Q, KC_W, COMBO_END};
const uint16_t PROGMEM jk_escape_combo[] = {RGUI_T(KC_J), RALT_T(KC_K),
                                            COMBO_END};
const uint16_t PROGMEM lscln_tab_combo[] = {RSFT_T(KC_L), RCTL_T(KC_SCLN),
                                            COMBO_END};

combo_t key_combos[] = {
    COMBO(fd_escape_combo, KC_ESC),
    COMBO(qw_tab_combo, KC_TAB),
    COMBO(jk_escape_combo, KC_ESC),
    COMBO(lscln_tab_combo, KC_TAB),
};

bool get_combo_must_tap(uint16_t combo_index, combo_t *combo) { return true; }

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // keymap for default
  [0] = LAYOUT_universal(
    KC_Q         , KC_W       , KC_E        , KC_R         , KC_T          ,                            KC_Y     , KC_U        , KC_I        , KC_O        , KC_P     ,
    LCTL_T(KC_A) ,LSFT_T(KC_S), LALT_T(KC_D), LGUI_T(KC_F) , KC_G          ,                            KC_H     , RGUI_T(KC_J), RALT_T(KC_K), RSFT_T(KC_L), RCTL_T(KC_SCLN),
    KC_Z         , KC_X       , KC_C        , KC_V         , KC_B          ,                            KC_N     , KC_M        , KC_COMM     , KC_DOT      , KC_SLSH  ,
    KC_LCTL      ,TT(2)       , TT(2)       , LT(3,KC_LNG2), LT(1,KC_LNG1), LT(2, KC_SPC),              KC_BSPC  , LT(2,KC_ENT), LT(2,KC_ENT), LT(2,KC_ENT), LT(2,KC_ENT), KC_PSCR
  ),

  [1] = LAYOUT_universal(
    KC_F1    , KC_F2      , KC_F3    , KC_F4      , KC_F5,                                   KC_F6      , KC_F7    , KC_F8    , KC_F9    , KC_F10   ,
    KC_ESC   , KC_EXLM    , S(KC_6)  , S(KC_INT3) , S(KC_8),                                 S(KC_INT1), KC_HOME  , KC_PGUP  , _______  , KC_SCLN  ,
    S(KC_EQL), S(KC_LBRC) , S(KC_7)  , S(KC_2)    , S(KC_RBRC),                              KC_LBRC   , KC_END   , KC_PGDN  , KC_F11   , KC_F12   ,
    KC_INT1  , KC_EQL     , S(KC_3)  , KC_LNG2    , _______,                                 _______   , _______  , _______  , KC_LNG2  , KC_RALT  , KC_RGUI, _______
  ),

 [2] = LAYOUT_universal(
    KC_TAB        , KC_7                , KC_8               , KC_9                    ,      KC_0   ,  KC_NUHS  , KC_GRV   , KC_QUOT       , KC_LBRC         , KC_RBRC  ,
    LCTL_T(KC_ESC), LSFT_T( KC_4 )      , LALT_T(KC_5)       , LGUI_T(KC_6), S(KC_SCLN),      KC_LEFT,  RGUI_T( KC_DOWN )   , RALT_T(KC_UP) , RSFT_T(KC_RIGHT), RCTL_T(KC_QUOT)  ,
    KC_SLSH       , KC_1                , KC_2               , KC_3     ,KC_MINS       ,      KC_EQL ,  KC_LEFT             , KC_DOWN       , KC_RIGHT        , _______  ,
    KC_ESC        , KC_0     , KC_DOT   , KC_DEL   , KC_ENT  , KC_BSPC  , _______      ,      TO(0)  ,   _______            , _______       , _______         , _______
  ),

 

  [3] = LAYOUT_universal(
    RM_TOGG  , AML_TO   , AML_I50  , AML_D50  , _______  ,                            _______  , _______  , SSNP_HOR , SSNP_VRT , SSNP_FRE ,
   RGB_MOD   , RGB_HUI  , RGB_SAI  , RGB_VAI  , SCRL_DVI ,                            KC_LEFT, KC_DOWN  , KC_UP  , KC_RIGHT  , _______  ,
    RGB_RMOD , RGB_HUD  , RGB_SAD  , RGB_VAD  , SCRL_DVD ,                            CPI_D1K  , CPI_D100 , CPI_I100 , CPI_I1K  , KBC_SAVE ,
    QK_BOOT  , KBC_RST  , _______  , _______  , _______  , _______  ,      _______  , _______  , _______  , _______  , KBC_RST  , QK_BOOT
  ),

  // Transparent typing layer automatically enabled by trackball movement.
  [4] = LAYOUT_universal(
    _______  , _______  , _______  , _______  , _______  ,                            _______  , KC_BTN1  , BTN3_SCRL, KC_BTN2  , _______  ,
    _______  , _______  , _______  , _______  , _______  ,                            _______  , _______  , KC_BTN3  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  ,                            _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,      _______  , _______  , _______  , _______  , _______  , _______
  ),
};
// clang-format on

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode != BTN3_SCRL) {
        return true;
    }

    if (record->event.pressed) {
        btn3_scroll_held  = true;
        btn3_scroll_used  = false;
        btn3_scroll_timer = timer_read();
        keyball_set_scroll_mode(true);
    } else {
        bool tapped = !btn3_scroll_used && timer_elapsed(btn3_scroll_timer) < TAPPING_TERM;

        btn3_scroll_held = false;
        keyball_set_scroll_mode(layer_state_is(3));
        if (tapped) {
            tap_code(KC_BTN3);
        }
    }

    return false;
}

bool is_mouse_record_user(uint16_t keycode, keyrecord_t *record) {
    return keycode == BTN3_SCRL;
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (btn3_scroll_held && (mouse_report.v != 0 || mouse_report.h != 0)) {
        btn3_scroll_used = true;
    }
    return mouse_report;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    // Auto enable scroll mode whenever layer 3 is active.
    keyball_set_scroll_mode(btn3_scroll_held || layer_state_cmp(state, 3));
    return state;
}

#ifdef OLED_ENABLE

#include "lib/oledkit/oledkit.h"
#include "oled_art.h"
static void render_oled_status(void) {
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}

static void render_oled_offhand_test(void) {
    oled_write_raw_P(hello_oled, sizeof(hello_oled));
}

void oledkit_render_info_user(void) { render_oled_status(); }

void oledkit_render_logo_user(void) { render_oled_offhand_test(); }
#endif
