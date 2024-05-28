#include "IndexingFile.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

uint8_t A_ASCII = 65;
uint8_t Z_ASCII = 90;
uint8_t a_ASCII = 97;
uint8_t z_ASCII = 122;

void IndexingFile::RecursiveTraversal(std::string file_or_folder) {
    if (fs::is_directory(file_or_folder)) {
        for (const auto& internal_files_or_folders: fs::directory_iterator{file_or_folder}) {
            RecursiveTraversal(internal_files_or_folders.path().string());
        }
    } else {
        std::ifstream file(file_or_folder);
        size_t words_counter = 0;

        std::string line;
        size_t lines_counter = 0;

        std::string buffer;
        while (std::getline(file, line)) {
            ++lines_counter;

            for (char symbol: line) {
                if (!((symbol >= A_ASCII && symbol <= Z_ASCII) || (symbol >= a_ASCII && symbol <= z_ASCII))) {
                    if (buffer.size() > 0) {
                        ++words_counter;
                        bool is_there_such_idx = false;

                        for (size_t i = 0; i < words_and_dids_[buffer].size(); ++i) {
                            if (words_and_dids_[buffer][i].doc_idx_ == docs_.size()) {
                                is_there_such_idx = true;
                                ++words_and_dids_[buffer][i].frequency_;
                                words_and_dids_[buffer][i].lines_.push_back(lines_counter);
                                break;
                            }
                        }

                        if (!is_there_such_idx) {
                            words_and_dids_[buffer].emplace_back(docs_.size(), 1);
                            for (size_t i = 0; i < words_and_dids_[buffer].size(); ++i) {
                                if (words_and_dids_[buffer][i].doc_idx_ == docs_.size()) {
                                    words_and_dids_[buffer][i].lines_.push_back(lines_counter);
                                }
                            }
                        }
                    }
                    buffer.clear();
                } else {
                    buffer.push_back(std::tolower(symbol));
                }
            }
        }

        docs_.emplace_back(std::move(file_or_folder), words_counter);
    }
}

void IndexingFile::CreateIndexingFile() {
    for (std::string& argument: arguments_) {
        RecursiveTraversal(std::move(argument));
    }

    std::ofstream docs_info(indexing_file_ + "_docs.txt");
    for (size_t i = 0; i < docs_.size(); ++i) {
        docs_info << docs_[i].name_ << '\t' << docs_[i].words_count_ << '\n';
    }

    std::ofstream map_info(indexing_file_ + ".txt");
    for (const auto& word_and_dids: words_and_dids_) {
        map_info << word_and_dids.first << '\t';
        for (size_t i = 0; i < word_and_dids.second.size(); ++i) {
            map_info << word_and_dids.second[i].doc_idx_ << ' ' << word_and_dids.second[i].frequency_ << ' ';
            for (size_t j = 0; j < word_and_dids.second[i].lines_.size(); ++j) {
                map_info << word_and_dids.second[i].lines_[j] << ' ';
            }
            map_info << '\t';
        }
        map_info << '\n';
    }
}