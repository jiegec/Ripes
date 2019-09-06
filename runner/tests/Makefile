PREFIX := riscv64-elf-
MAKEFLAGS += --no-builtin-rules
.SUFFIXES:

all: $(patsubst %.c,%,$(wildcard *.c))

%.o: %.c
	$(PREFIX)gcc $^ -o $@ -march=rv32im -mabi=ilp32 -fPIE -ffreestanding -nostdlib

%: %.o
	$(PREFIX)ld $^ -T linker.lds -o $@ -melf32lriscv
