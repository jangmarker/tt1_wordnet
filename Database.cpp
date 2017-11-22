#include "Database.h"

#include <algorithm>

Database::Database(FileAccess &files)
    : mFiles(files)
{
    loadSynsetsForPos("n");
    loadSynsetsForPos("a");
    loadSynsetsForPos("v");
    loadSynsetsForPos("r");
}

Synset Database::synsetByOffset(const std::string &pos, SynsetOffset offset)
{
    auto *synset = mSynsetsByIndex[pos[0]][offset];
    return *synset;
}

std::vector<Synset> Database::synsetsByIndexWord(const std::string &pos, std::string index_word)
{
    // TODO create cache
    LemmaIndex index;
    mFiles.indexFileForPos(pos) >> index;

    auto indexItem = std::find_if(index.cbegin(), index.cend(), [&](const LemmaIndexItem& item) {
        return item.lemma == index_word;
    });

    std::vector<Synset> synsets;
    synsets.reserve(indexItem->synsets.size());
    for (const SynsetOffset offset : indexItem->synsets) {
        synsets.push_back(synsetByOffset(pos, offset));
    }

    return synsets;
}

void Database::loadSynsetsForPos(const std::string &pos)
{
    auto &database = mFiles.dataFileForPos(pos);

    database >> mSynsets[pos[0]];

    for (Synset &synset : mSynsets[pos[0]]) {
        mSynsetsByIndex[pos[0]].insert(std::make_pair(synset.offset, &synset));

        SynsetIdentifier synsetIdentifier = std::make_pair(pos[0], synset.offset);
        for (SynsetPointer &pointer : synset.pointers) {
            SynsetIdentifier targetIdentifier = std::make_pair(pos_to_str[pointer.pos][0], pointer.offset);
            mSynsetsBeingPointedAt[targetIdentifier].insert(synsetIdentifier);
        }
    }
}
