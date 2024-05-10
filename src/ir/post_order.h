// SPDX-FileCopyrightText: Copyright 2021 yuzu Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "ir/abstract_syntax_list.h"
#include "ir/basic_block.h"

namespace Shader::IR {

BlockList PostOrder(const AbstractSyntaxNode& root);

} // namespace Shader::IR
