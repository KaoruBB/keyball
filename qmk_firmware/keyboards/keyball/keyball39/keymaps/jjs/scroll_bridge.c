// Copyright 2026 Jiyan Jonas Schneider
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#include "lib/keyball/scroll_bridge_protocol.h"
#include "raw_hid.h"
#include "scroll_bridge.h"

#define SCROLL_BRIDGE_TIMEOUT 1500

static bool     scroll_bridge_connected;
static uint8_t  scroll_bridge_sequence;
static uint32_t scroll_bridge_heartbeat;

__attribute__((weak)) void scroll_bridge_on_motion(void) {}

static void scroll_bridge_init_packet(uint8_t *packet, uint8_t message) {
    memset(packet, 0, KEYBALL_SCROLL_BRIDGE_PACKET_SIZE);
    packet[KEYBALL_SCROLL_BRIDGE_OFFSET_MAGIC_0]  = KEYBALL_SCROLL_BRIDGE_MAGIC_0;
    packet[KEYBALL_SCROLL_BRIDGE_OFFSET_MAGIC_1]  = KEYBALL_SCROLL_BRIDGE_MAGIC_1;
    packet[KEYBALL_SCROLL_BRIDGE_OFFSET_MAGIC_2]  = KEYBALL_SCROLL_BRIDGE_MAGIC_2;
    packet[KEYBALL_SCROLL_BRIDGE_OFFSET_VERSION]  = KEYBALL_SCROLL_BRIDGE_VERSION;
    packet[KEYBALL_SCROLL_BRIDGE_OFFSET_MESSAGE]  = message;
    packet[KEYBALL_SCROLL_BRIDGE_OFFSET_SEQUENCE] = scroll_bridge_sequence++;
}

static uint16_t scroll_bridge_resolution(void) {
#ifdef POINTING_DEVICE_HIRES_SCROLL_ENABLE
    return pointing_device_get_hires_scroll_resolution();
#else
    return 1;
#endif
}

static bool scroll_bridge_is_connected(void) {
    if (scroll_bridge_connected && timer_elapsed32(scroll_bridge_heartbeat) > SCROLL_BRIDGE_TIMEOUT) {
        scroll_bridge_connected = false;
    }
    return scroll_bridge_connected;
}

static void scroll_bridge_send_hello(void) {
    uint8_t packet[KEYBALL_SCROLL_BRIDGE_PACKET_SIZE];
    scroll_bridge_init_packet(packet, KEYBALL_SCROLL_BRIDGE_DEVICE_HELLO);
    keyball_scroll_bridge_write_u16(packet, KEYBALL_SCROLL_BRIDGE_OFFSET_RESOLUTION, scroll_bridge_resolution());
    raw_hid_send(packet, sizeof(packet));
}

void raw_hid_receive(uint8_t *data, uint8_t length) {
    if (length != KEYBALL_SCROLL_BRIDGE_PACKET_SIZE || data[KEYBALL_SCROLL_BRIDGE_OFFSET_MAGIC_0] != KEYBALL_SCROLL_BRIDGE_MAGIC_0 || data[KEYBALL_SCROLL_BRIDGE_OFFSET_MAGIC_1] != KEYBALL_SCROLL_BRIDGE_MAGIC_1 || data[KEYBALL_SCROLL_BRIDGE_OFFSET_MAGIC_2] != KEYBALL_SCROLL_BRIDGE_MAGIC_2 || data[KEYBALL_SCROLL_BRIDGE_OFFSET_VERSION] != KEYBALL_SCROLL_BRIDGE_VERSION) {
        return;
    }

    switch (data[KEYBALL_SCROLL_BRIDGE_OFFSET_MESSAGE]) {
        case KEYBALL_SCROLL_BRIDGE_HOST_HELLO:
            scroll_bridge_send_hello();
            break;
        case KEYBALL_SCROLL_BRIDGE_HOST_HEARTBEAT:
            scroll_bridge_connected = true;
            scroll_bridge_heartbeat = timer_read32();
            scroll_bridge_send_hello();
            break;
        case KEYBALL_SCROLL_BRIDGE_HOST_GOODBYE:
            scroll_bridge_connected = false;
            break;
    }
}

bool keyball_on_require_hires_scroll(void) {
    return scroll_bridge_is_connected();
}

bool keyball_on_process_scroll_report(report_mouse_t *report) {
    if (!scroll_bridge_is_connected()) {
        return false;
    }

    if (report->v != 0 || report->h != 0) {
        uint8_t packet[KEYBALL_SCROLL_BRIDGE_PACKET_SIZE];
        scroll_bridge_on_motion();
        scroll_bridge_init_packet(packet, KEYBALL_SCROLL_BRIDGE_DEVICE_SCROLL);
        packet[KEYBALL_SCROLL_BRIDGE_OFFSET_FLAGS] = keyball_get_scroll_mode() ? KEYBALL_SCROLL_BRIDGE_FLAG_SCROLL_MODE : 0;
        keyball_scroll_bridge_write_i16(packet, KEYBALL_SCROLL_BRIDGE_OFFSET_VERTICAL, report->v);
        keyball_scroll_bridge_write_i16(packet, KEYBALL_SCROLL_BRIDGE_OFFSET_HORIZONTAL, report->h);
        keyball_scroll_bridge_write_u16(packet, KEYBALL_SCROLL_BRIDGE_OFFSET_RESOLUTION, scroll_bridge_resolution());
        raw_hid_send(packet, sizeof(packet));
    }

    return true;
}
