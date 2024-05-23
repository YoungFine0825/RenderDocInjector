/*
    CONTRIBUTORS:
        Sean Pesce

*/
#ifdef _WIN32
#include "sp/dx/d3d9/d3d9_main.h"


extern "C" UINT_PTR d3d9_original_procs[SP_D3D9_EXPORT_COUNT_] = { 0 };


__SP_NAMESPACE
namespace dx   {
namespace d3d9 {

const std::string filename = SP_DX_LIB_NAME_D3D9_ ".dll";
const char* const export_names[SP_D3D9_EXPORT_COUNT_] = { "D3DPERF_BeginEvent", "D3DPERF_EndEvent", "D3DPERF_GetStatus", "D3DPERF_QueryRepeatFrame", "D3DPERF_SetMarker", "D3DPERF_SetOptions", "D3DPERF_SetRegion", "DebugSetLevel", "DebugSetMute", "Direct3D9EnableMaximizedWindowedModeShim", "Direct3DCreate9", "Direct3DCreate9Ex", "Direct3DShaderValidatorCreate9", "PSGPError", "PSGPSampleTexture" };

} // namespace d3d9
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp



#endif // _WIN32
