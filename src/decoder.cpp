#include <algorithm>
#include "common/assert.h"
#include "decoder.h"
#include "instruction_util.h"

namespace Shader::Gcn {

namespace bit {
template <typename T>
T extract(T value, uint32_t lst, uint32_t fst) {
    return (value >> fst) & ~(~T(0) << (lst - fst + 1));
}
} // namespace bit

GcnDecodeContext::GcnDecodeContext() = default;

GcnDecodeContext::~GcnDecodeContext() = default;

void GcnDecodeContext::decodeInstruction(GcnCodeSlice& code) {
    const u32 token = code.at(0);

    GcnInstEncoding encoding = getInstructionEncoding(token);
    ASSERT_MSG(encoding != GcnInstEncoding::ILLEGAL, "illegal encoding {}", (u32)encoding);
    u32 encodingLen = getEncodingLength(encoding);

    // Clear the instruction
    m_instruction = GcnShaderInstruction();
    // Decode
    if (encodingLen == sizeof(u32)) {
        decodeInstruction32(encoding, code);
    } else {
        decodeInstruction64(encoding, code);
    }

    // Update instruction meta info.
    updateInstructionMeta(encoding);

    // Detect literal constant. Only 32 bits instructions may have literal constant.
    // Note: Literal constant decode must be performed after meta info updated.
    if (encodingLen == sizeof(u32)) {
        decodeLiteralConstant(encoding, code);
    }

    repairOperandType();
}

GcnInstEncoding GcnDecodeContext::getInstructionEncoding(u32 hexInstruction) {
    GcnInstEncoding instructionEncoding = GcnInstEncoding::ILLEGAL;
    // Try all instructions encoding masks from longest to shortest until
    // legal GcnInstEncoding is found.

    instructionEncoding =
        static_cast<GcnInstEncoding>(hexInstruction & (u32)GcnEncodingMask::MASK_9bit);

    switch (instructionEncoding) {
    case GcnInstEncoding::SOP1:
    case GcnInstEncoding::SOPP:
    case GcnInstEncoding::SOPC:
        return instructionEncoding;

    default:
        break;
    }

    /// GcnEncodingMask::Mask7bit
    instructionEncoding =
        static_cast<GcnInstEncoding>(hexInstruction & (u32)GcnEncodingMask::MASK_7bit);

    switch (instructionEncoding) {
    case GcnInstEncoding::VOP1:
    case GcnInstEncoding::VOPC:
        return instructionEncoding;

    default:
        break;
    }

    /// GcnEncodingMask::Mask6bit
    instructionEncoding =
        static_cast<GcnInstEncoding>(hexInstruction & (u32)GcnEncodingMask::MASK_6bit);

    switch (instructionEncoding) {
    case GcnInstEncoding::VOP3:
    case GcnInstEncoding::EXP:
    case GcnInstEncoding::VINTRP:
    case GcnInstEncoding::DS:
    case GcnInstEncoding::MUBUF:
    case GcnInstEncoding::MTBUF:
    case GcnInstEncoding::MIMG:
        return instructionEncoding;

    default:
        break;
    }

    /// GcnEncodingMask::Mask5bit
    instructionEncoding =
        static_cast<GcnInstEncoding>(hexInstruction & (u32)GcnEncodingMask::MASK_5bit);

    switch (instructionEncoding) {
    case GcnInstEncoding::SMRD:
        return instructionEncoding;

    default:
        break;
    }

    /// GcnEncodingMask::Mask4bit
    instructionEncoding =
        static_cast<GcnInstEncoding>(hexInstruction & (u32)GcnEncodingMask::MASK_4bit);

    switch (instructionEncoding) {
    case GcnInstEncoding::SOPK:
        return instructionEncoding;

    default:
        break;
    }

    /// GcnEncodingMask::Mask2bit
    instructionEncoding =
        static_cast<GcnInstEncoding>(hexInstruction & (u32)GcnEncodingMask::MASK_2bit);

    switch (instructionEncoding) {
    case GcnInstEncoding::SOP2:
        return instructionEncoding;

    default:
        break;
    }

    /// GcnEncodingMask::Mask1bit
    instructionEncoding =
        static_cast<GcnInstEncoding>(hexInstruction & (u32)GcnEncodingMask::MASK_1bit);

    switch (instructionEncoding) {
    case GcnInstEncoding::VOP2:
        return instructionEncoding;

    default:
        break;
    }

    /// If no legal GcnInstEncoding found return GcnInstEncoding::ILLEGAL
    return GcnInstEncoding::ILLEGAL;
}

u32 GcnDecodeContext::getEncodingLength(GcnInstEncoding encoding) {
    u32 instLength = 0;

    switch (encoding) {
    case GcnInstEncoding::SOP1:
    case GcnInstEncoding::SOPP:
    case GcnInstEncoding::SOPC:
    case GcnInstEncoding::SOPK:
    case GcnInstEncoding::SOP2:
    case GcnInstEncoding::VOP1:
    case GcnInstEncoding::VOPC:
    case GcnInstEncoding::VOP2:
    case GcnInstEncoding::SMRD:
    case GcnInstEncoding::VINTRP:
        instLength = sizeof(u32);
        break;

    case GcnInstEncoding::VOP3:
    case GcnInstEncoding::MUBUF:
    case GcnInstEncoding::MTBUF:
    case GcnInstEncoding::MIMG:
    case GcnInstEncoding::DS:
    case GcnInstEncoding::EXP:
        instLength = sizeof(u64);
        break;
    }
    return instLength;
}

u32 GcnDecodeContext::getOpMapOffset(GcnInstEncoding encoding) {
    u32 offset = 0;
    switch (encoding) {
    case GcnInstEncoding::SOP1:
        offset = (u32)GcnOpcodeMap::OP_MAP_SOP1;
        break;
    case GcnInstEncoding::SOPP:
        offset = (u32)GcnOpcodeMap::OP_MAP_SOPP;
        break;
    case GcnInstEncoding::SOPC:
        offset = (u32)GcnOpcodeMap::OP_MAP_SOPC;
        break;
    case GcnInstEncoding::VOP1:
        offset = (u32)GcnOpcodeMap::OP_MAP_VOP1;
        break;
    case GcnInstEncoding::VOPC:
        offset = (u32)GcnOpcodeMap::OP_MAP_VOPC;
        break;
    case GcnInstEncoding::VOP3:
        offset = (u32)GcnOpcodeMap::OP_MAP_VOP3;
        break;
    case GcnInstEncoding::EXP:
        offset = (u32)GcnOpcodeMap::OP_MAP_EXP;
        break;
    case GcnInstEncoding::VINTRP:
        offset = (u32)GcnOpcodeMap::OP_MAP_VINTRP;
        break;
    case GcnInstEncoding::DS:
        offset = (u32)GcnOpcodeMap::OP_MAP_DS;
        break;
    case GcnInstEncoding::MUBUF:
        offset = (u32)GcnOpcodeMap::OP_MAP_MUBUF;
        break;
    case GcnInstEncoding::MTBUF:
        offset = (u32)GcnOpcodeMap::OP_MAP_MTBUF;
        break;
    case GcnInstEncoding::MIMG:
        offset = (u32)GcnOpcodeMap::OP_MAP_MIMG;
        break;
    case GcnInstEncoding::SMRD:
        offset = (u32)GcnOpcodeMap::OP_MAP_SMRD;
        break;
    case GcnInstEncoding::SOPK:
        offset = (u32)GcnOpcodeMap::OP_MAP_SOPK;
        break;
    case GcnInstEncoding::SOP2:
        offset = (u32)GcnOpcodeMap::OP_MAP_SOP2;
        break;
    case GcnInstEncoding::VOP2:
        offset = (u32)GcnOpcodeMap::OP_MAP_VOP2;
        break;
    }
    return offset;
}

u32 GcnDecodeContext::mapEncodingOp(GcnInstEncoding encoding, GcnOpcode opcode) {
    // Map from uniform opcode to encoding specific opcode.

    u32 encodingOp = 0;

    if (encoding == GcnInstEncoding::VOP3) {
        if (opcode >= GcnOpcode::V_CMP_F_F32 && opcode <= GcnOpcode::V_CMPX_T_U64) {
            u32 op = static_cast<u32>(opcode) - static_cast<u32>(GcnOpcodeMap::OP_MAP_VOPC);
            encodingOp = op + static_cast<u32>(GcnOpMapVOP3VOPX::VOP3_TO_VOPC);
        } else if (opcode >= GcnOpcode::V_CNDMASK_B32 && opcode <= GcnOpcode::V_CVT_PK_I16_I32) {
            u32 op = static_cast<u32>(opcode) - static_cast<u32>(GcnOpcodeMap::OP_MAP_VOP2);
            encodingOp = op + static_cast<u32>(GcnOpMapVOP3VOPX::VOP3_TO_VOP2);
        } else if (opcode >= GcnOpcode::V_NOP && opcode <= GcnOpcode::V_MOVRELSD_B32) {
            u32 op = static_cast<u32>(opcode) - static_cast<u32>(GcnOpcodeMap::OP_MAP_VOP1);
            encodingOp = op + static_cast<u32>(GcnOpMapVOP3VOPX::VOP3_TO_VOP1);
        } else {
            encodingOp = static_cast<u32>(opcode) - static_cast<u32>(GcnOpcodeMap::OP_MAP_VOP3);
        }
    } else {
        u32 mapOffset = getOpMapOffset(encoding);
        encodingOp = static_cast<u32>(opcode) - mapOffset;
    }

    return encodingOp;
}

bool GcnDecodeContext::hasAdditionalLiteral(GcnInstEncoding encoding, u32 opcode) {
    // Some instructions require an additional literal constant following the binary stream
    // even if LiteralConst is not specified in src operand.

    bool hasLiteral = false;
    switch (encoding) {
    case GcnInstEncoding::SOPK: {
        if (opcode == (u32)GcnOpcodeSOPK::S_SETREG_IMM32_B32) {
            hasLiteral = true;
        }
        break;
    }
    case GcnInstEncoding::VOP2: {
        if (opcode == (u32)GcnOpcodeVOP2::V_MADMK_F32 ||
            opcode == (u32)GcnOpcodeVOP2::V_MADAK_F32) {
            hasLiteral = true;
        }
        break;
    }
    }
    return hasLiteral;
}

void GcnDecodeContext::updateInstructionMeta(GcnInstEncoding encoding) {
    u32 encodingOp = mapEncodingOp(encoding, m_instruction.opcode);
    GcnInstFormat instFormat = gcnInstructionFormat(encoding, encodingOp);

    ASSERT_MSG(instFormat.srcType != GcnScalarType::Undefined &&
                   instFormat.dstType != GcnScalarType::Undefined,
               "TODO: Instruction format table not complete, please fix it manually.");

    m_instruction.opClass = instFormat.instructionClass;
    m_instruction.category = instFormat.instructionCategory;
    m_instruction.encoding = encoding;
    m_instruction.srcCount = instFormat.srcCount;
    m_instruction.length = getEncodingLength(encoding);

    // Update src operand scalar type.
    auto setOperandType = [&instFormat](GcnInstOperand& src) {
        // Only update uninitialized numeric type.
        if (src.type == GcnScalarType::Undefined) {
            src.type = instFormat.srcType;
        }
    };

    std::for_each_n(std::begin(m_instruction.src), m_instruction.srcCount, setOperandType);

    // Update dst operand scalar type.
    switch (m_instruction.dstCount) {
    case 2: {
        if (m_instruction.dst[1].type == GcnScalarType::Undefined) {
            // Only VOP3B has an additional sdst operand,
            // and it must be Uint64
            m_instruction.dst[1].type = GcnScalarType::Uint64;
        }
    }
        [[fallthrough]];
    case 1: {
        if (m_instruction.dst[0].type == GcnScalarType::Undefined) {
            m_instruction.dst[0].type = instFormat.dstType;
        }
    }
    }
}

void GcnDecodeContext::repairOperandType() {
    // Some instructions' operand type is not uniform,
    // it's best to change the instruction table's format and fix them there,
    // but it's a hard work.
    // We fix them here.
    switch (m_instruction.opcode) {
    case GcnOpcode::V_MAD_U64_U32:
        m_instruction.src[2].type = GcnScalarType::Uint64;
        break;
    case GcnOpcode::V_MAD_I64_I32:
        m_instruction.src[2].type = GcnScalarType::Sint64;
        break;
    case GcnOpcode::V_ADDC_U32:
        m_instruction.src[2].type = GcnScalarType::Uint64;
        break;
    default:
        break;
    }
}

GcnOperandField GcnDecodeContext::getOperandField(u32 code) {
    GcnOperandField field = {};

    if (code >= ScalarGPRMin && code <= ScalarGPRMax) {
        field = GcnOperandField::ScalarGPR;
    } else if (code >= SignedConstIntPosMin && code <= SignedConstIntPosMax) {
        field = GcnOperandField::SignedConstIntPos;
    } else if (code >= SignedConstIntNegMin && code <= SignedConstIntNegMax) {
        field = GcnOperandField::SignedConstIntNeg;
    } else if (code >= VectorGPRMin && code <= VectorGPRMax) {
        field = GcnOperandField::VectorGPR;
    } else {
        field = static_cast<GcnOperandField>(code);
    }

    return field;
}

void GcnDecodeContext::decodeInstruction32(GcnInstEncoding encoding, GcnCodeSlice& code) {
    u32 hexInstruction = code.readu32();
    switch (encoding) {
    case GcnInstEncoding::SOP1:
        decodeInstructionSOP1(hexInstruction);
        break;
    case GcnInstEncoding::SOPP:
        decodeInstructionSOPP(hexInstruction);
        break;
    case GcnInstEncoding::SOPC:
        decodeInstructionSOPC(hexInstruction);
        break;
    case GcnInstEncoding::SOPK:
        decodeInstructionSOPK(hexInstruction);
        break;
    case GcnInstEncoding::SOP2:
        decodeInstructionSOP2(hexInstruction);
        break;
    case GcnInstEncoding::VOP1:
        decodeInstructionVOP1(hexInstruction);
        break;
    case GcnInstEncoding::VOPC:
        decodeInstructionVOPC(hexInstruction);
        break;
    case GcnInstEncoding::VOP2:
        decodeInstructionVOP2(hexInstruction);
        break;
    case GcnInstEncoding::SMRD:
        decodeInstructionSMRD(hexInstruction);
        break;
    case GcnInstEncoding::VINTRP:
        decodeInstructionVINTRP(hexInstruction);
        break;
    default:
        UNREACHABLE();
    }
}

void GcnDecodeContext::decodeInstruction64(GcnInstEncoding encoding, GcnCodeSlice& code) {
    u64 hexInstruction = code.readu64();
    switch (encoding) {
    case GcnInstEncoding::VOP3:
        decodeInstructionVOP3(hexInstruction);
        break;
    case GcnInstEncoding::MUBUF:
        decodeInstructionMUBUF(hexInstruction);
        break;
    case GcnInstEncoding::MTBUF:
        decodeInstructionMTBUF(hexInstruction);
        break;
    case GcnInstEncoding::MIMG:
        decodeInstructionMIMG(hexInstruction);
        break;
    case GcnInstEncoding::DS:
        decodeInstructionDS(hexInstruction);
        break;
    case GcnInstEncoding::EXP:
        decodeInstructionEXP(hexInstruction);
        break;
    default:
        UNREACHABLE();
    }
}

void GcnDecodeContext::decodeLiteralConstant(GcnInstEncoding encoding, GcnCodeSlice& code) {
    bool hasLiteral = false;
    do {
        // Detect if it's a special instruction.
        hasLiteral = hasAdditionalLiteral(encoding, mapEncodingOp(encoding, m_instruction.opcode));

        if (hasLiteral) {
            u32 encodingOp = mapEncodingOp(encoding, m_instruction.opcode);
            GcnInstFormat instFormat = gcnInstructionFormat(encoding, encodingOp);

            u32 literalConst = code.readu32();
            m_instruction.src[m_instruction.srcCount].field = GcnOperandField::LiteralConst;
            m_instruction.src[m_instruction.srcCount].type = instFormat.srcType;
            m_instruction.src[m_instruction.srcCount].literalConst = literalConst;
            // The source count information can not be detect through encoding,
            // so we fix it.
            ++m_instruction.srcCount;
            break;
        }

        // Find if the instruction contains a literal constant
        auto isLiteralSrc = [](GcnInstOperand& src) {
            return src.field == GcnOperandField::LiteralConst;
        };

        auto iter =
            std::find_if(std::begin(m_instruction.src), std::end(m_instruction.src), isLiteralSrc);

        hasLiteral = (iter != std::end(m_instruction.src));
        if (hasLiteral) {
            u32 literalConst = code.readu32();
            iter->literalConst = literalConst;
            break;
        }

    } while (false);

    if (hasLiteral) {
        // Increase instruction length by 4
        // if literal constant appended.
        m_instruction.length += sizeof(u32);
    }
}

void GcnDecodeContext::decodeInstructionSOP1(u32 hexInstruction) {
    u32 ssrc0 = bit::extract(hexInstruction, 7, 0);
    u32 op = bit::extract(hexInstruction, 15, 8);
    u32 sdst = bit::extract(hexInstruction, 22, 16);

    m_instruction.opcode = static_cast<GcnOpcode>(op + static_cast<u32>(GcnOpcodeMap::OP_MAP_SOP1));

    m_instruction.src[0].field = getOperandField(ssrc0);
    m_instruction.src[0].code = ssrc0;
    m_instruction.dst[0].field = getOperandField(sdst);
    m_instruction.dst[0].code = sdst;
    m_instruction.dstCount = 1;
}

void GcnDecodeContext::decodeInstructionSOPP(u32 hexInstruction) {
    u32 op = bit::extract(hexInstruction, 22, 16);

    m_instruction.opcode = static_cast<GcnOpcode>(op + static_cast<u32>(GcnOpcodeMap::OP_MAP_SOPP));

    m_instruction.control.sopp = *reinterpret_cast<GcnInstControlSOPP*>(&hexInstruction);
}

void GcnDecodeContext::decodeInstructionSOPC(u32 hexInstruction) {
    u32 ssrc0 = bit::extract(hexInstruction, 7, 0);
    u32 ssrc1 = bit::extract(hexInstruction, 15, 8);
    u32 op = bit::extract(hexInstruction, 22, 16);

    m_instruction.opcode = static_cast<GcnOpcode>(op + static_cast<u32>(GcnOpcodeMap::OP_MAP_SOPC));

    m_instruction.src[0].field = getOperandField(ssrc0);
    m_instruction.src[0].code = ssrc0;
    m_instruction.src[1].field = getOperandField(ssrc1);
    m_instruction.src[1].code = ssrc1;
}

void GcnDecodeContext::decodeInstructionSOPK(u32 hexInstruction) {
    u32 sdst = bit::extract(hexInstruction, 22, 16);
    u32 op = bit::extract(hexInstruction, 27, 23);

    m_instruction.opcode = static_cast<GcnOpcode>(op + static_cast<u32>(GcnOpcodeMap::OP_MAP_SOPK));

    m_instruction.dst[0].field = getOperandField(sdst);
    m_instruction.dst[0].code = sdst;
    m_instruction.dstCount = 1;

    m_instruction.control.sopk = *reinterpret_cast<GcnInstControlSOPK*>(&hexInstruction);
}

void GcnDecodeContext::decodeInstructionSOP2(u32 hexInstruction) {
    u32 ssrc0 = bit::extract(hexInstruction, 7, 0);
    u32 ssrc1 = bit::extract(hexInstruction, 15, 8);
    u32 sdst = bit::extract(hexInstruction, 22, 16);
    u32 op = bit::extract(hexInstruction, 29, 23);

    m_instruction.opcode = static_cast<GcnOpcode>(op + static_cast<u32>(GcnOpcodeMap::OP_MAP_SOP2));

    m_instruction.src[0].field = getOperandField(ssrc0);
    m_instruction.src[0].code = ssrc0;
    m_instruction.src[1].field = getOperandField(ssrc1);
    m_instruction.src[1].code = ssrc1;
    m_instruction.dst[0].field = getOperandField(sdst);
    m_instruction.dst[0].code = sdst;
    m_instruction.dstCount = 1;
}

void GcnDecodeContext::decodeInstructionVOP1(u32 hexInstruction) {
    u32 src0 = bit::extract(hexInstruction, 8, 0);
    u32 op = bit::extract(hexInstruction, 16, 9);
    u32 vdst = bit::extract(hexInstruction, 24, 17);

    m_instruction.opcode = static_cast<GcnOpcode>(op + static_cast<u32>(GcnOpcodeMap::OP_MAP_VOP1));

    m_instruction.src[0].field = getOperandField(src0);
    m_instruction.src[0].code =
        m_instruction.src[0].field == GcnOperandField::VectorGPR ? src0 - VectorGPRMin : src0;
    m_instruction.dst[0].field = GcnOperandField::VectorGPR;
    m_instruction.dst[0].code = vdst;
    m_instruction.dstCount = 1;

    GcnOpcodeVOP1 vop1Op = static_cast<GcnOpcodeVOP1>(op);
    if (vop1Op == GcnOpcodeVOP1::V_READFIRSTLANE_B32) {
        m_instruction.dst[1].field = getOperandField(vdst);
        m_instruction.dst[1].type = GcnScalarType::Uint32;
        m_instruction.dst[1].code = vdst;
    }
}

void GcnDecodeContext::decodeInstructionVOPC(u32 hexInstruction) {
    u32 src0 = bit::extract(hexInstruction, 8, 0);
    u32 vsrc1 = bit::extract(hexInstruction, 16, 9);
    u32 op = bit::extract(hexInstruction, 24, 17);

    m_instruction.opcode = static_cast<GcnOpcode>(op + static_cast<u32>(GcnOpcodeMap::OP_MAP_VOPC));

    m_instruction.src[0].field = getOperandField(src0);
    m_instruction.src[0].code =
        m_instruction.src[0].field == GcnOperandField::VectorGPR ? src0 - VectorGPRMin : src0;
    m_instruction.src[1].field = GcnOperandField::VectorGPR;
    m_instruction.src[1].code = vsrc1;
    // VOPC dst is forced to VCC.
    // In order to be unified with VOP3 encoding,
    // we store it to dst[1]
    m_instruction.dst[1].field = GcnOperandField::VccLo;
    m_instruction.dst[1].type = GcnScalarType::Uint64;
    m_instruction.dst[1].code = static_cast<u32>(GcnOperandField::VccLo);
}

void GcnDecodeContext::decodeInstructionVOP2(u32 hexInstruction) {
    u32 src0 = bit::extract(hexInstruction, 8, 0);
    u32 vsrc1 = bit::extract(hexInstruction, 16, 9);
    u32 vdst = bit::extract(hexInstruction, 24, 17);
    u32 op = bit::extract(hexInstruction, 30, 25);

    m_instruction.opcode = static_cast<GcnOpcode>(op + static_cast<u32>(GcnOpcodeMap::OP_MAP_VOP2));

    m_instruction.src[0].field = getOperandField(src0);
    m_instruction.src[0].code =
        m_instruction.src[0].field == GcnOperandField::VectorGPR ? src0 - VectorGPRMin : src0;
    m_instruction.src[1].field = GcnOperandField::VectorGPR;
    m_instruction.src[1].code = vsrc1;
    m_instruction.dst[0].field = GcnOperandField::VectorGPR;
    m_instruction.dst[0].code = vdst;
    m_instruction.dstCount = 1;

    GcnOpcodeVOP2 vop2Op = static_cast<GcnOpcodeVOP2>(op);
    if (vop2Op == GcnOpcodeVOP2::V_READLANE_B32 || vop2Op == GcnOpcodeVOP2::V_WRITELANE_B32) {
        // vsrc1 is scalar for lane instructions
        m_instruction.src[1].field = getOperandField(vsrc1);
        // dst is sgpr
        m_instruction.dst[1].field = GcnOperandField::ScalarGPR;
        m_instruction.dst[1].type = GcnScalarType::Uint32;
        m_instruction.dst[1].code = vdst;
    } else if (isVop3BEncoding(m_instruction.opcode)) {
        m_instruction.dst[1].field = GcnOperandField::VccLo;
        m_instruction.dst[1].type = GcnScalarType::Uint64;
        m_instruction.dst[1].code = static_cast<u32>(GcnOperandField::VccLo);
    }
}

void GcnDecodeContext::decodeInstructionSMRD(u32 hexInstruction) {
    u32 sbase = bit::extract(hexInstruction, 14, 9);
    u32 sdst = bit::extract(hexInstruction, 21, 15);
    u32 op = bit::extract(hexInstruction, 26, 22);

    m_instruction.opcode = static_cast<GcnOpcode>(op + static_cast<u32>(GcnOpcodeMap::OP_MAP_SMRD));

    m_instruction.src[0].field = GcnOperandField::ScalarGPR;
    m_instruction.src[0].code = sbase;
    m_instruction.dst[0].field = GcnOperandField::ScalarGPR;
    m_instruction.dst[0].code = sdst;
    m_instruction.dstCount = 1;

    m_instruction.control.smrd = *reinterpret_cast<GcnInstControlSMRD*>(&hexInstruction);

    if (op <= static_cast<u32>(GcnOpcodeSMRD::S_LOAD_DWORDX16)) {
        m_instruction.control.smrd.count = 1 << op;
    } else if (op >= static_cast<u32>(GcnOpcodeSMRD::S_BUFFER_LOAD_DWORD) &&
               op <= static_cast<u32>(GcnOpcodeSMRD::S_BUFFER_LOAD_DWORDX16)) {
        m_instruction.control.smrd.count = 1 << (op - 8);
    }

    if (m_instruction.control.smrd.imm == 0) {
        u32 code = m_instruction.control.smrd.offset;
        m_instruction.src[1].field = getOperandField(code);
        m_instruction.src[1].type = GcnScalarType::Uint32;
        m_instruction.src[1].code = code;
    }
}

void GcnDecodeContext::decodeInstructionVINTRP(u32 hexInstruction) {
    u32 vsrc = bit::extract(hexInstruction, 7, 0);
    u32 op = bit::extract(hexInstruction, 17, 16);
    u32 vdst = bit::extract(hexInstruction, 25, 18);

    m_instruction.opcode =
        static_cast<GcnOpcode>(op + static_cast<u32>(GcnOpcodeMap::OP_MAP_VINTRP));

    m_instruction.src[0].field = GcnOperandField::VectorGPR;
    m_instruction.src[0].code = vsrc;
    m_instruction.dst[0].field = GcnOperandField::VectorGPR;
    m_instruction.dst[0].code = vdst;
    m_instruction.dstCount = 1;

    m_instruction.control.vintrp = *reinterpret_cast<GcnInstControlVINTRP*>(&hexInstruction);
}

void GcnDecodeContext::decodeInstructionVOP3(u64 hexInstruction) {
    u32 vdst = bit::extract(hexInstruction, 7, 0);
    u32 sdst = bit::extract(hexInstruction, 14, 8); // For VOP3B
    u32 op = bit::extract(hexInstruction, 25, 17);
    u32 src0 = bit::extract(hexInstruction, 40, 32);
    u32 src1 = bit::extract(hexInstruction, 49, 41);
    u32 src2 = bit::extract(hexInstruction, 58, 50);

    if (op >= static_cast<u32>(GcnOpcodeVOP3::V_CMP_F_F32) &&
        op <= static_cast<u32>(GcnOpcodeVOP3::V_CMPX_T_U64)) {
        // Map from VOP3 to VOPC
        u32 vopcOp = op - static_cast<u32>(GcnOpMapVOP3VOPX::VOP3_TO_VOPC);
        m_instruction.opcode =
            static_cast<GcnOpcode>(vopcOp + static_cast<u32>(GcnOpcodeMap::OP_MAP_VOPC));
    } else if (op >= static_cast<u32>(GcnOpcodeVOP3::V_CNDMASK_B32) &&
               op <= static_cast<u32>(GcnOpcodeVOP3::V_CVT_PK_I16_I32)) {
        // Map from VOP3 to VOP2
        u32 vop2Op = op - static_cast<u32>(GcnOpMapVOP3VOPX::VOP3_TO_VOP2);
        m_instruction.opcode =
            static_cast<GcnOpcode>(vop2Op + static_cast<u32>(GcnOpcodeMap::OP_MAP_VOP2));
    } else if (op >= static_cast<u32>(GcnOpcodeVOP3::V_NOP) &&
               op <= static_cast<u32>(GcnOpcodeVOP3::V_MOVRELSD_B32)) {
        // Map from VOP3 to VOP1
        u32 vop1Op = op - static_cast<u32>(GcnOpMapVOP3VOPX::VOP3_TO_VOP1);
        m_instruction.opcode =
            static_cast<GcnOpcode>(vop1Op + static_cast<u32>(GcnOpcodeMap::OP_MAP_VOP1));
    } else {
        // VOP3 encoding, do not map.
        m_instruction.opcode =
            static_cast<GcnOpcode>(op + static_cast<u32>(GcnOpcodeMap::OP_MAP_VOP3));
    }

    m_instruction.src[0].field = getOperandField(src0);
    m_instruction.src[0].code =
        m_instruction.src[0].field == GcnOperandField::VectorGPR ? src0 - VectorGPRMin : src0;
    m_instruction.src[1].field = getOperandField(src1);
    m_instruction.src[1].code =
        m_instruction.src[1].field == GcnOperandField::VectorGPR ? src1 - VectorGPRMin : src1;
    m_instruction.src[2].field = getOperandField(src2);
    m_instruction.src[2].code =
        m_instruction.src[2].field == GcnOperandField::VectorGPR ? src2 - VectorGPRMin : src2;
    m_instruction.dst[0].field = GcnOperandField::VectorGPR;
    m_instruction.dst[0].code = vdst;

    GcnOpcodeVOP3 vop3Op = static_cast<GcnOpcodeVOP3>(op);
    if (isVop3BEncoding(m_instruction.opcode)) {
        m_instruction.dst[1].field = GcnOperandField::ScalarGPR;
        m_instruction.dst[1].type = GcnScalarType::Uint64;
        m_instruction.dst[1].code = sdst;
    } else {
        if (vop3Op >= GcnOpcodeVOP3::V_CMP_F_F32 && vop3Op <= GcnOpcodeVOP3::V_CMPX_T_U64) {
            m_instruction.dst[1].field = getOperandField(vdst);
            m_instruction.dst[1].type = GcnScalarType::Uint64;
            m_instruction.dst[1].code = vdst;
        } else if (vop3Op >= GcnOpcodeVOP3::V_READLANE_B32 &&
                   vop3Op <= GcnOpcodeVOP3::V_WRITELANE_B32) {
            // vsrc1 is scalar for lane instructions
            m_instruction.src[1].field = getOperandField(src1);
            // dst is sgpr for lane instruction
            m_instruction.dst[1].field = GcnOperandField::ScalarGPR;
            m_instruction.dst[1].type = GcnScalarType::Uint32;
            m_instruction.dst[1].code = vdst;
        }
    }

    if (op >= static_cast<u32>(GcnOpcodeVOP3::V_ADD_I32) &&
        op <= static_cast<u32>(GcnOpcodeVOP3::V_DIV_SCALE_F64)) {
        // VOP3B has a sdst operand.
        m_instruction.dstCount = 2;
    } else {
        m_instruction.dstCount = 1;
    }

    m_instruction.control.vop3 = *reinterpret_cast<GcnInstControlVOP3*>(&hexInstruction);

    // update input modifier
    auto& control = m_instruction.control.vop3;
    for (u32 i = 0; i != 3; ++i) {
        if (control.abs & (1u << i)) {
            m_instruction.src[i].inputModifier.set(GcnInputModifier::Abs);
        }

        if (control.neg & (1u << i)) {
            m_instruction.src[i].inputModifier.set(GcnInputModifier::Neg);
        }
    }

    // update output modifier
    auto& outputMod = m_instruction.dst[0].outputModifier;

    outputMod.clamp = static_cast<bool>(control.clmp);
    switch (control.omod) {
    case 0:
        outputMod.multiplier = std::numeric_limits<float>::quiet_NaN();
        break;
    case 1:
        outputMod.multiplier = 2.0f;
        break;
    case 2:
        outputMod.multiplier = 4.0f;
        break;
    case 3:
        outputMod.multiplier = 0.5f;
        break;
    }
}

void GcnDecodeContext::decodeInstructionMUBUF(u64 hexInstruction) {
    u32 op = bit::extract(hexInstruction, 24, 18);
    u32 vaddr = bit::extract(hexInstruction, 39, 32);
    u32 vdata = bit::extract(hexInstruction, 47, 40);
    u32 srsrc = bit::extract(hexInstruction, 52, 48);
    u32 soffset = bit::extract(hexInstruction, 63, 56);

    m_instruction.opcode =
        static_cast<GcnOpcode>(op + static_cast<u32>(GcnOpcodeMap::OP_MAP_MUBUF));

    m_instruction.src[0].field = GcnOperandField::VectorGPR;
    m_instruction.src[0].code = vaddr;
    m_instruction.src[1].field = GcnOperandField::VectorGPR;
    m_instruction.src[1].code = vdata;
    m_instruction.src[2].field = GcnOperandField::ScalarGPR;
    m_instruction.src[2].code = srsrc;
    m_instruction.src[3].field = getOperandField(soffset);
    m_instruction.src[3].code = soffset;

    m_instruction.control.mubuf = *reinterpret_cast<GcnInstControlMUBUF*>(&hexInstruction);

    if (op >= static_cast<u32>(GcnOpcodeMUBUF::BUFFER_LOAD_FORMAT_X) &&
        op <= static_cast<u32>(GcnOpcodeMUBUF::BUFFER_LOAD_FORMAT_XYZW)) {
        m_instruction.control.mubuf.count = op + 1;
        m_instruction.control.mubuf.size = (op + 1) * sizeof(u32);
    } else if (op >= static_cast<u32>(GcnOpcodeMUBUF::BUFFER_STORE_FORMAT_X) &&
               op <= static_cast<u32>(GcnOpcodeMUBUF::BUFFER_STORE_FORMAT_XYZW)) {
        m_instruction.control.mubuf.count = op - 3;
        m_instruction.control.mubuf.size = (op - 3) * sizeof(u32);
    } else if (op >= static_cast<u32>(GcnOpcodeMUBUF::BUFFER_LOAD_DWORD) &&
               op <= static_cast<u32>(GcnOpcodeMUBUF::BUFFER_LOAD_DWORDX3)) {
        m_instruction.control.mubuf.count =
            op == static_cast<u32>(GcnOpcodeMUBUF::BUFFER_LOAD_DWORDX3) ? 3 : 1 << (op - 12);
        m_instruction.control.mubuf.size = m_instruction.control.mubuf.count * sizeof(u32);
    } else if (op >= static_cast<u32>(GcnOpcodeMUBUF::BUFFER_STORE_DWORD) &&
               op <= static_cast<u32>(GcnOpcodeMUBUF::BUFFER_STORE_DWORDX3)) {
        m_instruction.control.mubuf.count =
            op == static_cast<u32>(GcnOpcodeMUBUF::BUFFER_STORE_DWORDX3) ? 3 : 1 << (op - 28);
        m_instruction.control.mubuf.size = m_instruction.control.mubuf.count * sizeof(u32);
    } else if (op >= static_cast<u32>(GcnOpcodeMUBUF::BUFFER_LOAD_UBYTE) &&
               op <= static_cast<u32>(GcnOpcodeMUBUF::BUFFER_LOAD_SSHORT)) {
        m_instruction.control.mubuf.count = 1;
        if (op >= static_cast<u32>(GcnOpcodeMUBUF::BUFFER_LOAD_UBYTE) &&
            op <= static_cast<u32>(GcnOpcodeMUBUF::BUFFER_LOAD_SBYTE)) {
            m_instruction.control.mubuf.size = 1;
        } else {
            m_instruction.control.mubuf.size = 2;
        }
    } else if (op >= static_cast<u32>(GcnOpcodeMUBUF::BUFFER_STORE_BYTE) &&
               op <= static_cast<u32>(GcnOpcodeMUBUF::BUFFER_STORE_SHORT)) {
        m_instruction.control.mubuf.count = 1;
        if (op == static_cast<u32>(GcnOpcodeMUBUF::BUFFER_STORE_BYTE)) {
            m_instruction.control.mubuf.size = 1;
        } else {
            m_instruction.control.mubuf.size = 2;
        }
    } else if (op >= static_cast<u32>(GcnOpcodeMUBUF::BUFFER_ATOMIC_SWAP) &&
               op <= static_cast<u32>(GcnOpcodeMUBUF::BUFFER_ATOMIC_FMAX)) {
        m_instruction.control.mubuf.count = 1;
        m_instruction.control.mubuf.size = sizeof(u32);
    } else if (op >= static_cast<u32>(GcnOpcodeMUBUF::BUFFER_ATOMIC_SWAP_X2) &&
               op <= static_cast<u32>(GcnOpcodeMUBUF::BUFFER_ATOMIC_FMAX_X2)) {
        m_instruction.control.mubuf.count = 2;
        m_instruction.control.mubuf.size = sizeof(u32) * 2;
    }
}

void GcnDecodeContext::decodeInstructionMTBUF(u64 hexInstruction) {
    u32 op = bit::extract(hexInstruction, 18, 16);
    u32 vaddr = bit::extract(hexInstruction, 39, 32);
    u32 vdata = bit::extract(hexInstruction, 47, 40);
    u32 srsrc = bit::extract(hexInstruction, 52, 48);
    u32 soffset = bit::extract(hexInstruction, 63, 56);

    m_instruction.opcode =
        static_cast<GcnOpcode>(op + static_cast<u32>(GcnOpcodeMap::OP_MAP_MTBUF));

    m_instruction.src[0].field = GcnOperandField::VectorGPR;
    m_instruction.src[0].code = vaddr;
    m_instruction.src[1].field = GcnOperandField::VectorGPR;
    m_instruction.src[1].code = vdata;
    m_instruction.src[2].field = GcnOperandField::ScalarGPR;
    m_instruction.src[2].code = srsrc;
    m_instruction.src[3].field = getOperandField(soffset);
    m_instruction.src[3].code = soffset;

    m_instruction.control.mtbuf = *reinterpret_cast<GcnInstControlMTBUF*>(&hexInstruction);

    if (op >= static_cast<u32>(GcnOpcodeMTBUF::TBUFFER_LOAD_FORMAT_X) &&
        op <= static_cast<u32>(GcnOpcodeMTBUF::TBUFFER_LOAD_FORMAT_XYZW)) {
        m_instruction.control.mtbuf.count = op + 1;
    } else if (op >= static_cast<u32>(GcnOpcodeMTBUF::TBUFFER_STORE_FORMAT_X) &&
               op <= static_cast<u32>(GcnOpcodeMTBUF::TBUFFER_STORE_FORMAT_XYZW)) {
        m_instruction.control.mtbuf.count = op - 3;
    }
}

u32 GcnDecodeContext::getMimgModifier(GcnOpcode opcode) {
    GcnMimgModifierFlags flags = {};

    switch (opcode) {
    case GcnOpcode::IMAGE_SAMPLE:
        break;
    case GcnOpcode::IMAGE_SAMPLE_CL:
        flags.set(GcnMimgModifier::LodClamp);
        break;
    case GcnOpcode::IMAGE_SAMPLE_D:
        flags.set(GcnMimgModifier::Derivative);
        break;
    case GcnOpcode::IMAGE_SAMPLE_D_CL:
        flags.set(GcnMimgModifier::Derivative, GcnMimgModifier::LodClamp);
        break;
    case GcnOpcode::IMAGE_SAMPLE_L:
        flags.set(GcnMimgModifier::Lod);
        break;
    case GcnOpcode::IMAGE_SAMPLE_B:
        flags.set(GcnMimgModifier::LodBias);
        break;
    case GcnOpcode::IMAGE_SAMPLE_B_CL:
        flags.set(GcnMimgModifier::LodBias, GcnMimgModifier::LodClamp);
        break;
    case GcnOpcode::IMAGE_SAMPLE_LZ:
        flags.set(GcnMimgModifier::Level0);
        break;
    case GcnOpcode::IMAGE_SAMPLE_C:
        flags.set(GcnMimgModifier::Pcf);
        break;
    case GcnOpcode::IMAGE_SAMPLE_C_CL:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::LodClamp);
        break;
    case GcnOpcode::IMAGE_SAMPLE_C_D:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::Derivative);
        break;
    case GcnOpcode::IMAGE_SAMPLE_C_D_CL:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::Derivative, GcnMimgModifier::LodClamp);
        break;
    case GcnOpcode::IMAGE_SAMPLE_C_L:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::Lod);
        break;
    case GcnOpcode::IMAGE_SAMPLE_C_B:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::LodBias);
        break;
    case GcnOpcode::IMAGE_SAMPLE_C_B_CL:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::LodBias, GcnMimgModifier::LodClamp);
        break;
    case GcnOpcode::IMAGE_SAMPLE_C_LZ:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::Level0);
        break;
    case GcnOpcode::IMAGE_SAMPLE_O:
        flags.set(GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_SAMPLE_CL_O:
        flags.set(GcnMimgModifier::LodClamp, GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_SAMPLE_D_O:
        flags.set(GcnMimgModifier::Derivative, GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_SAMPLE_D_CL_O:
        flags.set(GcnMimgModifier::Derivative, GcnMimgModifier::LodClamp, GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_SAMPLE_L_O:
        flags.set(GcnMimgModifier::Lod, GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_SAMPLE_B_O:
        flags.set(GcnMimgModifier::LodBias, GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_SAMPLE_B_CL_O:
        flags.set(GcnMimgModifier::LodBias, GcnMimgModifier::LodClamp, GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_SAMPLE_LZ_O:
        flags.set(GcnMimgModifier::Level0, GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_SAMPLE_C_O:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_SAMPLE_C_CL_O:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::LodClamp, GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_SAMPLE_C_D_O:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::Derivative, GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_SAMPLE_C_D_CL_O:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::Derivative, GcnMimgModifier::LodClamp,
                  GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_SAMPLE_C_L_O:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::Lod, GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_SAMPLE_C_B_O:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::LodBias, GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_SAMPLE_C_B_CL_O:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::LodBias, GcnMimgModifier::LodClamp,
                  GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_SAMPLE_C_LZ_O:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::Level0, GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_GATHER4:
        break;
    case GcnOpcode::IMAGE_GATHER4_CL:
        flags.set(GcnMimgModifier::LodClamp);
        break;
    case GcnOpcode::IMAGE_GATHER4_L:
        flags.set(GcnMimgModifier::Lod);
        break;
    case GcnOpcode::IMAGE_GATHER4_B:
        flags.set(GcnMimgModifier::LodBias);
        break;
    case GcnOpcode::IMAGE_GATHER4_B_CL:
        flags.set(GcnMimgModifier::LodBias, GcnMimgModifier::LodClamp);
        break;
    case GcnOpcode::IMAGE_GATHER4_LZ:
        flags.set(GcnMimgModifier::Level0);
        break;
    case GcnOpcode::IMAGE_GATHER4_C:
        flags.set(GcnMimgModifier::Pcf);
        break;
    case GcnOpcode::IMAGE_GATHER4_C_CL:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::LodClamp);
        break;
    case GcnOpcode::IMAGE_GATHER4_C_L:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::Lod);
        break;
    case GcnOpcode::IMAGE_GATHER4_C_B:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::LodBias);
        break;
    case GcnOpcode::IMAGE_GATHER4_C_B_CL:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::LodBias, GcnMimgModifier::LodClamp);
        break;
    case GcnOpcode::IMAGE_GATHER4_C_LZ:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::Level0);
        break;
    case GcnOpcode::IMAGE_GATHER4_O:
        flags.set(GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_GATHER4_CL_O:
        flags.set(GcnMimgModifier::LodClamp, GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_GATHER4_L_O:
        flags.set(GcnMimgModifier::Lod, GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_GATHER4_B_O:
        flags.set(GcnMimgModifier::LodBias, GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_GATHER4_B_CL_O:
        flags.set(GcnMimgModifier::LodBias, GcnMimgModifier::LodClamp, GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_GATHER4_LZ_O:
        flags.set(GcnMimgModifier::Level0, GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_GATHER4_C_O:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_GATHER4_C_CL_O:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::LodClamp, GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_GATHER4_C_L_O:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::Lod, GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_GATHER4_C_B_O:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::LodBias, GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_GATHER4_C_B_CL_O:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::LodBias, GcnMimgModifier::LodClamp,
                  GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_GATHER4_C_LZ_O:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::Level0, GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_SAMPLE_CD:
        flags.set(GcnMimgModifier::CoarseDerivative);
        break;
    case GcnOpcode::IMAGE_SAMPLE_CD_CL:
        flags.set(GcnMimgModifier::CoarseDerivative, GcnMimgModifier::LodClamp);
        break;
    case GcnOpcode::IMAGE_SAMPLE_C_CD:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::CoarseDerivative);
        break;
    case GcnOpcode::IMAGE_SAMPLE_C_CD_CL:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::CoarseDerivative,
                  GcnMimgModifier::LodClamp);
        break;
    case GcnOpcode::IMAGE_SAMPLE_CD_O:
        flags.set(GcnMimgModifier::CoarseDerivative, GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_SAMPLE_CD_CL_O:
        flags.set(GcnMimgModifier::CoarseDerivative, GcnMimgModifier::LodClamp,
                  GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_SAMPLE_C_CD_O:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::CoarseDerivative, GcnMimgModifier::Offset);
        break;
    case GcnOpcode::IMAGE_SAMPLE_C_CD_CL_O:
        flags.set(GcnMimgModifier::Pcf, GcnMimgModifier::CoarseDerivative,
                  GcnMimgModifier::LodClamp, GcnMimgModifier::Offset);
        break;
    }

    return flags.raw();
}

void GcnDecodeContext::decodeInstructionMIMG(u64 hexInstruction) {
    u32 op = bit::extract(hexInstruction, 24, 18);
    u32 vaddr = bit::extract(hexInstruction, 39, 32);
    u32 vdata = bit::extract(hexInstruction, 47, 40);
    u32 srsrc = bit::extract(hexInstruction, 52, 48);
    u32 ssamp = bit::extract(hexInstruction, 57, 53);

    m_instruction.opcode = static_cast<GcnOpcode>(op + static_cast<u32>(GcnOpcodeMap::OP_MAP_MIMG));

    m_instruction.src[0].field = GcnOperandField::VectorGPR;
    m_instruction.src[0].code = vaddr;
    m_instruction.src[1].field = GcnOperandField::VectorGPR;
    m_instruction.src[1].code = vdata;
    m_instruction.src[2].field = GcnOperandField::ScalarGPR;
    m_instruction.src[2].code = srsrc;
    m_instruction.src[3].field = GcnOperandField::ScalarGPR;
    m_instruction.src[3].code = ssamp;

    m_instruction.control.mimg = *reinterpret_cast<GcnInstControlMIMG*>(&hexInstruction);
    m_instruction.control.mimg.mod = getMimgModifier(m_instruction.opcode);
}

void GcnDecodeContext::decodeInstructionDS(u64 hexInstruction) {
    u32 op = bit::extract(hexInstruction, 25, 18);
    u32 addr = bit::extract(hexInstruction, 39, 32);
    u32 data0 = bit::extract(hexInstruction, 47, 40);
    u32 data1 = bit::extract(hexInstruction, 55, 48);
    u32 vdst = bit::extract(hexInstruction, 63, 56);

    m_instruction.opcode = static_cast<GcnOpcode>(op + static_cast<u32>(GcnOpcodeMap::OP_MAP_DS));

    m_instruction.src[0].field = GcnOperandField::VectorGPR;
    m_instruction.src[0].code = addr;
    m_instruction.src[1].field = GcnOperandField::VectorGPR;
    m_instruction.src[1].code = data0;
    m_instruction.src[2].field = GcnOperandField::VectorGPR;
    m_instruction.src[2].code = data1;
    m_instruction.dst[0].field = GcnOperandField::VectorGPR;
    m_instruction.dst[0].code = vdst;
    m_instruction.dstCount = 1;

    m_instruction.control.ds = *reinterpret_cast<GcnInstControlDS*>(&hexInstruction);

    auto instFormat = gcnInstructionFormat(GcnInstEncoding::DS, op);

    m_instruction.control.ds.dual = (GcnOpcodeDS)op == GcnOpcodeDS::DS_WRITE2_B32 ||
                                    (GcnOpcodeDS)op == GcnOpcodeDS::DS_WRXCHG2_RTN_B32 ||
                                    (GcnOpcodeDS)op == GcnOpcodeDS::DS_READ2_B32 ||
                                    (GcnOpcodeDS)op == GcnOpcodeDS::DS_WRITE2_B64 ||
                                    (GcnOpcodeDS)op == GcnOpcodeDS::DS_WRXCHG2_RTN_B64 ||
                                    (GcnOpcodeDS)op == GcnOpcodeDS::DS_READ2_B64;

    m_instruction.control.ds.sign = instFormat.srcType == GcnScalarType::Sint32;

    m_instruction.control.ds.relative = op >= static_cast<u32>(GcnOpcodeDS::DS_ADD_SRC2_U32) &&
                                        op <= static_cast<u32>(GcnOpcodeDS::DS_MAX_SRC2_F64);

    m_instruction.control.ds.stride = (GcnOpcodeDS)op == GcnOpcodeDS::DS_WRITE2ST64_B32 ||
                                      (GcnOpcodeDS)op == GcnOpcodeDS::DS_WRXCHG2ST64_RTN_B32 ||
                                      (GcnOpcodeDS)op == GcnOpcodeDS::DS_READ2ST64_B32 ||
                                      (GcnOpcodeDS)op == GcnOpcodeDS::DS_WRITE2ST64_B64 ||
                                      (GcnOpcodeDS)op == GcnOpcodeDS::DS_WRXCHG2ST64_RTN_B64 ||
                                      (GcnOpcodeDS)op == GcnOpcodeDS::DS_READ2ST64_B64;

    if ((GcnOpcodeDS)op == GcnOpcodeDS::DS_WRITE_B8 || (GcnOpcodeDS)op == GcnOpcodeDS::DS_READ_I8 ||
        (GcnOpcodeDS)op == GcnOpcodeDS::DS_READ_U8) {
        m_instruction.control.ds.size = 1;
    } else if ((GcnOpcodeDS)op == GcnOpcodeDS::DS_WRITE_B16 ||
               (GcnOpcodeDS)op == GcnOpcodeDS::DS_READ_I16 ||
               (GcnOpcodeDS)op == GcnOpcodeDS::DS_READ_U16) {
        m_instruction.control.ds.size = 2;
    } else {
        if (instFormat.srcType == GcnScalarType::Sint32 ||
            instFormat.srcType == GcnScalarType::Uint32) {
            m_instruction.control.ds.size = 4;
        } else if (instFormat.srcType == GcnScalarType::Sint64 ||
                   instFormat.srcType == GcnScalarType::Uint64) {
            m_instruction.control.ds.size = 8;
        } else {
            m_instruction.control.ds.size = 0;
        }
    }
}

void GcnDecodeContext::decodeInstructionEXP(u64 hexInstruction) {
    u32 vsrc0 = bit::extract(hexInstruction, 39, 32);
    u32 vsrc1 = bit::extract(hexInstruction, 47, 40);
    u32 vsrc2 = bit::extract(hexInstruction, 55, 48);
    u32 vsrc3 = bit::extract(hexInstruction, 63, 56);

    m_instruction.opcode = GcnOpcode::EXP;

    m_instruction.src[0].field = GcnOperandField::VectorGPR;
    m_instruction.src[0].code = vsrc0;
    m_instruction.src[1].field = GcnOperandField::VectorGPR;
    m_instruction.src[1].code = vsrc1;
    m_instruction.src[2].field = GcnOperandField::VectorGPR;
    m_instruction.src[2].code = vsrc2;
    m_instruction.src[3].field = GcnOperandField::VectorGPR;
    m_instruction.src[3].code = vsrc3;

    m_instruction.control.exp = *reinterpret_cast<GcnInstControlEXP*>(&hexInstruction);
}

///////////////////////////////////////////////////////////////

inline void gcnInstCastToSOP1(const GcnShaderInstruction& ins, GcnShaderInstSOP1& result) {
    result.opcode = ins.opcode;
    result.length = ins.length;
    result.opClass = ins.opClass;

    result.ssrc0 = ins.src[0];
    result.sdst = ins.dst[0];
}

inline void gcnInstCastToSOP2(const GcnShaderInstruction& ins, GcnShaderInstSOP2& result) {
    result.opcode = ins.opcode;
    result.length = ins.length;
    result.opClass = ins.opClass;

    result.ssrc0 = ins.src[0];
    result.ssrc1 = ins.src[1];
    result.sdst = ins.dst[0];
}

inline void gcnInstCastToSOPK(const GcnShaderInstruction& ins, GcnShaderInstSOPK& result) {
    result.opcode = ins.opcode;
    result.length = ins.length;
    result.opClass = ins.opClass;

    result.control = ins.control.sopk;
    result.sdst = ins.dst[0];
}

inline void gcnInstCastToSOPC(const GcnShaderInstruction& ins, GcnShaderInstSOPC& result) {
    result.opcode = ins.opcode;
    result.length = ins.length;
    result.opClass = ins.opClass;

    result.ssrc0 = ins.src[0];
    result.ssrc1 = ins.src[1];
}

inline void gcnInstCastToSOPP(const GcnShaderInstruction& ins, GcnShaderInstSOPP& result) {
    result.opcode = ins.opcode;
    result.length = ins.length;
    result.opClass = ins.opClass;

    result.control = ins.control.sopp;
}

inline void gcnInstCastToVOP1(const GcnShaderInstruction& ins, GcnShaderInstVOP1& result) {
    result.opcode = ins.opcode;
    result.length = ins.length;
    result.opClass = ins.opClass;

    result.src0 = ins.src[0];
    result.vdst = ins.dst[0];
}

inline void gcnInstCastToVOP2(const GcnShaderInstruction& ins, GcnShaderInstVOP2& result) {
    result.opcode = ins.opcode;
    result.length = ins.length;
    result.opClass = ins.opClass;

    result.src0 = ins.src[0];
    result.vsrc1 = ins.src[1];
    result.vdst = ins.dst[0];
}

inline void gcnInstCastToVOP3(const GcnShaderInstruction& ins, GcnShaderInstVOP3& result) {
    result.opcode = ins.opcode;
    result.length = ins.length;
    result.opClass = ins.opClass;

    result.control = ins.control.vop3;
    result.src0 = ins.src[0];
    result.src1 = ins.src[1];
    result.src2 = ins.src[2];
    result.vdst = ins.dst[0];
    result.sdst = ins.dst[1];
}

inline void gcnInstCastToVOPC(const GcnShaderInstruction& ins, GcnShaderInstVOPC& result) {
    result.opcode = ins.opcode;
    result.length = ins.length;
    result.opClass = ins.opClass;

    result.src0 = ins.src[0];
    result.vsrc1 = ins.src[1];
}

inline void gcnInstCastToSMRD(const GcnShaderInstruction& ins, GcnShaderInstSMRD& result) {
    result.opcode = ins.opcode;
    result.length = ins.length;
    result.opClass = ins.opClass;

    result.control = ins.control.smrd;
    result.sbase = ins.src[0];
    result.sdst = ins.dst[0];

    if (!result.control.imm) {
        result.offset = ins.src[1];
    }
}

inline void gcnInstCastToMUBUF(const GcnShaderInstruction& ins, GcnShaderInstMUBUF& result) {
    result.opcode = ins.opcode;
    result.length = ins.length;
    result.opClass = ins.opClass;

    result.control = ins.control.mubuf;
    result.vaddr = ins.src[0];
    result.vdata = ins.src[1];
    result.srsrc = ins.src[2];
    result.soffset = ins.src[3];
}

inline void gcnInstCastToMTBUF(const GcnShaderInstruction& ins, GcnShaderInstMTBUF& result) {
    result.opcode = ins.opcode;
    result.length = ins.length;
    result.opClass = ins.opClass;

    result.control = ins.control.mtbuf;
    result.vaddr = ins.src[0];
    result.vdata = ins.src[1];
    result.srsrc = ins.src[2];
    result.soffset = ins.src[3];
}

inline void gcnInstCastToMIMG(const GcnShaderInstruction& ins, GcnShaderInstMIMG& result) {
    result.opcode = ins.opcode;
    result.length = ins.length;
    result.opClass = ins.opClass;

    result.control = ins.control.mimg;
    result.vaddr = ins.src[0];
    result.vdata = ins.src[1];
    result.srsrc = ins.src[2];
    result.ssamp = ins.src[3];
}

inline void gcnInstCastToVINTRP(const GcnShaderInstruction& ins, GcnShaderInstVINTRP& result) {
    result.opcode = ins.opcode;
    result.length = ins.length;
    result.opClass = ins.opClass;

    result.control = ins.control.vintrp;
    result.vsrc = ins.src[0];
    result.vdst = ins.dst[0];
}

inline void gcnInstCastToDS(const GcnShaderInstruction& ins, GcnShaderInstDS& result) {
    result.opcode = ins.opcode;
    result.length = ins.length;
    result.opClass = ins.opClass;

    result.control = ins.control.ds;
    result.addr = ins.src[0];
    result.data0 = ins.src[1];
    result.data1 = ins.src[2];
    result.vdst = ins.dst[0];
}

inline void gcnInstCastToEXP(const GcnShaderInstruction& ins, GcnShaderInstEXP& result) {
    result.opcode = ins.opcode;
    result.length = ins.length;
    result.opClass = ins.opClass;

    result.control = ins.control.exp;
    result.vsrc0 = ins.src[0];
    result.vsrc1 = ins.src[1];
    result.vsrc2 = ins.src[2];
    result.vsrc3 = ins.src[3];

    u32 target = result.control.target;
    if (target >= 0 && target <= 7) {
        result.target = GcnExportTarget::Mrt;
    } else if (target == 8) {
        result.target = GcnExportTarget::MrtZ;
    } else if (target == 9) {
        result.target = GcnExportTarget::Null;
    } else if (target >= 12 && target <= 15) {
        result.target = GcnExportTarget::Pos;
    } else if (target >= 32 && target <= 63) {
        result.target = GcnExportTarget::Param;
    } else {
        UNREACHABLE_MSG("Error export target value.");
    }
}

GcnInstructionVariant gcnInstructionConvert(const GcnShaderInstruction& ins) {
    GcnInstructionVariant result = {};
    switch (ins.encoding) {
    case GcnInstEncoding::SOP1:
        result = GcnShaderInstSOP1();
        gcnInstCastToSOP1(ins, std::get<GcnShaderInstSOP1>(result));
        break;
    case GcnInstEncoding::SOPP:
        result = GcnShaderInstSOPP();
        gcnInstCastToSOPP(ins, std::get<GcnShaderInstSOPP>(result));
        break;
    case GcnInstEncoding::SOPC:
        result = GcnShaderInstSOPC();
        gcnInstCastToSOPC(ins, std::get<GcnShaderInstSOPC>(result));
        break;
    case GcnInstEncoding::VOP1:
        result = GcnShaderInstVOP1();
        gcnInstCastToVOP1(ins, std::get<GcnShaderInstVOP1>(result));
        break;
    case GcnInstEncoding::VOPC:
        result = GcnShaderInstVOPC();
        gcnInstCastToVOPC(ins, std::get<GcnShaderInstVOPC>(result));
        break;
    case GcnInstEncoding::VOP3:
        result = GcnShaderInstVOP3();
        gcnInstCastToVOP3(ins, std::get<GcnShaderInstVOP3>(result));
        break;
    case GcnInstEncoding::EXP:
        result = GcnShaderInstEXP();
        gcnInstCastToEXP(ins, std::get<GcnShaderInstEXP>(result));
        break;
    case GcnInstEncoding::VINTRP:
        result = GcnShaderInstVINTRP();
        gcnInstCastToVINTRP(ins, std::get<GcnShaderInstVINTRP>(result));
        break;
    case GcnInstEncoding::DS:
        result = GcnShaderInstDS();
        gcnInstCastToDS(ins, std::get<GcnShaderInstDS>(result));
        break;
    case GcnInstEncoding::MUBUF:
        result = GcnShaderInstMUBUF();
        gcnInstCastToMUBUF(ins, std::get<GcnShaderInstMUBUF>(result));
        break;
    case GcnInstEncoding::MTBUF:
        result = GcnShaderInstMTBUF();
        gcnInstCastToMTBUF(ins, std::get<GcnShaderInstMTBUF>(result));
        break;
    case GcnInstEncoding::MIMG:
        result = GcnShaderInstMIMG();
        gcnInstCastToMIMG(ins, std::get<GcnShaderInstMIMG>(result));
        break;
    case GcnInstEncoding::SMRD:
        result = GcnShaderInstSMRD();
        gcnInstCastToSMRD(ins, std::get<GcnShaderInstSMRD>(result));
        break;
    case GcnInstEncoding::SOPK:
        result = GcnShaderInstSOPK();
        gcnInstCastToSOPK(ins, std::get<GcnShaderInstSOPK>(result));
        break;
    case GcnInstEncoding::SOP2:
        result = GcnShaderInstSOP2();
        gcnInstCastToSOP2(ins, std::get<GcnShaderInstSOP2>(result));
        break;
    case GcnInstEncoding::VOP2:
        result = GcnShaderInstVOP2();
        gcnInstCastToVOP2(ins, std::get<GcnShaderInstVOP2>(result));
        break;
    }
    return result;
}

} // namespace Shader::Gcn
