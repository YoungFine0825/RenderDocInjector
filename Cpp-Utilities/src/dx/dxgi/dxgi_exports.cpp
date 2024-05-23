/*
    CONTRIBUTORS:
        Sean Pesce

*/
#ifdef _WIN32
#include "sp/os/windows/com/com_main.h"
#include "sp/log.h"
#include "sp/string.h"
#include "sp/dx/dxgi/dxgi_errors.h"
#include "sp/dx/dxgi/dxgi_exports.h"
#include "sp/dx/dxgi/dxgi_hooks.h"


extern "C" UINT_PTR dxgi_original_procs[SP_DXGI_EXPORT_COUNT_];



//  Creates a DXGI 1.0 factory that you can use to generate other DXGI objects.
HRESULT WINAPI CreateDXGIFactory(REFIID riid, void **ppFactory)
{
    SP_LOG_DBG("\nIntercepted DXGI::" __FUNCTION__ "(REFIID riid, void **ppFactory):");
    HRESULT result = ((CreateDXGIFactory_t)dxgi_original_procs[sp::dx::dxgi::CreateDXGIFactory])(riid, ppFactory);
    SP_LOG_DBG("    riid        = %s\n    ppFactory   = 0x%p  ->  0x%p\n    Return code = %s (0x%x)\n",
                    sp::os::win::com::guid::to_string(riid).c_str(), ppFactory, *ppFactory, sp::dx::dxgi::err::name(result).c_str(), result);
    return result;
}


//  Creates a DXGI 1.1 factory that you can use to generate other DXGI objects.
HRESULT WINAPI CreateDXGIFactory1(REFIID riid, void **ppFactory)
{
    SP_LOG_DBG("\nIntercepted DXGI::" __FUNCTION__ "(REFIID riid, void **ppFactory):");
    HRESULT result = ((CreateDXGIFactory1_t)dxgi_original_procs[sp::dx::dxgi::CreateDXGIFactory1])(riid, ppFactory);
    SP_LOG_DBG("    riid        = %s\n    ppFactory   = 0x%p  ->  0x%p\n    Return code = %s (0x%x)\n",
                    sp::os::win::com::guid::to_string(riid).c_str(), ppFactory, *ppFactory, sp::dx::dxgi::err::name(result).c_str(), result);
    return result;
}


//  Creates a DXGI 1.3 factory that you can use to generate other DXGI objects.
HRESULT WINAPI CreateDXGIFactory2(UINT Flags, REFIID riid, void **ppFactory)
{
    SP_LOG_DBG("\nIntercepted DXGI::" __FUNCTION__ "(UINT Flags, REFIID riid, void **ppFactory):");
    HRESULT result = ((CreateDXGIFactory2_t)dxgi_original_procs[sp::dx::dxgi::CreateDXGIFactory2])(Flags, riid, ppFactory);
    SP_LOG_DBG("    Flags        = 0x%x (%d)\n    riid        = %s\n    ppFactory   = 0x%p  ->  0x%p\n    Return code = %s (0x%x)\n",
                    Flags, Flags, sp::os::win::com::guid::to_string(riid).c_str(), ppFactory, *ppFactory, sp::dx::dxgi::err::name(result).c_str(), result);
    return result;
}


// Allows a process to indicate that it's resilient to any of its graphics devices being removed.
// NOTE: Not supported until next Windows update
HRESULT WINAPI DXGIDeclareAdapterRemovalSupport()
{
    SP_LOG("\nWARNING: Intercepted call to unsupported function DXGI::" __FUNCTION__ "()    (Returning DXGI_ERROR_INVALID_CALL)\n");
    return DXGI_ERROR_INVALID_CALL; // Function not yet supported
    //HRESULT result = ((DXGIDeclareAdapterRemovalSupport_t)dxgi_original_procs[?])();
    //return result;
}


