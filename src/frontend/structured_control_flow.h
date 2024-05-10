// SPDX-FileCopyrightText: Copyright 2021 yuzu Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "ir/abstract_syntax_list.h"
#include "ir/basic_block.h"
#include "ir/value.h"
#include "frontend/control_flow_graph.h"
#include "object_pool.h"

namespace Shader::Gcn {

[[nodiscard]] IR::AbstractSyntaxList BuildASL(ObjectPool<IR::Inst>& inst_pool,
                                              ObjectPool<IR::Block>& block_pool, CFG& cfg);

} // namespace Shader::Gcn
