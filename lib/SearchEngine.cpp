#include "SearchEngine.h"
#include <fstream>
#include <iostream>
#include <stack>
#include <algorithm>
#include <cmath>

const float k = 2.0;
const float b = 0.75;

void SearchEngine::PrepareData() {
    std::ifstream docs_file(docs_filename_);

    std::string line;

    while (std::getline(docs_file, line)) {
        std::string name;

        size_t i = 0;
        while (line[i] != '\t') {
            name.push_back(line[i]);
            ++i;
        }
        ++i;

        size_t words_count = 0;
        std::string buffer_for_words_count;

        while (i < line.size()) {
            buffer_for_words_count.push_back(line[i]);
            ++i;
        }

        words_count = std::stoull(buffer_for_words_count);
        docs_len_sum_ += words_count;

        docs_.emplace_back(std::move(name), words_count);
    }

    std::ifstream index_file(index_filename_);

    while (std::getline(index_file, line)) {
        std::string word;

        size_t i = 0;
        while (line[i] != '\t') {
            word.push_back(line[i]);
            ++i;
        }
        ++i;

        size_t doc_idx;
        size_t frequency;
        std::vector<std::size_t> lines;
        while (i < line.size()) {
            if (line[i] == '\t') {
                lines.clear();
                ++i;
                continue;
            }

            std::string buffer_for_doc_idx;
            while (line[i] != ' ') {
                buffer_for_doc_idx.push_back(line[i]);
                ++i;
            }
            ++i;

            doc_idx = std::stoull(buffer_for_doc_idx);

            std::string buffer_for_frequency;
            while (line[i] != ' ') {
                buffer_for_frequency.push_back(line[i]);
                ++i;
            }
            ++i;

            frequency = std::stoull(buffer_for_frequency);

            while (line[i] != '\t') {
                std::string buffer_for_line;
                while (line[i] != ' ') {
                    buffer_for_line.push_back(line[i]);
                    ++i;
                }
                ++i;
                lines.push_back(std::stoull(buffer_for_line));
            }

            words_and_dids_[word].emplace(doc_idx, frequency, std::move(lines));
        }
    }
}

void SearchEngine::DoOrRequest(const std::string& first_word, const std::string& second_word) {
    auto i = words_and_dids_[first_word].begin();
    auto j = words_and_dids_[second_word].begin();

    std::set<DocForWord, cmp_idx> new_set;

    while (i != words_and_dids_[first_word].end() && j != words_and_dids_[second_word].end()) {
        if (i->doc_idx_ == j->doc_idx_) {
            size_t new_doc_idx = i->doc_idx_;
            size_t new_frequency = i->frequency_ + j->frequency_;
            std::vector<size_t> new_lines = i->lines_;

            for (size_t line: j->lines_) {
                new_lines.push_back(line);
            }

            new_set.emplace(new_doc_idx, new_frequency, std::move(new_lines));

            ++i;
            ++j;
        } else if (i->doc_idx_ < j->doc_idx_) {
            size_t new_doc_idx = i->doc_idx_;
            size_t new_frequency = i->frequency_;
            std::vector<size_t> new_lines = i->lines_;

            new_set.emplace(new_doc_idx, new_frequency, std::move(new_lines));

            ++i;
        } else if (j->doc_idx_ < i->doc_idx_) {
            size_t new_doc_idx = j->doc_idx_;
            size_t new_frequency = j->frequency_;
            std::vector<size_t> new_lines = j->lines_;

            new_set.emplace(new_doc_idx, new_frequency, std::move(new_lines));

            ++j;
        }
    }

    while (i != words_and_dids_[first_word].end()) {
        size_t new_doc_idx = i->doc_idx_;
        size_t new_frequency = i->frequency_;
        std::vector<size_t> new_lines = i->lines_;

        new_set.emplace(new_doc_idx, new_frequency, std::move(new_lines));

        ++i;
    }

    while (j != words_and_dids_[second_word].end()) {
        size_t new_doc_idx = j->doc_idx_;
        size_t new_frequency = j->frequency_;
        std::vector<size_t> new_lines = j->lines_;

        new_set.emplace(new_doc_idx, new_frequency, std::move(new_lines));

        ++j;
    }

    words_and_dids_[first_word + " or " + second_word] = std::move(new_set);
}