// Retrieves a debugging interface.
HRESULT WINAPI DXGIGetDebugInterface(REFIID riid, void **ppDebug)
{
    SP_LOG_DBG("\nIntercepted DXGI::" __FUNCTION__ "(REFIID riid, void **ppDebug)    (Redirecting to DXGIGetDebugInterface1)");
    // Redirect to DXGIGetDebugInterface1
    HRESULT result = ((DXGIGetDebugInterface1_t)dxgi_original_procs[sp::dx::dxgi::DXGIGetDebugInterface1])(0, riid, ppDebug);
    SP_LOG_DBG("    riid        = %s\n    ppDebug     = 0x%p  ->  0x%p\n    Return code = %s (0x%x)\n",
                    sp::os::win::com::guid::to_string(riid).c_str(), ppDebug, *ppDebug, sp::dx::dxgi::err::name(result).c_str(), result);
    return result;
}


//  Undocumented DXGI export
void* SP_DXGI_ALTCALL CompatValue(const char *strUnknown, void **ppUnknown)
{
    SP_LOG("\nIntercepted DXGI::" __FUNCTION__ "(const char *strUnknown, void **ppUnknown):");
    void* result = ((CompatValue_t)dxgi_original_procs[sp::dx::dxgi::CompatValue])(strUnknown, ppUnknown);
    SP_LOG("    strUnknown   = %s\n    ppUnknown    = 0x%p  ->  0x%p\n    Return value = 0x%p\n",
                strUnknown, ppUnknown, *ppUnknown, result);
    return result;
}


// Retrieves an interface that Windows Store apps use for debugging the Microsoft DirectX Graphics Infrastructure (DXGI).
HRESULT WINAPI DXGIGetDebugInterface1(UINT Flags, REFIID riid, void **ppDebug)
{
    SP_LOG_DBG("\nIntercepted DXGI::" __FUNCTION__ "(UINT Flags, REFIID riid, void **ppDebug)");
    HRESULT result = ((DXGIGetDebugInterface1_t)dxgi_original_procs[sp::dx::dxgi::DXGIGetDebugInterface1])(Flags, riid, ppDebug);
    SP_LOG_DBG("    Flags       = 0x%x (%d)\n    riid        = %s\n    ppDebug     = 0x%p  ->  0x%p\n    Return code = %s (0x%x)\n",
                    Flags, Flags, sp::os::win::com::guid::to_string(riid).c_str(), ppDebug, *ppDebug, sp::dx::dxgi::err::name(result).c_str(), result);
    return result;
}


//  Undocumented DXGI export
HRESULT WINAPI DXGID3D10CreateDevice(HMODULE hModule, IDXGIFactory *pFactory, IDXGIAdapter *pAdapter, UINT Flags, void *pUnknown, void **ppDevice)
{
    SP_LOG_DBG("\nIntercepted DXGI::" __FUNCTION__ "(HMODULE hModule, IDXGIFactory *pFactory, IDXGIAdapter *pAdapter, UINT Flags, void *pUnknown, void **ppDevice)\n");
    HRESULT result = ((DXGID3D10CreateDevice_t)dxgi_original_procs[sp::dx::dxgi::DXGID3D10CreateDevice])(hModule, pFactory, pAdapter, Flags, pUnknown, ppDevice);
    return result;
}

//  Undocumented DXGI export
HRESULT WINAPI DXGID3D10CreateLayeredDevice(void *pUnknown1, void *pUnknown2, void *pUnknown3, void *pUnknown4, void *pUnknown5)
{
    SP_LOG_DBG("\nIntercepted DXGI::" __FUNCTION__ "(void *pUnknown1, void *pUnknown2, void *pUnknown3, void *pUnknown4, void *pUnknown5)\n");
    HRESULT result = ((DXGID3D10CreateLayeredDevice_t)dxgi_original_procs[sp::dx::dxgi::DXGID3D10CreateLayeredDevice])(pUnknown1, pUnknown2, pUnknown3, pUnknown4, pUnknown5);
    return result;
}

