#include "converterjson.h"
#include <io.h>

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::vector<std::string> textDocuments;
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

    for (const auto& it : config["files"]) {
        const std::string fileName = it;
        try {
            std::ifstream txtFile(fileName);
            if (!txtFile.is_open()) {
                throw std::runtime_error("Could not open file " + fileName);
            }
            std::string text;
            std::getline(txtFile, text);
            textDocuments.emplace_back(text);
            txtFile.close();
        }
        catch (const std::exception & ex) {
            std::cout << ex.what() << std::endl;
        }

    }
    const std::string searchEngineName = config["config"]["name"];
    std::cout << searchEngineName + " started!"<< std::endl;
    return textDocuments;
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
    std::ofstream answerFile("answers.json");
    nlohmann::ordered_json answer;
    nlohmann::ordered_json request;

    for (int i = 0; i < answers.size(); i++) {
        std::string num;
        if (i < 10) num = "00";
        if (i > 10 && i < 100) num = "0";
        if (!answers[i].empty()) {
            nlohmann::ordered_json data;
//            nlohmann::json pair;
            data["result"] = "true";
//            for (const auto& it : answers[i]) {
            for ( int j = 0; j < answers[i].size(); j++ ) {
                nlohmann::ordered_json pair;
                pair["docid"] = answers[i][j].first;
                pair["ranc"] = std::stod(std::to_string(answers[i][j].second));
//                std::string text;
//                text = "docid: " + std::to_string(it.first) + ", ranc: " + std::to_string(it.second);
//                data["relevance"].emplace_back(text);
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