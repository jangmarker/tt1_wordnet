#ifndef TT1_WORDNET_INDEX_H
#define TT1_WORDNET_INDEX_H

#include "wordnet.h"

using Index = std::vector<LemmaIndex>;

Index loadIndex(std::istream& data);
Synset loadSynset(std::istream& data, SynsetOffset targetOffset);

#endif //TT1_WORDNET_INDEX_H
