// Made by Sean Pesce

/* Credits:
 *  Some portions of this code were inspired by or altered from code
 *  written by atom0s from the CheatEngine forums. User page for atom0s
 *  can be found here: 
 *  http://www.cheatengine.org/forum/profile.php?mode=viewprofile&u=8422&sid=9e8a3b96f0c5924b7a2fc85204bb8704
 */

#include "AsmInject_x86.hpp"
#include <set>
#ifdef _MSC_VER
#include <Windows.h>
#endif

// For every injection, save the location we're injecting to and check if we've already injected there before
static std::set<uint32_t> injected_locations;

// Checks if the specified address already has a code injection present
static void check_valid_injection(uint32_t address)
{
    // @TODO: can still be a bug where we inject not at the exact same place but within 5 bytes, and partially overwrite the previous jump
    if (injected_locations.find(address) == injected_locations.end())
    {
        injected_locations.insert(address);
    }
    else
    {
        fprintf(stderr, "Attempted to inject at same location twice. @%d\n", address);
        #ifdef _MSC_VER
            char* error_str = (char*)malloc(100);
            sprintf_s(error_str, 100, "Attempted to inject at same location twice. @%d", address);
            MessageBox(NULL, error_str, NULL, MB_OK);
        #endif
        abort();
    }
}

// Removes a previous injection entry
void forget_injection(uint32_t address)
{
    injected_locations.erase(address);
}

void inject_jmp_5b(uint8_t *inject_at, uint32_t *return_jmp_addr, int nops, void *asm_code)
{
    // The remaining 4 bytes of the instruction are the operand, specifying the offset from this address to the code cave:
    #ifdef _MSC_VER
        // Using a Microsoft compiler; jump straight to the code cave:
		write_jmp_rel32(inject_at, asm_code, nops);
    #else
        // Using non-MS compiler; GCC ASM starts +3 bytes from &asm_code:
        write_jmp_rel32(inject_at, (uint8_t*)asm_code + SP_ASM_FUNC_START_OFFSET, nops);
    #endif // _MSC_VER

    // Calculate the address of the next instruction after the injected JMP and write it to the in-line ASM function's return JMP:
    *return_jmp_addr = ((uint32_t)inject_at + JMP_REL32_INSTR_LENGTH);
}



// Writes a JMP rel8 instruction from write_to to jmp_to, and inserts trailing NOPs (if necessary):
void write_jmp_rel8(void *write_to, void *jmp_to, int nops)
{
    check_valid_injection((uint32_t)write_to);

    *(uint8_t*)write_to = JMP_REL8_INSTR_OPCODE; // Write opcode byte
    
    *((uint8_t*)write_to+1) = (int8_t)calculate_jmp_offset(write_to, jmp_to, JMP_REL8_INSTR_LENGTH); // Write operand byte

    // Erase trailing garbage bytes from overwritten instruction(s) at write address:
    memset((void*)((uint8_t*)write_to + JMP_REL8_INSTR_LENGTH), NOP_INSTR_OPCODE, nops);
}



// Writes a JMP rel8 instruction from write_to using the given offset, and inserts trailing NOPs if necessary
void write_jmp_rel8(void *write_to, int8_t offset, int nops)
{
    check_valid_injection((uint32_t)write_to);

    *(uint8_t*)write_to = JMP_REL8_INSTR_OPCODE; // Write opcode byte
    
    *((int8_t*)write_to+1) = offset; // Write operand byte

    // Erase trailing garbage bytes from overwritten instruction(s) at write address:
    memset((void*)((uint8_t*)write_to + JMP_REL8_INSTR_LENGTH), NOP_INSTR_OPCODE, nops);
}



// Writes a JMP rel32 instruction from write_to to jmp_to, and inserts trailing NOPs (if necessary):
void write_jmp_rel32(void *write_to, void *jmp_to, int nops)
{
    check_valid_injection((uint32_t)write_to);

    *(uint8_t*)write_to = JMP_REL32_INSTR_OPCODE; // Write opcode byte
    
    *(uint32_t*)((uint8_t*)write_to+1) = (int32_t)calculate_jmp_offset(write_to, jmp_to, JMP_REL32_INSTR_LENGTH); // Write operand bytes

    // Erase trailing garbage bytes from overwritten instruction(s) at write address:
    memset((void*)((uint8_t*)write_to + JMP_REL32_INSTR_LENGTH), NOP_INSTR_OPCODE, nops);
}



