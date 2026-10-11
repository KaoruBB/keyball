// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H
#include "keymap_japanese.h"

enum layers {
    LINUX_BASE,
    LINUX_NUM,
    LINUX_SHORTCUT,
    LINUX_SETTINGS,
    MAC_BASE,
    MAC_NUM,
    MAC_SHORTCUT,
    MAC_SETTINGS,
    WIN_BASE,
    WIN_NUM,
    WIN_SHORTCUT,
    WIN_SETTINGS,
    MOUSE,
};

enum custom_keycodes {
    PROFILE_LINUX = KEYBALL_SAFE_RANGE,
    PROFILE_MAC,
    PROFILE_WINDOWS,
};

// PDF labels above a key are chords; labels below it are hold actions.
// Layer 2 is retained as a reference, with no key that activates it.
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [LINUX_BASE] = LAYOUT_right_ball(
        KC_TAB,         KC_Q,           KC_W,           KC_E,       KC_R,       KC_T,       KC_Y,       KC_U,       KC_I,       KC_O,           KC_P,               KC_EQL,
        LCTL_T(KC_ESC), LCTL_T(KC_A),   LGUI_T(KC_S),   KC_D,       KC_F,       KC_G,       KC_H,       KC_J,       KC_K,       RGUI_T(KC_L),   RCTL_T(KC_SCLN),    RCTL_T(KC_QUOT),
        KC_LSFT,        KC_Z,           KC_X,           KC_C,       KC_V,       KC_B,       KC_N,       KC_M,       KC_COMM,    KC_DOT,         KC_SLSH,            RSFT_T(KC_MINS),
        KC_NO, LT(LINUX_SETTINGS, KC_KB_MUTE), TT(LINUX_NUM), LALT_T(KC_SPC), KC_BSPC,    KC_RSFT, KC_RALT, RCTL_T(KC_ENT)
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
        KC_TRNS,        LCA(KC_1),      LCA(KC_2),      LCA(KC_3),  LCA(KC_4),  LCA(KC_5),  LCA(KC_N),      LCA(KC_M),  KC_TRNS,    KC_TRNS,        KC_LNG4,            KC_TRNS,
        KC_0, KC_DOT, KC_LSFT, KC_LGUI, LCA(KC_BSPC),                                   KC_RGUI, KC_RSFT, KC_TRNS
    ),

    [LINUX_SETTINGS] = LAYOUT_right_ball(
        UG_TOGG,        KC_F1,          KC_F2,          KC_F3,      KC_F4,      KC_F5,      KC_F6,          KC_F7,      KC_F12,     KC_F9,          KC_KB_MUTE,         KC_KB_VOLUME_DOWN,
        KC_LCTL,        LCTL_T(KC_F11), KC_F12,         KC_F13,     KC_F14,     KC_F15,     KC_LEFT,        KC_DOWN,    KC_UP,      KC_RGHT,        KC_TRNS,            KC_KB_VOLUME_UP,
        UG_PREV,        RGB_M_P,        RGB_M_B,        UG_SPDU,    UG_SPDD,    RGB_M_R,    RGB_M_TW,       RGB_M_T,    RGB_M_X,    DB_TOGG,        KC_F20,             KBC_SAVE,
        KBC_RST, KBC_RST, LCTL(LGUI(KC_Q)), PROFILE_LINUX, PROFILE_MAC,                  PROFILE_WINDOWS, KC_TRNS, HYPR(KC_K)
    ),

    [MAC_BASE] = LAYOUT_right_ball(
        KC_TAB,         KC_Q,           KC_W,           KC_E,       KC_R,       KC_T,       KC_Y,           KC_U,       KC_I,       KC_O,           KC_P,               KC_EQL,
        LCTL_T(KC_ESC), LCTL_T(KC_A),   LALT_T(KC_S),   KC_D,       KC_F,       KC_G,       KC_H,           KC_J,       KC_K,       RALT_T(KC_L),   RCTL_T(KC_SCLN),    RCTL_T(KC_QUOT),
        KC_LSFT,        KC_Z,           KC_X,           KC_C,       KC_V,       KC_B,       KC_N,           KC_M,       KC_COMM,    KC_DOT,         KC_SLSH,            RSFT_T(KC_MINS),
        LCTL_T(KC_F3), LT(MAC_SETTINGS, KC_KB_MUTE), TT(MAC_NUM), KC_SPC, KC_BSPC,        KC_RSFT, KC_LGUI, RCTL_T(KC_ENT)
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
        KC_TRNS,        LCA(KC_1),      LCA(KC_2),      LCA(KC_3),  LCA(KC_4),  LCA(KC_5),  LCA(KC_N),      LCA(KC_M),  KC_TRNS,    KC_TRNS,        KC_LNG4,            KC_TRNS,
        KC_0, KC_DOT, KC_LSFT, KC_LGUI, LCA(KC_BSPC),                                   KC_RSFT, KC_LGUI, KC_TRNS
    ),

    [MAC_SETTINGS] = LAYOUT_right_ball(
        UG_TOGG,        KC_F1,          KC_F2,          KC_F3,      KC_F4,      KC_F5,      KC_F6,          KC_F7,      KC_F8,      KC_F9,          KC_KB_MUTE,         KC_KB_VOLUME_DOWN,
        KC_LCTL,        LCTL_T(KC_F11), KC_F12,         KC_F13,     KC_F14,     KC_F15,     KC_LEFT,        KC_DOWN,    KC_UP,      KC_RGHT,        KC_TRNS,            KC_KB_VOLUME_UP,
        UG_PREV,        RGB_M_P,        RGB_M_B,        UG_SPDU,    UG_SPDD,    RGB_M_R,    RGB_M_TW,       RGB_M_T,    RGB_M_X,    DB_TOGG,        KC_F20,             KBC_SAVE,
        KBC_RST, KBC_RST, LCTL(LGUI(KC_Q)), PROFILE_LINUX, PROFILE_MAC,                  PROFILE_WINDOWS, KC_TRNS, HYPR(KC_K)
    ),
    [WIN_BASE] = LAYOUT_right_ball(
        KC_TAB,         KC_Q,           KC_W,           KC_E,       KC_R,       KC_T,       KC_Y,       KC_U,       KC_I,       KC_O,           KC_P,               KC_EQL,
        LCTL_T(KC_ESC), LCTL_T(KC_A),   LGUI_T(KC_S),   KC_D,       KC_F,       KC_G,       KC_H,       KC_J,       KC_K,       RGUI_T(KC_L),   RCTL_T(KC_SCLN),    RCTL_T(KC_QUOT),
        KC_LSFT,        KC_Z,           KC_X,           KC_C,       KC_V,       KC_B,       KC_N,       KC_M,       KC_COMM,    KC_DOT,         KC_SLSH,            RSFT_T(KC_MINS),
        KC_NO, LT(WIN_SETTINGS, KC_KB_MUTE), LT(WIN_NUM, KC_LNG2), LALT_T(KC_SPC), KC_BSPC, KC_RSFT, RALT_T(KC_LNG1), RCTL_T(KC_ENT)
    ),

    [WIN_NUM] = LAYOUT_right_ball(
        KC_GRV,         KC_1,           KC_2,           KC_3,       KC_4,       KC_5,       KC_6,           KC_7,       KC_8,       KC_9,           KC_0,               JP_YEN,
        KC_LCTL,        LCTL_T(KC_6),   LGUI_T(KC_7),   KC_8,       KC_9,       KC_0,       S(KC_BTN1),     KC_BTN1,    KC_BTN2,    RGUI_T(KC_LBRC),RCTL_T(KC_RBRC),    KC_RCTL,
        KC_LSFT,        SCRL_MO,        KC_TRNS,        KC_LBRC,    KC_RBRC,    KC_TRNS,    LGUI(KC_X),     KC_PGDN,    KC_PGUP,    KC_DOT,         KC_BSLS,            RSFT_T(KC_MINS),
        KC_TRNS, KC_TRNS, KC_TRNS, KC_LGUI, KC_BSPC,                                     KC_RSFT, KC_TRNS, LCTL(JP_COLN)
    ),

    [WIN_SHORTCUT] = LAYOUT_right_ball(
        KC_LALT,        LCTL(KC_1),     LCTL(KC_2),     LCTL(KC_3), LCTL(KC_4), LCTL(KC_5), KC_F6,          KC_F7,      KC_F8,      LCA(KC_O),      LCA(KC_P),          LCA(KC_R),
        KC_TRNS,        LCTL(KC_6),     LCTL(KC_7),     LCTL(KC_8), LCTL(KC_9), LCTL(KC_0), LCA(KC_H),      LCA(KC_J),  LCA(KC_K),  LCA(KC_L),      KC_LNG1,            KC_TRNS,
        KC_TRNS,        LCA(KC_1),      LCA(KC_2),      LCA(KC_3),  LCA(KC_4),  LCA(KC_5),  LCA(KC_N),      LCA(KC_M),  KC_TRNS,    KC_TRNS,        KC_LNG2,            KC_TRNS,
        KC_0, KC_DOT, KC_LSFT, KC_LGUI, LCA(KC_BSPC),                                   KC_RGUI, KC_RSFT, KC_TRNS
    ),

    [WIN_SETTINGS] = LAYOUT_right_ball(
        UG_TOGG,        KC_F1,          KC_F2,          KC_F3,      KC_F4,      KC_F5,      KC_F6,          KC_F7,      KC_F12,     KC_F9,          KC_KB_MUTE,         KC_KB_VOLUME_DOWN,
        KC_LCTL,        LCTL_T(KC_F11), KC_F12,         KC_F13,     KC_F14,     KC_F15,     KC_LEFT,        KC_DOWN,    KC_UP,      KC_RGHT,        KC_TRNS,            KC_KB_VOLUME_UP,
        UG_PREV,        RGB_M_P,        RGB_M_B,        UG_SPDU,    UG_SPDD,    RGB_M_R,    RGB_M_TW,       RGB_M_T,    RGB_M_X,    DB_TOGG,        KC_F20,             KBC_SAVE,
        KBC_RST, KBC_RST, LCTL(LGUI(KC_Q)), PROFILE_LINUX, PROFILE_MAC,                  PROFILE_WINDOWS, KC_TRNS, HYPR(KC_K)
    ),

    [MOUSE] = LAYOUT_right_ball(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_BTN1, KC_BTN2, SCRL_MO, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS
    ),
};

