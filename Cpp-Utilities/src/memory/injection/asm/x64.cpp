/*
    CONTRIBUTORS:
        Sean Pesce

*/

#include "sp/memory/injection/asm/x64.h"


__SP_NAMESPACE
namespace mem  {
namespace code {
namespace x64  {

 std::unordered_map<uint64_t, TrackChainInjection*> injected_locations;

// Writes bytecode for the series of instructions to perform an absolute JMP r64 (using JMP %rax)
//  and restore the register upon returning. Also overwrites remaining garbage bytecode with
//  the specified number of NOP instructions.
int write_jmp_rax_14b(void *write_to, void *jmp_to, int nops)
{
    int writeOffset = 0; // Keep track of the offset to write to (relative to the injection point)

    // PUSH rax:
    *(uint8_t*)write_to = PUSH_RAX_INSTR;
    writeOffset += PUSH_RAX_INSTR_LENGTH;

    // MOVABS rax, imm64:
    *(uint16_t*)((uint8_t*)write_to + writeOffset) = *(uint16_t*)MOVABS_RAX_INSTR_OPCODE; // Opcode of MOVABS rax, imm64
    writeOffset += MOVABS_OPCODE_LENGTH;
    *(uint64_t*)((uint8_t*)write_to + writeOffset) = (uint64_t)((uint8_t*)jmp_to + SP_ASM_FUNC_START_OFFSET_X64); // Operand of MOVABS rax, imm64
    writeOffset += MOVABS_OPERAND_LENGTH;

    // JMP rax:
    *(uint16_t*)((uint8_t*)write_to + writeOffset) = *(uint16_t*)JMP_ABS_RAX_INSTR;
    writeOffset += JMP_ABS_RAX_INSTR_LENGTH;

    // POP rax:
    *(uint8_t*)((uint8_t*)write_to + writeOffset) = POP_RAX_INSTR;

    // Erase trailing garbage bytes from overwritten instruction(s):
    memset((void*)((uint8_t*)write_to + writeOffset + POP_RAX_INSTR_LENGTH), NOP_INSTR_OPCODE, nops);

    return writeOffset;
}


} // namespace x64
} // namespace code
} // namespace mem
__SP_NAMESPACE_CLOSE // namespace sp
