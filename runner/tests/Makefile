PREFIX := riscv64-elf-
MAKEFLAGS += --no-builtin-rules
.SUFFIXES:

.PHONY: all clean
all: $(patsubst %.c,%,$(wildcard *.c))

clean:
	rm -rf $(patsubst %.c,%,$(wildcard *.c)) $(patsubst %.c,%.o,$(wildcard *.c))

%.o: %.c lib.h
	$(PREFIX)gcc -c $< -o $@ -march=rv32im -mabi=ilp32 -ffreestanding -nostdlib

%: %.o
	$(PREFIX)ld $^ -T linker.lds -o $@ -melf32lriscv

.SECONDARY:
