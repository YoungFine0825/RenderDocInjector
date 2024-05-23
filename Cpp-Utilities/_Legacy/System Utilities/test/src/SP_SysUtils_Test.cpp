// Made by Sean P

#include "SP_SysUtils_Test.hpp"

#ifdef _MSC_VER
int __cdecl main()
#else
int main()
#endif // _MSC_VER
{
	sp_print_intro("Demo Program for SP_SysUtil Functions"); // Print the program name and author information

	if(is_32bit())
	{
		std::cout << "Process mode: 32-bit" << std::endl;
	}
	else
	{
		std::cout << "Process mode: 64-bit" << std::endl;
	}
	#ifdef _WIN32
		std::cout << "WOW64 process? ";
		if(is_wow64_process())
		{
			std::cout << "Yes" << std::endl;
		}
		else
		{
			std::cout << "No" << std::endl;
		}
	#endif // _WIN32
	std::cout << "Process base: " << get_process_base() << std::endl;
	MEMORY_BASIC_INFORMATION mem_info;
	virtual_query(get_process_base(), &mem_info, sizeof(mem_info));
	std::cout << "2nd memory region starts at: " << next_mem_region(&mem_info, &mem_info) << std::endl;
	std::cout << "3rd memory region starts at: " << next_mem_region(mem_info.BaseAddress) << std::endl;
	std::cout << "Page size: " << get_page_size() << " bytes" << std::endl;
	std::cout << "Starting address of main() function: " << (void *)&main << std::endl;
	std::cout << "Base address of page containing main(): " << get_page_base((void *)&main) << std::endl;
	std::cout << "Memory protection settings:    R=" << MEM_PROTECT_R << "   X=" << MEM_PROTECT_X << "   RX=" << MEM_PROTECT_RX << "   RWX=" << MEM_PROTECT_RWX << std::endl;
	std::cout << "Setting protection to X at main..." << std::endl;
	set_mem_protection((void *)&main, 1,  MEM_PROTECT_X);
	std::cout << "Memory protection at main(): " << get_mem_protection((void *)&main) << std::endl;
	std::cout << "Setting protection to RWX at main()..." << std::endl;
	set_mem_protection((void *)&main, 1,  MEM_PROTECT_RWX);  // This might not print the "correct" value on Windows due to the way Windows handles RWX protection
	std::cout << "New memory protection at main(): " << get_mem_protection((void *)&main) << std::endl << std::endl;

	enter_to_continue("Press enter to continue...", "\n");

	return 0;
}