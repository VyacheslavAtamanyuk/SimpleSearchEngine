#include "Parser.h"
#include <iostream>

void Parser::ParseArguments() {
    if (!std::strcmp(argv_[1], "--indexing")) {
        std::string indexing_filename = argv_[2];

        std::vector<std::string> arguments;
        for (size_t i = 0; i < argc_ - 3; ++i) {
            arguments.push_back(argv_[i + 3]);
        }

        IndexingFile file(std::move(indexing_filename), std::move(arguments));
        file.CreateIndexingFile();
    } else if (!std::strcmp(argv_[1], "--search")) {
        std::string indexing_filename = argv_[2];
        std::string docs_filename = argv_[3];

        std::vector<std::string> splited_request;

        splited_request.push_back("(");
        for (size_t i = 0; i < argc_ - 4; ++i) {
            std::string buffer = argv_[i + 4];

            if (argv_[i + 4][0] == '(') {
                for (size_t j = 0; j < buffer.size(); ++j) {
                    buffer[j] = std::tolower(buffer[j]);
                }

                while (buffer[0] == '(') {
                    splited_request.push_back("(");
                    buffer.erase(buffer.begin());
                }

                splited_request.push_back(std::move(buffer));
            } else if (std::string(argv_[i + 4]).back() == ')') {
                for (size_t j = 0; j < buffer.size(); ++j) {
                    buffer[j] = std::tolower(buffer[j]);
                }

                size_t counter = 0;
                while (buffer.back() == ')') {
                    ++counter;
                    buffer.pop_back();
                }

                splited_request.push_back(std::move(buffer));

                for (size_t j = 0; j < counter; ++j) {
                    splited_request.push_back(")");
                }
            } else {
                std::string original = buffer;

                for (size_t j = 0; j < buffer.size(); ++j) {
                    buffer[j] = std::tolower(buffer[j]);
                }

                if ((buffer == "and" && original != "AND" && original != "and") || (buffer == "or" && original != "OR" && original != "or")) {
                    stream_ << "Invalid request";

                    if (call_from_main_) {
                        std::cout << stream_.str();
                    }

                    return;
                }

                splited_request.push_back(std::move(buffer));
            }
        }
        splited_request.push_back(")");

        SearchEngine search_engine(std::move(splited_request), std::move(indexing_filename), std::move(docs_filename), stream_, call_from_main_);
        search_engine.PrepareData();
        search_engine.GetTopFiveDocuments();

        if (call_from_main_) {
            std::cout << stream_.str();
        }
    }
}