all: setup build run

setup:
	@rm -rf ./build/*

build: FORCE
	@nasm -felf32 ./boot/boot.asm -o ./build/boot.o
	@i686-elf-gcc -c ./kernel/kernel.c -o ./build/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	@i686-elf-gcc -T ./linker/linker.ld -o ./output/os.bin -ffreestanding -O2 -nostdlib ./build/boot.o ./build/kernel.o -lgcc
	# Create GRUB ISO
	@cp ./output/os.bin ./grub/boot/os.bin
	@grub-mkrescue -o ./output/os.iso ./grub
run:
	@./utils/start.sh
#	@qemu-system-i386 -drive format=raw,file=./build/boot.bin -vnc :0

FORCE:
