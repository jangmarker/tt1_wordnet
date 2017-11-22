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

Please note that task 4 is not implemented as of now.

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