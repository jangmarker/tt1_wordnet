#include "Database.h"

#include <algorithm>
#include <iostream>

Database::Database(FileAccess &files)
    : mFiles(files)
{
    loadSynsetsForPos('n');
    loadSynsetsForPos('a');
    loadSynsetsForPos('v');
    loadSynsetsForPos('r');
}

std::set<OtherSynsetIdAndPointer> Database::synsetsPointingAt(Synset *synset) {
    return mSynsetByPointingAt[synset->id()];
}

Synset *Database::synsetByIdentifier(SynsetIdentifier identifier) {
    return mSynsetsByIndex[identifier.first][identifier.second];
}

std::vector<Synset> Database::synsetsByIndexWord(PartOfSpeech pos, std::string index_word)
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
        synsets.push_back(*synsetByIdentifier(std::make_pair(pos, offset)));
    }

    return synsets;
}

std::vector<OtherSynsetIdAndPointer> Database::connectedSynsets(SynsetIdentifier ownId, Database::Direction direction)
{
    std::vector<OtherSynsetIdAndPointer> connected;

    Synset *self = synsetByIdentifier(ownId);

    if (direction & Database::Outgoing) {
        for (SynsetPointer &synsetPointer : self->pointers)
            connected.emplace_back(synsetPointer.pointedToId(), &synsetPointer);
    }

    if (direction & Database::Incoming) {
        std::set<OtherSynsetIdAndPointer> synsetsPointingAtSelf = synsetsPointingAt(self);
        connected.reserve(connected.size() + synsetsPointingAtSelf.size());
        std::copy(synsetsPointingAtSelf.begin(), synsetsPointingAtSelf.end(), std::back_inserter(connected));
    }

    return connected;
};

std::vector<OtherSynsetIdAndPointer> Database::shortestPath(SynsetIdentifier origin, SynsetIdentifier target, bool directed)
{
    const OtherSynsetIdAndPointer nullId = std::make_pair(std::make_pair('0', 0), nullptr);
    std::map<SynsetIdentifier, int> distance;
    std::map<SynsetIdentifier, OtherSynsetIdAndPointer> previous;

    const auto smallestOnTop = [&distance](const SynsetIdentifier& a, const SynsetIdentifier& b) -> bool {
        if (distance[a] == distance[b]) {
            // to assure absolute order
            if (a.second == b.second) {
                return a.first < b.first;
            }
            return a.second < b.second;
        }
        return distance[a] < distance[b];
    };
    std::set<SynsetIdentifier, decltype(smallestOnTop)> nodes(smallestOnTop);
    for (auto& db : mSynsets) {
        for (Synset& synset : db.second) {
            SynsetIdentifier id = std::make_pair(db.first, synset.offset);
            distance[id] = (id == origin) ? 0 : std::numeric_limits<int>::max();
            previous[id] = nullId;
            nodes.insert(id);
        }
    }

    while (!nodes.empty()) {
        SynsetIdentifier current = *nodes.begin();
        nodes.erase(current);
        Direction direction = directed ? Outgoing : Both;
        for (const OtherSynsetIdAndPointer& neighborAndEdge : connectedSynsets(current, direction)) {
            SynsetIdentifier neighbor = neighborAndEdge.first;
            if (nodes.find(neighbor) != nodes.end()) {
                auto new_distance = distance[current] == std::numeric_limits<int>::max() ? distance[current]
                                                                                         : (distance[current] + 1);
                if (new_distance < distance[neighbor]) {
                    distance[neighbor] = new_distance;
                    previous[neighbor] = std::make_pair(current, neighborAndEdge.second);
                    nodes.erase(neighbor);
                    nodes.insert(neighbor);
                }
            }
            if (current == target) {
                std::vector<OtherSynsetIdAndPointer> path;
                path.insert(path.cbegin(), std::make_pair(current, nullptr));
                while (previous[current] != nullId) {
                    path.insert(path.cbegin(), previous[current]);
                    current = previous[current].first;
                }
                return path;
            }
        }
    }
}

void Database::loadSynsetsForPos(PartOfSpeech pos)
{
    auto &database = mFiles.dataFileForPos(pos);

    database >> mSynsets[pos];

    for (Synset &synset : mSynsets[pos]) {
        mSynsetsByIndex[pos].insert(std::make_pair(synset.offset, &synset));

        SynsetIdentifier synsetIdentifier = std::make_pair(pos, synset.offset);
        for (SynsetPointer &pointer : synset.pointers) {
            SynsetIdentifier targetIdentifier = pointer.pointedToId();
            mSynsetByPointingAt[targetIdentifier].insert(std::make_pair(synsetIdentifier, &pointer));
        }
    }
}
