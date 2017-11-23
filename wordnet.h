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

    bool semantic() {
        return sourceTarget == 0;
    }
};

struct SynsetConnection {

    SynsetConnection() = default;

    SynsetConnection(const SynsetIdentifier &otherId, SynsetPointer *pointer)
        : otherId(otherId)
        , pointer(pointer)
    { }

    SynsetIdentifier otherId = std::make_pair('\0', 0);
    SynsetPointer *pointer = nullptr;

    bool operator==(const SynsetConnection &other) const {
        return other.otherId == otherId && other.pointer == pointer;
    }

    bool operator!=(const SynsetConnection &other) const {
        return !(*this == other);
    }

    bool operator<(const SynsetConnection &other) const {
        return this->otherId < other.otherId || this->pointer->offset < other.pointer->offset
                                             || this->pointer->pos < other.pointer->pos;
    }
};
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
