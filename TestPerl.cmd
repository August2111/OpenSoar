@echo off
cd /D %~dp0

rem perl -ne 'print "#define $$1 $$2\n" if /^MAKE_RESOURCE\((\w+), (\d+)\);/;' src/Resources.hpp
REM perl -ne "print \"#define $1 $2\n\" if /^MAKE_RESOURCE\((\w+), (\d+)\);/;" src/Resources.hpp >output/include/res1.h

REM rsvg-convert $(4) $$< -o $$@
REM rsvg-convert --version

REM D:/Programs/ImageMagick/convert -background white -layers flatten +matte +dither -compress none -type optimize -colors 256 -crop 50%%x100%% -scene 1 Data/graphics/launcher.svg output/data/graphics/launcher_640_1.bmp
REM D:/Programs/ImageMagick/convert -background white -layers flatten +matte +dither -compress none -type optimize -colors 256 -scene 1 Data/graphics/launcher.svg output/data/graphics/launcher_640.bmp

REM D:/Programs/ImageMagick/convert -background white -layers flatten +matte +dither -compress none -type optimize -colors 256 -crop 50\%x100\% -scene 1 output/data/graphics/launcher_640_1.bmp output/data/graphics/launcher_640_2.bmp

REM D:/Programs/ImageMagick/convert -crop 25%%x100%% -scene 1 output/data/graphics/launcher_640.bmp output/data/graphics/launcher_640_%%d.bmp

D:\Programs\7-Zip\7z.exe a output/data/temp/AUTHORS.gz AUTHORS
pause
