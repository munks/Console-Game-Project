@echo off
echo Compile Start
windres resource.rc -O coff resource.res
x86_64-w64-mingw32-g++ main.cpp resource.res -o main.exe -mconsole
pause