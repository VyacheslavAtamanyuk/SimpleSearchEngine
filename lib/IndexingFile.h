#pragma once

#include <string>
#include <vector>
#include <map>

struct Document {
    Document(std::string name, size_t words_count): name_(std::move(name)), words_count_(words_count) {}
    std::string name_;
    size_t words_count_;
};

struct DocForWord {
    DocForWord(size_t doc_idx, size_t frequency): doc_idx_(doc_idx), frequency_(frequency) {}

    DocForWord(size_t doc_idx, size_t frequency, std::vector<size_t> lines): doc_idx_(doc_idx), frequency_(frequency), lines_(std::move(lines)) {}

    size_t doc_idx_;
    size_t frequency_;
    std::vector<size_t> lines_;
};

class IndexingFile {
public:
    IndexingFile(std::string indexing_file, std::vector<std::string> arguments):
    indexing_file_(std::move(indexing_file)),
    arguments_(std::move(arguments)) {}

    void RecursiveTraversal(std::string file_or_folder);
    void CreateIndexingFile();
private:
    std::string indexing_file_;
    std::vector<std::string> arguments_;

    std::vector<Document> docs_;
    std::map<std::string, std::vector<DocForWord>> words_and_dids_;
};