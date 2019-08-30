#include <QCoreApplication>
#include <QFileInfo>
#include <QDebug>
#include <iostream>

#include "assembler.h"
#include "parser.h"
#include "pipeline.h"

using namespace std;

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);

    if (argc != 2) {
        return 1;
    }

    Assembler assembler;
    Parser* parser = Parser::getParser();
    QFile file(argv[1]);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString data = file.readAll();
        const QByteArray& res = assembler.assembleBinaryFile(QTextDocument(data));
        parser->loadFromByteArrayIntoData(assembler.getDataSegment());
        parser->loadFromByteArray(res);
        file.close();
    }

    Pipeline* pipeline = Pipeline::getPipeline();
    while (!pipeline->isFinished()) {
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
            case Pipeline::ECALL::exit: {
                break;
            }
        }
    }

    printf("\n\nCycle count: %d\n", pipeline->getCycleCount());
    printf("Instructions issued: %d\n", pipeline->getInstructionsExecuted());

    return 0;
}
