@echo off
cd /D %~dp0../..

echo %CD%
PATH=%CD%;%CD%\build\cmake\python;%PATH%
python build/cmake/python/Start-CMake-XCSoar.py  xcsoar mgw122  15

pause
