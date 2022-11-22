@echo off
REM python D:/Programs/Meson/tools/cmake2meson.py %~dp0

cd /D %~dp0

set VS=2022
REM call "C:\Program Files (x86)\Microsoft Visual Studio\%VS%\Professional\VC\Auxiliary\Build\vcvars64.bat"
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

PATH=%PATH%;D:\Programs\Meson
REM D:\\Programs\\Meson\\meson.exe --internal regenerate D:\\build_ws\\GenICam\\branches\\BRANCH_aug\\BRANCH_AutoFocus\\output D:\\build_ws\\GenICam\\branches\\BRANCH_aug\\BRANCH_AutoFocus\\src --backend=vs2019

REM pause 

if exist "output\MesonTest.sln" set DO_CREATE=--wipe
REM meson setup --backend vs --wipe output src 
REM meson setup --backend vs%VS% %DO_CREATE% output src 

meson setup --backend vs%VS% %DO_CREATE% meson ./ 

pause