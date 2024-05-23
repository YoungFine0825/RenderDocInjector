// Author: Sean Pesce

/*  
    CREDITS:
    
    The overarching scan procedure is inspired by the algorithm written by SteveAndrews
    on the Cheat Engine forums. His profile can be found here:
    http://forum.cheatengine.org/profile.php?mode=viewprofile&u=332503&sid=d456d53e8550ae5efe3c04f60d4a081e

        The specific thread where SteveAndrews' code was posted:
        http://forum.cheatengine.org/viewtopic.php?p=5582229&sid=c47e69a1330187cd8035016ab56c736d
    
    

    The find_aob algorithm is heavily modified from an algorithm written by the user fdsasdf
    on the UnKnoWnCheaTs forum. His profile can be found here:
    https://www.unknowncheats.me/forum/members/645501.html

        The specific post where fdsasdf's code can be found:
        https://www.unknowncheats.me/forum/1064093-post3.html
    


    SEE ALSO:

    A github repository comparing benchmarks of various FindPattern algorithms (including fdsasdf's):
    https://github.com/learn-more/findpattern-bench/
*/

#include "SP_AoB_Scan.hpp"


///////////// Global Flags & Variables /////////////

bool AOBS_SKIP_EXECUTABLE_MEM = _AOBS_SKIP_EXECUTABLE_MEM_DEFAULT_;	// If true, memory with "Executable" flag will NOT be searched
bool AOBS_SKIP_MAPPED_MEM = _AOBS_SKIP_MAPPED_MEM_DEFAULT_;			// If true, MEM_MAPPED memory will NOT be searched

size_t MAX_AOB_LENGTH = _MAX_AOB_LENGTH_DEFAULT_;				// Maximum allowed length for queried byte arrays
size_t MAX_AOBSCAN_RESULTS = _MAX_AOBSCAN_RESULTS_DEFAULT_;		// Configurable upper limit for number of results that can be returned from an AoB scan




//////////////////// Functions /////////////////////


// Scans all accessible regions of current process memory for an array of
//  bytes (AoB), beginning at the given starting address:
void *aob_scan(uint8_t *aob, size_t length, bool *mask, void *start, std::vector<uint8_t*> *results)
{
    MEMORY_BASIC_INFORMATION region;
    void *result = NULL;

    if(aob == NULL || length <= 0)
    {
        set_error(SP_ERROR_INVALID_PARAMETER);
        return NULL;
    }
    else if(length > MAX_AOB_LENGTH)
    {
        set_error(SP_ERROR_INSUFFICIENT_BUFFER); // AoB is too long
        return NULL;
    }

    if(start != NULL)
    {
        // Get the region of the starting address, if it was specified:
        if(virtual_query(start, &region, sizeof(region)) < sizeof(region))
        {
            // Invalid address; set error number
            set_error(SP_ERROR_INVALID_ADDRESS);
            return NULL;
        }
    }
    else
    {
        region.BaseAddress = NULL; // First call to next_mem_region(&region, &region) gets process base if region.BaseAddress == NULL
        region.RegionSize = 0;
        region.Protect = MEM_PROTECT_NONE;
    }

    do
    {
        if(is_aob_scannable(&region) && region.RegionSize >= length)
        {
            result = find_aob((uint8_t*)region.BaseAddress, region.RegionSize, aob, length, mask, results);

            if(result != NULL)
            {
                if(result == aob) // Found our own search AoB in the stack
                {
                    result = NULL; // Don't let the scan return the address of the search array
                }
                else if(results == NULL)
                {
					// Only return first result
                    return result;
                }
				else
				{
					result = NULL; // Set current result to NULL to continue searching

					if (results->size() >= MAX_AOBSCAN_RESULTS)
					{
						// Max results reached; stop searching
						return (void*)results->at(0);
					}
				}
            }
            
        }
    }while(next_mem_region(&region, &region) != NULL);


	if (results == NULL || results->size() == 0)
	{
		set_error(SP_NO_ERROR);
		return NULL;
	}
	else
	{
		return (void*)results->at(0);
	}
}


// Convenient override functions for aob_scan:
void *aob_scan(const char *str_aob, void *start, std::vector<uint8_t*> *results)
{
    size_t str_aob_len = std::char_traits<char>::length(str_aob); // Length of str_aob >= length of final byte array
    std::vector<uint8_t> aob(str_aob_len);
    std::vector<char> mask(str_aob_len);
    size_t length = string_to_aob(str_aob, aob.data(), (bool*)mask.data());
	
	void *result;
	uint32_t err = SP_NO_ERROR;
	if ((result = aob_scan(aob.data(), length, (bool*)mask.data(), start, results)) == NULL) err = get_error();
	// @TODO: Free memory?
	set_error(err);
	return result;
}