// https://docs.qmk.fm/features/key_overrides
// US logical keys are translated only in Windows, including shifted symbols.
static bool windows_profile;
#define JIS_OVERRIDE(mods, key, output, negmods) \
    { .trigger = key, .trigger_mods = mods, .replacement = output, \
      .layers = ~(layer_state_t)0, .negative_mod_mask = negmods, \
      .suppressed_mods = MOD_MASK_SHIFT, .options = ko_options_default, .enabled = &windows_profile }
static const key_override_t jis_overrides[] = {
    JIS_OVERRIDE(0, KC_GRV, JP_GRV, MOD_MASK_SHIFT),
    JIS_OVERRIDE(MOD_MASK_SHIFT, KC_GRV, JP_TILD, 0),
    JIS_OVERRIDE(MOD_MASK_SHIFT, KC_2, JP_AT, 0),
    JIS_OVERRIDE(MOD_MASK_SHIFT, KC_6, JP_CIRC, 0),
    JIS_OVERRIDE(MOD_MASK_SHIFT, KC_7, JP_AMPR, 0),
    JIS_OVERRIDE(MOD_MASK_SHIFT, KC_8, JP_ASTR, 0),
    JIS_OVERRIDE(MOD_MASK_SHIFT, KC_9, JP_LPRN, 0),
    JIS_OVERRIDE(MOD_MASK_SHIFT, KC_0, JP_RPRN, 0),
    JIS_OVERRIDE(MOD_MASK_SHIFT, KC_MINS, JP_UNDS, 0),
    JIS_OVERRIDE(0, KC_EQL, JP_EQL, MOD_MASK_SHIFT),
    JIS_OVERRIDE(MOD_MASK_SHIFT, KC_EQL, JP_PLUS, 0),
    JIS_OVERRIDE(0, KC_LBRC, JP_LBRC, MOD_MASK_SHIFT),
    JIS_OVERRIDE(MOD_MASK_SHIFT, KC_LBRC, JP_LCBR, 0),
    JIS_OVERRIDE(0, KC_RBRC, JP_RBRC, MOD_MASK_SHIFT),
    JIS_OVERRIDE(MOD_MASK_SHIFT, KC_RBRC, JP_RCBR, 0),
    JIS_OVERRIDE(MOD_MASK_SHIFT, KC_SCLN, JP_COLN, 0),
    JIS_OVERRIDE(0, KC_QUOT, JP_QUOT, MOD_MASK_SHIFT),
    JIS_OVERRIDE(MOD_MASK_SHIFT, KC_QUOT, JP_DQUO, 0),
    JIS_OVERRIDE(0, KC_BSLS, JP_BSLS, MOD_MASK_SHIFT),
    JIS_OVERRIDE(MOD_MASK_SHIFT, KC_BSLS, JP_PIPE, 0),
};
const key_override_t *key_overrides[] = {
    &jis_overrides[0],
    &jis_overrides[1],
    &jis_overrides[2],
    &jis_overrides[3],
    &jis_overrides[4],
    &jis_overrides[5],
    &jis_overrides[6],
    &jis_overrides[7],
    &jis_overrides[8],
    &jis_overrides[9],
    &jis_overrides[10],
    &jis_overrides[11],
    &jis_overrides[12],
    &jis_overrides[13],
    &jis_overrides[14],
    &jis_overrides[15],
    &jis_overrides[16],
    &jis_overrides[17],
    &jis_overrides[18],
    &jis_overrides[19],
};

