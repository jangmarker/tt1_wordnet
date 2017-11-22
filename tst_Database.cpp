#include "3rdparty/catch.h"

#include "Database.h"

#include <string>

const char *calculator("calculator n 2 3 @ ~ + 2 0 09887034 02938886  ");
const char *word("word n 10 6 @ ~ #p %p + - 10 6 06286395 06738162 06642138 07169242 07140659 07227301 13627327 09537144 06674188 06431740  ");
const char *licenseHeader = R"HEADER(
  1 This software and database is being provided to you, the LICENSEE, by
  2 Princeton University under the following license.  By obtaining, using
  3 and/or copying this software and database, you agree that you have
  4 read, understood, and will comply with these terms and conditions.:
  5
  6 Permission to use, copy, modify and distribute this software and
  7 database and its documentation for any purpose and without fee or
  8 royalty is hereby granted, provided that you agree to comply with
  9 the following copyright notice and statements, including the disclaimer,
  10 and that the same appear on ALL copies of the software, database and
  11 documentation, including modifications that you make for internal
  12 use or for distribution.
  13
  14 WordNet 3.0 Copyright 2006 by Princeton University.  All rights reserved.
  15
  16 THIS SOFTWARE AND DATABASE IS PROVIDED "AS IS" AND PRINCETON
  17 UNIVERSITY MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR
  18 IMPLIED.  BY WAY OF EXAMPLE, BUT NOT LIMITATION, PRINCETON
  19 UNIVERSITY MAKES NO REPRESENTATIONS OR WARRANTIES OF MERCHANT-
  20 ABILITY OR FITNESS FOR ANY PARTICULAR PURPOSE OR THAT THE USE
  21 OF THE LICENSED SOFTWARE, DATABASE OR DOCUMENTATION WILL NOT
  22 INFRINGE ANY THIRD PARTY PATENTS, COPYRIGHTS, TRADEMARKS OR
  23 OTHER RIGHTS.
  24
  25 The name of Princeton University or Princeton may not be used in
  26 advertising or publicity pertaining to distribution of the software
  27 and/or database.  Title to copyright in this software, database and
  28 any associated documentation shall at all times remain with
  29 Princeton University and LICENSEE agrees to preserve same.
)HEADER";

const char *oneSynsetNoHeader = "00000000 03 n 01 entity 0 003 ~ 00001930 n 0000 ~ 00002137 n 0000 ~ 04424418 n 0000 | that which is perceived or known or inferred to have its own distinct existence (living or nonliving)  ";
const char *oneSynsetWithHeader = \
"  1 This software and database is being provided to you, the LICENSEE, by  \n" \
"  2 Princeton University under the following license.  By obtaining, using  \n" \
"  3 and/or copying this software and database, you agree that you have  \n" \
"  4 read, understood, and will comply with these terms and conditions.:  \n" \
"  5   \n" \
"  6 Permission to use, copy, modify and distribute this software and  \n" \
"  7 database and its documentation for any purpose and without fee or  \n" \
"  8 royalty is hereby granted, provided that you agree to comply with  \n" \
"  9 the following copyright notice and statements, including the disclaimer,  \n" \
"  10 and that the same appear on ALL copies of the software, database and  \n" \
"  11 documentation, including modifications that you make for internal  \n" \
"  12 use or for distribution.  \n" \
"  13   \n" \
"  14 WordNet 3.0 Copyright 2006 by Princeton University.  All rights reserved.  \n" \
"  15   \n" \
"  16 THIS SOFTWARE AND DATABASE IS PROVIDED \"AS IS\" AND PRINCETON  \n" \
"  17 UNIVERSITY MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR  \n" \
"  18 IMPLIED.  BY WAY OF EXAMPLE, BUT NOT LIMITATION, PRINCETON  \n" \
"  19 UNIVERSITY MAKES NO REPRESENTATIONS OR WARRANTIES OF MERCHANT-  \n" \
"  20 ABILITY OR FITNESS FOR ANY PARTICULAR PURPOSE OR THAT THE USE  \n" \
"  21 OF THE LICENSED SOFTWARE, DATABASE OR DOCUMENTATION WILL NOT  \n" \
"  22 INFRINGE ANY THIRD PARTY PATENTS, COPYRIGHTS, TRADEMARKS OR  \n" \
"  23 OTHER RIGHTS.  \n" \
"  24   \n" \
"  25 The name of Princeton University or Princeton may not be used in  \n" \
"  26 advertising or publicity pertaining to distribution of the software  \n" \
"  27 and/or database.  Title to copyright in this software, database and  \n" \
"  28 any associated documentation shall at all times remain with  \n" \
"  29 Princeton University and LICENSEE agrees to preserve same.  \n" \
"00001740 03 n 01 entity 0 003 ~ 00001930 n 0000 ~ 00002137 n 0000 ~ 04424418 n 0000 | that which is perceived or known or inferred to have its own distinct existence (living or nonliving)  \n" \
"00001930 03 n 01 physical_entity 0 007 @ 00001740 n 0000 ~ 00002452 n 0000 ~ 00002684 n 0000 ~ 00007347 n 0000 ~ 00020827 n 0000 ~ 00029677 n 0000 ~ 14580597 n 0000 | an entity that has physical existence  \n" \
"00002137 03 n 02 abstraction 0 abstract_entity 0 010 @ 00001740 n 0000 + 00692329 v 0101 ~ 00023100 n 0000 ~ 00024264 n 0000 ~ 00031264 n 0000 ~ 00031921 n 0000 ~ 00033020 n 0000 ~ 00033615 n 0000 ~ 05810143 n 0000 ~ 07999699 n 0000 | a general concept formed by extracting common features from specific examples\n";

TEST_CASE("parse calculator index line", "[index]") {
    LemmaIndex index;
    auto stream = std::istringstream(calculator);
    stream >> index;
    REQUIRE(index.size() == 1);
    REQUIRE(index[0].lemma == std::string("calculator"));
    REQUIRE(index[0].pos == POS::Pos_Noun);
    REQUIRE(index[0].synsets.size() == 2);
    REQUIRE(index[0].synsets[0] == 9887034);
    REQUIRE(index[0].synsets[1] == 2938886);
}

TEST_CASE("parse word index line", "[index]") {
    LemmaIndex  index;
    auto stream = std::istringstream(word);
    stream >> index;
    REQUIRE(index.size() == 1);
    REQUIRE(index[0].lemma == std::string("word"));
    REQUIRE(index[0].synsets.size() == 10);
    REQUIRE(index[0].synsets == SynsetOffsets{
                                    6286395, 6738162, 6642138, 7169242,
                                    7140659, 7227301, 13627327, 9537144,
                                    6674188, 6431740
                                });
}

TEST_CASE("skip license header", "[index]") {
    LemmaIndex index;
    auto stream = std::istringstream(licenseHeader);
    stream >> index;
    REQUIRE(index.empty());
}

TEST_CASE("read synset from offset", "[synset]") {
    auto stream = std::istringstream(oneSynsetNoHeader);
    Synset synset = loadSynset(stream, 0);
    REQUIRE(synset.offset == 0);
    REQUIRE(synset.type == Synset::Noun);
    REQUIRE(synset.gloss == " that which is perceived or known or inferred to have its own distinct existence (living or nonliving)  ");
}

TEST_CASE("read synset from offset with header", "[synset]") {
    auto stream = std::istringstream(oneSynsetWithHeader);
    Synset synset = loadSynset(stream, 2137);
    REQUIRE(synset.offset == 2137);
    REQUIRE(synset.type == Synset::Noun);
    REQUIRE(synset.gloss == " a general concept formed by extracting common features from specific examples");
}