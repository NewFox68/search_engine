#include "converterjson.h"
#include "invertedindex.h"
#include "searchserver.h"
#include <cmath>
#include <vector>

int main() {
    ConverterJSON converter;
    InvertedIndex index;
    index.UpdateDocumentBase(converter.GetTextDocuments());

    SearchServer server(index);

    auto vec = server.search(converter.GetRequests());

    std::vector<std::vector<std::pair<int, float> > > ans;

    for (const auto& it : vec) {
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

}