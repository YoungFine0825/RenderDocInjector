/*
    CONTRIBUTORS:
        Sean Pesce

*/
#ifdef _WIN32
#include "sp/os/windows/com/com_main.h"
#include "sp/string.h"

__SP_NAMESPACE
namespace os  {
namespace win {
namespace com {


namespace guid {
std::string to_string(const GUID &guid)
{
    return sp::str::format("%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                                    (unsigned long) guid.Data1,
                                    (unsigned short)guid.Data2,
                                    (unsigned short)guid.Data3,
                                    (unsigned short)guid.Data4[0],
                                    (unsigned short)guid.Data4[1],
                                    (unsigned short)guid.Data4[2],
                                    (unsigned short)guid.Data4[3],
                                    (unsigned short)guid.Data4[4],
                                    (unsigned short)guid.Data4[5],
                                    (unsigned short)guid.Data4[6],
                                    (unsigned short)guid.Data4[7]);
}
} // namespace guid


} // namespace com
} // namespace win
} // namespace os
__SP_NAMESPACE_CLOSE // namespace sp


#endif // _WIN32
