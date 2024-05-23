/*
    CONTRIBUTORS:
        Sean Pesce

    Functions generated with a modified version of Lin "MavenLin" Min's DLL Wrapper Generator:
        https://github.com/mavenlin/Dll_jmp_Gen
*/
#ifdef _WIN32
#include "sp/dx/dxgi/dxgi_hooks.h"
#include "sp/main.h"


extern "C" UINT_PTR dxgi_original_procs[SP_DXGI_EXPORT_COUNT_];


__SP_NAMESPACE
namespace dx   {
namespace dxgi {

void hook_exports()
{
    // Store addresses of original exported functions from dxgi.dll
    for (int i = 0; i < SP_DXGI_EXPORT_COUNT_; i++) {
        dxgi_original_procs[i] = (UINT_PTR)GetProcAddress(global::lib::chain, (LPCSTR)sp::dx::dxgi::export_names[i]);
    }
}

} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


#ifdef _WIN64
// In-line assembly isn't supported on VC x64; see dxgi_hooks_asm.asm
#else
extern "C" __declspec(naked) void __stdcall ApplyCompatResolutionQuirking_jmp() { __asm {jmp dxgi_original_procs[0 * 4]} }
extern "C" __declspec(naked) void __stdcall CompatString_jmp() { __asm {jmp dxgi_original_procs[1 * 4]} }
extern "C" __declspec(naked) void __stdcall CompatValue_jmp() { __asm {jmp dxgi_original_procs[2 * 4]} }
extern "C" __declspec(naked) void __stdcall CreateDXGIFactory_jmp() { __asm {jmp dxgi_original_procs[3 * 4]} }
extern "C" __declspec(naked) void __stdcall CreateDXGIFactory1_jmp() { __asm {jmp dxgi_original_procs[4 * 4]} }
extern "C" __declspec(naked) void __stdcall CreateDXGIFactory2_jmp() { __asm {jmp dxgi_original_procs[5 * 4]} }
extern "C" __declspec(naked) void __stdcall DXGID3D10CreateDevice_jmp() { __asm {jmp dxgi_original_procs[6 * 4]} }
extern "C" __declspec(naked) void __stdcall DXGID3D10CreateLayeredDevice_jmp() { __asm {jmp dxgi_original_procs[7 * 4]} }
extern "C" __declspec(naked) void __stdcall DXGID3D10ETWRundown_jmp() { __asm {jmp dxgi_original_procs[8 * 4]} }
extern "C" __declspec(naked) void __stdcall DXGID3D10GetLayeredDeviceSize_jmp() { __asm {jmp dxgi_original_procs[9 * 4]} }
extern "C" __declspec(naked) void __stdcall DXGID3D10RegisterLayers_jmp() { __asm {jmp dxgi_original_procs[10 * 4]} }
extern "C" __declspec(naked) void __stdcall DXGIDumpJournal_jmp() { __asm {jmp dxgi_original_procs[11 * 4]} }
extern "C" __declspec(naked) void __stdcall DXGIGetDebugInterface1_jmp() { __asm {jmp dxgi_original_procs[12 * 4]} }
extern "C" __declspec(naked) void __stdcall DXGIReportAdapterConfiguration_jmp() { __asm {jmp dxgi_original_procs[13 * 4]} }
extern "C" __declspec(naked) void __stdcall DXGIRevertToSxS_jmp() { __asm {jmp dxgi_original_procs[14 * 4]} }
extern "C" __declspec(naked) void __stdcall PIXBeginCapture_jmp() { __asm {jmp dxgi_original_procs[15 * 4]} }
extern "C" __declspec(naked) void __stdcall PIXEndCapture_jmp() { __asm {jmp dxgi_original_procs[16 * 4]} }
extern "C" __declspec(naked) void __stdcall PIXGetCaptureState_jmp() { __asm {jmp dxgi_original_procs[17 * 4]} }
extern "C" __declspec(naked) void __stdcall SetAppCompatStringPointer_jmp() { __asm {jmp dxgi_original_procs[18 * 4]} }
extern "C" __declspec(naked) void __stdcall UpdateHMDEmulationStatus_jmp() { __asm {jmp dxgi_original_procs[19 * 4]} }
#endif // _WIN64


#endif // _WIN32
