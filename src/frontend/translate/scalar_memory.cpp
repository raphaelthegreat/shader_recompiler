// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "frontend/translate/translate.h"

namespace Shader::Gcn {

void Load(IR::IREmitter& ir, int num_dwords, const IR::Value& handle,
          IR::ScalarReg dst_reg, const IR::U32U64& address) {
    const u32 max_read_dwords = std::min(num_dwords, 4);
    const auto do_load = [&](u32 num_dwords, const IR::U32U64& offset) {
        return handle.IsEmpty() ? ir.ReadConst(num_dwords, offset)
                                : ir.ReadConstBuffer(num_dwords, handle, offset);
    };

    if (num_dwords == 1) {
        const IR::Value value = do_load(max_read_dwords, address);
        ir.SetScalarReg(dst_reg, IR::U32{value});
        return;
    }
    const u32 num_reads = num_dwords / 4;
    for (int i = 0; i < num_reads; i++) {
        const u32 offset = i * 4;
        const IR::U64 new_address = offset != 0 ? ir.IAdd(address, ir.Imm32(offset * 4))
                                                : address;
        const IR::Value value = do_load(4, new_address);
        for (int i = 0; i < 4; i++) {
            ir.SetScalarReg(dst_reg + offset + i, IR::U32{ir.CompositeExtract(value, i)});
        }
    }
}

void Translator::S_LOAD_DWORD(int num_dwords, const GcnInst& inst) {
    const auto& smrd = inst.control.smrd;
    const IR::ScalarReg sbase = IR::ScalarReg(inst.src[0].code * 2);
    const IR::U32 offset = smrd.imm ? ir.Imm32(smrd.offset * 4)
                                    : IR::U32{ir.GetScalarReg(IR::ScalarReg(smrd.offset))};
    const IR::U64 base = ir.PackUint2x32(ir.CompositeConstruct(ir.GetScalarReg(sbase),
                                                               ir.GetScalarReg(sbase + 1)));
    const IR::U64 address = ir.IAdd(base, offset);
    const IR::ScalarReg dst_reg{inst.dst[0].code};
    Load(ir, num_dwords, {}, dst_reg, address);
}

void Translator::S_BUFFER_LOAD_DWORD(int num_dwords, const GcnInst& inst) {
    const auto& smrd = inst.control.smrd;
    const IR::ScalarReg sbase = IR::ScalarReg(inst.src[0].code * 2);
    const IR::U32 offset = smrd.imm ? ir.Imm32(smrd.offset)
                                    : IR::U32{ir.GetScalarReg(IR::ScalarReg(smrd.offset))};
    const IR::Value vsharp = ir.CompositeConstruct(ir.GetScalarReg(sbase),
                                                   ir.GetScalarReg(sbase + 1),
                                                   ir.GetScalarReg(sbase + 2),
                                                   ir.GetScalarReg(sbase + 3));
    const IR::ScalarReg dst_reg{inst.dst[0].code};
    Load(ir, num_dwords, vsharp, dst_reg, offset);
}

} // namespace Shader::Gcn
