PREFIX := riscv64-elf-
MAKEFLAGS += --no-builtin-rules
.SUFFIXES:

.PHONY: all clean run
all: $(patsubst %.c,%,$(wildcard *.c))

clean:
	rm -rf $(patsubst %.c,%,$(wildcard *.c)) $(patsubst %.c,%.o,$(wildcard *.c))

%.o: %.c lib.h
	$(PREFIX)gcc -c $< -o $@ -march=rv32im -mabi=ilp32 -ffreestanding -nostdlib -O2

%: %.o
	$(PREFIX)ld $^ -T linker.lds -o $@ -melf32lriscv

run: all
	echo $(patsubst %.c,%,$(wildcard *.c)) | xargs -n 1 ../RipesRunner --elf

.SECONDARY:
