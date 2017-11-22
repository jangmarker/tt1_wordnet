#ifndef TT1_WORDNET_DATABASE_H
#define TT1_WORDNET_DATABASE_H


#include <set>
#include "FileAccess.h"

class Database {
public:
    explicit Database(FileAccess &files);

    enum Direction { Incoming = 0x1, Outgoing = 0x2 , Both = Incoming | Outgoing};

public:
    std::set<OtherSynsetIdAndPointer> synsetsPointingAt(Synset *synset);
    Synset *synsetByIdentifier(SynsetIdentifier identifier);
    std::vector<Synset> synsetsByIndexWord(PartOfSpeech pos, std::string index_word);

    std::vector<OtherSynsetIdAndPointer> connectedSynsets(SynsetIdentifier ownId, Direction = Both);
    std::vector<OtherSynsetIdAndPointer> shortestPath(SynsetIdentifier origin, SynsetIdentifier target, bool directed = false);

private:
    void loadSynsetsForPos(PartOfSpeech pos);

private:
    FileAccess &mFiles;

    std::map<char, SynsetDatabase> mSynsets;
    std::map<char, std::map<SynsetOffset, Synset*>> mSynsetsByIndex;
    std::map<SynsetIdentifier, std::set<OtherSynsetIdAndPointer>> mSynsetByPointingAt;
};


#endif //TT1_WORDNET_DATABASE_H
