// Made by Sean P


#include "SP_IO.hpp"


#ifdef max
	#undef max	// Fixes issues with <std::streamsize>::max() on Windows
#endif // max


// Prints the program name and author information:
void sp_print_intro(const char* program_name)
{
    std::cout << program_name << std::endl; // Print the name of the program
    std::cout << "Creator:  Sean Pesce" << std::endl << std::endl;
}


// Prints a prompt message and waits for the user to press Enter (Return) key:
void enter_to_continue(const char* prompt)
{
    std::cin.clear(); // Clear the input buffer
    std::cout << prompt; // Print the prompt message
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Wait for user input
}

// Prints a prompt message, waits for the user to press Enter (Return) key,
//  and prints a final message before continuing:
void enter_to_continue(const char* prompt, const char* continue_msg)
{
    std::cin.clear(); // Clear the input buffer
    std::cout << prompt; // Print the prompt message
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Wait for user input
    std::cout << continue_msg; // Print the continuing message
}

