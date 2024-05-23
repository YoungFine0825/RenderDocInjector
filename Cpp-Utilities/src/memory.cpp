/*
    CONTRIBUTORS:
        Sean Pesce

*/

#include "sp/memory.h"


__SP_NAMESPACE
namespace mem {


#ifndef _WIN32
// On Windows, this a wrapper function for VirtualQuery with no functional changes (see header).
// On Linux, this function attempts to emulate the usage of the VirtualQuery function from the Windows API.
size_t virtual_query(void *address, MEMORY_BASIC_INFORMATION *buff, size_t length)
{
    // Resolve /proc/self/maps path (/proc/$PID/maps):
    char self_maps_file_path[32];
    sprintf(self_maps_file_path, MEM_MAP_FILE_DIR_FORMAT, (int)getpid());

    std::ifstream in_fs(self_maps_file_path); // Open file input stream

    MEMORY_BASIC_INFORMATION region; // Memory info struct
    std::string line; // Holds each line of data from /proc/self/maps file as they are parsed
    size_t total = length / sizeof(MEMORY_BASIC_INFORMATION); // Get the number of regions to parse
    size_t count = 0; // Number of regions that have been parsed thus far
    void *prev_region_end = 0; // Last address inide the previous region (used to determine if an address lies between process regions)
                               // Parse each memory region to find the requested starting region:
    while (std::getline(in_fs, line) && count != total)
    {
        parse_map_region(line.c_str(), &region);
        if (region.BaseAddress <= address && (uint8_t *)region.BaseAddress + region.RegionSize > address)
        {
            // Starting region has been found
            memcpy(buff + count, &region, sizeof(MEMORY_BASIC_INFORMATION)); // Copy first region's info to buffer

                                                                             // Adjust region base and size to account for not always starting at the region's true base address:
            void *page = get_page_base(address); // Round down to the nearest page boundary (like the real VirtualQuery function)
            buff->RegionSize -= (size_t)((uint8_t*)page - (uint8_t*)((buff + count)->BaseAddress));
            buff->BaseAddress = page;
            buff->AllocationBase = region.BaseAddress;


            count++;
            break;
        }
        else if (address >= prev_region_end && address < region.BaseAddress)
        {
            // Requested address does not lie within a region for this process; set permissions to none:
            buff->Protect = MEM_PROTECT_NONE;
            buff->AllocationBase = prev_region_end;
            buff->BaseAddress = get_page_base(prev_region_end);
            buff->RegionSize = (size_t)((uint8_t*)region.BaseAddress - (uint8_t*)prev_region_end);
            buff->Type = MEM_PRIVATE;
            count++;
            break;
        }
        prev_region_end = (uint8_t *)region.BaseAddress + region.RegionSize; // Store region size and end point
    }
    if (count == 0) // Starting region was not valid; set error number
    {
        set_error(SP_ERROR_INVALID_PARAMETER);
    }
    // If buffer room still exists, get info for subsequent regions:
    while (std::getline(in_fs, line) && count != total)
    {
        parse_map_region(line.c_str(), &region);
        memcpy(buff + count, &region, sizeof(MEMORY_BASIC_INFORMATION));
        count++;
    }
    in_fs.close();
    return count * sizeof(MEMORY_BASIC_INFORMATION);
}



// Get the base adress of the currrent process:
void* get_process_base()
{
    // Resolve /proc/self/maps path (/proc/$PID/maps):
    char self_maps_file_path[32];
    sprintf(self_maps_file_path, MEM_MAP_FILE_DIR_FORMAT, (int)getpid());
    std::ifstream in_fs(self_maps_file_path); // Open file input stream
    void *process_base = NULL; // Will hold the base address of the process
    std::string line; // Will hold the first line of /proc/self/maps, which contains the base address

    if (std::getline(in_fs, line))
    {
        sscanf(line.c_str(), "%p-", &process_base);
        return process_base;
    }
    else
    {
        // getline() failed
        return NULL;
    }
}


// Parses a line from /proc/self/maps and stores the info in a MEMORY_BASIC_INFORMATION struct:
void parse_map_region(const char *maps_entry, MEMORY_BASIC_INFORMATION *mem_info)
{
    void *start, *end; // Starting and ending addresses of the current region being parsed
    char flag_permissions[4] = { '\0', '\0', '\0', '\0' }, // Read/Write/Execute flags
         flag_shared,    // "Shared" or "Private" flag character
         mapped_dev[6] = { '\0', '\0', '\0', '\0', '\0', '\0' }; // If region is mapped to a file, this is the device number where the file resides

    // Parse entry text:
    sscanf(maps_entry, "%p-%p %3c%c %*x %5c", &start, &end, flag_permissions, &flag_shared, mapped_dev);

    // Set region base address and size:
    mem_info->BaseAddress = (void*)start;
    mem_info->RegionSize = (size_t)((uint8_t*)end - (uint8_t*)start);

    // Set permissions flags:
    mem_info->Protect = 0;
    if (strcmp(flag_permissions, "---") == 0)
    {
        mem_info->Protect = MEM_PROTECT_NONE;
    }
    else
    {
        if (flag_permissions[0] == 'r')
        {
            mem_info->Protect |= MEM_PROTECT_R;
        }
        if (flag_permissions[1] == 'w')
        {
            mem_info->Protect |= MEM_PROTECT_W;
        }
        if (flag_permissions[2] == 'x')
        {
            mem_info->Protect |= MEM_PROTECT_X;
        }
    }

    // Set the region type (Private/Mapped/Image):
    mem_info->Type = 0;
    if (flag_shared == 'p')
    {
        mem_info->Type = MEM_PRIVATE;
    }
    else if (strcmp(mapped_dev, "00:00") != 0)
    {
        mem_info->Type = MEM_MAPPED;
    }
    /* @TODO:   -Determine if regions are MEM_IMAGE using /proc/$PID/exe simlink ?
    *           -More research on meanings of MEM_IMAGE, MEM_MAPPED etc.
    *           -Read "Understanding the Linux Kernel" Chapter 9.3: Memory Regions; Section 16.2: Memory Mapping
    *           -Read "Understanding the Linux Virtual Memory Manager" Section 4.4 Memory Regions
    */
}
#endif // !_WIN32


// Obtain the MEMORY_BASIC_INFORMATION struct for the first region in memory whose
//  base address is higher than the last address in the current region:
void* next_region(MEMORY_BASIC_INFORMATION *current, MEMORY_BASIC_INFORMATION *next)
{
    MEMORY_BASIC_INFORMATION buf; // Temporary buffer (serves a few purposes)
    if (next == NULL)
    { // If next is null, use temporary buffer but still return the next region's base address
        next = &buf;
    }
    if (current == NULL)
    { // No "current" memory region specified; get the first memory region in the process  
        if (sp::mem::virtual_query(get_process_base(), next, sizeof(MEMORY_BASIC_INFORMATION)) < sizeof(MEMORY_BASIC_INFORMATION))
        {
            // virtual_query failed; return NULL
            return NULL;
        }
        else
        {
            return next->BaseAddress;
        }
    }
    else
    {
        if (virtual_query(current->BaseAddress, next, sizeof(buf)) < sizeof(buf)) // Obtain current region info
        {   // virtual_query failed
            return NULL;
        }
        else
        {
            // Move past the current region:
            if (virtual_query((void*)(reinterpret_cast<uint8_t*>(next->BaseAddress) + next->RegionSize), next, sizeof(buf)) < sizeof(buf))
            {
                // virtual_query failed
                return NULL;
            }
            return next->BaseAddress;
        }
    }
}


// Obtain the base address for the first region in memory whose
//  base address is higher than the given address:
void* next_region(void *current)
{
    MEMORY_BASIC_INFORMATION buf; // Temporary buffer
    if (current == NULL)
    { // No "current" memory region specified; get the first memory region in the process  
        return get_process_base();
    }
    else
    {
        if (virtual_query(current, &buf, sizeof(buf)) < sizeof(buf)) // Obtain current region info
        {   // virtual_query failed
            return NULL;
        }
        else
        {
            // Move past the current region:
            if (virtual_query((void*)(reinterpret_cast<uint8_t*>(buf.BaseAddress) + buf.RegionSize), &buf, sizeof(buf)) < sizeof(buf))
            {
                // virtual_query failed
                return NULL;
            }
            return buf.BaseAddress;
        }
    }
}



} // namespace mem
__SP_NAMESPACE_CLOSE // namespace sp