//  Undocumented DXGI export
SIZE_T WINAPI DXGID3D10GetLayeredDeviceSize(const void *pLayers, UINT NumLayers)
{
    SP_LOG_DBG("\nIntercepted DXGI::" __FUNCTION__ "(const void *pLayers, UINT NumLayers)\n");
    SIZE_T result = ((DXGID3D10GetLayeredDeviceSize_t)dxgi_original_procs[sp::dx::dxgi::DXGID3D10GetLayeredDeviceSize])(pLayers, NumLayers);
    return result;
}

//  Undocumented DXGI export
HRESULT WINAPI DXGID3D10RegisterLayers(const void *pLayers, UINT NumLayers)
{
    SP_LOG_DBG("\nIntercepted DXGI::" __FUNCTION__ "(const void *pLayers, UINT NumLayers)\n");
    HRESULT result = ((DXGID3D10RegisterLayers_t)dxgi_original_procs[sp::dx::dxgi::DXGID3D10RegisterLayers])(pLayers, NumLayers);
    return result;
}


//  Undocumented DXGI export
void SP_DXGI_ALTCALL ApplyCompatResolutionQuirking(void *pUnknown1, void *pUnknown2)
{
    SP_LOG("\nIntercepted DXGI::" __FUNCTION__ "(void *pUnknown1, void *pUnknown2)    (Unconfirmed function prototype)\n");
    ((ApplyCompatResolutionQuirking_t)dxgi_original_procs[sp::dx::dxgi::ApplyCompatResolutionQuirking])(pUnknown1, pUnknown2);
}

//  Undocumented DXGI export
int WINAPI CompatString(const char *strUnknown1, void **ppUnknown, char *strUnknown2, bool bUnknown)
{
    SP_LOG("\nIntercepted DXGI::" __FUNCTION__ "(const char *strUnknown1, void **ppUnknown, char *strUnknown2, bool bUnknown)    (Unconfirmed function prototype)\n");
    int result = ((CompatString_t)dxgi_original_procs[sp::dx::dxgi::CompatString])(strUnknown1, ppUnknown, strUnknown2, bUnknown);
    return result;
}

//  Undocumented DXGI export
void SP_DXGI_ALTCALL DXGIDumpJournal(void(WINAPI *funcUnknown)(const char*))
{
    SP_LOG("\nIntercepted DXGI::" __FUNCTION__ "(void(WINAPI *funcUnknown)(const char*))    (Unconfirmed function prototype)\n");
    ((DXGIDumpJournal_t)dxgi_original_procs[sp::dx::dxgi::DXGIDumpJournal])(funcUnknown);
}

//  Undocumented DXGI export
void* WINAPI DXGIRevertToSxS()
{
    SP_LOG("\nIntercepted DXGI::" __FUNCTION__ "()    (Unconfirmed function prototype)\n");
    void* result = ((DXGIRevertToSxS_t)dxgi_original_procs[sp::dx::dxgi::DXGIRevertToSxS])();
    return result;
}

//  Undocumented DXGI export
void SP_DXGI_ALTCALL SetAppCompatStringPointer(void *pUnknown, const char *strUnknown)
{
    SP_LOG("\nIntercepted DXGI::" __FUNCTION__ "(void *pUnknown, const char *strUnknown)    (Unconfirmed function prototype)\n");
    ((SetAppCompatStringPointer_t)dxgi_original_procs[sp::dx::dxgi::SetAppCompatStringPointer])(pUnknown, strUnknown);
}

//  Undocumented DXGI export
long SP_DXGI_ALTCALL UpdateHMDEmulationStatus(bool bUnknown)
{
    SP_LOG("\nIntercepted DXGI::" __FUNCTION__ "(bool bUnknown)    (Unconfirmed function prototype)\n");
    long result = ((UpdateHMDEmulationStatus_t)dxgi_original_procs[sp::dx::dxgi::UpdateHMDEmulationStatus])(bUnknown);
    return result;
}



#endif // _WIN32
