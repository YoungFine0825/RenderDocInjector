/*
    CONTRIBUTORS:
        Sean Pesce

    Functions generated with a modified version of Lin "MavenLin" Min's DLL Wrapper Generator:
        https://github.com/mavenlin/Dll_jmp_Gen
*/
#ifdef _WIN32
#include "sp/dx/d3d9/d3d9_hooks.h"
#include "sp/main.h"


extern "C" UINT_PTR d3d9_original_procs[SP_D3D9_EXPORT_COUNT_];


__SP_NAMESPACE
namespace dx   {
namespace d3d9 {

void hook_exports()
{
    // Store addresses of original exported functions from d3d9.dll
    for (int i = 0; i < SP_D3D9_EXPORT_COUNT_; i++) {
        d3d9_original_procs[i] = (UINT_PTR)GetProcAddress(global::lib::chain, (LPCSTR)sp::dx::d3d9::export_names[i]);
    }
}

} // namespace d3d9
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


#ifdef _WIN64
// In-line assembly isn't supported on VC x64; see d3d9_hooks_asm.asm
#else
extern "C" __declspec(naked) void __stdcall D3DPERF_BeginEvent_jmp() { __asm {jmp d3d9_original_procs[0 * 4]} }
extern "C" __declspec(naked) void __stdcall D3DPERF_EndEvent_jmp() { __asm {jmp d3d9_original_procs[1 * 4]} }
extern "C" __declspec(naked) void __stdcall D3DPERF_GetStatus_jmp() { __asm {jmp d3d9_original_procs[2 * 4]} }
extern "C" __declspec(naked) void __stdcall D3DPERF_QueryRepeatFrame_jmp() { __asm {jmp d3d9_original_procs[3 * 4]} }
extern "C" __declspec(naked) void __stdcall D3DPERF_SetMarker_jmp() { __asm {jmp d3d9_original_procs[4 * 4]} }
extern "C" __declspec(naked) void __stdcall D3DPERF_SetOptions_jmp() { __asm {jmp d3d9_original_procs[5 * 4]} }
extern "C" __declspec(naked) void __stdcall D3DPERF_SetRegion_jmp() { __asm {jmp d3d9_original_procs[6 * 4]} }
extern "C" __declspec(naked) void __stdcall DebugSetLevel_jmp() { __asm {jmp d3d9_original_procs[7 * 4]} }
extern "C" __declspec(naked) void __stdcall DebugSetMute_jmp() { __asm {jmp d3d9_original_procs[8 * 4]} }
extern "C" __declspec(naked) void __stdcall Direct3D9EnableMaximizedWindowedModeShim_jmp() { __asm {jmp d3d9_original_procs[9 * 4]} }
extern "C" __declspec(naked) void __stdcall Direct3DCreate9_jmp() { __asm {jmp d3d9_original_procs[10 * 4]} }
extern "C" __declspec(naked) void __stdcall Direct3DCreate9Ex_jmp() { __asm {jmp d3d9_original_procs[11 * 4]} }
extern "C" __declspec(naked) void __stdcall Direct3DShaderValidatorCreate9_jmp() { __asm {jmp d3d9_original_procs[12 * 4]} }
extern "C" __declspec(naked) void __stdcall PSGPError_jmp() { __asm {jmp d3d9_original_procs[13 * 4]} }
extern "C" __declspec(naked) void __stdcall PSGPSampleTexture_jmp() { __asm {jmp d3d9_original_procs[14 * 4]} }
#endif // _WIN64


#endif // _WIN32
