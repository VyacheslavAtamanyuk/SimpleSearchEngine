#include "lib/Parser.h"

int main(int argc, char** argv) {
    std::ostringstream stream;
    Parser parser(argc, argv, stream, true);
    parser.ParseArguments();
}
