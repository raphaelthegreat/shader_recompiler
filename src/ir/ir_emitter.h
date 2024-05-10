// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <cstring>
#include <type_traits>

#include "ir/attribute.h"
#include "ir/basic_block.h"
#include "ir/condition.h"
#include "ir/value.h"

namespace Shader::IR {

class IREmitter {
public:
    explicit IREmitter(Block& block_) : block{&block_}, insertion_point{block->end()} {}
    explicit IREmitter(Block& block_, Block::iterator insertion_point_)
        : block{&block_}, insertion_point{insertion_point_} {}

    Block* block;

    [[nodiscard]] U1 Imm1(bool value) const;
    [[nodiscard]] U8 Imm8(u8 value) const;
    [[nodiscard]] U16 Imm16(u16 value) const;
    [[nodiscard]] U32 Imm32(u32 value) const;
    [[nodiscard]] U32 Imm32(s32 value) const;
    [[nodiscard]] F32 Imm32(f32 value) const;
    [[nodiscard]] U64 Imm64(u64 value) const;
    [[nodiscard]] U64 Imm64(s64 value) const;
    [[nodiscard]] F64 Imm64(f64 value) const;

    U1 ConditionRef(const U1& value);
    void Reference(const Value& value);

    void PhiMove(IR::Inst& phi, const Value& value);

    void Prologue();
    void Epilogue();
    void EmitVertex(const U32& stream);
    void EndPrimitive(const U32& stream);

    template <typename T = U32>
    [[nodiscard]] T GetScalarReg(IR::ScalarReg reg);
    template <typename T = U32>
    [[nodiscard]] T GetVectorReg(IR::VectorReg reg);
    void SetScalarReg(IR::ScalarReg reg, const U32F32& value);
    void SetVectorReg(IR::VectorReg reg, const U32F32& value);

    [[nodiscard]] U1 GetGotoVariable(u32 id);
    void SetGotoVariable(u32 id, const U1& value);

    [[nodiscard]] U1 GetScc();
    [[nodiscard]] U1 GetVcc();
    [[nodiscard]] U1 GetExec();

    void SetScc(const U1& value);
    void SetVcc(const U1& value);
    void SetExec(const U1& value);

    [[nodiscard]] U1 Condition(IR::Condition cond);

    [[nodiscard]] F32 GetAttribute(IR::Attribute attribute);
    [[nodiscard]] F32 GetAttribute(IR::Attribute attribute, const U32& vertex);
    [[nodiscard]] U32 GetAttributeU32(IR::Attribute attribute);
    [[nodiscard]] U32 GetAttributeU32(IR::Attribute attribute, const U32& vertex);
    void SetAttribute(IR::Attribute attribute, const F32& value, const U32& vertex);

    void SetFragColor(u32 index, u32 component, const F32& value);
    void SetFragDepth(const F32& value);

    [[nodiscard]] U32 WorkgroupIdX();
    [[nodiscard]] U32 WorkgroupIdY();
    [[nodiscard]] U32 WorkgroupIdZ();

    [[nodiscard]] U32U64 ReadShared(int bit_size, bool is_signed, const U32& offset);
    void WriteShared(int bit_size, const Value& value, const U32& offset);

    [[nodiscard]] Value ReadConst(int num_dwords, const U64& address);
    [[nodiscard]] Value ReadConstBuffer(int num_dwords, const Value& handle, const U32& offset);

    [[nodiscard]] Value LocalInvocationId();
    [[nodiscard]] U32 LocalInvocationIdX();
    [[nodiscard]] U32 LocalInvocationIdY();
    [[nodiscard]] U32 LocalInvocationIdZ();

    [[nodiscard]] U32 InvocationId();
    [[nodiscard]] U32 InvocationInfo();
    [[nodiscard]] U32 SampleId();

    [[nodiscard]] U1 GetZeroFromOp(const Value& op);
    [[nodiscard]] U1 GetSignFromOp(const Value& op);
    [[nodiscard]] U1 GetCarryFromOp(const Value& op);
    [[nodiscard]] U1 GetOverflowFromOp(const Value& op);
    [[nodiscard]] U1 GetSparseFromOp(const Value& op);
    [[nodiscard]] U1 GetInBoundsFromOp(const Value& op);

    [[nodiscard]] Value CompositeConstruct(const Value& e1, const Value& e2);
    [[nodiscard]] Value CompositeConstruct(const Value& e1, const Value& e2, const Value& e3);
    [[nodiscard]] Value CompositeConstruct(const Value& e1, const Value& e2, const Value& e3,
                                           const Value& e4);
    [[nodiscard]] Value CompositeExtract(const Value& vector, size_t element);
    [[nodiscard]] Value CompositeInsert(const Value& vector, const Value& object, size_t element);

