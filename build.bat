echo on

rem Set paths
set CC="%PATH%\gcc.exe"
set LD="%PATH%\ld.exe"
set NASM="%PATH%\nasm.exe"
set OBJCOPY="%PATH%\objcopy.exe"

rem Set build directory
set BUILD_DIR=build
if not exist %BUILD_DIR% mkdir %BUILD_DIR%

rem Set source directories
set BOOT_DIR=boot
set COMMON_DIR=common
set KERNEL_DIR=kernel

rem Build the kernel
%CC% -ffreestanding -c -o %BUILD_DIR%\entry.o %KERNEL_DIR%\entry.c
%CC% -ffreestanding -c -o %BUILD_DIR%\irq.o %KERNEL_DIR%\interrupt\irq.c
%CC% -ffreestanding -c -o %BUILD_DIR%\tty.o %KERNEL_DIR%\tty\tty.c
%CC% -ffreestanding -c -o %BUILD_DIR%\keyboard.o %KERNEL_DIR%\keyboard\keyboard.c
%CC% -ffreestanding -c -o %BUILD_DIR%\pci.o %KERNEL_DIR%\pci\pci.c
%CC% -ffreestanding -c -o %BUILD_DIR%\paging.o %KERNEL_DIR%\memory\paging.c
%CC% -ffreestanding -c -o %BUILD_DIR%\heap.o %KERNEL_DIR%\memory\heap.c

rem Build common functions
%CC% -ffreestanding -c -o %BUILD_DIR%\common.o %KERNEL_DIR%\common\common.c
%CC% -ffreestanding -c -o %BUILD_DIR%\string.o %KERNEL_DIR%\common\string.c
%CC% -ffreestanding -c -o %BUILD_DIR%\stdlib.o %KERNEL_DIR%\common\stdlib.c
%CC% -ffreestanding -c -o %BUILD_DIR%\stdio.o %KERNEL_DIR%\common\stdio.c
%CC% -ffreestanding -c -o %BUILD_DIR%\array.o %KERNEL_DIR%\common\array.c

%LD% -Ttext 0x200000 -o %BUILD_DIR%\kernel.bin %BUILD_DIR%\entry.o %BUILD_DIR%\irq.o %BUILD_DIR%\tty.o %BUILD_DIR%\keyboard.o %BUILD_DIR%\pci.o %BUILD_DIR%\common.o %BUILD_DIR%\string.o %BUILD_DIR%\stdlib.o %BUILD_DIR%\stdio.o %BUILD_DIR%\array.o %BUILD_DIR%\paging.o %BUILD_DIR%\heap.o

%OBJCOPY% %BUILD_DIR%\kernel.bin -O binary

rem Build the secondary bootloader and compile together with kernel
%NASM% -fbin -o %BUILD_DIR%\secondary.bin %BOOT_DIR%\secondary.asm
rem Build the primary bootloader and assemble all parts together
%NASM% -fbin -o %BUILD_DIR%\primary.bin %BOOT_DIR%\primary.asm

rem Prepare boot image
if exist %BUILD_DIR%\primary.bin (
	del boot.img
	copy %BUILD_DIR%\primary.bin boot.img
)

pause