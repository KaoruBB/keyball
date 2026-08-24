// Copyright 2026 Jiyan Jonas Schneider
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdint.h>

#define KEYBALL_SCROLL_BRIDGE_PACKET_SIZE 32
#define KEYBALL_SCROLL_BRIDGE_MAGIC_0 'K'
#define KEYBALL_SCROLL_BRIDGE_MAGIC_1 'B'
#define KEYBALL_SCROLL_BRIDGE_MAGIC_2 'S'
#define KEYBALL_SCROLL_BRIDGE_VERSION 1

enum keyball_scroll_bridge_message {
    KEYBALL_SCROLL_BRIDGE_HOST_HELLO     = 0x01,
    KEYBALL_SCROLL_BRIDGE_HOST_GOODBYE   = 0x02,
    KEYBALL_SCROLL_BRIDGE_HOST_HEARTBEAT = 0x03,
    KEYBALL_SCROLL_BRIDGE_DEVICE_HELLO   = 0x81,
    KEYBALL_SCROLL_BRIDGE_DEVICE_SCROLL  = 0x82,
};

enum keyball_scroll_bridge_offset {
    KEYBALL_SCROLL_BRIDGE_OFFSET_MAGIC_0    = 0,
    KEYBALL_SCROLL_BRIDGE_OFFSET_MAGIC_1    = 1,
    KEYBALL_SCROLL_BRIDGE_OFFSET_MAGIC_2    = 2,
    KEYBALL_SCROLL_BRIDGE_OFFSET_VERSION    = 3,
    KEYBALL_SCROLL_BRIDGE_OFFSET_MESSAGE    = 4,
    KEYBALL_SCROLL_BRIDGE_OFFSET_SEQUENCE   = 5,
    KEYBALL_SCROLL_BRIDGE_OFFSET_FLAGS      = 6,
    KEYBALL_SCROLL_BRIDGE_OFFSET_VERTICAL   = 8,
    KEYBALL_SCROLL_BRIDGE_OFFSET_HORIZONTAL = 10,
    KEYBALL_SCROLL_BRIDGE_OFFSET_RESOLUTION = 12,
};

enum keyball_scroll_bridge_flag {
    KEYBALL_SCROLL_BRIDGE_FLAG_SCROLL_MODE = 1 << 0,
};

static inline int16_t keyball_scroll_bridge_read_i16(const uint8_t *packet, uint8_t offset) {
    return (int16_t)((uint16_t)packet[offset] | ((uint16_t)packet[offset + 1] << 8));
}

static inline uint16_t keyball_scroll_bridge_read_u16(const uint8_t *packet, uint8_t offset) {
    return (uint16_t)packet[offset] | ((uint16_t)packet[offset + 1] << 8);
}

static inline void keyball_scroll_bridge_write_i16(uint8_t *packet, uint8_t offset, int16_t value) {
    packet[offset]     = (uint8_t)value;
    packet[offset + 1] = (uint8_t)((uint16_t)value >> 8);
}

static inline void keyball_scroll_bridge_write_u16(uint8_t *packet, uint8_t offset, uint16_t value) {
    packet[offset]     = (uint8_t)value;
    packet[offset + 1] = (uint8_t)(value >> 8);
}
