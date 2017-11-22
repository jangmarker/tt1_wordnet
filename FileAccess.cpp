#include "FileAccess.h"

#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>

namespace {
    static const std::map<std::string, std::string> pointerTypeToName{
            {"!", "Antonym"},
            {"#m", "Member holonym"},
            {"#p", "Part holonym"},
            {"#s", "Substance holonym"},
            {"$", "Verb Group"},
            {"%m", "Member meronym"},
            {"%p", "Part meronym"},
            {"%s", "Substance meronym"},
            {"&", "Similar to"},
            {"*", "Entailment"},
            {"+", "Derivationally related form"},
            {"-c", "Member of this domain - TOPIC"},
            {"-r", "Member of this domain - REGION"},
            {"-u", "Member of this domain - USAGE"},
            {";c", "Domain of synset - TOPIC"},
            {";r", "Domain of synset - REGION"},
            {";u", "Domain of synset - USAGE"},
            {"<", "Participle of verb"},
            {"=", "Attribute"},
            {">", "Cause"},
            {"@", "Hypernym"},
            {"@i", "Instance Hypernym"},
            {"\\_a", "Derived from adjective"}, // _<pointed at type>
            {"\\_n", "Pertainym (pertains to noun)"}, // _<pointed at type>
            {"^", "Also see"},
            {"~", "Hyponym"},
            {"~i", "Instance Hyponym"},
    };
};

std::istream &operator>>(std::istream &stream, LemmaIndexItem &index)
{
    std::string lemma;
    char pos;
    int synsetCount;
    int _ptrCount;
    std::string _ptrSymbol;
    int _senseCount;
    int _tagSenseCount;
    SynsetOffset offset;

    std::string lineString;
    if (std::getline(stream, lineString)) {
        std::istringstream line(lineString);
        line >> lemma >> pos >> synsetCount >> _ptrCount;

        for (int i = 0; i < _ptrCount; ++i)
            line >> _ptrSymbol;

        line >> _senseCount >> _tagSenseCount;

        SynsetOffsets offsets;
        for (int i = 0; i < synsetCount; ++i) {
            line >> offset;
            offsets.push_back(offset);
        }

        index.lemma = lemma;
        index.pos = pos;
        index.synsets = offsets;
    }

    return stream;
}

Synset loadSynset(std::istream &data, SynsetOffset targetOffset) {
    Synset synset;

    data.seekg(targetOffset);
    data >> synset;

    if (synset.offset != targetOffset)
        throw "Offsets don't match";

    return synset;
}

std::istream &operator>>(std::istream &stream, SynsetPointer &pointer)
{
    std::string pointerSymbol;
    SynsetOffset synsetOffset;
    char pos;
    int sourceTarget;

    stream >> pointerSymbol >> synsetOffset >> pos >> std::hex >> sourceTarget >> std::dec;

    pointer.type = pointerSymbol;
    pointer.offset = synsetOffset;
    pointer.pos = pos;
    pointer.sourceTarget = sourceTarget;

    return stream;
}

std::istream &operator>>(std::istream &stream, Synset &synset)
{
    std::string line;
    std::getline(stream, line);

    if (line.empty())
        throw "Could not find offset";

    auto synsetLine = std::istringstream(line);

    SynsetOffset offset;
    int lexFileNumber;
    char type;
    unsigned long wordCount;
    std::string _word;
    int _lexId;
    std::vector<std::string> words;
    unsigned long pointerCount;
    std::vector<SynsetPointer> pointers;
    std::string gloss;

    synsetLine >> offset >> lexFileNumber >> type >> std::hex >> wordCount >> std::dec;

    words.reserve(wordCount);
    for (int i = 0; i < wordCount; ++i) {
        synsetLine >> _word >> std::hex >> _lexId >> std::dec;
        // TODO strip adjective's (...)
        words.push_back(_word);
    }

    synsetLine >> pointerCount;

    pointers.reserve(pointerCount);
    for (int i = 0; i < pointerCount; ++i) {
        SynsetPointer pointer;
        synsetLine >> pointer;
        pointers.push_back(pointer);
    }

    while (gloss != "|")
        synsetLine >> gloss;

    // TODO trim?
    std::getline(synsetLine, gloss);

    synset.offset = offset;
    synset.lexFileNumber = lexFileNumber;
    synset.type = type;
    synset.words = words;
    synset.pointers = pointers;
    synset.gloss = gloss;

    return stream;
}

std::ostream &operator<<(std::ostream &stream, const Synset &synset)
{
    return stream
           << synset.type << " "
           << std::setfill('0') << std::setw(8) << synset.offset << std::setw(0)
           << synset.gloss;
}

std::ostream &operator<<(std::ostream &stream, const SynsetPointer &synsetPointer)
{
    std::string lookupType = synsetPointer.type == "\\" ? (std::string("\\") + synsetPointer.type) : synsetPointer.type;
    return stream
           << synsetPointer.type
           << " "
           << "(" << pointerTypeToName.at(lookupType) << ")";
}

std::ostream &operator<<(std::ostream &stream, const std::vector<std::string> &words)
{
    for (auto &word : words) {
        // TODO output word type
        stream << '\t' << word << std::endl;
    }
    return stream;
}

namespace {
    static const std::map<std::string, std::string> posToSuffix{
            {"n", "noun"},
            {"a", "adj"},
            {"r", "adv"},
            {"v", "verb"}
    };
}

FileAccess::FileAccess(std::string folder)
    : mFolder(std::move(folder))
{

}

std::istream &FileAccess::indexFileForPos(const std::string &pos)
{
    const auto fileName = std::string("index.") + posToSuffix.at(pos);
    return fromCache(fileName);
}

std::istream &FileAccess::dataFileForPos(const std::string &pos)
{
    const auto fileName = std::string("data.") + posToSuffix.at(pos);
    return fromCache(fileName);
}

std::istream &FileAccess::fromCache(std::string fileName) {
    auto it = mStreamCache.find(fileName);
    if (it != mStreamCache.end()) {
        return it->second;
    }
    return mStreamCache.emplace(fileName, std::ifstream(mFolder + fileName)).first->second;
}
