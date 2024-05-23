// Made by Sean Pesce

#include "AsmInject_x64.hpp"



/**
	Recommended method of injection on 64-bit systems due to simplicity and unlimited range.
	
	Notes:
       Space required: 14 bytes
       Registers preserved? Yes
	   
	   
	Absolute 64-bit jump technique:

	JMP QWORD PTR [%RIP+0x0]
	DQ imm64 	; imm64 is the address of injected code
	
 */
void inject_jmp_14b(void *inject_at, void *ret_to, int nops, void *asm_code)
{
	write_jmp_rm64_rip(inject_at, 0, nops + DQ_INSTR_LENGTH); // Write the JMP [%rip+0x0] instruction

	*(uint64_t*)(((uint8_t*)inject_at) + JMP_RM64_INSTR_LENGTH) = (uint64_t)asm_code; // Overwrite next instruction with address of injected function

	*(uint64_t*)ret_to = (uint64_t)(((uint8_t*)inject_at) + JMP_RM64_INSTR_LENGTH + DQ_INSTR_LENGTH);
}




/* Injects code using a JMP r/m64 instruction at the given address.
 *  Notes:
 *      Space required: 14 bytes
 *      Registers preserved? No
 *          User must start their code with POP %rax and end their code with PUSH %rax/MOVABS %rax, ret_to/JMP %rax
 *	WARNING: This function is deprecated. Use inject_jmp_14b() instead.
 */
void inject_jmp_14b_deprecated(void *inject_at, void *ret_to, int nops, void *asm_code)
{
    // Write the injected bytecode and store the final write offset (relative to the injection point):
    int pop_rax_offset = write_jmp_rax_14b(inject_at, asm_code, nops); // The returned offset is also the offset of the POP %rax instruction

    // Direct the user's return JMP to the POP %rax instruction:
    *(uint64_t*)ret_to = (uint64_t)((uint8_t*)inject_at + pop_rax_offset);
}



/* Injects code using a JMP rel32 instruction at the given address.
 *  Notes:
 *      Immediate space required: 5 bytes
 *      Local space required: 19 bytes (local code cave)
 *          PUSH %rax               // 1 byte
 *          MOVABS %rax, imm64      // 10 bytes; imm64 is the address of the injected code
 *          JMP %rax                // 2 bytes
 *          POP %rax                // 1 byte
 *          JMP rel32                // 5 bytes; rel32 is the offset to the address of the first original instruction after the injection point
 *      Registers preserved? No
 *              User must start their code with POP %rax and end their code with PUSH %rax (before the final JMP instruction)
 */
void inject_jmp_5b(void *inject_at, void *ret_to, int nops, void *asm_code,
                  void *local_trampoline, int tramp_nops)
{
    // Write the injected JMP rel32 instruction and local trampoline:
    write_bytecode_5b(inject_at, nops, local_trampoline, tramp_nops, asm_code);

    // Obtain the offset of the POP %rax instruction in the local trampoline function:
    int pop_rax_offset = PUSH_RAX_INSTR_LENGTH + MOVABS_INSTR_LENGTH + JMP_ABS_RAX_INSTR_LENGTH;

    // Direct the user's return JMP to the POP %rax instruction in the local trampoline function:
    *(uint64_t*)ret_to = (uint64_t)((uint8_t*)inject_at + pop_rax_offset);
}



/* Injects code using a JMP rel8 instruction at the given address.
 *  Notes:
 *      Immediate space required: 2 bytes
 *      Local space required: 16 bytes (local code cave)
 *          PUSH %rax               // 1 byte
 *          MOVABS %rax, imm64      // 10 bytes; imm64 is the address of the injected code
 *          JMP %rax                // 2 bytes
 *          POP %rax                // 1 byte
 *          JMP rel8                // 2 bytes; rel8 is the offset to the address of the first original instruction after the injection point
 *      Registers preserved? No
 *              User must start their code with POP %rax and end their code with PUSH %rax (before the final JMP instruction)
 */
