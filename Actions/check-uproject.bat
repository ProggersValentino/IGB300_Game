echo Hello World!
echo off
if exist IGB300_Geme.uproject (
	echo Exists!
	exit /b 0
) else (
	echo DoesNotExist!
	exit /b 1
)
