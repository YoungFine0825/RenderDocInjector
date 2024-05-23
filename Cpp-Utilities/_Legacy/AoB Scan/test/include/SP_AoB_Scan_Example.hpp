// Made by Sean P

#pragma once

#ifndef SP_AOB_Scan_EXAMPLE_HPP
    #define SP_AOB_SCAN_EXAMPLE_HPP

#include <iostream>     // cout, endl

#include "SP_AoB_Scan.hpp" // The functions being demonstrated in this program
#include "SP_IO.hpp"    // sp_print_intro()

// Prints a formatted byte array
void print_aob(uint8_t *aob, bool *mask, size_t len);
void print_aob(uint8_t *aob, size_t len);

// Prints a formatted AoB mask
void print_mask(bool *mask, size_t len);

// This function is never called; it's just used to test AoB Scanning
void dummy_function();

#endif // SP_AOB_SCAN_EXAMPLE_HPP