void inject_jmp_2b(void *inject_at, void *ret_to, int nops, void *asm_code,
                  void *local_trampoline, int tramp_nops)
{
    // Write the injected JMP rel8 instruction and local trampoline:
    write_bytecode_2b(inject_at, nops, local_trampoline, tramp_nops, asm_code);

    // Obtain the offset of the POP %rax instruction in the local trampoline function:
    int pop_rax_offset = PUSH_RAX_INSTR_LENGTH + MOVABS_INSTR_LENGTH + JMP_ABS_RAX_INSTR_LENGTH;

    // Direct the user's return JMP to the POP %rax instruction in the local trampoline function:
    *(uint64_t*)ret_to = (uint64_t)((uint8_t*)inject_at + pop_rax_offset);
}



// Helper function that writes bytecode for 5-byte JMP injections and the
//  local trampoline functions they utilize.
void write_bytecode_5b(void *inject_at, int nops, void *local_trampoline, int tramp_nops, void *jmp_to)
{
    // Write the injected JMP rel32 instruction:
    write_jmp_rel32(inject_at, local_trampoline, nops);

    // Create the local trampoline function:
    int retJmpOffset = write_jmp_rax_14b(local_trampoline, (void*)jmp_to, tramp_nops+JMP_REL32_INSTR_LENGTH); // Extra NOPs because some NOPs will be overwritten with the "JMP rel32" returning JMP
    
    // Obtain the write offset of the local trampoline function's returning JMP rel32 instruction (relative to local_trampoline):
    retJmpOffset += POP_RAX_INSTR_LENGTH;

    // Write the local trampoline's returning JMP rel8 instruction:
    write_jmp_rel32((uint8_t*)local_trampoline+retJmpOffset, (uint8_t*)inject_at+JMP_REL32_INSTR_LENGTH, tramp_nops);
}



// Helper function that writes bytecode for 2-byte JMP injections and the
//  local trampoline functions they utilize.
void write_bytecode_2b(void *inject_at, int nops, void *local_trampoline, int tramp_nops, void *jmp_to)
{
    // Write the injected JMP rel8 instruction:
    write_jmp_rel8(inject_at, local_trampoline, nops);

    // Create the local trampoline function:
    int retJmpOffset = write_jmp_rax_14b(local_trampoline, (void*)jmp_to, tramp_nops+JMP_REL8_INSTR_LENGTH); // Extra NOPs because some NOPs will be overwritten with the "JMP rel8" returning JMP
    
    // Obtain the write offset of the local trampoline function's returning JMP rel8 instruction (relative to local_trampoline):
    retJmpOffset += POP_RAX_INSTR_LENGTH;

    // Write the local trampoline's returning JMP rel8 instruction:
    write_jmp_rel8((uint8_t*)local_trampoline+retJmpOffset, (uint8_t*)inject_at+JMP_REL8_INSTR_LENGTH, tramp_nops);
}



// Writes bytecode for the series of instructions to perform an absolute JMP r64 (using JMP %rax)
//  and restore the register upon returning. Also overwrites remaining garbage bytecode with
//  the specified number of NOP instructions.
int write_jmp_rax_14b(void *write_to, void *jmp_to, int nops)
{
    int writeOffset = 0; // Keep track of the offset to write to (relative to the injection point)

    // Begin writing at the injection point...
    //
    // PUSH %rax:
    *(uint8_t*)write_to = PUSH_RAX_INSTR;
    writeOffset += PUSH_RAX_INSTR_LENGTH;
    //
    //
    // MOVABS %rax, imm64:
    *(uint16_t*)((uint8_t*)write_to + writeOffset) = *(uint16_t*)MOVABS_RAX_INSTR_OPCODE; // Opcode of MOVABS %rax, imm64
    writeOffset += MOVABS_OPCODE_LENGTH;
    *(uint64_t*)((uint8_t*)write_to + writeOffset) = (uint64_t)((uint8_t*)jmp_to + SP_ASM_FUNC_START_OFFSET); // Operand of MOVABS %rax, imm64
    writeOffset += MOVABS_OPERAND_LENGTH;
    //
    //
    // JMP %rax:
    *(uint16_t*)((uint8_t*)write_to + writeOffset) = *(uint16_t*)JMP_ABS_RAX_INSTR;
    writeOffset += JMP_ABS_RAX_INSTR_LENGTH;
    //
    //
    // POP %rax:
    *(uint8_t*)((uint8_t*)write_to + writeOffset) = POP_RAX_INSTR;

    // Erase trailing garbage bytes from overwritten instruction(s):
    memset((void*)((uint8_t*)write_to + writeOffset + POP_RAX_INSTR_LENGTH), NOP_INSTR_OPCODE, nops);

    return writeOffset;
}



