#ifndef TT1_WORDNET_INDEX_H
#define TT1_WORDNET_INDEX_H

#include "wordnet.h"

using LemmaIndex = std::vector<LemmaIndexItem>;

std::istream &operator>>(std::istream &stream, LemmaIndexItem &index);
std::istream &operator>>(std::istream &stream, LemmaIndex &index);

Synset loadSynset(std::istream& data, SynsetOffset targetOffset);
std::istream &operator>>(std::istream &stream, SynsetPointer &pointer);
std::istream &operator>>(std::istream &stream, Synset &synset);

#endif //TT1_WORDNET_INDEX_H
