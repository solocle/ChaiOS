nasm -f bin setvesamode.asm -o A:\vesamode.crm
pushd A:\
call A:\copyramdisk.bat
popd
pause
