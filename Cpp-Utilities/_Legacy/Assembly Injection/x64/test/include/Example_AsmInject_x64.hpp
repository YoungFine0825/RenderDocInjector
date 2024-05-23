// Made by Sean Pesce

#pragma once

#ifndef EXAMPLE_ASM_INJECT_X64_HPP
    #define EXAMPLE_ASM_INJECT_X64_HPP


#include <iostream> // cout, cin, getline
#include <string> // string
#include "AsmInject_x64.hpp" // Assembly Injection functions to be demonstrated with this program
#include "SP_SysUtils.hpp" // set_mem_protection()

int NUMBER = 12; // Will be printed as feedback to show when the injection occurs

#ifdef _WIN32
    // Define Windows-specific values here
    #define MEM_PROTECT_SIZE 70
#else
    // Define Unix-specific values here
    #define MEM_PROTECT_SIZE 2
#endif // _WIN32


#ifdef _MSC_VER // Using a Microsoft compiler
    // Depending on the compiler, many things can vary:
    #define MAIN_OFFSET 205     // The injection point
    #define NOP_COUNT 45         // The type of instructions used (more importantly here, the number of leftover bytes from those overwritten instructions)
    extern "C" void asm_code_example();
    extern "C" uint64_t JUMPBACK_ADDRESS; // The address used to jump out of the assembly code cave to resume normal program logic
#else
    // Non-Microsoft compiler
    #define MAIN_OFFSET 190
    #define NOP_COUNT 41
    void asm_code_example();
    uint64_t JUMPBACK_ADDRESS; // The address used to jump out of the assembly code cave to resume normal program logic
#endif // _MSC_VER


#endif // EXAMPLE_ASM_INJECT_X64_HPP