// Writes a JMP rm64 (specifically, JMP [%rip]) instruction from write_to to jmp_to, and inserts trailing NOPs if necessary
void write_jmp_rm64_rip(void *write_to, void* jmp_to, int nops)
{
	*(uint16_t*)write_to = *(uint16_t*)JMP_RM64_RIP_OPCODE; // Write the JMP r/m64 instruction opcode

	*(uint32_t*)((uint8_t*)write_to + JMP_RM64_OPCODE_LENGTH) = (uint32_t)calculate_jmp_offset(write_to, jmp_to, JMP_RM64_INSTR_LENGTH); // Write the JMP r/m64 instruction operand

	memset(((uint8_t*)write_to) + JMP_RM64_INSTR_LENGTH, NOP_INSTR_OPCODE, nops); // Write the trailing NOP instructions
}



// Writes a JMP rm64 (specifically, JMP [%rip]) instruction from write_to using the given offset, and inserts trailing NOPs if necessary
void write_jmp_rm64_rip(void *write_to, int32_t offset, int nops)
{
	*(uint16_t*)write_to = *(uint16_t*)JMP_RM64_RIP_OPCODE; // Write the JMP r/m64 instruction opcode

	*(uint32_t*)((uint8_t*)write_to + JMP_RM64_OPCODE_LENGTH) = offset; // Write the JMP r/m64 instruction operand

	memset(((uint8_t*)write_to) + JMP_RM64_INSTR_LENGTH, NOP_INSTR_OPCODE, nops); // Write the trailing NOP instructions
}



// Writes a JMP rel8 instruction from write_to to jmp_to, and inserts trailing NOPs if necessary
void write_jmp_rel8(void *write_to, void *jmp_to, int nops)
{
    *(uint8_t*)write_to = JMP_REL8_INSTR_OPCODE; // Write opcode byte
    
    *((int8_t*)write_to+1) = (int8_t)calculate_jmp_offset(write_to, jmp_to, JMP_REL8_INSTR_LENGTH); // Write operand byte

    // Erase trailing garbage bytes from overwritten instruction(s) at write address:
    memset((void*)((uint8_t*)write_to + JMP_REL8_INSTR_LENGTH), NOP_INSTR_OPCODE, nops);
}


// Writes a JMP rel8 instruction from write_to using the given offset, and inserts trailing NOPs if necessary
void write_jmp_rel8(void *write_to, int8_t offset, int nops)
{
    *(uint8_t*)write_to = JMP_REL8_INSTR_OPCODE; // Write opcode byte
    
    *((int8_t*)write_to+1) = offset; // Write operand byte

    // Erase trailing garbage bytes from overwritten instruction(s) at write address:
    memset((void*)((uint8_t*)write_to + JMP_REL8_INSTR_LENGTH), NOP_INSTR_OPCODE, nops);
}



// Writes a JMP rel32 instruction from write_to to jmp_to, and inserts trailing NOPs if necessary
void write_jmp_rel32(void *write_to, void *jmp_to, int nops)
{
    *(uint8_t*)write_to = JMP_REL32_INSTR_OPCODE; // Write opcode byte
    
    *(uint32_t*)((uint8_t*)write_to+1) = (int32_t)calculate_jmp_offset(write_to, jmp_to, JMP_REL32_INSTR_LENGTH); // Write operand bytes

    // Erase trailing garbage bytes from overwritten instruction(s) at write address:
    memset((void*)((uint8_t*)write_to + JMP_REL32_INSTR_LENGTH), NOP_INSTR_OPCODE, nops);
}



