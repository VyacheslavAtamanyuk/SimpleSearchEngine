#pragma once

#include "IndexingFile.h"
#include "SearchEngine.h"
#include <cstring>
#include <sstream>

class Parser {
public:
    Parser(int argc, char** argv, std::ostringstream& stream, bool call_from_main):
    argc_(argc),
    argv_(argv),
    stream_(stream),
    call_from_main_(call_from_main) {}

    void ParseArguments();

private:
    int argc_;
    char** argv_;
    std::ostringstream& stream_;
    bool call_from_main_;
};