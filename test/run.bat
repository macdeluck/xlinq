@echo off
set ROOT=%~dp0
set OUT_WIN=out/win
mkdir "%OUT_WIN%" > NUL
cd "%OUT_WIN%" > NUL
ctest
cd %ROOT%
pause