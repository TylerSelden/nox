all: setup build run

setup:
	@rm -rf ./build/*

build: FORCE
	@gcc -ffreestanding -c -fno-pie -m32 ./kernel/kernel.c -o ./build/kernel.o
	@nasm ./lib/kernel/kernel_entry.asm -f elf -o build/kernel_entry.o
	@ld ./build/kernel_entry.o ./build/kernel.o -m elf_i386 -Ttext 0x1000 --oformat binary -o ./build/kernel.bin
	
	@nasm -f bin ./boot/boot.asm -o ./build/boot.bin
	
	@cat ./build/boot.bin ./build/kernel.bin > ./build/os.bin
	
#	@truncate -s 1440k ./build/os.bin

run:
	@./utils/start.sh
#	@qemu-system-i386 -drive format=raw,file=./build/boot.bin -vnc :0

FORCE:
