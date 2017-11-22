#ifndef TT1_WORDNET_OUTPUT_H_H
#define TT1_WORDNET_OUTPUT_H_H

#include "wordnet.h"

#include <fstream>

std::ostream &operator<<(std::ostream &stream, const Synset &synset);
std::ostream &operator<<(std::ostream &stream, const SynsetPointer &synsetPointer);
std::ostream &operator<<(std::ostream &stream, const std::vector<std::string>& words);

#endif //TT1_WORDNET_OUTPUT_H_H
