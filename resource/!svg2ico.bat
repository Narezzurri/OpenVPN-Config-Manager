@REM  Dependancies:
@REM  - VxKex (Optional) (https://github.com/i486/VxKex)
@REM  - Inkscape (https://github.com/inkscape/inkscape)
@REM  - ImageMagick (https://github.com/ImageMagick/ImageMagick)
@REM  You should install these softwares to launch this batch.
@REM  Tiny:Weight=300 Grade=200 Sharp OpticalSize=20px
@REM  Taskbar:Weight=300 Grade=0 Outlined OpticalSize=24px
@REM  General:Weight=400 Grade=0 Outlined OpticalSize=40px,48px
@echo off
for %%i in (%*) do (
	echo Creating png from %%i
	VxKexLdr inkscape %%i\%%~nxiTiny.svg --export-type=png --export-width=16 --export-height=16 --export-filename=%%i\%%~nxi_16.png
	VxKexLdr inkscape %%i\%%~nxiTiny.svg --export-type=png --export-width=20 --export-height=20 --export-filename=%%i\%%~nxi_20.png
	VxKexLdr inkscape %%i\%%~nxiTaskbar.svg --export-type=png --export-width=24 --export-height=24 --export-filename=%%i\%%~nxi_24.png
	VxKexLdr inkscape %%i\%%~nxiTaskbar.svg --export-type=png --export-width=30 --export-height=30 --export-filename=%%i\%%~nxi_30.png
	VxKexLdr inkscape %%i\%%~nxi#40.svg --export-type=png --export-width=32 --export-height=32 --export-filename=%%i\%%~nxi_32.png
	VxKexLdr inkscape %%i\%%~nxi#40.svg --export-type=png --export-width=36 --export-height=36 --export-filename=%%i\%%~nxi_36.png
	VxKexLdr inkscape %%i\%%~nxi#40.svg --export-type=png --export-width=40 --export-height=40 --export-filename=%%i\%%~nxi_40.png
	VxKexLdr inkscape %%i\%%~nxi#48.svg --export-type=png --export-width=48 --export-height=48 --export-filename=%%i\%%~nxi_48.png
	VxKexLdr inkscape %%i\%%~nxi#48.svg --export-type=png --export-width=60 --export-height=60 --export-filename=%%i\%%~nxi_60.png
	VxKexLdr inkscape %%i\%%~nxi#48.svg --export-type=png --export-width=64 --export-height=64 --export-filename=%%i\%%~nxi_64.png
	VxKexLdr inkscape %%i\%%~nxi#48.svg --export-type=png --export-width=72 --export-height=72 --export-filename=%%i\%%~nxi_72.png
	VxKexLdr inkscape %%i\%%~nxi#48.svg --export-type=png --export-width=80 --export-height=80 --export-filename=%%i\%%~nxi_80.png
	VxKexLdr inkscape %%i\%%~nxi#48.svg --export-type=png --export-width=96 --export-height=96 --export-filename=%%i\%%~nxi_96.png
	VxKexLdr inkscape %%i\%%~nxi#48.svg --export-type=png --export-width=128 --export-height=128 --export-filename=%%i\%%~nxi_128.png
	VxKexLdr inkscape %%i\%%~nxi#48.svg --export-type=png --export-width=256 --export-height=256 --export-filename=%%i\%%~nxi_256.png
)
echo ================================================
echo Ready to pack?
pause
for %%i in (%*) do (
	echo Creating ico from %%i
	magick %%i\%%~nxi_16.png %%i\%%~nxi_20.png %%i\%%~nxi_24.png %%i\%%~nxi_30.png %%i\%%~nxi_32.png %%i\%%~nxi_36.png %%i\%%~nxi_40.png %%i\%%~nxi_48.png %%i\%%~nxi_60.png %%i\%%~nxi_64.png %%i\%%~nxi_72.png %%i\%%~nxi_80.png %%i\%%~nxi_96.png %%i\%%~nxi_128.png %%i\%%~nxi_256.png %%~nxi.ico
	del %%i\%%~nxi_16.png
	del %%i\%%~nxi_20.png
	del %%i\%%~nxi_24.png
	del %%i\%%~nxi_30.png
	del %%i\%%~nxi_32.png
	del %%i\%%~nxi_36.png
	del %%i\%%~nxi_40.png
	del %%i\%%~nxi_48.png
	del %%i\%%~nxi_60.png
	del %%i\%%~nxi_64.png
	del %%i\%%~nxi_72.png
	del %%i\%%~nxi_80.png
	del %%i\%%~nxi_96.png
	del %%i\%%~nxi_128.png
	del %%i\%%~nxi_256.png
)
echo Done.
pause