#include "invertedindex.h"
#include <algorithm>
#include <utility>

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs = std::move(input_docs);
    int doc_index = 0;

    for (const auto& doc : docs) {
        std::istringstream stream(doc);
        std::string word;
        std::vector<std::string> words;
        while (std::getline(stream, word, ' ')) {
            words.push_back(word);
        }
        for (const auto& wrd: words) {
            Entry entry;
            entry.count = count(words.begin(), words.end(), wrd);
            entry.doc_id = doc_index;
            if (freq_dictionary.find(wrd) == freq_dictionary.end()) {
                freq_dictionary[wrd].push_back(entry);
            }
            else if (freq_dictionary.find(wrd) != freq_dictionary.end()
                && freq_dictionary[wrd].back().doc_id != doc_index) {
                freq_dictionary[wrd].push_back(entry);
                }
        }
        ++doc_index;
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) {
    auto result = freq_dictionary[word];
    return result;
}