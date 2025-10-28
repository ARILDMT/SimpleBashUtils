# SimpleBashUtils

Implementation of Unix text utilities `cat` and `grep` in C.

## Overview

This project contains custom implementations of two fundamental Unix command-line utilities:
- **cat** - concatenate and display files
- **grep** - search text using patterns

Both utilities are written in C (C11 standard) following structured programming principles and the Google C++ Style Guide.

## Features

### cat

A simplified version of the Unix `cat` utility with the following options:

| Option | Description |
|--------|-------------|
| `-b` | Number non-empty output lines |
| `-e` | Display end-of-line characters as `$` (implies `-v`) |
| `-E` | Display end-of-line characters as `$` |
| `-n` | Number all output lines |
| `-s` | Squeeze multiple adjacent blank lines |
| `-t` | Display tabs as `^I` (implies `-v`) |
| `-T` | Display tabs as `^I` |
| `-v` | Display non-printing characters (except tabs and newlines) |

**Usage:**
```bash
./cat [OPTION]... [FILE]...
```

### grep

A simplified version of the Unix `grep` utility with support for regular expressions and flag combinations:

| Option | Description |
|--------|-------------|
| `-e` | Specify a pattern |
| `-i` | Ignore case distinctions |
| `-v` | Invert match (select non-matching lines) |
| `-c` | Print count of matching lines |
| `-l` | Print names of files with matches |
| `-n` | Prefix each line with line number |
| `-h` | Suppress file name prefix on output |
| `-s` | Suppress error messages about nonexistent/unreadable files |
| `-f` | Take patterns from file |
| `-o` | Print only matched parts of lines |

**Flag Combinations:**
The utility supports combining multiple flags (e.g., `-iv` for case-insensitive inverse matching, `-in` for case-insensitive matching with line numbers).

**Usage:**
```bash
./grep [OPTION]... PATTERN [FILE]...
```

## Building

Each utility has its own Makefile in the respective directory.

### Building cat:
```bash
cd src/cat
make
```

### Building grep:
```bash
cd src/grep
make
```

## Project Structure

```
SimpleBashUtils/
├── src/
│   ├── cat/
│   │   ├── cat.c           # Main implementation
│   │   ├── cat.h           # Header file
│   │   ├── Makefile        # Build configuration
│   │   └── test_cat.sh     # Test script
│   └── grep/
│       ├── grep.c          # Main implementation
│       ├── grep.h          # Header file
│       ├── Makefile        # Build configuration
│       └── test_grep.sh    # Test script
└── README.md
```

## Testing

Both utilities include shell scripts for testing:

```bash
# Test cat
cd src/cat
bash test_cat.sh

# Test grep
cd src/grep
bash test_grep.sh
```

## Implementation Details

### cat
- Handles multiple files and concatenates them to standard output
- Supports all basic `cat` flags including GNU-specific variants
- Implements line numbering, blank line squeezing, and special character visualization
- Reads from stdin when no files are specified

### grep
- Uses POSIX regular expressions (regex.h library)
- Supports multiple patterns via `-e` flag or pattern file via `-f`
- Handles multiple input files with proper file name prefixing
- Implements case-insensitive matching and inverse matching
- Supports output modes: full lines, counts, file names only, or matched parts only
- **Full support for flag combinations** (e.g., `-iv`, `-in`, `-vn`, `-cil`, etc.)

## Technical Requirements

- **Language:** C11 standard
- **Compiler:** gcc
- **Standards:** POSIX.1-2017
- **Code Style:** Google C++ Style Guide
- **Libraries:** Standard C library, POSIX regex

## License

This project is open source and available under the terms specified in the LICENSE file.
