#ifndef INVERTEDINDEX_H
#define INVERTEDINDEX_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>

struct Entry {
    size_t doc_id = 0, count = 0;
    //Данный оператор необходим для проведения тестовых сценариев
    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id &&
        count == other.count);
    }
};
class InvertedIndex {
public:     InvertedIndex() = default;
    /**
     * Метод индексирует один документ для запуска
     * процесса индексации в нескольких потоках
     */
    void InvIndexing(std::string doc, int doc_index);
    /**
    * Обновить или заполнить базу документов, по которой будем совершать
    поиск
    * @param texts_input содержимое документов
    */
    void UpdateDocumentBase(std::vector<std::string> input_docs);
    /**
    Метод определяет количество вхождений слова word в загруженной базе
    документов
    * @param word слово, частоту вхождений которого необходимо определить
    * @return возвращает подготовленный список с частотой слов
    */
    std::vector<Entry> GetWordCount(const std::string& word);
    friend Entry;

private:
    std::vector<std::string> docs; // список содержимого документов
    std::map<std::string, std::vector<Entry>> freq_dictionary; // частотный словарь
};

#endif