#ifndef CONVERTER_JSON_H
#define CONVERTER_JSON_H

#include <iostream>
#include <fstream>
#include <string>
#include "nlohmann/json.hpp"
#include <vector>
#include <string>
#include <thread>
#include <mutex>


class ConverterJSON {
    std::vector<std::string> textDoc;
public:
    ConverterJSON() = default;
    void NormalizeSpaсesToLower(const std::string fName, int index);
    /**
    * Метод получения содержимого файлов
    * @return Возвращает список с содержимым файлов перечисленных
    *       в config.json
    */
    std::vector<std::string> GetTextDocuments();
    /**
    * Метод считывает поле max_responses для определения предельного
    *  количества ответов на один запрос
    * @return
    */
    int GetResponsesLimit() const;
    /**
    * Метод получения запросов из файла requests.json
    * @return возвращает список запросов из файла requests.json
    */
    std::vector<std::string> GetRequests();
    /**
    * Положить в файл answers.json результаты поисковых запросов
    */
    void putAnswers(std::vector<std::vector<std::pair<int, float>>>
    answers);

};

#endif