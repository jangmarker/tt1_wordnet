# Dependencies

The program is mainly based on the C++ standard library, except for:
* Catch (http://catch-lib.net/) - a unit testing framework for C++
* CLI11 (https://github.com/CLIUtils/CLI11) - framework to parse CLI parameters

The two dependencies are included in `3rdparty/` and are to no concern of a user.

# Usage

Follows the task rather closely.

**Use `--db=path/to/dict/` (don't forget a trailing directory separator) to pass the path to your database.**
**Otherwise the current working directory will be assumed to hold the database.**

`--help` can be used to gain information about possible parameters:
```
Usage: ./tt1_wordnet [OPTIONS] [SUBCOMMAND]

Options:
  -h,--help                   Print this help message and exit
  --db TEXT=./                Path to the dict/ directory, where the WordNet database is located.

Subcommands:
  query                       
  semrels                     
  shortestpath
```

## Task 1 - query
```
Usage: ./tt1_wordnet query [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  --pos TEXT in {a,n,r,v}     Part of speech
  --iw TEXT                   Index word
  --full                      Show index words for all synsets

```

Example invocation: `./tt1_wordnet --db=/home/user/WordNet/dict/ query --pos=n --iw=calculator --full`

## Task 2 - semrels
```
Usage: ./tt1_wordnet semrels [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  --pos TEXT in {a,n,r,v}     Part of speech
  --offset UINT               Offset
  --dir TEXT in {any,in,out}  Relation direction
  --full                      Show index words for all connected synsets

```

## Task 3 - shortestpath
```
Usage: ./tt1_wordnet shortestpath [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  --pos1 TEXT in {a,n,r,v}    Part of speech
  --offset1 UINT              Offset
  --pos2 TEXT in {a,n,r,v}    Target part of speech
  --offset2 UINT              Target offset
  --directed                  Only search directed
  --full                      Show index words for all connected synsets
```

## Task 4 - eccentricity
```
Usage: ./tt1_wordnet eccentricity [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  --pos TEXT in {a,n,r,v}     Part of speech
  --offset UINT               Offset
  --directed                  Only search directed
```

# Code Organisation

## `main.cpp`
* command line parsing
* query of `Database` for the necessary data
* printing of the data

## `Database.{h,cpp}`
* implementation of algorithms on the data
* loading of all data on construction
* maintenance of indexes of the data

## `FileAccess.{h,cpp}`
* low level cache of input streams
* mapping of requests for certain data to files

## `Input.{h,cpp}`
* implementation of `operator>>` for the different data structures

Reading of data is based on `std::istringstream`. It automatically parses text data into
C++ data types like `int`, `std:string` (up to the next whitespace), and custom structures.
Custom structures are read using `operator>>` functions, which are implemented in `Input.cpp`.

## `Output.{h,cpp}`
* implementation of `output<<` for visualization of data

Writing data on `std::cout` also works via operator overloads (of `operator<<`). The necessary operators
are implement in `Output.cpp`.

## `wordnet.h`
Definition of data structures. They are closely modelled after the file based database, so that they
can be read by the operators in `Input.{h,cpp}`.

## Development Environment
This was developed using CLion (https://www.jetbrains.com/clion/), but every other C++-IDE with CMake
support should be enough:
- QtCreator
- Visual Studio
- KDevelop
- CLion

Of course, an editor can be enough, too.

Prerequisites:
- C++14 compiler (Clang, GCC, Visual Studio)
- CMake

Steps to compile:
```bash
tt1_wordnet$ mkdir build
build$ cmake ..
build$ cmake --build .
build$ ./tt1_wordnet --db=path/to/WordNet/ --help
```

## A few common structures

### PartOfSpeech (pos)
Is defined as `char`. Unfortunately, the CLI parameter parser only knows strings (`std::string`) as option values.
These strings are converted to `char` like so: `char partOfSpeech = partOfSpeechOptionValue[0]`.

### LemmaIndexItem
One line in the index files.

### SynsetPointer
Structure modelling a pointer between two synsets.

### SynsetConnection
A SynsetConnection is relative to an "origin" element: `SynsetConnection::pointer` is the pointer between the
two synsets. It is important that "origin" does not own the pointer, i.e. the pointer can also be pointing at origin.
It depends on the context of where `SynsetConnection` is used.

### SynsetIdentifier
A Synset can be identified by the database it is saved in (noun, verb, adjective, adverb) and its offset in the database.
The `SynsetIdentifier` simply allows to handle these two data points in one object (`first` is the one-character-name for
the database, `second` the offset in the database).