// Writes a JMP rel32 instruction from write_to using the given offset, and inserts trailing NOPs if necessary
void write_jmp_rel32(void *write_to, int32_t offset, int nops)
{
    *(uint8_t*)write_to = JMP_REL32_INSTR_OPCODE; // Write opcode byte
    
    *(int32_t*)((uint8_t*)write_to+1) = offset; // Write operand bytes

    // Erase trailing garbage bytes from overwritten instruction(s) at write address:
    memset((void*)((uint8_t*)write_to + JMP_REL32_INSTR_LENGTH), NOP_INSTR_OPCODE, nops);
}



// Writes a CALL rel32 instruction from write_to to procedure, and inserts trailing NOPs if necessary
void write_call_rel32(void *write_to, void *procedure, int nops)
{
    *(uint8_t*)write_to = CALL_REL32_INSTR_OPCODE; // Write opcode byte
    
    *(uint32_t*)((uint8_t*)write_to+1) = (int32_t)calculate_jmp_offset(write_to, procedure, CALL_REL32_INSTR_LENGTH); // Write operand bytes

    // Erase trailing garbage bytes from overwritten instruction(s) at write address:
    memset((void*)((uint8_t*)write_to + CALL_REL32_INSTR_LENGTH), NOP_INSTR_OPCODE, nops);
}



// Writes a CALL rel32 instruction from write_to using the given offset, and inserts trailing NOPs if necessary
void write_call_rel32(void *write_to, int32_t offset, int nops)
{
    *(uint8_t*)write_to = CALL_REL32_INSTR_OPCODE; // Write opcode byte
    
    *(int32_t*)((uint8_t*)write_to+1) = offset; // Write operand bytes

    // Erase trailing garbage bytes from overwritten instruction(s) at write address:
    memset((void*)((uint8_t*)write_to + CALL_REL32_INSTR_LENGTH), NOP_INSTR_OPCODE, nops);
}



// Writes a "near return" (RET) instruction to write_to and inserts trailing NOPs if necessary
void write_ret(void *write_to, int nops)
{
	*(uint8_t*)write_to = RET_INSTR_OPCODE; // Write instruction
	
	// Erase trailing garbage bytes from overwritten instruction at write address:
    memset((void*)((uint8_t*)write_to + RET_INSTR_LENGTH), NOP_INSTR_OPCODE, nops);
}



// Writes a "far return" (RET) instruction to write_to and inserts trailing NOPs if necessary
void write_ret_far(void *write_to, int nops)
{
	*(uint8_t*)write_to = RET_FAR_INSTR_OPCODE; // Write instruction
	
	// Erase trailing garbage bytes from overwritten instruction at write address:
    memset((void*)((uint8_t*)write_to + RET_INSTR_LENGTH), NOP_INSTR_OPCODE, nops);
}



// Writes a "near return, pop imm16 bytes from stack" (RET imm16) instruction to write_to and
//  inserts trailing NOPs if necessary
void write_ret_imm16(void *write_to, uint16_t pop_bytes, int nops)
{
	*(uint8_t*)write_to = RET_IMM16_INSTR_OPCODE; // Write opcode byte

	*(uint16_t*)((uint8_t*)write_to+1) = pop_bytes; // Write operand bytes
	
	// Erase trailing garbage bytes from overwritten instruction at write address:
    memset((void*)((uint8_t*)write_to + RET_IMM16_INSTR_LENGTH), NOP_INSTR_OPCODE, nops);
}



// Writes a "far return, pop imm16 bytes from stack" (RET imm16) instruction to write_to and
//  inserts trailing NOPs if necessary
void write_ret_far_imm16(void *write_to, uint16_t pop_bytes, int nops)
{
	*(uint8_t*)write_to = RET_FAR_IMM16_INSTR_OPCODE; // Write opcode byte

	*(uint16_t*)((uint8_t*)write_to+1) = pop_bytes; // Write operand bytes
	
	// Erase trailing garbage bytes from overwritten instruction at write address:
    memset((void*)((uint8_t*)write_to + RET_IMM16_INSTR_LENGTH), NOP_INSTR_OPCODE, nops);
}



// Calculates the offset between a JMP rel instruction and some address:
int64_t calculate_jmp_offset(void *from, void *to, int instr_length)
{
    return ((uint64_t)to - (uint64_t)from - instr_length);
}

