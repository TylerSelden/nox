all: setup build run

setup:
	@rm -rf ./build/*

build: FORCE
	@nasm -f bin ./boot/boot.asm -o ./build/boot.bin

run:
	@./utils/start.sh
#	@qemu-system-i386 -drive format=raw,file=./build/boot.bin -vnc :0

FORCE:
