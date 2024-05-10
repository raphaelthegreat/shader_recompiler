// SPDX-FileCopyrightText: Copyright 2021 yuzu Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "common/assert.h"

void assert_fail_impl() {
    std::fflush(stdout);
    std::abort();
}

[[noreturn]] void unreachable_impl() {
    std::fflush(stdout);
    std::abort();
    throw std::runtime_error("Unreachable code");
}
