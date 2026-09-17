for %%i in (%*) do (
	move /Y %%~nxiTiny.svg %%~nxi
	move /Y %%~nxiTaskbar.svg %%~nxi
	move /Y %%~nxi#40.svg %%~nxi
	move /Y %%~nxi#48.svg %%~nxi
)