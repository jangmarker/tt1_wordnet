#include "FileAccess.h"
#include "Database.h"

#include <iostream>

static const std::string databaseFolder("/home/privat/Projects/WordNet-3.0/dict/");

int main() {
    std::string pos("a");
    std::string indexWord("sweet");
    bool full = true;

    FileAccess fileAccess(databaseFolder);
    Database database(fileAccess);

    const auto synsets = database.synsetsByIndexWord(pos, indexWord);


    if (synsets.empty()) {
        std::cout << "Could not find" << indexWord << std::endl;
    } else {
        for (const auto &synset : synsets) {
            // TODO format offset and type
            std::cout << synset.type << " " << synset.offset << synset.gloss << std::endl;
        }
    }


    return 0;
}