static uint8_t scroll_keys;

static void select_profile(uint8_t base) {
    key_override_off();
    windows_profile = base == WIN_BASE;
    key_override_on();
    scroll_keys = 0;
    set_auto_mouse_enable(false);
    layer_clear();
    default_layer_set((layer_state_t)1 << base);
    set_auto_mouse_enable(true);
    keyball_set_scroll_mode(false);
}

void keyboard_post_init_user(void) {
    select_profile(LINUX_BASE);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // QMK overrides see raw Mod-Tap codes; translate their tap events explicitly.
    if (windows_profile && IS_QK_MOD_TAP(keycode) && record->tap.count) {
        uint8_t tap_key = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
        if (process_key_override(tap_key, record)) {
            if (record->event.pressed) register_code(tap_key);
            else unregister_code(tap_key);
        }
        return false;
    }
    switch (keycode) {
        case SCRL_MO: {
            // Track the left and right scroll keys independently across layer changes.
            uint8_t key = record->event.key.row < MATRIX_ROWS / 2 ? 1 : 2;
            if (record->event.pressed) scroll_keys |= key;
            else scroll_keys &= ~key;
            keyball_set_scroll_mode(scroll_keys || layer_state_cmp(layer_state, get_highest_layer(default_layer_state) + 3));
            return false;
        }
        case PROFILE_LINUX:
            if (record->event.pressed && get_highest_layer(default_layer_state) != LINUX_BASE) select_profile(LINUX_BASE);
            return false;
        case PROFILE_WINDOWS:
            if (record->event.pressed && get_highest_layer(default_layer_state) != WIN_BASE) select_profile(WIN_BASE);
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
    layer_state_t profile_layers = (layer_state_t)0xF << base;
    state &= profile_layers | ((layer_state_t)1 << MOUSE);
    bool manual_layer = (state & profile_layers) != 0;
    if (manual_layer) state = remove_auto_mouse_layer(state, true);
    set_auto_mouse_enable(!manual_layer);
    keyball_set_scroll_mode(scroll_keys || layer_state_cmp(state, base + 3));
    return state;
}

#ifdef OLED_ENABLE
#    include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
    uint8_t base = get_highest_layer(default_layer_state);
    oled_set_cursor(0, 0);
    // Four OLED rows: profile, ball motion, ball settings, layer/auto-mouse status.
    oled_write_P(base == WIN_BASE ? PSTR("Win JIS  ") : base == MAC_BASE ? PSTR("Mac US   ") : PSTR("Linux US "), false);
    if (layer_state_cmp(layer_state, MOUSE)) {
        oled_write_P(PSTR("Mouse       "), false);
    } else {
        oled_write_char('L', false);
        oled_write_char('0' + get_highest_layer(layer_state | default_layer_state) - base, false);
        oled_write_P(PSTR("          "), false);
    }
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}
#endif
