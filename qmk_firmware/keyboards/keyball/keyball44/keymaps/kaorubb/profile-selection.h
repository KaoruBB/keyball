// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
#include <stdbool.h>
#include <stdint.h>

enum profile_id { PROFILE_LINUX = 0, PROFILE_MAC = 4, PROFILE_WINDOWS = 8, PROFILE_UNKNOWN = 255 };
typedef struct { uint8_t current, pending; bool manual; } profile_selection_t;

static inline void profile_selection_init(profile_selection_t *selection) {
    *selection = (profile_selection_t){.current = PROFILE_LINUX, .pending = PROFILE_UNKNOWN};
}

static inline void profile_selection_reconnect(profile_selection_t *selection) {
    selection->manual = false;
    selection->pending = PROFILE_LINUX;
}

static inline void profile_selection_detect(profile_selection_t *selection, uint8_t profile) {
    if (!selection->manual && profile != PROFILE_UNKNOWN) selection->pending = profile;
}

static inline bool profile_selection_apply(profile_selection_t *selection, bool input_busy) {
    if (input_busy || selection->pending == PROFILE_UNKNOWN) return false;
    uint8_t profile = selection->pending;
    selection->pending = PROFILE_UNKNOWN;
    if (profile == selection->current) return false;
    selection->current = profile;
    return true;
}

static inline void profile_selection_cycle(profile_selection_t *selection) {
    selection->manual = true;
    selection->pending = PROFILE_UNKNOWN;
    selection->current = selection->current == PROFILE_WINDOWS ? PROFILE_LINUX : selection->current + 4;
}
