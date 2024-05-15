NASMPARAMS = -felf32
GCCPARAMS  = -nostdlib -MMD -Iinclude -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Wno-unused-function -Wno-return-type -g
LDPARAMS   = -ffreestanding -O2 -nostdlib -lgcc
CPATHS     = ./drivers** ./include** ./kernel** ./lib**

SRCS      := $(shell find $(CPATHS) -name "*.c")

all: build run

debug: build run_debug



clean:
	@rm -rf ./build/*

boot:
	@nasm $(NASMPARAMS) ./boot/boot.asm -o ./build/boot.o

gcc:
	@$(foreach SRC,$(SRCS),i686-elf-gcc $(GCCPARAMS) -c $(SRC) -o ./build/$(notdir $(SRC:.c=.o));)

ld:
	@i686-elf-gcc $(LDPARAMS) -T ./utils/linker.ld -o ./output/os.bin ./build/*.o

grub:
	@cp ./output/os.bin ./utils/grub/boot/os.bin
	@grub-mkrescue -o ./output/os.iso ./utils/grub




build: clean boot gcc ld grub

run:
	@./utils/start.sh

run_debug:
	@./utils/debug.sh

.PHONY: all debug boot gcc ld grub build debug run_debug
