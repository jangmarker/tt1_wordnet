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

    std::string partOfSpeech2;
    unsigned int offset2;

    auto* query = app.add_subcommand("query");
    query->add_set("--pos", partOfSpeech, partOfSpeechOptions, "Part of speech")->required();
    query->add_option("--iw", indexWord, "Index word")->required();
    auto full = query->add_flag("--full", "Show index words for all synsets");

    auto* semrels = app.add_subcommand("semrels");
    semrels->add_set("--pos", partOfSpeech, partOfSpeechOptions, "Part of speech")->required();
    semrels->add_option("--offset", offset, "Offset")->required();
    semrels->add_set("--dir", relationDirection, {"in", "out", "any"}, "Relation direction")->required();
    auto semrelsFull = semrels->add_flag("--full", "Show index words for all connected synsets");

    auto* shortestpath = app.add_subcommand("shortestpath");
    shortestpath->add_set("--pos1", partOfSpeech, partOfSpeechOptions, "Part of speech")->required();
    shortestpath->add_option("--offset1", offset, "Offset")->required();
    shortestpath->add_set("--pos2", partOfSpeech2, partOfSpeechOptions, "Target part of speech")->required();
    shortestpath->add_option("--offset2", offset2, "Target offset")->required();
    auto shortestpathDirected = shortestpath->add_flag("--directed", "Only search directed");
    auto shortestpathFull = shortestpath->add_flag("--full", "Show index words for all connected synsets");

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
                    std::cout << synset.words << std::endl;
                }
            }
        }
    } else if (*semrels) {
        const auto synset = database.synsetByOffset(partOfSpeech, offset);

        const auto is_semantic = [](const SynsetPointer &pointer) { return pointer.sourceTarget == 0; };

        Database::Direction direction = relationDirection == "any" ? Database::Both :
                                        relationDirection == "in" ? Database::Incoming : Database::Outgoing;
        SynsetIdentifier originIdentifier = std::make_pair(synset->type, synset->offset);
        std::vector<OtherSynsetIdAndPointer> connected = database.connectedSynsets(originIdentifier, direction);

        for (const OtherSynsetIdAndPointer &otherSynsetIdAndPointer : connected) {
            if (is_semantic(*otherSynsetIdAndPointer.second)) {
                Synset *otherSynset = database.synsetByIdentifier(otherSynsetIdAndPointer.first);
                std::cout << *(otherSynsetIdAndPointer.second) << " " << *otherSynset << std::endl;
                if (*semrelsFull)
                    std::cout << otherSynset->words << std::endl;
            }
        }
    } else if (*shortestpath) {
        SynsetIdentifier origin = std::make_pair(partOfSpeech[0], offset);
        SynsetIdentifier target = std::make_pair(partOfSpeech2[0], offset2);

        std::vector<OtherSynsetIdAndPointer> shortestPath = database.shortestPath(origin, target, *shortestpathDirected);
        SynsetPointer *lastEdge = nullptr;
        for (OtherSynsetIdAndPointer id : shortestPath) {
            Synset *synset = database.synsetByIdentifier(id.first);
            if (lastEdge)
                std::cout << *(lastEdge) << " ";
            std::cout << *(synset) << std::endl;
            lastEdge = id.second;
            if (*shortestpathFull)
                std::cout << synset->words << std::endl;
        }
    } else {
        // TODO task 4 is not implemented.. two days is not enough time :-(
    }

    return 0;
}