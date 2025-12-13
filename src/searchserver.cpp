#include "searchserver.h"
#include <algorithm>
#include <set>


std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string> &queries_input) {
    std::vector<std::vector<RelativeIndex>> result;
    ConverterJSON converter;

    for (const auto& request : queries_input) { // проходим по каждому запросу
        std::vector<RelativeIndex> rel_index; // создаем вектор структур RelativeIndex
        float max_rank = 0.0f; // создаем переменную максимального рейтинга
        std::set <std::string> request_words; // создаем множество для уникальных слов запроса
        std::istringstream stream(request);  // распарсиваем строку запроса на слова и помещаем их в множество
        std::string word;
        std::vector<std::string> words;
        while (std::getline(stream, word, ' ')) {
            request_words.insert(word);
        }
        for (const auto& wrd : request_words) { // берем слово из множества уникальных слов запроса
            std::vector<Entry> entry = _index.GetWordCount(wrd); // и получаем для него вектор структур Entry
            if (!entry.empty()) { // если этот вектор не пустой,
                for (const auto& en : entry) {
                    RelativeIndex r_index; // то создаем структуру RelativeIndex и если вектор структур RelativeIndex пустой
                    if (rel_index.empty()) { // присваиваем ее полям значения первого элемента вектора структур Entry
                        r_index.doc_id = en.doc_id;
                        r_index.rank = static_cast<float>(en.count);
                        max_rank = std::max(max_rank, r_index.rank); // присваиваем переменной максимального
                                                                        // рейтинга значение rank первого элемента
                        rel_index.push_back(r_index); // помещаем первое значение в вектор структур RelativeIndex
                    }
                    else { // если вектор структур не пустой, то проверяем наличие в нем элемента с тем же docid
                        auto it = std::find_if(rel_index.begin(), rel_index.end(),[en](RelativeIndex idx){return en.doc_id == idx.doc_id;});
                        if (it != rel_index.end()) { // если такой элемент есть,
                            it->rank += static_cast<float>(en.count);  // то увеличиваем его rank
                            max_rank = std::max(max_rank, it->rank); // и присваиваем максимальный rank, если он больше
                        }
                        else { // если такого элемента еще нет, то вносим его в вектор структур и проверяем rank на максимум
                            r_index.doc_id = en.doc_id;
                            r_index.rank = static_cast<float>(en.count);
                            max_rank = std::max(max_rank, r_index.rank);
                            rel_index.push_back(r_index);
                        }
                    }
                }
            }
        }
        std::sort(rel_index.begin(), rel_index.end(), [](const RelativeIndex a, const RelativeIndex b){return a > b;});
        std::vector<RelativeIndex> rel_index_final;
         if (rel_index.size() > converter.GetResponsesLimit()){
            for (auto it = rel_index.begin(); it != rel_index.begin() + converter.GetResponsesLimit(); ++it) {
                it->rank = it->rank / max_rank;
                rel_index_final.push_back(*it);
            }
             result.push_back(rel_index_final);
         }
         else {
             for (auto & it : rel_index) {
                 it.rank = it.rank / max_rank;
                 rel_index_final.push_back(it);
             }
             result.push_back(rel_index_final);
         }
    }

    return result;
}