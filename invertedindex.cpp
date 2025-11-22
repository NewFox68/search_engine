#include "invertedindex.h"
#include <algorithm>
#include <utility>
#include <mutex>
#include <thread>

std::mutex mtx;

void InvertedIndex::InvIndexing(std::string doc, int doc_index) {
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

        mtx.lock();

        if (freq_dictionary.find(wrd) == freq_dictionary.end()) {
            freq_dictionary[wrd].push_back(entry);
        }
        else if (freq_dictionary.find(wrd) != freq_dictionary.end()
            && freq_dictionary[wrd].back().doc_id != doc_index) {
            freq_dictionary[wrd].push_back(entry);
            }

        mtx.unlock();
    }
}

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs = std::move(input_docs);

    for (int doc_index = 0; doc_index < docs.size(); ++doc_index) {
        std::thread thread([&]() {
            this->InvIndexing(docs[doc_index], doc_index);
        });

        thread.join();
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) {
    auto result = freq_dictionary[word];
    return result;
}