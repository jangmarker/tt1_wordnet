#include "Database.h"

#include "searchable_queue.h"

#include <algorithm>
#include <iostream>

Database::Database(FileAccess &files)
    : mFiles(files)
{
    loadSynsetsForPos("n");
    loadSynsetsForPos("a");
    loadSynsetsForPos("v");
    loadSynsetsForPos("r");
}

std::set<OtherSynsetIdAndPointer> Database::synsetsPointingAt(const Synset &synset) {
    SynsetIdentifier identifier = std::make_pair(pos_to_str[synset.type][0], synset.offset);
    return mSynsetByPointingAt[identifier];
}

Synset *Database::synsetByIdentifier(SynsetIdentifier identifier) {
    return mSynsetsByIndex[identifier.first][identifier.second];
}

Synset *Database::synsetByOffset(const std::string &pos, SynsetOffset offset)
{
    auto *synset = mSynsetsByIndex[pos[0]][offset];
    return synset;
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
        synsets.push_back(*synsetByOffset(pos, offset));
    }

    return synsets;
}

std::vector<OtherSynsetIdAndPointer> Database::connectedSynsets(SynsetIdentifier ownId, Database::Direction direction)
{
    std::vector<OtherSynsetIdAndPointer> connected;

    Synset *self = synsetByIdentifier(ownId);

    if (direction & Database::Outgoing) {
        for (SynsetPointer &synsetPointer : self->pointers)
            connected.emplace_back(std::make_pair(pos_to_str[synsetPointer.pos][0], synsetPointer.offset), &synsetPointer);
    }

    if (direction & Database::Incoming) {
        std::set<OtherSynsetIdAndPointer> synsetsPointingAtSelf = synsetsPointingAt(*self);
        connected.reserve(connected.size() + synsetsPointingAtSelf.size());
        std::copy(synsetsPointingAtSelf.begin(), synsetsPointingAtSelf.end(), std::back_inserter(connected));
    }

    return connected;
};

std::ostream &operator<<(std::ostream &stream, const SynsetIdentifier &id)
{
    stream << id.first << " " << id.second;
}

std::vector<SynsetIdentifier> Database::shortestPath(SynsetIdentifier origin, SynsetIdentifier target)
{
    const SynsetIdentifier nullId = std::make_pair('0', 0);
    std::map<SynsetIdentifier, int> distance;
    std::map<SynsetIdentifier, SynsetIdentifier> previous;

    const auto smallestOnTop = [&distance](const SynsetIdentifier& a, const SynsetIdentifier& b) -> bool {
        return distance[a] > distance[b];
    };
    searchable_queue<SynsetIdentifier, std::vector<SynsetIdentifier>, decltype(smallestOnTop)> nodes(smallestOnTop);
    for (auto& db : mSynsets) {
        for (Synset& synset : db.second) {
            SynsetIdentifier id = std::make_pair(db.first, synset.offset);
            distance[id] = id == origin ? 0 : std::numeric_limits<int>::max();
            previous[id] = nullId;
            nodes.push(id);
        }
    }

    std::cout << "target " << target << std::endl;
    while (!nodes.empty()) {
        SynsetIdentifier current = nodes.top();
        nodes.pop();
        std::cout << "current " << current << " " << distance[current] << std::endl;
        // TODO directed/undirected
        for (const OtherSynsetIdAndPointer& neighborAndEdge : connectedSynsets(current)) {
            SynsetIdentifier neighbor = neighborAndEdge.first;
            if (nodes.find(neighbor) != nodes.cend()) {
                auto new_distance = distance[current] + 1;
                if (new_distance < distance[neighbor]) {
                    distance[neighbor] = new_distance;
                    previous[neighbor] = current;
                    nodes.update();
                }
            }
            if (current == target) {
                std::vector<SynsetIdentifier> path;
                path.insert(path.cbegin(), current);
                while (previous[current] != nullId) {
                    current = previous[current];
                    path.insert(path.cbegin(), current);
                }
                return path;
            }
        }
    }
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
            mSynsetByPointingAt[targetIdentifier].insert(std::make_pair(synsetIdentifier, &pointer));
        }
    }
}