void *aob_scan(char *str_aob, void *start, std::vector<uint8_t*> *results)
{
	size_t str_aob_len = std::char_traits<char>::length(str_aob); // Length of str_aob >= length of final byte array
	std::vector<uint8_t> aob(str_aob_len);
	std::vector<char> mask(str_aob_len);
	size_t length = string_to_aob((const char *)str_aob, aob.data(), (bool*)mask.data());
	
	void *result;
	uint32_t err = SP_NO_ERROR;
	if ((result = aob_scan(aob.data(), length, (bool*)mask.data(), start, results)) == NULL) err = get_error();
	// @TODO: Free memory?
	set_error(err);
	return result;
}

void *aob_scan(std::string& str_aob, void *start, std::vector<uint8_t*> *results)
{
    size_t str_aob_len = std::char_traits<char>::length(str_aob.c_str()); // Length of str_aob >= length of final byte array
    std::vector<uint8_t> aob(str_aob_len);
    std::vector<char> mask(str_aob_len);
    size_t length = string_to_aob(str_aob.c_str(), aob.data(), (bool*)mask.data());

	void *result;
	uint32_t err = SP_NO_ERROR;
	if ((result = aob_scan(aob.data(), length, (bool*)mask.data(), start, results)) == NULL) err = get_error();
	// @TODO: Free memory?
	set_error(err);
	return result;
}



// Searches for an AoB within a given section of memory:
void *find_aob(uint8_t *base, size_t region_size, uint8_t *aob, size_t length, bool *mask, std::vector<uint8_t*> *results)
{
    uint8_t *end = base + region_size - length;

    for(; base <= end; ++base)
    {
        if(mask != NULL && compare_byte_arrays(base, aob, length, mask))
        {
            if(base != aob) // Make sure the matching AoB isn't just our search array in the stack
            {
				if (results == NULL)
				{
					return base; 
				}
				else
				{
					results->push_back((uint8_t*)base); // Add current result to the results list
					if (results->size() >= MAX_AOBSCAN_RESULTS)
					{
						// Max results reached; stop searching
						return (void*)results->at(0);
					}
				}
            }
        }
        else if(compare_byte_arrays(base, aob, length)) // No mask
        {
			if (base != aob) // Make sure the matching AoB isn't just our search array in the stack
			{
				if (results == NULL)
				{
					return base;
				}
				else
				{
					results->push_back((uint8_t*)base); // Add current result to the results list
					if (results->size() >= MAX_AOBSCAN_RESULTS)
					{
						// Max results reached; stop searching
						return (void*)results->at(0);
					}
				}
			}
        }
    }
    
	if (results == NULL || results->size() == 0)
	{
		return NULL;
	}
	else
	{
		return (void*)results->at(0);
	}
}


// Returns true if both byte arrays are identical (excluding wildcard bytes):
bool compare_byte_arrays(uint8_t *mem, uint8_t *aob, size_t length, bool *mask)
{
    uint8_t *end = aob + length;
    for(; aob < end; mem++, aob++, mask++)
    {
        if(*mask && *mem != *aob)
        {
            return false;
        }
    }
    return true;
}


// Returns true if both byte arrays are identical:
bool compare_byte_arrays(uint8_t *mem, uint8_t *aob, size_t length) // No mask
{
    uint8_t *end = aob + length;
    for(; aob < end; mem++, aob++)
    {
        if(*mem != *aob)
        {
            return false;
        }
    }
    return true;
}

// Override of compare_byte_arrays
// Compares the byte array value of a string that represents a byte array to a real byte array
bool compare_byte_arrays(std::string& str_aob, uint8_t *byte_aob, bool use_mask)
{
	size_t str_aob_len = str_aob.length(); // Length of str_aob >= length of final byte array
	std::vector<uint8_t> aob(str_aob_len);
	std::vector<char> mask(str_aob_len);
	size_t length = string_to_aob(str_aob, aob.data(), (bool*)mask.data());
	if(use_mask)
		return compare_byte_arrays(aob.data(), byte_aob, length, (bool*)mask.data());
	else
		return compare_byte_arrays(aob.data(), byte_aob, length);
}


