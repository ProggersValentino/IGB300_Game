@echo off
echo Deleting Binaries and Intermediates folders...

rmdir /s /q "Binaries"
rmdir /s /q "Intermediate"

echo Done.
"./IGB300_Geme.uproject"
