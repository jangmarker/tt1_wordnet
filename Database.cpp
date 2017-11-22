#include "Database.h"

#include <algorithm>

Database::Database(FileAccess &files)
    : mFiles(files)
{

}

std::vector<Synset> Database::synsetsByIndexWord(std::string pos, std::string index_word) {
    // TODO create cache
    LemmaIndex index;
    mFiles.indexFileForPos(pos) >> index;

    auto &database = mFiles.dataFileForPos(pos);

    auto indexItem = std::find_if(index.cbegin(), index.cend(), [&](const LemmaIndexItem& item) {
        return item.lemma == index_word;
    });

    std::vector<Synset> synsets;
    synsets.reserve(indexItem->synsets.size());
    for (const SynsetOffset offset : indexItem->synsets) {
        synsets.push_back(loadSynset(database, offset));
    }

    return synsets;
}
