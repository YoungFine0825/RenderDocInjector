// Made by Sean Pesce

#include "Example_AsmInject_x86.hpp"

int main()
{
    std::cout << "Press enter to begin.";
    std::string garbage;
    // Wait for user input, allowing time to use a disassembler to determine injection information:
    getline(std::cin, garbage);
    // Doesn't matter what the user inputs, we aren't using it.
    garbage[0] = '\0'; // This statement helps avoid compiler errors

    for(int i = 0; i < 10; i++)
    {

        // Decrease NUMBER by 2:
        NUMBER -= 2;

        std::cout << "Value = " << NUMBER << std::endl; // Print NUMBER

        // Increase NUMBER by 3:
        NUMBER += 3;

        // This conditional will never execute; it's here to stop compiler optimizations that might break the program:
        if(NUMBER == 1){ std::cout << "Value = " << NUMBER << std::endl; }

        // Decrement NUMBER:
        NUMBER--; // Memory location: &main + MAIN_OFFSET
        // Note that NUMBER doesn't change between loop iterations, unless injection is used to remove or alter the above statement(s)

        // Halfway through the program, inject ASM to remove the decrement statement:
        if(i == 4)
        {
            // Unprotect the memory:
            uint32_t old_protect; // old_protect is not utilized in Unix (might add this functionality later) @todo
            set_mem_protection((uint8_t*)&main + MAIN_OFFSET, MEM_PROTECT_SIZE, MEM_PROTECT_RWX, &old_protect);
            
            // Write the jump to the assembly function:
            inject_jmp_5b(((uint8_t*)&main + MAIN_OFFSET), &JUMPBACK_ADDRESS, NOP_COUNT, (void*)&asm_code_example);
            
            std::cout << "Injecting assembly at " << (int*)((uint8_t*)&main + MAIN_OFFSET) <<
                        " with return address " << (int*)JUMPBACK_ADDRESS << "..." << std::endl;
        }
        // After injection, NUMBER will change in future iterations of the loop.
    }
    return 0;
}

#ifdef _MSC_VER
// Microsoft compiler; use Visual Studio in-line ASM:
void __declspec(naked) __stdcall asm_code_example()
{
	__asm
	{
		JMP JUMPBACK_ADDRESS
	}
}
#else
// Non-Microsoft compiler; use GCC in-line ASM:
void asm_code_example()
{
    // The first ASM instruction is +3 from &asm_code_example when using GCC/G++
    __asm__ volatile
    (
        "JMP [JUMPBACK_ADDRESS]" // To reference an outside variable, might need to prefix it with "_"
    );
}
#endif // _MSC_VER