    [[nodiscard]] Value Select(const U1& condition, const Value& true_value,
                               const Value& false_value);

    void Barrier();
    void WorkgroupMemoryBarrier();
    void DeviceMemoryBarrier();

    template <typename Dest, typename Source>
    [[nodiscard]] Dest BitCast(const Source& value);

    [[nodiscard]] U64 PackUint2x32(const Value& vector);
    [[nodiscard]] Value UnpackUint2x32(const U64& value);

    [[nodiscard]] U32 PackFloat2x16(const Value& vector);
    [[nodiscard]] Value UnpackFloat2x16(const U32& value);

    [[nodiscard]] U32 PackHalf2x16(const Value& vector);
    [[nodiscard]] Value UnpackHalf2x16(const U32& value);

    [[nodiscard]] F64 PackDouble2x32(const Value& vector);
    [[nodiscard]] Value UnpackDouble2x32(const F64& value);

    [[nodiscard]] F32F64 FPAdd(const F32F64& a, const F32F64& b);
    [[nodiscard]] F32F64 FPMul(const F32F64& a, const F32F64& b);
    [[nodiscard]] F32F64 FPFma(const F32F64& a, const F32F64& b, const F32F64& c);

    [[nodiscard]] F32F64 FPAbs(const F32F64& value);
    [[nodiscard]] F32F64 FPNeg(const F32F64& value);
    [[nodiscard]] F32F64 FPAbsNeg(const F32F64& value, bool abs, bool neg);

    [[nodiscard]] F32 FPCos(const F32& value);
    [[nodiscard]] F32 FPSin(const F32& value);
    [[nodiscard]] F32 FPExp2(const F32& value);
    [[nodiscard]] F32 FPLog2(const F32& value);
    [[nodiscard]] F32F64 FPRecip(const F32F64& value);
    [[nodiscard]] F32F64 FPRecipSqrt(const F32F64& value);
    [[nodiscard]] F32 FPSqrt(const F32& value);
    [[nodiscard]] F32F64 FPSaturate(const F32F64& value);
    [[nodiscard]] F32F64 FPClamp(const F32F64& value, const F32F64& min_value,
                                 const F32F64& max_value);
    [[nodiscard]] F32F64 FPRoundEven(const F32F64& value);
    [[nodiscard]] F32F64 FPFloor(const F32F64& value);
    [[nodiscard]] F32F64 FPCeil(const F32F64& value);
    [[nodiscard]] F32F64 FPTrunc(const F32F64& value);

    [[nodiscard]] U1 FPEqual(const F32F64& lhs, const F32F64& rhs, bool ordered = true);
    [[nodiscard]] U1 FPNotEqual(const F32F64& lhs, const F32F64& rhs, bool ordered = true);
    [[nodiscard]] U1 FPLessThanEqual(const F32F64& lhs, const F32F64& rhs, bool ordered = true);
    [[nodiscard]] U1 FPGreaterThanEqual(const F32F64& lhs, const F32F64& rhs, bool ordered = true);
    [[nodiscard]] U1 FPLessThan(const F32F64& lhs, const F32F64& rhs, bool ordered = true);
    [[nodiscard]] U1 FPGreaterThan(const F32F64& lhs, const F32F64& rhs, bool ordered = true);
    [[nodiscard]] U1 FPIsNan(const F32F64& value);
    [[nodiscard]] U1 FPOrdered(const F32F64& lhs, const F32F64& rhs);
    [[nodiscard]] U1 FPUnordered(const F32F64& lhs, const F32F64& rhs);
    [[nodiscard]] F32F64 FPMax(const F32F64& lhs, const F32F64& rhs);
    [[nodiscard]] F32F64 FPMin(const F32F64& lhs, const F32F64& rhs);

