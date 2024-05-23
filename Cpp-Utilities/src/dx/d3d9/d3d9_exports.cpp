/*
    CONTRIBUTORS:
        Sean Pesce

*/
#ifdef _WIN32
#include "sp/log.h"
#include "sp/string.h"
#include "sp/dx/d3d9/d3d9_exports.h"
#include "sp/dx/d3d9/d3d9_hooks.h"


extern "C" UINT_PTR d3d9_original_procs[SP_D3D9_EXPORT_COUNT_];



//  Creates an IDirect3D9 object and returns an interface to it.
IDirect3D9* WINAPI Direct3DCreate9(UINT SDKVersion)
{
    SP_LOG_DBG("\nIntercepted D3D9::" __FUNCTION__ "(UINT SDKVersion):");
    IDirect3D9* result = ((Direct3DCreate9_t)d3d9_original_procs[sp::dx::d3d9::Direct3DCreate9])(SDKVersion);
    SP_LOG_DBG("    SDKVersion = %u (0x%x)\n", SDKVersion, SDKVersion);
    return result;
}


//  Creates an IDirect3D9Ex object and returns an interface to it.
HRESULT WINAPI Direct3DCreate9Ex(UINT SDKVersion, IDirect3D9Ex **ppD3D)
{
    SP_LOG_DBG("\nIntercepted D3D9::" __FUNCTION__ "(UINT SDKVersion, IDirect3D9Ex **ppD3D):");
    HRESULT result = ((Direct3DCreate9Ex_t)d3d9_original_procs[sp::dx::d3d9::Direct3DCreate9Ex])(SDKVersion, ppD3D);
    SP_LOG_DBG("    SDKVersion = %u (0x%x)\n    ppD3D      = 0x%p  ->  0x%p\n", SDKVersion, SDKVersion, ppD3D, *ppD3D);
    return result;
}


//  This function is internal and not intended to be used by software developers.
void WINAPI DebugSetMute()
{
    SP_LOG_DBG("\nIntercepted D3D9::" __FUNCTION__ "()\n");
    ((DebugSetMute_t)d3d9_original_procs[sp::dx::d3d9::DebugSetMute])();
}


//  This function is internal and not intended to be used by software developers.
void WINAPI Direct3DShaderValidatorCreate9()
{
    SP_LOG_DBG("\nIntercepted D3D9::" __FUNCTION__ "()\n");
    ((Direct3DShaderValidatorCreate9_t)d3d9_original_procs[sp::dx::d3d9::Direct3DShaderValidatorCreate9])();
}



#endif // _WIN32
