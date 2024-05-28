#pragma once
#include "IndexingFile.h"
#include <sstream>
#include <set>

struct cmp_idx {
    bool operator() (const DocForWord& first, const DocForWord& second) const {
        return first.doc_idx_ < second.doc_idx_;
    }
};

class SearchEngine {
public:
    SearchEngine(std::vector<std::string> request,
                 std::string index_filename,
                 std::string docs_filename,
                 std::ostringstream& stream,
                 bool call_from_main):
    request_(std::move(request)),
    index_filename_(std::move(index_filename)),
    docs_filename_(std::move(docs_filename)),
    docs_len_sum_(0),
    stream_(stream),
    call_from_main_(call_from_main) {}

    void PrepareData();

    void DoOrRequest(const std::string& first_word, const std::string& second_word);
    void DoAndRequest(const std::string& first_word, const std::string& second_word);

    float GetScore(const DocForWord* key_for_word, const std::string& word);
    std::vector<std::pair<float, const DocForWord*>> SortDocsByScores(const std::string& word);

    void GetTopFiveDocuments();

private:
    std::vector<std::string> request_;
    std::string index_filename_;
    std::string docs_filename_;

    size_t docs_len_sum_;

    std::vector<Document> docs_;
    std::map<std::string, std::set<DocForWord, cmp_idx>> words_and_dids_;

    std::ostringstream& stream_;
    bool call_from_main_;
};