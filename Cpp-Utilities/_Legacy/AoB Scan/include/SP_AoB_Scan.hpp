// Author: Sean Pesce

#pragma once

#ifndef SP_AOB_Scan_HPP
    #define SP_AOB_SCAN_HPP

#include <cstdint>  // uint8_t
#include <string>   // string, char_traits<char>::length
#include <stdio.h>  // sscanf()
#include <string>   // string
#include <sstream>  // stringstream
#include <vector>   // vector

#include "SP_SysUtils.hpp" // get_page_size(), get_process_base(), virtual_query(), memory protection constants


/////////////////// Constants ///////////////////

#define _AOBS_SKIP_EXECUTABLE_MEM_DEFAULT_ false
#define _AOBS_SKIP_MAPPED_MEM_DEFAULT_ true
#define _MAX_AOB_LENGTH_DEFAULT_ 512
#define _MAX_AOBSCAN_RESULTS_DEFAULT_ 50



///////////// Global Flags & Variables /////////////

extern bool AOBS_SKIP_EXECUTABLE_MEM;    // If true, memory with "Executable" flag will NOT be searched
extern bool AOBS_SKIP_MAPPED_MEM;        // If true, MEM_MAPPED memory will NOT be searched

extern size_t MAX_AOB_LENGTH;			// Maximum allowed length for queried byte arrays
extern size_t MAX_AOBSCAN_RESULTS;		// Configurable upper limit for number of results that can be returned from an AoB scan



///////////// Function Prototypes /////////////

/*
    aob_scan(uint8_t*, bool*, size_t, void*)

    Scans the memory of a process for a given byte array.

    @param aob      The byte array being searched for.
    @param length   The length of the byte array being searched for.
                    NOTE: The byte array and mask array MUST be the same size (unless mask is NULL).
	@param mask     [Optional parameter] The mask array, used to determine whether each byte of aob is a wildcard
                    byte (aob[n] does not have to be matched when mask[n] == false).
    @param start    [Optional parameter] The address where the scan will begin searching. Only addresses larger
                    than the "start" parameter will be searched.
	@param results	[Optional parameter] If non-NULL, stores up to MAX_AOBSCAN_RESULTS addresses of
					matching byte arrays.

    @return     The address of the first matching byte array in memory, if one exists; if not, returns NULL.
*/
void *aob_scan(uint8_t *aob, size_t length, bool *mask = NULL, void *start = NULL, std::vector<uint8_t*> *results = NULL);

// Overloaded versions of aob_scan; see @aob_scan(uint8_t*, size_t, bool*, void*, std::vector<uint8_t*>*) for more info:
void *aob_scan(const char *str_aob, void *start = NULL, std::vector<uint8_t*> *results = NULL);	// Calls string_to_aob before calling @aob_scan(uint8_t*, size_t, bool*, void*, std::vector<uint8_t*>*)
void *aob_scan(char *str_aob, void *start = NULL, std::vector<uint8_t*> *results = NULL);		// Calls string_to_aob before calling @aob_scan(uint8_t*, size_t, bool*, void*, std::vector<uint8_t*>*)
void *aob_scan(std::string& str_aob, void *start = NULL, std::vector<uint8_t*> *results = NULL);// Calls string_to_aob before calling @aob_scan(uint8_t*, size_t, bool*, void*, std::vector<uint8_t*>*)



/*
    find_aob(uint8_t*, size_t, uint8_t*, bool*, size_t*)

    Searches a memory region for a given byte array.

    @param base         The base address of the memory region being searched.
    @param region_size  The size of the memory region being searched.
                        NOTE: region_size must be equal to or larger than the "length" parameter.
    @param aob          The byte array being searched for.
    @param mask         [Optional parameter] The mask array to determine whether each byte of aob is a wildcard
                        byte (aob[n] does not have to be matched when mask[n] == false).
    @param length       The length of the byte array being searched for.
                        NOTE: The byte array and mask array MUST be the same size (unless
                        mask is NULL).
	@param results		[Optional parameter] If non-NULL, stores up to MAX_AOBSCAN_RESULTS addresses of
						matching byte arrays.

    @return     The address of the first matching byte array in memory, if one exists; if not, returns NULL.
*/
void *find_aob(uint8_t *base, size_t region_size, uint8_t *aob, size_t length, bool *mask = NULL, std::vector<uint8_t*> *results = NULL);



/*
    compare_byte_arrays(uint8_t*, uint8_t*, bool*, size_t)

    Takes two equal-length byte arrays and determines whether they are identical, excluding
    masked (wildcard) bytes. To compare byte arrays without a mask, use @compare_byte_arrays(uint8_t*, uint8_t*, size_t).

    @param mem      The first byte array (when using aob_scan(), this is a chunk of a memory region).
    @param aob      The second byte array (when using aob_scan(), this is the AoB being searched for).
    @param length   The length of the given byte arrays (both arrays should be the same size).
	@param mask     [Optional parameter] The mask array used to determine which bytes are wildcards (aob[n] does
                    not have to match mem[n] when mask[n] == false).

    @return     true if the byte arrays match; false otherwise.
*/
bool compare_byte_arrays(uint8_t *mem, uint8_t *aob, size_t length, bool *mask);