    [[nodiscard]] U32U64 IAdd(const U32U64& a, const U32U64& b);
    [[nodiscard]] U32U64 ISub(const U32U64& a, const U32U64& b);
    [[nodiscard]] U32 IMul(const U32& a, const U32& b);
    [[nodiscard]] U32 IDiv(const U32& a, const U32& b, bool is_signed = false);
    [[nodiscard]] U32U64 INeg(const U32U64& value);
    [[nodiscard]] U32 IAbs(const U32& value);
    [[nodiscard]] U32U64 ShiftLeftLogical(const U32U64& base, const U32& shift);
    [[nodiscard]] U32U64 ShiftRightLogical(const U32U64& base, const U32& shift);
    [[nodiscard]] U32U64 ShiftRightArithmetic(const U32U64& base, const U32& shift);
    [[nodiscard]] U32 BitwiseAnd(const U32& a, const U32& b);
    [[nodiscard]] U32 BitwiseOr(const U32& a, const U32& b);
    [[nodiscard]] U32 BitwiseXor(const U32& a, const U32& b);
    [[nodiscard]] U32 BitFieldInsert(const U32& base, const U32& insert, const U32& offset,
                                     const U32& count);
    [[nodiscard]] U32 BitFieldExtract(const U32& base, const U32& offset, const U32& count,
                                      bool is_signed = false);
    [[nodiscard]] U32 BitReverse(const U32& value);
    [[nodiscard]] U32 BitCount(const U32& value);
    [[nodiscard]] U32 BitwiseNot(const U32& value);

    [[nodiscard]] U32 FindSMsb(const U32& value);
    [[nodiscard]] U32 FindUMsb(const U32& value);
    [[nodiscard]] U32 SMin(const U32& a, const U32& b);
    [[nodiscard]] U32 UMin(const U32& a, const U32& b);
    [[nodiscard]] U32 IMin(const U32& a, const U32& b, bool is_signed);
    [[nodiscard]] U32 SMax(const U32& a, const U32& b);
    [[nodiscard]] U32 UMax(const U32& a, const U32& b);
    [[nodiscard]] U32 IMax(const U32& a, const U32& b, bool is_signed);
    [[nodiscard]] U32 SClamp(const U32& value, const U32& min, const U32& max);
    [[nodiscard]] U32 UClamp(const U32& value, const U32& min, const U32& max);

    [[nodiscard]] U1 ILessThan(const U32& lhs, const U32& rhs, bool is_signed);
    [[nodiscard]] U1 IEqual(const U32U64& lhs, const U32U64& rhs);
    [[nodiscard]] U1 ILessThanEqual(const U32& lhs, const U32& rhs, bool is_signed);
    [[nodiscard]] U1 IGreaterThan(const U32& lhs, const U32& rhs, bool is_signed);
    [[nodiscard]] U1 INotEqual(const U32& lhs, const U32& rhs);
    [[nodiscard]] U1 IGreaterThanEqual(const U32& lhs, const U32& rhs, bool is_signed);

    [[nodiscard]] U1 LogicalOr(const U1& a, const U1& b);
    [[nodiscard]] U1 LogicalAnd(const U1& a, const U1& b);
    [[nodiscard]] U1 LogicalXor(const U1& a, const U1& b);
    [[nodiscard]] U1 LogicalNot(const U1& value);

    [[nodiscard]] U32U64 ConvertFToS(size_t bitsize, const F32F64& value);
    [[nodiscard]] U32U64 ConvertFToU(size_t bitsize, const F32F64& value);
    [[nodiscard]] U32U64 ConvertFToI(size_t bitsize, bool is_signed, const F32F64& value);
    [[nodiscard]] F32F64 ConvertSToF(size_t dest_bitsize, size_t src_bitsize, const Value& value);
    [[nodiscard]] F32F64 ConvertUToF(size_t dest_bitsize, size_t src_bitsize, const Value& value);
    [[nodiscard]] F32F64 ConvertIToF(size_t dest_bitsize, size_t src_bitsize, bool is_signed, const Value& value);

    [[nodiscard]] U32U64 UConvert(size_t result_bitsize, const U32U64& value);
    [[nodiscard]] F16F32F64 FPConvert(size_t result_bitsize, const F16F32F64& value);

    [[nodiscard]] Value ImageQueryDimension(const Value& handle, const IR::U32& lod,
                                            const IR::U1& skip_mips);

private:
    IR::Block::iterator insertion_point;

    template <typename T = Value, typename... Args>
    T Inst(Opcode op, Args... args) {
        auto it{block->PrependNewInst(insertion_point, op, {Value{args}...})};
        return T{Value{&*it}};
    }

    template <typename T>
        requires(sizeof(T) <= sizeof(u32) && std::is_trivially_copyable_v<T>)
    struct Flags {
        Flags() = default;
        Flags(T proxy_) : proxy{proxy_} {}

        T proxy;
    };

    template <typename T = Value, typename FlagType, typename... Args>
    T Inst(Opcode op, Flags<FlagType> flags, Args... args) {
        u32 raw_flags{};
        std::memcpy(&raw_flags, &flags.proxy, sizeof(flags.proxy));
        auto it{block->PrependNewInst(insertion_point, op, {Value{args}...}, raw_flags)};
        return T{Value{&*it}};
    }
};

} // namespace Shader::IR
