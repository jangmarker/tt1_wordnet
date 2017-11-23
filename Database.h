#ifndef TT1_WORDNET_DATABASE_H
#define TT1_WORDNET_DATABASE_H


#include <set>
#include "FileAccess.h"

struct DijkstraResult {
    std::map<SynsetIdentifier, int> distance;
    std::map<SynsetIdentifier, SynsetConnection> previous;
};

class Database {
public:
    explicit Database(FileAccess &files);

    enum Direction { Incoming = 0x1, Outgoing = 0x2 , Both = Incoming | Outgoing};

public:
    std::set<SynsetConnection> synsetsPointingAt(Synset *synset);
    Synset *synsetByIdentifier(SynsetIdentifier identifier);
    std::vector<Synset> synsetsByIndexWord(PartOfSpeech pos, std::string index_word);

    std::vector<SynsetConnection> connectedSynsets(SynsetIdentifier ownId, Direction = Both);
    std::vector<SynsetConnection> shortestPath(SynsetIdentifier origin, SynsetIdentifier target, bool directed = false);
    std::pair<SynsetIdentifier, int> eccentricity(SynsetIdentifier origin, bool directed);
    DijkstraResult dijkstra(SynsetIdentifier origin, SynsetIdentifier target, bool directed = false);

private:
    void loadSynsetsForPos(PartOfSpeech pos);

private:
    FileAccess &mFiles;

    // data
    std::map<char, SynsetDatabase> mSynsets;

    // index to query by Part Of Speech, then by offset
    std::map<char, std::map<SynsetOffset, Synset*>> mSynsetsByIndex;
    // index to query Synsets that point at a synset (incoming connections)
    std::map<SynsetIdentifier, std::set<SynsetConnection>> mSynsetByPointingAt;
};


#endif //TT1_WORDNET_DATABASE_H