// Checks if a region of memory should be scanned with the current scan settings:
bool is_aob_scannable(MEMORY_BASIC_INFORMATION *mem_info)
{
    uint32_t allow_mask = MEM_PROTECT_R | MEM_PROTECT_W | MEM_PROTECT_X | MEM_PROTECT_RX | MEM_PROTECT_WX | MEM_PROTECT_RW | MEM_PROTECT_RWX; // Protected regions we do want to search

	uint32_t protect_mask = MEM_PROTECT_NONE; // Protected regions we don't want to search
	if(AOBS_SKIP_EXECUTABLE_MEM) // Check if executable memory should be skipped
	{
		protect_mask |= (MEM_PROTECT_X | MEM_PROTECT_RX | MEM_PROTECT_WX | MEM_PROTECT_RWX);
	}

	uint32_t type_mask = 0;
	if(AOBS_SKIP_MAPPED_MEM) // Check if mapped memory should be searched
	{
		type_mask |= MEM_MAPPED;
	}

	#ifdef _WIN32
	allow_mask |= PAGE_WRITECOMBINE;
	return (
		(mem_info->Protect & allow_mask)
		//&& !(mem_info->Protect & protect_mask)
		//&& !(mem_info->Protect & type_mask)
		&& !(mem_info->Protect & PAGE_GUARD)
		//&& !(mem_info->Protect & PAGE_NOCACHE)
		//&& !(mem_info->State & MEM_FREE)
		);
	#else
	return (
		(mem_info->Protect & allow_mask)
		&& (mem_info->Protect != MEM_PROTECT_NONE)
		&& !(mem_info->Protect & protect_mask)
		&& !(mem_info->Protect & type_mask)
		);
	#endif // _WIN32
}


// Converts a C string-formatted, null-terminated string representing an AoB to a true byte
//  array and generates a corresponding mask for determining wildcards.
size_t string_to_aob(const char *str_aob, uint8_t *aob, bool *mask)
{
    size_t str_aob_len = std::char_traits<char>::length(str_aob); // Length of str_aob != length of final byte array
    size_t pos = 0; // Position in final byte array

    for(int c = 0; c < (int)str_aob_len; c++ /* lol */)
    {
        if(str_aob[c] == ' ') // Skip spaces
        {
            while(str_aob[c+1] == ' ') // Allow multiple spaces (very forgiving)
            {
                c++;
            }
            continue;
        }
        else if(str_aob[c] == '?' || str_aob[c] == 'x' || str_aob[c] == 'X') // Check for wildcard
        { // Allow "?", "??", "x", "xx", "X", or "XX" to specify wildcard
            // Allow wildcard specification to be 1 or 2 REPEATED wildcard characters:
            if(str_aob[c] == '?' && str_aob[c+1] == '?')  
            {
                c++;
            }
            else if(str_aob[c] == 'x' && str_aob[c+1] == 'x')
            {
                c++;
            }
            else if(str_aob[c] == 'X' && str_aob[c+1] == 'X')
            {
                c++;
            }
            mask[pos] = false; // Set pattern mask to wildcard
            aob[pos] = 0;
            pos++;
            continue;
        }
        else
        {
            const char byte_chars[3] = { str_aob[c], str_aob[c+1], '\0' };

            sscanf_s(byte_chars, "%2x", (unsigned int*)&aob[pos]); // Parse next 2 characters as a hex byte and store in aob

            mask[pos] = true; // Set pattern mask to non-wildcard
            pos++;
            c++; // 2 characters were read
        }
    }

    return pos;
}


// Overload of string_to_aob using a non-constant char pointer:
size_t string_to_aob(char *str_aob, uint8_t *aob, bool *mask)
{
    return string_to_aob((const char *)str_aob, aob, mask);
}


// Overload of string_to_aob using a C++ string *:
size_t string_to_aob(std::string& str_aob, uint8_t *aob, bool *mask)
{
    return string_to_aob(str_aob.c_str(), aob, mask);
}


// Sets global search flags to their default values:
void reset_aob_scan_settings()
{
	AOBS_SKIP_EXECUTABLE_MEM = _AOBS_SKIP_EXECUTABLE_MEM_DEFAULT_;
	AOBS_SKIP_MAPPED_MEM = _AOBS_SKIP_MAPPED_MEM_DEFAULT_;
	MAX_AOB_LENGTH = _MAX_AOB_LENGTH_DEFAULT_;
	MAX_AOBSCAN_RESULTS = _MAX_AOBSCAN_RESULTS_DEFAULT_;
}

