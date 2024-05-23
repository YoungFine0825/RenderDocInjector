/*
    CONTRIBUTORS:
        Sean Pesce

*/
#ifdef _WIN32
#include "sp/dx/dxgi/dxgi_main.h"


extern "C" UINT_PTR dxgi_original_procs[SP_DXGI_EXPORT_COUNT_] = { 0 };


__SP_NAMESPACE
namespace dx   {
namespace dxgi {

const std::string filename = SP_DX_LIB_NAME_DXGI_ ".dll";
const char* const export_names[SP_DXGI_EXPORT_COUNT_] = { "ApplyCompatResolutionQuirking", "CompatString", "CompatValue", "CreateDXGIFactory", "CreateDXGIFactory1", "CreateDXGIFactory2", "DXGID3D10CreateDevice", "DXGID3D10CreateLayeredDevice", "DXGID3D10ETWRundown", "DXGID3D10GetLayeredDeviceSize", "DXGID3D10RegisterLayers", "DXGIDumpJournal", "DXGIGetDebugInterface1", "DXGIReportAdapterConfiguration", "DXGIRevertToSxS", "PIXBeginCapture", "PIXEndCapture", "PIXGetCaptureState", "SetAppCompatStringPointer", "UpdateHMDEmulationStatus" };

} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp



#endif // _WIN32
