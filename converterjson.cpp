#include "converterjson.h"

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::vector<std::string> textDocuments;
    std::ifstream configFile("config.json");
    nlohmann::json config;
    configFile >> config;
    configFile.close();

    if (config.find("config") != config.end()) {
        for (const auto& it : config["files"]) {
            const std::string fileName = it;
            std::ifstream txtFile(fileName);
            std::string text;
            std::getline(txtFile, text);
            textDocuments.emplace_back(text);
        }
    }
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
    answerFile << '{' << '\n' << "\t\"answers\": {\n";
    int answer_index = 1;
    for (const auto& it : answers) {
        if (!it.empty()) {
            answerFile <<  "\t\t\"request" + std::to_string(answer_index++) + "\": {\n";
            answerFile <<  "\t\t\t\"result\": " << "\"true\"" <<",\n ";
            answerFile <<  "\t\t\t\"relevance\": [\n";
//            std::pair<int, float> el;
            for (auto elem = it.begin(); elem != it.end() - 1; ++elem ) {
                answerFile << "\t\t\t\t[" << elem->first << ", " << elem->second << "]," << std::endl;
            }
            answerFile << "\t\t\t\t[" << it.rbegin()->first << ", " << it.rbegin()->second << "]" << std::endl;
            answerFile << "\t\t\t]\n";
        }
        else {
            answerFile <<  "\t\t\"request" + std::to_string(answer_index) + "\": {\n";
            answerFile <<  "\t\t\t\"result\": " << "\"false\"" <<",\n }";
        }
        answerFile << "\t\t}\n";
        answerFile << "\t}\n";
    }
    answerFile << '}' << std::endl;
    answerFile.close();
}