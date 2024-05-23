/*
	Author: Sean Pesce

	Multi-level pointer class
*/

#include "SpPointer.h"
#include "SP_SysUtils.hpp"

// Constructors/destructors
SpPointer::SpPointer(void *starting_address, std::vector<long> new_offsets)
{
	base = starting_address;
	offsets = new_offsets;
}

SpPointer::~SpPointer() {};


// Returns final resolved address (or NULL if memory is not accessible)
void *SpPointer::resolve()
{
	void *address = base;

	MEMORY_BASIC_INFORMATION mbi; // Used to check if memory is readable

	for (auto offset : offsets)
	{
		// Check if memory is readable
		if (virtual_query(address, &mbi, sizeof(MEMORY_BASIC_INFORMATION)) == 0)
			return NULL;

		#ifdef _WIN32
			if (mbi.State != MEM_COMMIT)
				return NULL;
		#endif // _WIN32

		if (mbi.Protect == MEM_PROTECT_NONE || mbi.Protect == MEM_PROTECT_X)
			return NULL;

		// Memory is readable, continue resolution of pointer
		address = (void*)((*(uint64_t*)address) + offset);
	}


	// Check if resolved address is readable
	if (virtual_query(address, &mbi, sizeof(MEMORY_BASIC_INFORMATION)) == 0)
		return NULL;

	#ifdef _WIN32
		if (mbi.State != MEM_COMMIT)
			return NULL;
	#endif // _WIN32

	if (mbi.Protect == MEM_PROTECT_NONE || mbi.Protect == MEM_PROTECT_X)
		return NULL;

	// Resolved address is readable
	return address;
}
