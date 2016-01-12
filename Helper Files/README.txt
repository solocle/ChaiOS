Directories in /langs should be expanded into the ChaiOS directory.

/
|---boot	#GRUB
|---ChaiOS
    |---[langs go here]
    |---System32
        |-->[System Files (.dll, .KRN)]
        |-->ramdisk.crd
    |---System64
        |-->[System Files (.dll, .KRN)]
        |-->ramdisk.crd
|-->[copyramdisk.bat, filelist.ini, mkinitrd.exe]	#Ramdisk stuff
|-->vesamode.crm					#Real mode module for VESA mode switching, compiled into ramdisk