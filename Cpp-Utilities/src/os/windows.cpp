/*
    CONTRIBUTORS:
        Sean Pesce

*/

#include "sp/os/win.h"

#ifdef _WIN32


__SP_NAMESPACE
namespace os  {
namespace win {


// Wrapper function for IsWow64Process that provides compatibility on a wider range
//    of systems:
BOOL is_wow64_process()
{
    BOOL is_wow64 = false;

    /*
        From Microsoft documentation:

        "IsWow64Process is not available on all supported versions of Windows.
        Use GetModuleHandle to get a handle to the DLL that contains the function
        and GetProcAddress to get a pointer to the function if available."
    */
    is_wow64_proc_t is_wow64_func = (is_wow64_proc_t)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");
    if (NULL != is_wow64_func)
    {
        if (!is_wow64_func(GetCurrentProcess(), (PBOOL)&is_wow64))
        {
            // Call failed; assume the process isn't running under WOW64? @TODO: handle this better?
        }
    }

    return is_wow64;
}


} // namespace win
} // namespace os
__SP_NAMESPACE_CLOSE // namespace sp


#endif // _WIN32
