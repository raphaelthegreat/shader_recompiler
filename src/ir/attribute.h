// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <fmt/format.h>
#include "common/types.h"

namespace Shader::IR {

enum class Attribute : u64 {
    RenderTarget0 = 0,
    RenderTarget1 = 1,
    RenderTarget2 = 2,
    RenderTarget3 = 3,
    RenderTarget4 = 4,
    RenderTarget5 = 5,
    RenderTarget6 = 6,
    RenderTarget7 = 7,
    DepthZ = 8,
    Null = 9,
    Position0 = 12,
    Position1 = 13,
    Position2 = 14,
    Position3 = 15,
    Param0 = 32,
    Param1 = 33,
    Param2 = 34,
    Param3 = 35,
    Param4 = 36,
    Param5 = 37,
    Param6 = 38,
    Param7 = 39,
    Param8 = 40,
    Param9 = 41,
    Param10 = 42,
    Param11 = 43,
    Param12 = 44,
    Param13 = 45,
    Param14 = 46,
    Param15 = 47,
    Param16 = 48,
    Param17 = 49,
    Param18 = 50,
    Param19 = 51,
    Param20 = 52,
    Param21 = 53,
    Param22 = 54,
    Param23 = 55,
    Param24 = 56,
    Param25 = 57,
    Param26 = 58,
    Param27 = 59,
    Param28 = 60,
    Param29 = 61,
    Param30 = 62,
    Param31 = 63,
};

constexpr size_t EXP_NUM_POS = 4;
constexpr size_t EXP_NUM_PARAM = 32;

[[nodiscard]] bool IsParam(Attribute attribute) noexcept;

[[nodiscard]] std::string NameOf(Attribute attribute);

} // namespace Shader::IR

template <>
struct fmt::formatter<Shader::IR::Attribute> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }
    auto format(const Shader::IR::Attribute& attribute, format_context& ctx) const {
        return fmt::format_to(ctx.out(), "{}", Shader::IR::NameOf(attribute));
    }
};
