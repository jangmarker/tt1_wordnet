#ifndef TT1_WORDNET_INDEX_H
#define TT1_WORDNET_INDEX_H

#include "wordnet.h"

#include <fstream>

class FileAccess {

public:
    explicit FileAccess(std::string folder);

public:
    std::istream &indexFileForPos(PartOfSpeech pos);
    std::istream &dataFileForPos(PartOfSpeech pos);

private:
    std::istream &fromCache(std::string fileName);

private:
    std::string mFolder;

    std::map<std::string, std::ifstream> mStreamCache;
};

#endif //TT1_WORDNET_INDEX_H
