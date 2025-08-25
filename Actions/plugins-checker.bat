echo off
cd Tools/plugins-checker/
plugin-checker.exe ModelingToolsEditorMode GameplayAbilities BlueprintFileUtils BlueprintStats AnimationWarping BlendStack OnlineSubsystemSteam OnlineFramework NiagaraUIRenderer
if %ERRORLEVEL% NEQ 0 (
	echo plugin-checker paniced
	cd ../../
	exit /b %ERRORLEVEL%
)
cd ../../
