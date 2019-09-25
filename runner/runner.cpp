#include <QCoreApplication>
#include <QFileInfo>
#include <QDebug>
#include <QString>
#include <iostream>
#include <fstream>

#include "assembler.h"
#include "parser.h"
#include "pipeline.h"
#include "elf.h"

using namespace std;

enum Task {
    PLUS = 0,
    MULT,
    FIB,
    SORT,
    FRUIT
};

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);

    if (argc != 5) {
        fprintf(stderr, "Usage: runner elf_file case_num input output\n");
        return 1;
    }

    Assembler assembler;
    Parser* parser = Parser::getParser();


    QFile file(argv[1]);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        elf32_ehdr* ehdr = (elf32_ehdr*)data.constData();
        if (ehdr->e_ident[0] != ELF_MAGIC) {
            fprintf(stderr, "No valid ELF magic found\n");
            return 1;
        }

        if (ehdr->e_machine != EM_RISCV) {
            fprintf(stderr, "Not a rv32 executable\n");
            return 1;
        }

        elf32_phdr* phdr = (elf32_phdr*)(((ptrdiff_t)ehdr) + ehdr->e_phoff);
        elf32_phdr* last_phdr = (elf32_phdr*)(((ptrdiff_t)phdr) + (ehdr->e_phentsize * ehdr->e_phnum));
        for(;phdr < last_phdr;++phdr) {
            char* src = data.data() + phdr->p_offset;
            if (phdr->p_type != 1) {
                // PT_LOAD
                continue;
            }
            if (phdr->p_flags & (1 << 0)) {
                fprintf(stderr, "Code: loaded to %08X of size %x\n", phdr->p_paddr, phdr->p_filesz);
                parser->loadFromByteArray(QByteArray(src, phdr->p_filesz), false, phdr->p_paddr);
            } else {
                fprintf(stderr, "Data: loaded to %08X of size %x\n", phdr->p_paddr, phdr->p_filesz);
                auto memPtr = Pipeline::getPipeline()->getDataMemoryPtr();
                for (ptrdiff_t offset = 0; offset < phdr->p_filesz;offset ++) {
                    memPtr->insert({phdr->p_paddr + offset, src[offset]});
                }
            }
        }
        file.close();
    }


    Pipeline* pipeline = Pipeline::getPipeline();
    pipeline->restart();
    pipeline->disableMemoryAccesses();
    int load_cycles = 0;
    bool data_loaded = false;
    bool call_exit = false;
    uint32_t answer_addr = 0;
    auto memPtr = pipeline->getRuntimeMemoryPtr();
    auto task = QString(argv[2]).toInt();

    uint32_t prob; // use to store problem-specific data for judging

    while (!pipeline->isFinished() && !call_exit) {
        pipeline->step();

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
            case Pipeline::ECALL::load_data: {
                if (!data_loaded) {
                    load_cycles = pipeline->getCycleCount();
                    data_loaded = true;
                }

                if (task >= 0 && task <= 4) {
                    fprintf(stderr, "Running on task %d\n", task);
                }
                ifstream input(argv[3]);
                if (input) {
                    uint32_t byteIndex = static_cast<uint32_t>(ecall_val.second);
                    fprintf(stderr, "Load data to %08x\n", byteIndex);

                    switch (task) {
                        case PLUS:
                        case MULT:
                        case FIB: {
                            int m, n;
                            input >> m >> n;
                            memPtr->write(byteIndex, m, 4);
                            memPtr->write(byteIndex + 4, n, 4);
                            break;
                        }
                        case SORT: {
                            int m;
                            input >> m;
                            prob = m;
                            // length
                            memPtr->write(byteIndex, m, 4);
                            byteIndex += 4;
                            // actual data
                            for (int i = 0; i < m; ++i) {
                                int temp;
                                input >> temp;
                                memPtr->write(byteIndex, temp, 4);
                                byteIndex += 4;
                            }
                            break;
                        }
                        case FRUIT: {
                            int n;
                            input >> n;
                            prob = n;
                            // length
                            memPtr->write(byteIndex, n, 4);
                            byteIndex += 4;
                            // actual data
                            for (int i = 0; i < n * 2; ++i) {
                                int temp;
                                input >> temp;
                                memPtr->write(byteIndex, temp, 4);
                                byteIndex += 4;
                            }
                            break;
                        }
                    }
                }
                break;
            }

            case Pipeline::ECALL::set_answer_addr: {
                answer_addr = static_cast<uint32_t>(ecall_val.second);
                break;
            }

            case Pipeline::ECALL::exit: {
                call_exit = true;
                break;
            }
        }
    }

    int stop_count = pipeline->getCycleCount();
    int user_count = stop_count - load_cycles;

    fprintf(stderr, "Instructions issued: %d\n", pipeline->getInstructionsExecuted());
    fprintf(stderr, "Total cycle count: %d\n", stop_count);
    if (data_loaded) {
        fprintf(stderr, "Total cycle count after loading data: %d\n", user_count);
    }

    ofstream out(argv[4]);
    switch (task) {
        case PLUS:
        case MULT:
        case FIB: {
            int ans = (int) memPtr->read(answer_addr);
            out << ans << endl;
            break;
        }

        case SORT:
        case FRUIT: {
            int m = prob;
            for (int i = 0; i < m; ++i) {
                int ans = (int) memPtr->read(answer_addr);
                out << ans << endl;
                memPtr += 4;
            }
            break;
        }
    }

    return 0;
}
