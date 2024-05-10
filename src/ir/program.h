// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <array>
#include <string>
#include "ir/abstract_syntax_list.h"
#include "ir/basic_block.h"

namespace Shader::IR {

struct Program {
    AbstractSyntaxList syntax_list;
    BlockList blocks;
    BlockList post_order_blocks;
};

[[nodiscard]] std::string DumpProgram(const Program& program);

} // namespace Shader::IR