void SearchEngine::DoAndRequest(const std::string& first_word, const std::string& second_word) {
    auto i = words_and_dids_[first_word].begin();
    auto j = words_and_dids_[second_word].begin();

    std::set<DocForWord, cmp_idx> new_set;
    while (i != words_and_dids_[first_word].end() && j != words_and_dids_[second_word].end()) {
        if (i->doc_idx_ == j->doc_idx_) {
            size_t new_doc_idx = i->doc_idx_;
            size_t new_frequency = i->frequency_ + j->frequency_;
            std::vector<size_t> new_lines = i->lines_;

            for (size_t line: j->lines_) {
                new_lines.push_back(line);
            }

            new_set.emplace(new_doc_idx, new_frequency, std::move(new_lines));

            ++i;
            ++j;
        } else if (i->doc_idx_ < j->doc_idx_) {
            ++i;
        } else if (j->doc_idx_ < i->doc_idx_) {
            ++j;
        }
    }

    words_and_dids_[first_word + " and " + second_word] = std::move(new_set);
}

float SearchEngine::GetScore(const DocForWord* key_for_word, const std::string& word) {
    float dll_avg = docs_len_sum_ / docs_.size();
    float dll_this_doc = docs_[key_for_word->doc_idx_].words_count_;
    float dft = words_and_dids_[word].size();
    float term_freq = key_for_word->frequency_;
    return ((term_freq * (k + 1)) / (term_freq + k * (1 - b + b * (dll_this_doc / dll_avg)))) * std::log(static_cast<float>(docs_.size()) / dft);
}

std::vector<std::pair<float, const DocForWord*>> SearchEngine::SortDocsByScores(const std::string& word) {
    std::vector<std::pair<float, const DocForWord*>> scores_and_indices;

    for (const DocForWord& doc: words_and_dids_[word]) {
        scores_and_indices.push_back(std::make_pair(GetScore(&doc, word), &doc));
    }

    std::sort(scores_and_indices.begin(), scores_and_indices.end(), [](
            const std::pair<float, const DocForWord*>& first,
            const std::pair<float, const DocForWord*>& second) { return first.first >= second.first;});

    return scores_and_indices;
}

void SearchEngine::GetTopFiveDocuments() {
    std::stack<std::string> words;
    std::stack<std::string> ops;

    for (const std::string& expression: request_) {
        if (expression == "and" || expression == "or") {
            ops.push(expression);
        } else if (expression == ")") {
            if (words.empty()) {
                stream_ << "Invalid request";
                return;
            }

            std::string first_word = words.top();
            words.pop();

            if (words.empty() && !ops.empty()) {
                stream_ << "Invalid request";
                return;
            } else if (words.empty() && ops.empty()) {
                words.push(first_word);
                continue;
            } else if (!words.empty() && ops.empty()) {
                stream_ << "Invalid request";
                return;
            }

            std::string second_word = words.top();
            words.pop();

            std::string op = ops.top();
            ops.pop();

            if (op == "or") {
                words.push(first_word + " or " + second_word);
                DoOrRequest(first_word, second_word);
            } else if (op == "and") {
                words.push(first_word + " and " + second_word);
                DoAndRequest(first_word, second_word);
            }

        } else if (expression != "(") {
            words.push(expression);
        }
    }

    if (ops.size() > 0 || (ops.size() == 0 && words.size() > 1)) {
        stream_ << "Invalid request";
        return;
    }

    std::vector<std::pair<float, const DocForWord*>> docs_sorted_by_scores = SortDocsByScores(words.top());

    if (docs_sorted_by_scores.empty()) {
        stream_ << "There is no files that satisfy this request";
        return;
    }

    size_t num_of_output_docs = std::min(docs_sorted_by_scores.size(), static_cast<unsigned long long>(5));

    stream_ << '\n';
    for (size_t i = 0; i < num_of_output_docs; ++i) {
        stream_ << "Filename: " << docs_[docs_sorted_by_scores[i].second->doc_idx_].name_ << '\t';
        stream_ << "Frequency: " << docs_sorted_by_scores[i].second->frequency_ << '\t';
        stream_ << "Lines: ";
        for (size_t line: docs_sorted_by_scores[i].second->lines_) {
            stream_ << line << ' ';
        }
        stream_ << '\n';
    }
}