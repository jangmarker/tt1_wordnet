#include "FileAccess.h"
#include "Database.h"

#include "3rdparty/CLI11.hpp"

#include <iostream>
#include <iomanip>

static const std::string databaseFolder("/home/privat/Projects/WordNet-3.0/dict/");

int main(int argc, char** argv) {
    CLI::App app{"Text Technologie 1 - Aufgabe 2 - Lösung Jan Marker"};

    std::set<std::string> partOfSpeechOptions{"n", "a", "v", "r"};

    std::string partOfSpeech;
    std::string indexWord;
    std::string relationDirection;
    unsigned int offset;

    auto* query = app.add_subcommand("query");
    query->add_set("--pos", partOfSpeech, partOfSpeechOptions, "Part of speech")->required();
    query->add_option("--iw", indexWord, "Index word")->required();
    auto full = query->add_flag("--full", "Show index words for all synsets");

    auto* semrels = app.add_subcommand("semrels");
    semrels->add_set("--pos", partOfSpeech, partOfSpeechOptions, "Part of speech")->required();
    semrels->add_option("--offset", offset, "Offset")->required();
    semrels->add_set("--dir", relationDirection, {"in", "out", "any"}, "Relation direction")->required();
    auto semrelsFull = semrels->add_flag("--full", "Show index words for all connected synsets");

    CLI11_PARSE(app, argc, argv);

    FileAccess fileAccess(databaseFolder);
    Database database(fileAccess);

    if (*query) {
        const auto synsets = database.synsetsByIndexWord(partOfSpeech, indexWord);

        if (synsets.empty()) {
            std::cout << "Could not find " << indexWord << std::endl;
        } else {
            for (const auto &synset : synsets) {
                std::cout << synset << std::endl;
                if (*full) {
                    for (auto &word : synset.words) {
                        // TODO output word type
                        std::cout << '\t' << word << std::endl;
                    }
                }
            }
        }
    } else if (*semrels) {
        const auto synset = database.synsetByOffset(partOfSpeech, offset);

        const auto is_semantic = [](const SynsetPointer &pointer) { return pointer.sourceTarget == 0; };

        if (relationDirection != "in") {
            std::vector<SynsetPointer> outgoing;
            std::copy_if(synset.pointers.begin(), synset.pointers.end(), std::back_inserter(outgoing), is_semantic);
            for (SynsetPointer &synsetPointer : outgoing) {
                const Synset &pointedAtSynset = database.synsetByOffset(pos_to_str[synsetPointer.pos], synsetPointer.offset);

                std::cout << synsetPointer << " " << pointedAtSynset << std::endl;
            }
        }

        if (relationDirection != "out") {
            std::set<std::pair<SynsetIdentifier, SynsetPointer *>> incomingSynsets = database.synsetsPointingAt(synset);
            for (auto &incomingSynset : incomingSynsets) {
                if (is_semantic(*incomingSynset.second)) {
                    // TODO eliminate duplicates in any
                    const Synset &originSynset = database.synsetByIdentifier(incomingSynset.first);
                    std::cout << (*incomingSynset.second) << " " << originSynset << std::endl;
                }
            }
        }

    }

    return 0;
}