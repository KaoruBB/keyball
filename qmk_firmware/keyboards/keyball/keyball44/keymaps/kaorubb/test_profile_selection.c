// SPDX-License-Identifier: GPL-2.0-or-later
#include "profile-selection.h"
#include <assert.h>
#include <stdio.h>

static void test_unknown_defaults_to_linux(void) {
    profile_selection_t selection;
    profile_selection_init(&selection);
    profile_selection_detect(&selection, PROFILE_UNKNOWN);
    assert(!profile_selection_apply(&selection, false));
    assert(selection.current == PROFILE_LINUX && !selection.manual);
}

static void test_detection_selects_each_profile(void) {
    profile_selection_t selection;
    profile_selection_init(&selection);
    const uint8_t profiles[] = {PROFILE_MAC, PROFILE_WINDOWS, PROFILE_LINUX};
    for (unsigned i = 0; i < sizeof(profiles); i++) {
        profile_selection_detect(&selection, profiles[i]);
        assert(profile_selection_apply(&selection, false));
        assert(selection.current == profiles[i]);
    }
}

static void test_input_defers_the_latest_detection(void) {
    profile_selection_t selection;
    profile_selection_init(&selection);
    profile_selection_detect(&selection, PROFILE_MAC);
    assert(!profile_selection_apply(&selection, true));
    assert(selection.current == PROFILE_LINUX);
    profile_selection_detect(&selection, PROFILE_WINDOWS);
    assert(!profile_selection_apply(&selection, true));
    assert(profile_selection_apply(&selection, false));
    assert(selection.current == PROFILE_WINDOWS);
}

static void test_cycle_cancels_pending_detection(void) {
    profile_selection_t selection;
    profile_selection_init(&selection);
    profile_selection_detect(&selection, PROFILE_WINDOWS);
    const uint8_t profiles[] = {PROFILE_MAC, PROFILE_WINDOWS, PROFILE_LINUX};
    for (unsigned i = 0; i < sizeof(profiles); i++) {
        profile_selection_cycle(&selection);
        assert(selection.current == profiles[i] && selection.manual);
        assert(!profile_selection_apply(&selection, false));
    }
}

static void test_late_detection_cannot_override_manual_choice(void) {
    profile_selection_t selection;
    profile_selection_init(&selection);
    profile_selection_cycle(&selection);
    profile_selection_detect(&selection, PROFILE_WINDOWS);
    assert(!profile_selection_apply(&selection, false));
    assert(selection.current == PROFILE_MAC);
}

static void test_reconnection_restores_automatic_selection(void) {
    profile_selection_t selection;
    profile_selection_init(&selection);
    profile_selection_cycle(&selection);
    profile_selection_reconnect(&selection);
    assert(!profile_selection_apply(&selection, true));
    assert(selection.current == PROFILE_MAC && !selection.manual);
    profile_selection_detect(&selection, PROFILE_UNKNOWN);
    assert(profile_selection_apply(&selection, false));
    assert(selection.current == PROFILE_LINUX);
    profile_selection_detect(&selection, PROFILE_WINDOWS);
    assert(profile_selection_apply(&selection, false));
    profile_selection_detect(&selection, PROFILE_WINDOWS);
    assert(!profile_selection_apply(&selection, false));
}

int main(void) {
    test_unknown_defaults_to_linux();
    test_detection_selects_each_profile();
    test_input_defers_the_latest_detection();
    test_cycle_cancels_pending_detection();
    test_late_detection_cannot_override_manual_choice();
    test_reconnection_restores_automatic_selection();
    puts("6 profile selection tests passed");
}
