#include "FileAccess.h"
#include "Database.h"

#include "3rdparty/CLI11.hpp"

#include <iostream>
#include <iomanip>

static const std::string databaseFolder("/home/privat/Projects/WordNet-3.0/dict/");

int main(int argc, char** argv) {
    CLI::App app{"Text Technologie 1 - Aufgabe 2 - Lösung Jan Marker"};

    std::string partOfSpeech;
    std::string indexWord;

    auto* query = app.add_subcommand("query");

    query->add_set("--pos", partOfSpeech, {"n", "a", "v", "r"}, "Part of Speech")->required();
    query->add_option("--iw", indexWord, "Index word")->required();
    auto full = query->add_flag("--full", "Show index words for all synsets");

    CLI11_PARSE(app, argc, argv);

    if (*query) {
        FileAccess fileAccess(databaseFolder);
        Database database(fileAccess);

        const auto synsets = database.synsetsByIndexWord(partOfSpeech, indexWord);

        if (synsets.empty()) {
            std::cout << "Could not find " << indexWord << std::endl;
        } else {
            for (const auto &synset : synsets) {
                // TODO format offset and type
                std::cout << pos_to_str[synset.type] << " "
                          << std::setfill('0') << std::setw(8) << synset.offset << std::setw(0)
                          << synset.gloss
                          << std::endl;
            }
        }
    }


    return 0;
}