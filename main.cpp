#include "Output.h"
#include "FileAccess.h"
#include "Database.h"

#include "3rdparty/CLI11.hpp"

#include <iostream>
#include <iomanip>

int main(int argc, char** argv) {
    CLI::App app{"Text Technologie 1 - Aufgabe 2 - Lösung Jan Marker"};

    std::set<std::string> partOfSpeechOptions{"n", "a", "v", "r"};

    std::string databaseFolder("./");

    std::string partOfSpeech;
    std::string indexWord;
    std::string relationDirection;
    unsigned int offset;

    std::string partOfSpeech2;
    unsigned int offset2;

    app.add_option("--db", databaseFolder, "Path to the dict/ directory, where the WordNet database is located.", true);

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

    auto* eccentricity = app.add_subcommand("eccentricity");
    eccentricity->add_set("--pos", partOfSpeech, partOfSpeechOptions, "Part of speech")->required();
    eccentricity->add_option("--offset", offset, "Offset")->required();
    auto eccentricityDirected = eccentricity->add_flag("--directed", "Only search directed");

    CLI11_PARSE(app, argc, argv);

    FileAccess fileAccess(databaseFolder);
    Database database(fileAccess);

    if (*query) { // Task 1
        const auto synsets = database.synsetsByIndexWord(partOfSpeech[0], indexWord);

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
    } else if (*semrels) { // Task 2
        const auto origin = database.synsetByIdentifier(std::make_pair(partOfSpeech[0], offset));
        Database::Direction direction = relationDirection == "any" ? Database::Both :
                                        relationDirection == "in" ? Database::Incoming : Database::Outgoing;

        std::vector<SynsetConnection> connections = database.connectedSynsets(origin->id(), direction);
        for (const SynsetConnection &connection : connections) {
            if (connection.pointer->semantic()) {
                Synset *other = database.synsetByIdentifier(connection.otherId);
                std::cout << *(connection.pointer) << " " << *other << std::endl;
                if (*semrelsFull)
                    std::cout << other->words << std::endl;
            }
        }
    } else if (*shortestpath) { // Task 3
        SynsetIdentifier origin = std::make_pair(partOfSpeech[0], offset);
        SynsetIdentifier target = std::make_pair(partOfSpeech2[0], offset2);

        std::vector<SynsetConnection> shortestPath = database.shortestPath(origin, target, *shortestpathDirected);
        SynsetPointer *lastEdge = nullptr;
        for (SynsetConnection connection : shortestPath) {
            Synset *synset = database.synsetByIdentifier(connection.otherId);
            if (lastEdge)
                std::cout << *(lastEdge) << " ";
            std::cout << *(synset) << std::endl;
            lastEdge = connection.pointer;
            if (*shortestpathFull)
                std::cout << synset->words << std::endl;
        }
    } else { // Task 4
        SynsetIdentifier origin = std::make_pair(partOfSpeech[0], offset);

        auto res = database.eccentricity(origin, *eccentricityDirected);
        std::cout << res.second << std::endl;
    }

    return 0;
}