#include "FileAccess.h"

#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>


namespace {
    static const std::map<PartOfSpeech, std::string> posToSuffix{
            {'n', "noun"},
            {'a', "adj"},
            {'r', "adv"},
            {'v', "verb"}
    };
}

FileAccess::FileAccess(std::string folder)
    : mFolder(std::move(folder))
{

}

std::istream &FileAccess::indexFileForPos(PartOfSpeech pos)
{
    const auto fileName = std::string("index.") + posToSuffix.at(pos);
    return fromCache(fileName);
}

std::istream &FileAccess::dataFileForPos(PartOfSpeech pos)
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
