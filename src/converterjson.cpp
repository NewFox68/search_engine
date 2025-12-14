#include "converterjson.h"
#include <io.h>
#include <regex>
#include <algorithm>
#include <mutex>
#include <filesystem>
#include <fstream>
#include <iostream>

std::mutex mtx2;

void ConverterJSON::NormalizeSpaсesToLower(const std::string fName, int index) {
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
        textDoc.insert(textDoc.begin() + index, finText);
        mtx2.unlock();
    }
    catch (const std::exception & ex) {
        std::cout << ex.what() << std::endl;
    }
}

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    nlohmann::json config;
    try {
        std::ifstream configFile("config.json");
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
    textDoc.resize(config["files"].size());
    threads.resize(config["files"].size());

    for (int i = 0; i < config["files"].size(); i++) {
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
    std::ifstream configFile("config.json");
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
    std::ifstream requestFile("requests.json");
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
    std::string fileName = "..\\config\\answers.json";
//    std::filesystem::path fpath{"config/answers.json"};
//    std::cout << fpath << std::endl;
    std::ofstream answerFile(fileName);
    nlohmann::ordered_json answer;
    nlohmann::ordered_json request;

    for (int i = 0; i < answers.size(); i++) {
        std::string num;
        if (i < 10) num = "00";
        if (i > 10 && i < 100) num = "0";
        if (!answers[i].empty()) {
            nlohmann::ordered_json data;
            data["result"] = "true";
            for ( int j = 0; j < answers[i].size(); j++ ) {
                nlohmann::ordered_json pair;
                pair["docid"] = answers[i][j].first;
                pair["ranc"] = std::stod(std::to_string(answers[i][j].second));
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