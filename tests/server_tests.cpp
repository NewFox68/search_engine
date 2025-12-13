#include "converterjson.h"
#include "invertedindex.h"
#include "searchserver.h"
#include "gtest/gtest.h"
#include <cmath>

using namespace std;

TEST(TestCaseSearchServer, TestSimple) {
    const vector<string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuccino"
        };
    const vector<string> request = {"milk water", "sugar"};
    const std::vector<vector<RelativeIndex>> expected = {
        {
            {2, 1},
            {0, 0.7},
            {1, 0.3}
        },
        {
        }
    };
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    std::vector<vector<RelativeIndex>> result = srv.search(request);
    ASSERT_EQ(result, expected);
}

TEST(TestCaseSearchServer, TestTop5) {
    const vector<string> docs = {
        "london is the capital of great britain",
        "paris is the capital of france",
        "berlin is the capital of germany",
        "rome is the capital of italy",
        "madrid is the capital of spain",
        "lisboa is the capital of portugal",
        "bern is the capital of switzerland",
        "moscow is the capital of russia",
        "kiev is the capital of ukraine",
        "minsk is the capital of belarus",
        "astana is the capital of kazakhstan",
        "beijing is the capital of china",
        "tokyo is the capital of japan",
        "bangkok is the capital of thailand",
        "welcome to moscow the capital of russia the third rome",
        "amsterdam is the capital of netherlands",
        "helsinki is the capital of finland",
        "oslo is the capital of norway",
        "stockholm is the capital of sweden",
        "riga is the capital of latvia",
        "tallinn is the capital of estonia",
        "warsaw is the capital of poland",
    };
    const vector<string> request = {"moscow is the capital of russia"};
    const std::vector<vector<RelativeIndex>> expected = {
        {
            {7, 1},
            {14, 1},
            {0, 0.666666687},
            {1, 0.666666687},
            {2, 0.666666687}
        }
    };
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    std::vector<vector<RelativeIndex>> result = srv.search(request);
    ASSERT_EQ(result, expected);
}

int main() {
    ConverterJSON converter;
    InvertedIndex index;
    index.UpdateDocumentBase(converter.GetTextDocuments());

    SearchServer server(index);

    auto vec = server.search(converter.GetRequests());

    std::vector<std::vector<std::pair<int, float> > > ans;

    for (auto it : vec) {
        std::vector<std::pair<int, float>> tmp_ans;
        for (auto el : it) {
            std::pair<int, float>  p;
             p.first = static_cast<int>(el.doc_id);
             p.second = el.rank;
            tmp_ans.push_back(p);
        }
        ans.push_back(tmp_ans);
    }

    converter.putAnswers(ans);

    for (const auto& it : vec) {
        std::cout << "{" << std::endl;
        for (auto el : it) {
            std::cout << "{" << el.doc_id << ", " << el.rank << "}" << std::endl;
        }
        std::cout << "}" << std::endl;
    }
}