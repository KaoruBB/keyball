// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H

enum layers {
    LINUX_BASE,
    LINUX_NUM,
    LINUX_SHORTCUT,
    LINUX_SETTINGS,
    MAC_BASE,
    MAC_NUM,
    MAC_SHORTCUT,
    MAC_SETTINGS,
};

enum custom_keycodes {
    PROFILE_LINUX = KEYBALL_SAFE_RANGE,
    PROFILE_MAC,
};

// PDF labels above a key are chords; labels below it are hold actions.
// Layer 2 is retained as a reference, with no key that activates it.
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [LINUX_BASE] = LAYOUT_right_ball(
        KC_TAB,         KC_Q,           KC_W,           KC_E,       KC_R,       KC_T,       KC_Y,       KC_U,       KC_I,       KC_O,           KC_P,               KC_EQL,
        LCTL_T(KC_ESC), LCTL_T(KC_A),   LGUI_T(KC_S),   KC_D,       KC_F,       KC_G,       KC_H,       KC_J,       KC_K,       RGUI_T(KC_L),   RCTL_T(KC_SCLN),    RCTL_T(KC_QUOT),
        KC_LSFT,        KC_Z,           KC_X,           KC_C,       KC_V,       KC_B,       KC_N,       KC_M,       KC_COMM,    KC_DOT,         KC_SLSH,            RSFT_T(KC_MINS),
        KC_NO, LT(LINUX_SETTINGS, KC_MUTE), TT(LINUX_NUM), LALT_T(KC_SPC), KC_BSPC,       KC_RSFT, KC_RALT, RCTL_T(KC_ENT)
    ),

    [LINUX_NUM] = LAYOUT_right_ball(
        KC_GRV,         KC_1,           KC_2,           KC_3,       KC_4,       KC_5,       KC_6,           KC_7,       KC_8,       KC_9,           KC_0,               KC_RALT,
        KC_LCTL,        LCTL_T(KC_6),   LGUI_T(KC_7),   KC_8,       KC_9,       KC_0,       S(KC_BTN1),     KC_BTN1,    KC_BTN2,    RGUI_T(KC_LBRC),RCTL_T(KC_RBRC),    KC_RCTL,
        KC_LSFT,        SCRL_MO,        KC_TRNS,        KC_LBRC,    KC_RBRC,    KC_TRNS,    LGUI(KC_X),     KC_PGDN,    KC_PGUP,    KC_DOT,         KC_BSLS,            RSFT_T(KC_MINS),
        KC_TRNS, KC_TRNS, KC_TRNS, KC_LGUI, KC_BSPC,                                     KC_RSFT, KC_RALT, LCTL(LSFT(KC_SCLN))
    ),

    [LINUX_SHORTCUT] = LAYOUT_right_ball(
        KC_LALT,        LCTL(KC_1),     LCTL(KC_2),     LCTL(KC_3), LCTL(KC_4), LCTL(KC_5), KC_F6,          KC_F7,      KC_F8,      LCA(KC_O),      LCA(KC_P),          LCA(KC_R),
        KC_TRNS,        LCTL(KC_6),     LCTL(KC_7),     LCTL(KC_8), LCTL(KC_9), LCTL(KC_0), LCA(KC_H),      LCA(KC_J),  LCA(KC_K),  LCA(KC_L),      KC_LNG1,            KC_TRNS,
        KC_TRNS,        LCA(KC_1),      LCA(KC_2),      LCA(KC_3),  LCA(KC_4),  LCA(KC_5),  LCA(KC_N),      LCA(KC_M),  KC_TRNS,    KC_TRNS,        KC_INT2,            KC_TRNS,
        KC_0, KC_DOT, KC_LSFT, KC_LGUI, LCA(KC_BSPC),                                   KC_RGUI, KC_RSFT, KC_TRNS
    ),

    [LINUX_SETTINGS] = LAYOUT_right_ball(
        UG_TOGG,        KC_F1,          KC_F2,          KC_F3,      KC_F4,      KC_F5,      KC_F6,          KC_F7,      KC_F12,     KC_F9,          KC_MUTE,            KC_VOLD,
        KC_LCTL,        LCTL_T(KC_F11), KC_F12,         KC_F13,     KC_F14,     KC_F15,     KC_LEFT,        KC_DOWN,    KC_UP,      KC_RGHT,        KC_TRNS,            KC_VOLU,
        UG_PREV,        RGB_M_P,        RGB_M_B,        UG_SPDU,    UG_SPDD,    RGB_M_R,    RGB_M_TW,       RGB_M_T,    RGB_M_X,    DB_TOGG,        KC_F20,             KBC_SAVE,
        KBC_RST, KBC_RST, LCTL(LGUI(KC_Q)), PROFILE_LINUX, PROFILE_MAC,                  KC_TRNS, KC_TRNS, HYPR(KC_K)
    ),

    [MAC_BASE] = LAYOUT_right_ball(
        KC_TAB,         KC_Q,           KC_W,           KC_E,       KC_R,       KC_T,       KC_Y,           KC_U,       KC_I,       KC_O,           KC_P,               KC_EQL,
        LCTL_T(KC_ESC), LCTL_T(KC_A),   LALT_T(KC_S),   KC_D,       KC_F,       KC_G,       KC_H,           KC_J,       KC_K,       RALT_T(KC_L),   RCTL_T(KC_SCLN),    RCTL_T(KC_QUOT),
        KC_LSFT,        KC_Z,           KC_X,           KC_C,       KC_V,       KC_B,       KC_N,           KC_M,       KC_COMM,    KC_DOT,         KC_SLSH,            RSFT_T(KC_MINS),
        LCTL_T(KC_F3), LT(MAC_SETTINGS, KC_MUTE), TT(MAC_NUM), KC_SPC, KC_BSPC,           KC_RSFT, KC_LGUI, RCTL_T(KC_ENT)
    ),

    [MAC_NUM] = LAYOUT_right_ball(
        KC_GRV,         KC_1,           KC_2,           KC_3,       KC_4,          KC_5,    KC_6,           KC_7,       KC_8,       KC_9,           KC_0,               KC_RALT,
        KC_LCTL,        LCTL_T(KC_6),   LALT_T(KC_7),   LSFT_T(KC_8),LGUI_T(KC_9), KC_0,    S(KC_BTN1),     KC_BTN1,    KC_BTN2,    RALT_T(KC_LBRC),RGUI_T(KC_RBRC),    KC_RCTL,
        KC_LSFT,        SCRL_MO,        KC_TRNS,        KC_LBRC,    KC_RBRC,       KC_TRNS, KC_N,           KC_M,       KC_COMM,    KC_DOT,         KC_BSLS,            RSFT_T(KC_MINS),
        KC_TRNS, KC_TRNS, KC_TRNS, KC_LGUI, KC_BSPC,                                     KC_RSFT, KC_LGUI, LCTL(LSFT(KC_SCLN))
    ),

    [MAC_SHORTCUT] = LAYOUT_right_ball(
        KC_LALT,        LCTL(KC_1),     LCTL(KC_2),     LCTL(KC_3), LCTL(KC_4), LCTL(KC_5), KC_F6,          KC_F7,      KC_F8,      LCA(KC_O),      LCA(KC_P),          LCA(KC_R),
        KC_TRNS,        LCTL(KC_6),     LCTL(KC_7),     LCTL(KC_8), LCTL(KC_9), LCTL(KC_0), LCA(KC_H),      LCA(KC_J),  LCA(KC_K),  LCA(KC_L),      KC_LNG1,            KC_TRNS,
        KC_TRNS,        LCA(KC_1),      LCA(KC_2),      LCA(KC_3),  LCA(KC_4),  LCA(KC_5),  LCA(KC_N),      LCA(KC_M),  KC_TRNS,    KC_TRNS,        KC_INT2,            KC_TRNS,
        KC_0, KC_DOT, KC_LSFT, KC_LGUI, LCA(KC_BSPC),                                   KC_RSFT, KC_LGUI, KC_TRNS
    ),

    [MAC_SETTINGS] = LAYOUT_right_ball(
        UG_TOGG,        KC_F1,          KC_F2,          KC_F3,      KC_F4,      KC_F5,      KC_F6,          KC_F7,      KC_F8,      KC_F9,          KC_MUTE,            KC_VOLD,
        KC_LCTL,        LCTL_T(KC_F11), KC_F12,         KC_F13,     KC_F14,     KC_F15,     KC_LEFT,        KC_DOWN,    KC_UP,      KC_RGHT,        KC_TRNS,            KC_VOLU,
        UG_PREV,        RGB_M_P,        RGB_M_B,        UG_SPDU,    UG_SPDD,    RGB_M_R,    RGB_M_TW,       RGB_M_T,    RGB_M_X,    DB_TOGG,        KC_F20,             KBC_SAVE,
        KBC_RST, KBC_RST, LCTL(LGUI(KC_Q)), PROFILE_LINUX, PROFILE_MAC,                  KC_TRNS, KC_TRNS, HYPR(KC_K)
    ),
};

