// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "recompiler.h"

#include "frontend/control_flow_graph.h"
#include "frontend/decode.h"
#include "frontend/structured_control_flow.h"
#include "ir/abstract_syntax_list.h"
#include "ir/basic_block.h"
#include "ir/post_order.h"
#include "object_pool.h"

namespace Shader::Optimization {
void SsaRewritePass(IR::BlockList& program);
void IdentityRemovalPass(IR::BlockList& program);
} // namespace Shader::Optimization

namespace Shader::Recompiler {

Shader::IR::BlockList GenerateBlocks(const Shader::IR::AbstractSyntaxList& syntax_list) {
    size_t num_syntax_blocks{};
    for (const auto& node : syntax_list) {
        if (node.type == Shader::IR::AbstractSyntaxNode::Type::Block) {
            ++num_syntax_blocks;
        }
    }
    Shader::IR::BlockList blocks;
    blocks.reserve(num_syntax_blocks);
    for (const auto& node : syntax_list) {
        if (node.type == Shader::IR::AbstractSyntaxNode::Type::Block) {
            blocks.push_back(node.data.block);
        }
    }
    return blocks;
}

bool recompile_shader(const std::span<const u32>& code) {
    Shader::Gcn::GcnCodeSlice slice(code.data(), code.data() + code.size());
    std::vector<Shader::Gcn::GcnInst> insList;
    Shader::Gcn::GcnDecodeContext decoder;

    // Decode and save instructions
    insList.reserve(code.size());
    while (!slice.atEnd()) {
        insList.emplace_back(decoder.decodeInstruction(slice));
    }

    Shader::ObjectPool<Shader::Gcn::Block> block_pool{64};
    Shader::ObjectPool<Shader::IR::Block> blk_pool{64};
    Shader::ObjectPool<Shader::IR::Inst> inst_pool{64};
    Shader::Gcn::CFG cfg{block_pool, insList};
    fmt::print("{}\n\n\n", cfg.Dot());
    const auto ret = Shader::Gcn::BuildASL(inst_pool, blk_pool, cfg);
    auto blocks = Shader::IR::PostOrder(ret.front());
    auto block = GenerateBlocks(ret);
    Shader::Optimization::SsaRewritePass(blocks);
    Shader::Optimization::IdentityRemovalPass(block);
    for (auto& blk : block) {
        fmt::print("{}\n\n", Shader::IR::DumpBlock(*blk));
    }

    return true;
}
} // namespace Shader::Recompiler
