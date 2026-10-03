@echo off
setlocal
rem ============================================================
rem  Undo register-layer.bat: remove the per-user implicit-layer value.
rem  Only this one registry value is touched; the manifest file is left alone.
rem ============================================================
set "MANIFEST=%~dp0renderdoc-layer-ungated.json"
set "KEY=HKCU\SOFTWARE\Khronos\Vulkan\ImplicitLayers"

echo Removing registry value:
echo   %KEY%
echo     "%MANIFEST%"
reg delete "%KEY%" /v "%MANIFEST%" /f
if errorlevel 1 (
    echo (not registered, or already removed)
) else (
    echo Removed.
)
exit /b 0
