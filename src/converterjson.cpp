#include "converterjson.h"
#include <io.h>
#include <regex>
#include <algorithm>
#include <mutex>
#include <filesystem>
#include <fstream>
#include <iostream>

std::mutex mtx2;

void ConverterJSON::NormalizeSpaсesToLower(const std::string fName, const int index) {
    try {
        std::ifstream txtFile(fName);
        if (!txtFile.is_open()) {
            throw std::runtime_error("Could not open file " + fName + "\n");
        }
        std::string text;
        std::getline(txtFile, text);
        txtFile.close();
        std::string finText = std::regex_replace(text, std::regex(" {2,}"), " ");
        std::transform(finText.begin(), finText.end(), finText.begin(), ::tolower);
        mtx2.lock();
        textDoc[index] = finText;
        mtx2.unlock();
    }
    catch (const std::exception & ex) {
        std::cout << ex.what() << std::endl;
    }
}

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    nlohmann::json config;
    try {
        auto fpath  = std::filesystem::absolute("./../../config/config.json");
        std::ifstream configFile(fpath);
        configFile >> config;
        configFile.close();
    }
    catch (const std::exception & ex) {
        std::cout << "config file is missing" << std::endl;
        exit(1);
    }

    try {
        if (config.find("config") == config.end()) {
            throw std::runtime_error("config file is empty");
        }
    }
    catch (const std::runtime_error &rex) {
        std::cout << rex.what() << std::endl;
        exit(2);
    }

    std::vector<std::thread> threads;
    textDoc.clear();
    const int threadSize = config["files"].size();
    textDoc.resize(threadSize);

    for (int i = 0; i < threadSize; i++) {
        const std::string fileName = config["files"][i];
        threads.emplace_back([this, fName = fileName, index = i](){this->NormalizeSpaсesToLower(fName, index);});
    }

    for (auto& thread : threads) { thread.join(); }

    const std::string searchEngineName = config["config"]["name"];
    std::cout << searchEngineName + " started!"<< std::endl;

    return textDoc;
}

int ConverterJSON::GetResponsesLimit() const {
    int max_resp = 0;
    auto fpath  = std::filesystem::absolute("./../../config/config.json");
    std::ifstream configFile(fpath);
    nlohmann::json config;
    configFile >> config;
    configFile.close();

    if (config.find("config") != config.end()) {
        max_resp = config["config"]["max_responses"].get<int>();
    }

    return max_resp;
}

std::vector<std::string> ConverterJSON::GetRequests() {
    std::vector<std::string> requests;
    auto fpath  = std::filesystem::absolute("./../../config/requests.json");
    std::ifstream requestFile(fpath);
    nlohmann::json req;
    requestFile >> req;
    requestFile.close();

    for (const auto& it : req["requests"]) {
        // std::string txt;
        // std::getline(it, txt);
        requests.emplace_back(it);
    }

    return requests;
}

void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float> > > answers) {
    auto fpath  = std::filesystem::absolute("./../../config/answers.json");
    std::ofstream answerFile(fpath);
    nlohmann::ordered_json answer;
    nlohmann::ordered_json request;

    for (int i = 0; i < answers.size(); i++) {
        std::string num;
        if (i < 10) num = "00";
        if (i > 10 && i < 100) num = "0";
        if (!answers[i].empty()) {
            nlohmann::ordered_json data;
            data["result"] = "true";
            for (auto & j : answers[i]) {
                nlohmann::ordered_json pair;
                pair["docid"] = j.first;
                pair["ranc"] = std::stod(std::to_string(j.second));
                data["relevance"].emplace_back(pair);
            }
            request["request" + num + std::to_string(i + 1)] = data;
        }
        if (answers[i].empty()) {
            nlohmann::ordered_json data;
            data["result"] = "false";
            request["request" + num + std::to_string(i + 1)] = data;
        }

        answer["answers"] = request;
    }

    answerFile << answer.dump(4);
    answerFile.close();
}