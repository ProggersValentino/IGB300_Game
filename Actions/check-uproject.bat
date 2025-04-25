echo Hello World!
echo off
if exist IGB300_Geme.uproject (
	echo Exists!
	exit /b 1
) else (
	echo DoesNotExist!
	exit /b 0
)
