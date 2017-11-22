#include "Database.h"

#include <sstream>
#include <string>

namespace {
    LemmaIndex parseIndexLine(const std::string &line)
    {
        std::istringstream indexLine(line);

        std::string lemma;
        std::string pos;
        int synsetCount;
        int _ptrCount;
        char _ptrSymbol;
        int _senseCount;
        int _tagSenseCount;
        SynsetOffset offset;

        indexLine >> lemma >> pos >> synsetCount >> _ptrCount;

        for (int i = 0; i < _ptrCount; ++i)
            indexLine >> _ptrSymbol;

        indexLine >> _senseCount >> _tagSenseCount;

        SynsetOffsets offsets;
        for (int i = 0; i < synsetCount; ++i) {
            indexLine >> offset;
            offsets.push_back(offset);
        }

        return {lemma, pos_map.at(pos), offsets};
    }

    SynsetPointer parsePointer(std::istringstream &synsetLine)
    {
        std::string pointerSymbol;
        SynsetOffset synsetOffset;
        std::string pos;
        int sourceTarget;

        synsetLine >> pointerSymbol >> synsetOffset >> pos >> sourceTarget;

        return {SynsetPointer::Unkown, synsetOffset, pos_map.at(pos)};
    }
}

// TODO implement as operator>> overloads

Index loadIndex(std::istream &data)
{
    Index index;
    std::string line;
    while (std::getline(data, line)) {
        if (line.empty() || line[0] == ' ')
            continue;

        index.push_back(parseIndexLine(line));
    }

    return index;
}

Synset loadSynset(std::istream &data, SynsetOffset targetOffset) {
    data.seekg(targetOffset);
    std::string line;
    std::getline(data, line);

    if (line.empty())
        throw "Could not find offset";

    auto synsetLine = std::istringstream(line);
    SynsetOffset offset;
    int lexFileNumber;
    std::string type;
    unsigned long wordCount;
    std::string _word;
    int _lexId;
    std::vector<std::string> words;
    unsigned long pointerCount;
    std::vector<SynsetPointer> pointers;
    std::string gloss;

    synsetLine >> offset >> lexFileNumber >> type >> wordCount;

    if (offset != targetOffset)
        throw "Offsets don't match";

    words.resize(wordCount);
    for (int i = 0; i < wordCount; ++i) {
        synsetLine >> _word >> _lexId;
        // TODO strip adjective's (...)
        words.push_back(_word);
    }

    synsetLine >> pointerCount;

    pointers.resize(pointerCount);
    for (int i = 0; i > pointerCount; ++i) {
        pointers.push_back(parsePointer(synsetLine));
    }

    while (gloss != "|")
        synsetLine >> gloss;

    // TODO trim?
    std::getline(synsetLine, gloss);

    return {
        offset,
        lexFileNumber,
        synsettype_map.at(type),
        words,
        pointers,
        gloss
    };
}
