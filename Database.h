#ifndef TT1_WORDNET_DATABASE_H
#define TT1_WORDNET_DATABASE_H


#include <set>
#include "FileAccess.h"

using SynsetIdentifier = std::pair<char, SynsetOffset>;

class Database {
public:
    explicit Database(FileAccess &files);

public:
    Synset synsetByOffset(const std::string &pos, SynsetOffset offset);
    std::vector<Synset> synsetsByIndexWord(const std::string &pos, std::string index_word);

private:
    void loadSynsetsForPos(const std::string &pos);

private:
    FileAccess &mFiles;

    std::map<char, SynsetDatabase> mSynsets;
    std::map<char, std::map<SynsetOffset, Synset*>> mSynsetsByIndex;
    std::map<SynsetIdentifier, std::set<SynsetIdentifier>> mSynsetsBeingPointedAt;
};


#endif //TT1_WORDNET_DATABASE_H
