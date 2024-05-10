// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "frontend/translate/translate.h"

namespace Shader::Gcn {

void Translator::IMAGE_GET_RESINFO(const GcnInst& inst) {
    IR::VectorReg dst_reg{inst.src[1].code};
    const IR::ScalarReg tsharp_reg{inst.src[2].code};
    const auto flags = ImageResFlags(inst.control.mimg.dmask);
    const IR::U32 lod = ir.GetVectorReg(IR::VectorReg(inst.src[0].code));
    const IR::Value tsharp = ir.CompositeConstruct(ir.GetScalarReg(tsharp_reg),
                                                   ir.GetScalarReg(tsharp_reg + 1),
                                                   ir.GetScalarReg(tsharp_reg + 2),
                                                   ir.GetScalarReg(tsharp_reg + 3));
    const IR::Value size = ir.ImageQueryDimension(tsharp, lod, ir.Imm1(false));

    if (flags.test(ImageResComponent::Width)) {
        ir.SetVectorReg(dst_reg++, IR::U32{ir.CompositeExtract(size, 0)});
    }
    if (flags.test(ImageResComponent::Height)) {
        ir.SetVectorReg(dst_reg++, IR::U32{ir.CompositeExtract(size, 1)});
    }
    if (flags.test(ImageResComponent::Depth)) {
        ir.SetVectorReg(dst_reg++, IR::U32{ir.CompositeExtract(size, 2)});
    }
    if (flags.test(ImageResComponent::MipCount)) {
        ir.SetVectorReg(dst_reg++, IR::U32{ir.CompositeExtract(size, 3)});
    }
}

} // namespace Shader::Gcn
