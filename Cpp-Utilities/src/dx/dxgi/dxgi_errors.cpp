/*
    CONTRIBUTORS:
        Sean Pesce

    References:
        https://msdn.microsoft.com/en-us/library/windows/desktop/bb509553(v=vs.85).aspx
*/
#ifdef _WIN32
#include "sp/dx/dxgi/dxgi_errors.h"

__SP_NAMESPACE
namespace dx   {
namespace dxgi {
namespace err  {


std::string name(HRESULT code)
{
    switch (code)
    {
        case S_OK:                                    return "S_OK";
            break;
        case DXGI_ERROR_ACCESS_DENIED:                return "DXGI_ERROR_ACCESS_DENIED";
            break;
        case DXGI_ERROR_ACCESS_LOST:                  return "DXGI_ERROR_ACCESS_LOST";
            break;
        //case DXGI_ERROR_ALREADY_EXISTS:               return "DXGI_ERROR_ALREADY_EXISTS";
        //    break;
        case DXGI_ERROR_CANNOT_PROTECT_CONTENT:       return "DXGI_ERROR_CANNOT_PROTECT_CONTENT";
            break;
        case DXGI_ERROR_DEVICE_HUNG:                  return "DXGI_ERROR_DEVICE_HUNG";
            break;
        case DXGI_ERROR_DEVICE_REMOVED:               return "DXGI_ERROR_DEVICE_REMOVED";
            break;
        case DXGI_ERROR_DEVICE_RESET:                 return "DXGI_ERROR_DEVICE_RESET";
            break;
        case DXGI_ERROR_DRIVER_INTERNAL_ERROR:        return "DXGI_ERROR_DRIVER_INTERNAL_ERROR";
            break;
        case DXGI_ERROR_FRAME_STATISTICS_DISJOINT:    return "DXGI_ERROR_FRAME_STATISTICS_DISJOINT";
            break;
        case DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE: return "DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE";
            break;
        case DXGI_ERROR_INVALID_CALL:                 return "DXGI_ERROR_INVALID_CALL";
            break;
        case DXGI_ERROR_MORE_DATA:                    return "DXGI_ERROR_MORE_DATA";
            break;
        case DXGI_ERROR_NAME_ALREADY_EXISTS:          return "DXGI_ERROR_NAME_ALREADY_EXISTS";
            break;
        case DXGI_ERROR_NONEXCLUSIVE:                 return "DXGI_ERROR_NONEXCLUSIVE";
            break;
        case DXGI_ERROR_NOT_CURRENTLY_AVAILABLE:      return "DXGI_ERROR_NOT_CURRENTLY_AVAILABLE";
            break;
        case DXGI_ERROR_NOT_FOUND:                    return "DXGI_ERROR_NOT_FOUND";
            break;
        case DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED:   return "DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED";
            break;
        case DXGI_ERROR_REMOTE_OUTOFMEMORY:           return "DXGI_ERROR_REMOTE_OUTOFMEMORY";
            break;
        case DXGI_ERROR_RESTRICT_TO_OUTPUT_STALE:     return "DXGI_ERROR_RESTRICT_TO_OUTPUT_STALE";
            break;
        case DXGI_ERROR_SDK_COMPONENT_MISSING:        return "DXGI_ERROR_SDK_COMPONENT_MISSING";
            break;
        case DXGI_ERROR_SESSION_DISCONNECTED:         return "DXGI_ERROR_SESSION_DISCONNECTED";
            break;
        case DXGI_ERROR_UNSUPPORTED:                  return "DXGI_ERROR_UNSUPPORTED";
            break;
        case DXGI_ERROR_WAIT_TIMEOUT:                 return "DXGI_ERROR_WAIT_TIMEOUT";
            break;
        case DXGI_ERROR_WAS_STILL_DRAWING:            return "DXGI_ERROR_WAS_STILL_DRAWING";
            break;
        default:
            return "UNKNOWN_ERROR";
            break;
    }
}


} // namespace err
} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


#endif // _WIN32
