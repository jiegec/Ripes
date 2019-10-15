#include <QCoreApplication>
#include <QFileInfo>
#include <QDebug>
#include <QString>
#include <iostream>
#include <fstream>

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

// black magic: our prelude procedure takes some cycles,
// which needs to be reducted from user count
static const int CYCLE_OFFSET[] = { 34, 34, 34, 29, 41 };

enum Status {
    NORMAL = 0,
    TLE,
    RE
};

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    if (argc != 6) {
        fprintf(stderr, "Usage: runner elf_file case_num input output max_cycle\n");
        return 1;
    }

    auto parser = Parser::getParser();
    Pipeline* pipeline = Pipeline::getPipeline();

    // load ELF file
    QFile elf(argv[1]);
    if (elf.open(QIODevice::ReadOnly)) {
        QByteArray data = elf.readAll();
        elf32_ehdr* ehdr = (elf32_ehdr*)data.constData();
        if (ehdr->e_ident[0] != ELF_MAGIC) {
            fprintf(stderr, "No valid ELF magic found\n");
            return 1;
        }

        if (ehdr->e_machine != EM_RISCV) {
            fprintf(stderr, "Given ELF file is not of RV32 arch\n");
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
                auto dataMemPtr = pipeline->getDataMemoryPtr();
                for (ptrdiff_t offset = 0; offset < phdr->p_filesz;offset ++) {
                    dataMemPtr->insert({phdr->p_paddr + offset, src[offset]});
                }
            }
        }
        elf.close();
    }


    int load_cycles = 0;
    bool data_loaded = false;
    bool call_exit = false;
    uint32_t answer_addr = 0;
    uint32_t prob; // use to store problem-specific data for judging


    // prepare pipeline
    pipeline->restart();
    pipeline->disableRecord();
    auto memPtr = pipeline->getRuntimeMemoryPtr();

    auto task = QString(argv[2]).toInt();
    if (task >= 0 && task <= 4) {
        fprintf(stderr, "Running on task %d\n", task);
    } else {
        fprintf(stderr, "Invalid task number %d\n", task);
        return 1;
    }

    // run the pipeline for at most MAX_CYCLES
    int max_cycle = 100000000;
    sscanf(argv[5], "%d", &max_cycle);
    while (!pipeline->isFinished() && !call_exit && (!data_loaded || pipeline->getCycleCount() - load_cycles < max_cycle)) {
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
                    load_cycles = pipeline->getCycleCount() + CYCLE_OFFSET[task];
                    data_loaded = true;
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
                            for (int i = 0; i < n * 4; ++i) {
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
                call_exit = true;
                break;
            }

            case Pipeline::ECALL::exit: {
                call_exit = true;
                break;
            }
        }
    }


    // check cycles
    if (pipeline->getCycleCount() >= max_cycle + load_cycles) {
        // running for too many cycles
        fprintf(stderr, "Program has run for too many cycles\n");
        // status code and user cycle count will be written to stdout
        cout << TLE << " " << 0 << endl;
        return 0;
    }

    int stop_count = pipeline->getCycleCount();
    int user_count = stop_count - load_cycles;

    fprintf(stderr, "Instructions issued: %d\n", pipeline->getInstructionsExecuted());
    fprintf(stderr, "Total cycle count: %d\n", stop_count);

    if (data_loaded) {
        fprintf(stderr, "Total cycle count after loading data: %d\n", user_count);
    } else {
        fprintf(stderr, "Program did not load any data\n");
        return 1;
    }


    // write output file
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
                out << ans << "\n "[i < m-1 && task == FRUIT];
                answer_addr += 4;
            }
            break;
        }
    }

    // there must be some strange hack if we get negative user_count
    if (user_count < 1) {
        cout << RE << " " << 0 << endl;
        return 0;
    }

    // status code and user cycle count will be written to stdout
    cout << NORMAL << " " << user_count << endl;

    return 0;
}
