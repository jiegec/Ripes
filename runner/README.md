## How to generate elf for running

riscv64-elf-as source.asm -o source.o -march=rv32g -fPIC
riscv64-elf-ld source.o -o source --section-start=.text=0 -e 0 -melf32lriscv
runner --elf source