// Writes a JMP rel32 instruction from write_to using the given offset, and inserts trailing NOPs if necessary
void write_jmp_rel32(void *write_to, int32_t offset, int nops)
{
    check_valid_injection((uint32_t)write_to);

    *(uint8_t*)write_to = JMP_REL32_INSTR_OPCODE; // Write opcode byte
    
    *(int32_t*)((uint8_t*)write_to+1) = offset; // Write operand bytes

    // Erase trailing garbage bytes from overwritten instruction(s) at write address:
    memset((void*)((uint8_t*)write_to + JMP_REL32_INSTR_LENGTH), NOP_INSTR_OPCODE, nops);
}



// Writes a CALL rel32 instruction from write_to to procedure, and inserts trailing NOPs if necessary
void write_call_rel32(void *write_to, void *procedure, int nops)
{
    check_valid_injection((uint32_t)write_to);

    *(uint8_t*)write_to = CALL_REL32_INSTR_OPCODE; // Write opcode byte
    
    *(uint32_t*)((uint8_t*)write_to+1) = (int32_t)calculate_jmp_offset(write_to, procedure, CALL_REL32_INSTR_LENGTH); // Write operand bytes

    // Erase trailing garbage bytes from overwritten instruction(s) at write address:
    memset((void*)((uint8_t*)write_to + CALL_REL32_INSTR_LENGTH), NOP_INSTR_OPCODE, nops);
}



// Writes a CALL rel32 instruction from write_to using the given offset, and inserts trailing NOPs if necessary
void write_call_rel32(void *write_to, int32_t offset, int nops)
{
    check_valid_injection((uint32_t)write_to);

    *(uint8_t*)write_to = CALL_REL32_INSTR_OPCODE; // Write opcode byte
    
    *(int32_t*)((uint8_t*)write_to+1) = offset; // Write operand bytes

    // Erase trailing garbage bytes from overwritten instruction(s) at write address:
    memset((void*)((uint8_t*)write_to + CALL_REL32_INSTR_LENGTH), NOP_INSTR_OPCODE, nops);
}



// Writes a "near return" (RET) instruction to write_to and inserts trailing NOPs if necessary
void write_ret(void *write_to, int nops)
{
    check_valid_injection((uint32_t)write_to);

	*(uint8_t*)write_to = RET_INSTR_OPCODE; // Write instruction
	
	// Erase trailing garbage bytes from overwritten instruction at write address:
    memset((void*)((uint8_t*)write_to + RET_INSTR_LENGTH), NOP_INSTR_OPCODE, nops);
}



// Writes a "far return" (RET) instruction to write_to and inserts trailing NOPs if necessary
void write_ret_far(void *write_to, int nops)
{
    check_valid_injection((uint32_t)write_to);

	*(uint8_t*)write_to = RET_FAR_INSTR_OPCODE; // Write instruction
	
	// Erase trailing garbage bytes from overwritten instruction at write address:
    memset((void*)((uint8_t*)write_to + RET_INSTR_LENGTH), NOP_INSTR_OPCODE, nops);
}



// Writes a "near return, pop imm16 bytes from stack" (RET imm16) instruction to write_to and
//  inserts trailing NOPs if necessary
void write_ret_imm16(void *write_to, uint16_t pop_bytes, int nops)
{
    check_valid_injection((uint32_t)write_to);

	*(uint8_t*)write_to = RET_IMM16_INSTR_OPCODE; // Write opcode byte

	*(uint16_t*)((uint8_t*)write_to+1) = pop_bytes; // Write operand bytes
	
	// Erase trailing garbage bytes from overwritten instruction at write address:
    memset((void*)((uint8_t*)write_to + RET_IMM16_INSTR_LENGTH), NOP_INSTR_OPCODE, nops);
}



// Writes a "far return, pop imm16 bytes from stack" (RET imm16) instruction to write_to and
//  inserts trailing NOPs if necessary
void write_ret_far_imm16(void *write_to, uint16_t pop_bytes, int nops)
{
    check_valid_injection((uint32_t)write_to);

	*(uint8_t*)write_to = RET_FAR_IMM16_INSTR_OPCODE; // Write opcode byte

	*(uint16_t*)((uint8_t*)write_to+1) = pop_bytes; // Write operand bytes
	
	// Erase trailing garbage bytes from overwritten instruction at write address:
    memset((void*)((uint8_t*)write_to + RET_IMM16_INSTR_LENGTH), NOP_INSTR_OPCODE, nops);
}



// Calculates the offset between a JMP instruction and target address:
uint32_t calculate_jmp_offset(void *from, void *to, int instr_length)
{
    return ((uint32_t)to - (uint32_t)from - instr_length);
}



