#ifndef TT1_WORDNET_INDEX_H
#define TT1_WORDNET_INDEX_H

#include "wordnet.h"

using Index = std::vector<LemmaIndex>;

std::istream &operator>>(std::istream &stream, LemmaIndex &index);
std::istream &operator>>(std::istream &stream, Index &index);

Synset loadSynset(std::istream& data, SynsetOffset targetOffset);
std::istream &operator>>(std::istream &stream, SynsetPointer &pointer);
std::istream &operator>>(std::istream &stream, Synset &synset);

#endif //TT1_WORDNET_INDEX_H
