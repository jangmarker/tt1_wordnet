#ifndef TT1_WORDNET_DATABASE_H
#define TT1_WORDNET_DATABASE_H


#include "FileAccess.h"

class Database {
public:
    explicit Database(FileAccess &files);

public:
    std::vector<Synset> synsetsByIndexWord(std::string pos, std::string index_word);

private:
    FileAccess &mFiles;
};


#endif //TT1_WORDNET_DATABASE_H
