#ifndef TT1_WORDNET_WORDNET_H_H
#define TT1_WORDNET_WORDNET_H_H

#include <string>
#include <vector>
#include <map>

using SynsetOffset = unsigned int;

enum POS {
    Pos_Noun,
    Pos_Verb,
    Pos_Adjective,
    Pos_Adverb
};

static const std::map<std::string, POS> pos_map{
        {"n", POS::Pos_Noun},
        {"v", POS::Pos_Verb},
        {"a", POS::Pos_Adjective},
        {"r", POS::Pos_Adverb},
};

using SynsetOffsets = std::vector<SynsetOffset>;

struct LemmaIndex {
    std::string lemma;
    POS pos;
    SynsetOffsets synsets;
};

struct SynsetPointer {
    enum PointerType {
        Unkown // TODO
    };

    PointerType type;
    SynsetOffset offset;
    POS pos;
};

struct Synset {
    enum SynsetType {
        Noun,
        Verb,
        Adjective,
        AdjectiveSatellite,
        Adverb
    };

    SynsetOffset offset;
    int lexFileNumber;
    SynsetType type;
    std::vector<std::string> words;
    std::vector<SynsetPointer> pointers;
    std::string gloss;
};

static const std::map<std::string, Synset::SynsetType> synsettype_map{
        {"n", Synset::Noun},
        {"v", Synset::Verb},
        {"a", Synset::Adjective},
        {"s", Synset::AdjectiveSatellite},
        {"r", Synset::Adverb},
};

#endif //TT1_WORDNET_WORDNET_H_H
