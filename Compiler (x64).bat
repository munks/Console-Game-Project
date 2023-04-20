@echo off
echo Compile Start
windres include/resource.rc -O coff bin/resource.res
x86_64-w64-mingw32-g++ -Iinclude main.cpp game.cpp io.cpp map.cpp move.cpp battle.cpp bin/resource.res -o bin/main.exe -mconsole
pause