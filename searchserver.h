#ifndef SEARCHSERVER_H
#define SEARCHSERVER_H

#include "invertedindex.h"
#include "converterjson.h"
#include <string>
#include <vector>

struct RelativeIndex{
    size_t doc_id = -1;
    float rank = 0.0;
    bool operator ==(const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank - other.rank < 0.0001);
    }
    bool operator >(const RelativeIndex& other) const {
        return (rank > other.rank || (rank == other.rank && doc_id < other.doc_id));
    }

};
class SearchServer {
public:
    /**
    * @param idx в конструктор класса передаётся ссылка на класс
    InvertedIndex,
    *      чтобы SearchServer мог узнать частоту слов встречаемых в
    запросе
    */
    SearchServer(InvertedIndex& idx) : _index(idx){ };
    /**
    * Метод обработки поисковых запросов
    * @param queries_input поисковые запросы взятые из файла
    requests.json
    * @return возвращает отсортированный список релевантных ответов для
    заданных запросов
    */
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);

private:
    InvertedIndex _index;
};

#endif