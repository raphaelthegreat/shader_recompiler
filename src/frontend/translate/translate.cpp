// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "frontend/translate/translate.h"

namespace Shader::Gcn {

IR::U32F32 Translator::GetSrc(const InstOperand& operand) {
    switch (operand.field) {
    case OperandField::ScalarGPR:
        if (operand.type == ScalarType::Float32) {
            return ir.GetScalarReg<IR::F32>(IR::ScalarReg(operand.code));
        } else {
            return ir.GetScalarReg<IR::U32>(IR::ScalarReg(operand.code));
        }
    case OperandField::VectorGPR:
        if (operand.type == ScalarType::Float32) {
            return ir.GetVectorReg<IR::F32>(IR::VectorReg(operand.code));
        } else {
            return ir.GetVectorReg<IR::U32>(IR::VectorReg(operand.code));
        }
    case OperandField::ConstZero:
        return ir.Imm32(0U);
    case OperandField::SignedConstIntPos:
        return ir.Imm32(operand.code - SignedConstIntPosMin + 1);
    case OperandField::SignedConstIntNeg:
        return ir.Imm32(-s32(operand.code) + SignedConstIntNegMin - 1);
    case OperandField::LiteralConst:
        return ir.Imm32(operand.code);
    default:
        UNREACHABLE();
    }
}

void Translator::SetDst(const InstOperand& operand, const IR::U32F32& value) {
    switch (operand.field) {
    case OperandField::ScalarGPR:
        return ir.SetScalarReg(IR::ScalarReg(operand.code), value);
    case OperandField::VectorGPR:
        return ir.SetVectorReg(IR::VectorReg(operand.code), value);
    case OperandField::VccHi:
        break; // Ignore for now
    default:
        UNREACHABLE();
    }
}

void Translate(IR::Block* block, std::span<const GcnInst> inst_list) {
    if (inst_list.empty()) {
        return;
    }
    Translator translator{block};
    for (u32 i = 0; i < 16; i++) {
        translator.ir.SetScalarReg(IR::ScalarReg(i), translator.ir.Imm32(0U));
        translator.ir.SetVectorReg(IR::VectorReg(i), translator.ir.Imm32(0U));
    }
    for (const auto& inst : inst_list) {
        switch (inst.opcode) {
        case Opcode::S_MOV_B32:
            translator.S_MOV(inst);
            break;
        case Opcode::S_MUL_I32:
            translator.S_MUL_I32(inst);
            break;
        case Opcode::V_MOV_B32:
            translator.V_MOV(inst);
            break;
        case Opcode::V_MAC_F32:
            translator.V_MAC_F32(inst);
            break;
        case Opcode::S_SWAPPC_B64:
        case Opcode::S_WAITCNT:
            break; // Ignore for now.
        case Opcode::S_BUFFER_LOAD_DWORDX16:
            translator.S_BUFFER_LOAD_DWORD(16, inst);
            break;
        case Opcode::EXP:
            translator.EXP(inst);
            break;
        case Opcode::S_ENDPGM:
            break;
        default:
            UNREACHABLE();
        }
    }
}

} // namespace Shader::Gcn
