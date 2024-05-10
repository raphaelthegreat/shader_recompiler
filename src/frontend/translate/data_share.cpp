// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "frontend/translate/translate.h"

namespace Shader::Gcn {

void Translator::DS_READ(int bit_size, bool is_signed, bool is_pair,
                         const GcnInst& inst) {
    const IR::U32 addr{ir.GetVectorReg(IR::VectorReg(inst.src[0].code))};
    const IR::VectorReg dst_reg{inst.dst[0].code};
    if (is_pair) {
        const IR::U32 addr0 = ir.IAdd(addr, ir.Imm32(u32(inst.control.ds.offset0)));
        ir.SetVectorReg(dst_reg, ir.ReadShared(32, is_signed, addr0));
        const IR::U32 addr1 = ir.IAdd(addr, ir.Imm32(u32(inst.control.ds.offset1)));
        ir.SetVectorReg(dst_reg + 1, ir.ReadShared(32, is_signed, addr1));
    } else if (bit_size == 64) {
        const IR::Value data = ir.UnpackUint2x32(ir.ReadShared(bit_size, is_signed, addr));
        ir.SetVectorReg(dst_reg, IR::U32{ir.CompositeExtract(data, 0)});
        ir.SetVectorReg(dst_reg + 1, IR::U32{ir.CompositeExtract(data, 1)});
    } else {
        const IR::U32 data = ir.ReadShared(bit_size, is_signed, addr);
        ir.SetVectorReg(dst_reg, data);
    }
}

void Translator::DS_WRITE(int bit_size, bool is_signed, bool is_pair, const GcnInst& inst) {
    const IR::U32 addr{ir.GetVectorReg(IR::VectorReg(inst.src[0].code))};
    const IR::VectorReg data0{inst.src[1].code};
    const IR::VectorReg data1{inst.src[2].code};
    if (is_pair) {
        const IR::U32 addr0 = ir.IAdd(addr, ir.Imm32(u32(inst.control.ds.offset0)));
        ir.WriteShared(32, ir.GetVectorReg(data0), addr0);
        const IR::U32 addr1 = ir.IAdd(addr, ir.Imm32(u32(inst.control.ds.offset1)));
        ir.WriteShared(32, ir.GetVectorReg(data1), addr1);
    } else if (bit_size == 64) {
        const IR::U64 data = ir.PackUint2x32(ir.CompositeConstruct(ir.GetVectorReg(data0),
                                                                       ir.GetVectorReg(data0 + 1)));
        ir.WriteShared(bit_size, data, addr);
    } else {
        ir.WriteShared(bit_size, ir.GetVectorReg(data0), addr);
    }
}

} // namespace Shader::Gcn
