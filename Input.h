#ifndef TT1_WORDNET_INPUT_H
#define TT1_WORDNET_INPUT_H

#include "wordnet.h"

#include <fstream>

using LemmaIndex = std::vector<LemmaIndexItem>;

std::istream &operator>>(std::istream &stream, LemmaIndexItem &index);

Synset loadSynset(std::istream& data, SynsetOffset targetOffset);
std::istream &operator>>(std::istream &stream, SynsetPointer &pointer);
std::istream &operator>>(std::istream &stream, Synset &synset);

template<class LineItem>
std::istream &operator>>(std::istream &stream, std::vector<LineItem> &list)
{
    list.clear();

    std::string line;
    auto pos = stream.tellg();
    while (std::getline(stream, line)) {
        if (!line.empty() && line[0] != ' ') {
            stream.seekg(pos, std::ios_base::beg);
            break;
        }
        pos = stream.tellg();
    };

    while (stream.peek() != EOF) {
        LineItem item;
        stream >> item;
        list.push_back(item);
    }

    return stream;
}

#endif //TT1_WORDNET_INPUT_H
