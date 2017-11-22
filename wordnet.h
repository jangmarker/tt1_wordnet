#ifndef TT1_WORDNET_WORDNET_H_H
#define TT1_WORDNET_WORDNET_H_H

#include <string>
#include <vector>
#include <map>

using SynsetOffset = unsigned int;
using PartOfSpeech = char;

using SynsetOffsets = std::vector<SynsetOffset>;
using SynsetIdentifier = std::pair<char, SynsetOffset>;

struct LemmaIndexItem {
    std::string lemma;
    PartOfSpeech pos;
    SynsetOffsets synsets;
};

struct SynsetPointer {
    std::string type;
    SynsetOffset offset;
    PartOfSpeech pos;
    int sourceTarget;

    SynsetIdentifier pointedToId() const {
        return std::make_pair(pos, offset);
    }
};

using OtherSynsetIdAndPointer = std::pair<SynsetIdentifier, SynsetPointer*>;
using SynsetType = char;

struct Synset {
    SynsetOffset offset;
    int lexFileNumber;
    SynsetType type;
    std::vector<std::string> words;
    std::vector<SynsetPointer> pointers;
    std::string gloss;

    SynsetIdentifier id() const {
        return std::make_pair(type, offset);
    }
};

using SynsetDatabase = std::vector<Synset>;

#endif //TT1_WORDNET_WORDNET_H_H