static void select_profile(uint8_t base) {
    // Clear the old auto-mouse layer before changing its target (QMK ownership rule).
    layer_clear();
    default_layer_set((layer_state_t)1 << base);
    set_auto_mouse_layer(base + 1);
    keyball_set_scroll_mode(false);
}

void keyboard_post_init_user(void) {
    select_profile(LINUX_BASE);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case PROFILE_LINUX:
            if (record->event.pressed && get_highest_layer(default_layer_state) != LINUX_BASE) select_profile(LINUX_BASE);
            return false;
        case PROFILE_MAC:
            if (record->event.pressed && get_highest_layer(default_layer_state) != MAC_BASE) select_profile(MAC_BASE);
            return false;
    }
    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    uint8_t base = get_highest_layer(default_layer_state);
    // TT inverts its layer on release; a held key from the old profile must not restore that layer.
    state &= (layer_state_t)0xF << base;
    keyball_set_scroll_mode(layer_state_cmp(state, base + 3));
    return state;
}

#ifdef OLED_ENABLE
#    include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
    uint8_t base = get_highest_layer(default_layer_state);
    oled_set_cursor(0, 0);
    // Four OLED rows: profile, ball motion, ball settings, layer/auto-mouse status.
    oled_write_P(base == MAC_BASE ? PSTR("Mac US   L") : PSTR("Linux US L"), false);
    oled_write_char('0' + get_highest_layer(layer_state | default_layer_state) - base, false);
    oled_write_P(PSTR("          "), false);
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}
#endif
