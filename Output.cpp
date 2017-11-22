#include "Output.h"

#include <ostream>
#include <iomanip>

namespace {
    static const std::map<std::string, std::string> pointerTypeToName{
            {"!", "Antonym"},
            {"#m", "Member holonym"},
            {"#p", "Part holonym"},
            {"#s", "Substance holonym"},
            {"$", "Verb Group"},
            {"%m", "Member meronym"},
            {"%p", "Part meronym"},
            {"%s", "Substance meronym"},
            {"&", "Similar to"},
            {"*", "Entailment"},
            {"+", "Derivationally related form"},
            {"-c", "Member of this domain - TOPIC"},
            {"-r", "Member of this domain - REGION"},
            {"-u", "Member of this domain - USAGE"},
            {";c", "Domain of synset - TOPIC"},
            {";r", "Domain of synset - REGION"},
            {";u", "Domain of synset - USAGE"},
            {"<", "Participle of verb"},
            {"=", "Attribute"},
            {">", "Cause"},
            {"@", "Hypernym"},
            {"@i", "Instance Hypernym"},
            {"\\_a", "Derived from adjective"}, // _<pointed at type>
            {"\\_n", "Pertainym (pertains to noun)"}, // _<pointed at type>
            {"^", "Also see"},
            {"~", "Hyponym"},
            {"~i", "Instance Hyponym"},
    };
};

std::ostream &operator<<(std::ostream &stream, const Synset &synset)
{
    return stream
            << synset.type << " "
            << std::setfill('0') << std::setw(8) << synset.offset << std::setw(0)
            << synset.gloss;
}

std::ostream &operator<<(std::ostream &stream, const SynsetPointer &synsetPointer)
{
    std::string lookupType = synsetPointer.type == "\\" ? (std::string("\\") + synsetPointer.type) : synsetPointer.type;
    return stream
            << synsetPointer.type
            << " "
            << "(" << pointerTypeToName.at(lookupType) << ")";
}

std::ostream &operator<<(std::ostream &stream, const std::vector<std::string> &words)
{
    for (auto &word : words) {
        // TODO output word type
        stream << '\t' << word << std::endl;
    }
    return stream;
}
