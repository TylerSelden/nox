all: setup build run

setup:
	@rm -rf ./build/*

build: FORCE
	@nasm -f bin ./boot/boot.asm -o ./build/boot.bin

run:
	@qemu-system-x86_64 -display curses ./build/boot.bin

FORCE:
