// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <span>
#include "common/types.h"

namespace Shader::Recompiler {

bool recompile_shader(const std::span<const u32>& code);

} // namespace Shader::Recompiler
