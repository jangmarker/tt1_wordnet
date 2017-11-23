#include "Database.h"
#include "Input.h"

#include <algorithm>
#include <iostream>

namespace {
    const SynsetConnection nullId{std::make_pair('0', 0), nullptr};
}

Database::Database(FileAccess &files)
    : mFiles(files)
{
    loadSynsetsForPos('n');
    loadSynsetsForPos('a');
    loadSynsetsForPos('v');
    loadSynsetsForPos('r');
}

std::set<SynsetConnection> Database::synsetsPointingAt(Synset *synset) {
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

std::vector<SynsetConnection> Database::connectedSynsets(SynsetIdentifier ownId, Database::Direction direction)
{
    std::vector<SynsetConnection> connected;

    Synset *self = synsetByIdentifier(ownId);

    if (direction & Database::Outgoing) {
        for (SynsetPointer &synsetPointer : self->pointers)
            connected.emplace_back(synsetPointer.pointedToId(), &synsetPointer);
    }

    if (direction & Database::Incoming) {
        std::set<SynsetConnection> synsetsPointingAtSelf = synsetsPointingAt(self);
        connected.reserve(connected.size() + synsetsPointingAtSelf.size());
        std::copy(synsetsPointingAtSelf.begin(), synsetsPointingAtSelf.end(), std::back_inserter(connected));
    }

    return connected;
};

std::vector<SynsetConnection> Database::shortestPath(SynsetIdentifier origin, SynsetIdentifier target, bool directed)
{
    auto res = dijkstra(origin, target, directed);

    SynsetIdentifier current = target;

    std::vector<SynsetConnection> path;
    path.insert(path.cbegin(), SynsetConnection{current, nullptr});
    while (res.previous[current] != nullId) {
        path.insert(path.cbegin(), res.previous[current]);
        current = res.previous[current].otherId;
    }
    return path;
}

DijkstraResult Database::dijkstra(SynsetIdentifier origin, SynsetIdentifier target, bool directed)
{
    std::map<SynsetIdentifier, int> distance;
    std::map<SynsetIdentifier, SynsetConnection> previous;

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
        for (const SynsetConnection& neighborAndEdge : connectedSynsets(current, direction)) {
            SynsetIdentifier neighbor = neighborAndEdge.otherId;
            if (nodes.find(neighbor) != nodes.end()) {
                auto new_distance = distance[current] == std::numeric_limits<int>::max() ? distance[current]
                                                                                         : (distance[current] + 1);
                if (new_distance < distance[neighbor]) {
                    distance[neighbor] = new_distance;
                    previous[neighbor] = SynsetConnection{current, neighborAndEdge.pointer};
                    nodes.erase(neighbor);
                    nodes.insert(neighbor);
                }
            }
            if (current == target) {
                return {distance, previous};
            }
        }
    }

    return {distance, previous};
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
            mSynsetByPointingAt[targetIdentifier].insert({synsetIdentifier, &pointer});
        }
    }
}
