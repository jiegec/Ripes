#include <QCoreApplication>
#include <QFileInfo>
#include <QDebug>
#include <iostream>

#include "assembler.h"
#include "parser.h"
#include "pipeline.h"
#include "elf.h"

using namespace std;

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);

    if (argc != 3) {
        printf("Usage: runner [--asm|--elf] file\n");
        return 1;
    }

    Assembler assembler;
    Parser* parser = Parser::getParser();

    if (strcmp(argv[1], "--asm") == 0) {
        QFile file(argv[2]);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString data = file.readAll();
            const QByteArray& res = assembler.assembleBinaryFile(QTextDocument(data));
            parser->loadFromByteArrayIntoData(assembler.getDataSegment());
            parser->loadFromByteArray(res);
            file.close();
        }
    } else if(strcmp(argv[1], "--elf") == 0) {
        QFile file(argv[2]);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            elf32_ehdr* ehdr = (elf32_ehdr*)data.constData();
            if (ehdr->e_ident[0] != ELF_MAGIC) {
                printf("No valid ELF magic found\n");
                return 1;
            }

            if (ehdr->e_machine != EM_RISCV) {
                printf("Not a rv32 executable\n");
                return 1;
            }

            elf32_phdr* phdr = (elf32_phdr*)(((ptrdiff_t)ehdr) + ehdr->e_phoff);
            elf32_phdr* last_phdr = (elf32_phdr*)(((ptrdiff_t)phdr) + (ehdr->e_phentsize * ehdr->e_phnum));
            while(phdr < last_phdr) {
                char* src = data.data() + phdr->p_offset;
                if (phdr->p_type != 1) {
                    // PT_LOAD
                    continue;
                }
                if (phdr->p_flags & (1 << 0)) {
                    printf("Code: loaded to %08X of size %x\n", phdr->p_paddr, phdr->p_filesz);
                    parser->loadFromByteArray(QByteArray(src, phdr->p_filesz), false, phdr->p_paddr);
                } else {
                    printf("Data: loaded to %08X of size %x\n", phdr->p_paddr, phdr->p_filesz);
                    auto memPtr = Pipeline::getPipeline()->getDataMemoryPtr();
                    for (ptrdiff_t offset = 0; offset < phdr->p_filesz;offset ++) {
                        memPtr->insert({phdr->p_paddr + offset + DATASTART, src[offset]});
                    }
                }
                ++phdr;
            }
            file.close();
        }
    } else {
        printf("Usage: runner [--asm|--elf] file\n");
        return 1;
    }

    Pipeline* pipeline = Pipeline::getPipeline();
    std::vector<uint32_t> oldRegs(32, 0);
    pipeline->restart();
    while (!pipeline->isFinished()) {
        pipeline->step();
        std::vector<uint32_t> &current = *pipeline->getRegPtr();

        for (int i = 0; i < 32;i++) {
            if (oldRegs[i] != current[i]) {
                //printf("regs[%d]: %08x -> %08x\n", i, oldRegs[i], current[i]);
            }
        }
        oldRegs = current;

        auto ecall_val = pipeline->checkEcall(true);

        switch (ecall_val.first) {
            case Pipeline::ECALL::none:
                break;
            case Pipeline::ECALL::print_string: {
                printf("%s", Parser::getParser()->getStringAt(static_cast<uint32_t>(ecall_val.second)).toUtf8().data());
                break;
            }
            case Pipeline::ECALL::print_int: {
                printf("%d", ecall_val.second);
                break;
            }
            case Pipeline::ECALL::print_char: {
                printf("%c", ecall_val.second);
                break;
            }
            case Pipeline::ECALL::exit: {
                break;
            }
        }
    }

    printf("\n\nCycle count: %d\n", pipeline->getCycleCount());
    printf("Instructions issued: %d\n", pipeline->getInstructionsExecuted());

    return 0;
}
