#ifndef TT1_WORDNET_INDEX_H
#define TT1_WORDNET_INDEX_H

#include "wordnet.h"

#include <fstream>

using LemmaIndex = std::vector<LemmaIndexItem>;

std::istream &operator>>(std::istream &stream, LemmaIndexItem &index);
std::istream &operator>>(std::istream &stream, LemmaIndex &index);

Synset loadSynset(std::istream& data, SynsetOffset targetOffset);
std::istream &operator>>(std::istream &stream, SynsetPointer &pointer);
std::istream &operator>>(std::istream &stream, Synset &synset);

class FileAccess {

public:
    explicit FileAccess(std::string folder);

public:
    std::istream &indexFileForPos(std::string pos);
    std::istream &dataFileForPos(std::string pos);

private:
    std::istream &fromCache(std::string fileName);

private:
    std::string mFolder;

    std::map<std::string, std::ifstream> mStreamCache;
};

#endif //TT1_WORDNET_INDEX_H