/*
    compare_byte_arrays(uint8_t*, uint8_t*, size_t)

    Takes two equal-length byte arrays and determines whether they are identical.

    @param      See @compare_byte_arrays(uint8_t*, uint8_t*, size_t, bool*)

    @return     true if the byte arrays match; false otherwise.
*/
bool compare_byte_arrays(uint8_t *mem, uint8_t *aob, size_t length);

/*
    compare_byte_arrays(std::string&, uint8_t*, bool)

    Takes a byte array and a string representing a byte array (of equal length) and determines whether the
	byte array represented by the string is identical to the true byte array.

    @param str_aob  A string representing the first byte array.
	@param byte_aob The second byte array.
	@param use_mask If true, the byte arrays are compared using the wildcard mask generated when the string
	                was converted to a byte array.

    @return     true if the byte arrays match; false otherwise.
*/
bool compare_byte_arrays(std::string& str_aob, uint8_t *byte_aob, bool use_mask = true);



/*
    is_aob_scannable(MEMORY_BASIC_INFORMATION*)

    Checks whether a region of memory should be scanned, based on the current global scan
    settings and the memory access permissions of the given region.

    @param mem_info     A struct containing the necessary information to determine
                        whether the memory region will be searched.
*/
bool is_aob_scannable(MEMORY_BASIC_INFORMATION *mem_info);



/*
    string_to_aob(const char*, uint8_t*, bool*)

    Converts a constant (null-terminated) C string (representing an AoB) to a true byte array
    and generates a corresponding mask for determining wildcard bytes. A wildcard is a byte
    that does not have to be directly matched in order for the full AoB to return a match.
    
                -------------- STRING FORMATTING INFO --------------
    AoB strings should be formatted as follows; given a (literal) byte array:

        uint8_t example_bytes = { 0x0a, 0x1b, 0x2c, 0x3d, 0x4e, 0x5f };
    
    The string representation of the array could be any of the following:

        "0a 1b 2c 3d 4e 5f"     // Recommended formatting (for max readability)
        "0a 1B 2c 3D 4e 5F"     // Case is irrelevant
        "0a1b2c3d4e5f"          // Spaces are optional
        " 0A1b2C 3d   4E 5f  "  // Can have leading/trailing spaces and any number of spaces between bytes

    To specify a masked (wildcard) byte, use "?", "??", "x", "xx", "X", or "XX". If the 2nd
    and 3rd bytes of the array don't need to be matched, the string representation could be
    any of the following:

        "0a ?? ?? 3d 4e 5f"     // Recommended formatting (for max readability)
        "0a xx xx 3d 4e 5f"     // Alternate recommended formatting
        "0a XX XX 3d 4e 5f"     // Other alternate recommended formatting
        "0a ?? xx 3d 4e 5f"     // 'x', 'X', and '?' can be mixed
        "0a ?  ?  3d 4e 5f"     // Double wildcard characters is unnecessary ONLY IF USING SPACES OR NON-REPEATING WILDCARD CHARACTERS.
        "0a ?x    3d 4e 5f"     // N consecutive alternating wildcard characters can be used to specify N wildcard bytes
        "0a ????  3d 4e 5f"     // N*2 consecutive indentical wildcard characters can be used to specify N wildcard bytes
        "0a ???   3d 4e 5f"     // 2N-1 consecutive indentical wildcard characters can be used to specify N wildcard bytes

            ----------- END OF STRING FORMATTING INFO ------------

    @param str_aob  A C-styled null-terminated character array string representing an array
                    of byte values and/or wildcards (any value will be accepted for wildcard
                    bytes).
    @param aob      The buffer that will hold the true byte array.
    @param mask     The buffer that will hold the mask array that will be used during searches
                    to determine whether each byte is a wildcard.

    @return         The length of the resulting byte and mask arrays.
*/
size_t string_to_aob(const char *str_aob, uint8_t *aob, bool *mask);


/*
    string_to_aob(char*, uint8_t*, bool*)

    Converts a (null-terminated) C string (representing an AoB) to a true byte array
    and generates a corresponding mask for determining wildcards.

    See @string_to_aob(const char*, uint8_t*, bool*) for string format, parameter,
    and return value information.
*/
size_t string_to_aob(char *str_aob, uint8_t *aob, bool *mask);


/*
    string_to_aob(std::string&, uint8_t*, bool*)

    Converts a C++ string (representing an AoB) to a true byte array
    and generates a corresponding mask for determining wildcards.

    See @string_to_aob(const char*, uint8_t*, bool*) for string format, parameter,
    and return value information.
*/
size_t string_to_aob(std::string& str_aob, uint8_t *aob, bool *mask);



/*
    resetAobSearchSettings()

    Sets global search flags to their default values.
*/
void reset_aob_scan_settings();


#endif // SP_AOB_SCAN_HPP
