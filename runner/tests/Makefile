PREFIX := riscv64-elf-
MAKEFLAGS += --no-builtin-rules
.SUFFIXES:

all: $(patsubst %.c,%,$(wildcard *.c))

%.o: %.c
	$(PREFIX)gcc $^ -o $@ -march=rv32g -mabi=ilp32 -fPIC -ffreestanding -nostdlib

%: %.o
	$(PREFIX)ld $^ -T linker.lds -o $@ -melf32lriscv
