@echo off
if exist "%VS150COMNTOOLS%" goto BUILD
if exist "%VS140COMNTOOLS%" goto BUILD
if exist "%VS120COMNTOOLS%" goto BUILD
if exist "%VS110COMNTOOLS%" goto BUILD
if exist "%VS100COMNTOOLS%" goto BUILD
echo No build tools were found
goto EOF

:BUILD
set ROOT=%~dp0
set OUT_WIN=out/win
rmdir /s /q "%OUT_WIN%"
mkdir "%OUT_WIN%" > NUL
cd "%OUT_WIN%" > NUL
cmake %ROOT% && cmake --build .
cd %ROOT%
:EOF