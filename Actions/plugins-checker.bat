echo off
cd Tools/plugins-checker/
plugin-checker.exe ModelingToolsEditorMode GameplayAbilities
if %ERRORLEVEL% NEQ 0 (
	echo plugin-checker paniced
	cd ../../
	exit /b %ERRORLEVEL%
)
cd ../../
