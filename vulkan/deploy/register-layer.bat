@echo off
setlocal
rem ============================================================
rem  Register the ungated RenderDoc Vulkan layer for the CURRENT USER.
rem
rem  Per-user (HKCU): no administrator rights, affects only this user, and is
rem  undone by unregister-layer.bat (one registry value).
rem
rem  Effect while registered: every Vulkan program this user runs will load
rem  RenderDoc's layer (that is the point -- RenderDoc's own manifest gates it
rem  behind an env var that MuMu's service-launched renderer never sees).
rem ============================================================
set "MANIFEST=%~dp0renderdoc-layer-ungated.json"
set "KEY=HKCU\SOFTWARE\Khronos\Vulkan\ImplicitLayers"

if not exist "%MANIFEST%" (
    echo [ERROR] manifest not found: %MANIFEST%
    echo         run:  python tools\gen_manifest.py
    exit /b 1
)

reg add "%KEY%" /v "%MANIFEST%" /t REG_DWORD /d 0 /f
if errorlevel 1 (
    echo [ERROR] reg add failed
    exit /b 1
)

echo.
echo Registered (per-user, no admin):
echo   %KEY%
echo     "%MANIFEST%" = 0
echo.
echo Next: start the game in MuMu (Vulkan mode), then
echo   qrenderdoc -^> File -^> Attach to Running Instance
echo.
echo Undo with: deploy\unregister-layer.bat
exit /b 0
