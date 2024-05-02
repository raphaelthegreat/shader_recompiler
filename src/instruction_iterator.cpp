#include "instruction.h"
#include "instruction_iterator.h"
#include "instruction_util.h"

namespace Shader::Gcn {

GcnInstructionIterator::GcnInstructionIterator() = default;

GcnInstructionIterator::~GcnInstructionIterator() = default;

void GcnInstructionIterator::advanceProgramCounter(const GcnShaderInstruction& ins) {
    m_programCounter += ins.length;
}

void GcnInstructionIterator::resetProgramCounter(u32 pc) {
    m_programCounter = pc;
}

u32 GcnInstructionIterator::getBranchTarget(const GcnShaderInstruction& ins) {
    return calculateBranchTarget(m_programCounter, ins);
}

} // namespace Shader::Gcn
