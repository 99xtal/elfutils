# elfutils

These are core utilities to help Santa's elves solve Advent of Code puzzles to save Christmas.

## Solutions

### 2025
| Day | Solution | Description |
|-|-|-|
| 1 | `safecode` | Calculate the door code for the North Pole base using safe dial rotations |
| 2 | `prodeval` | Calculate the total sum of invalid product IDs in a set of ranges |

### 2024
| Day | Solution | Description |
|-|-|-|
| 1   | `locdiff` | Calculate total distance between two lists of location IDs |

## Building from Source

### Prerequisites

- GCC or Clang (C compiler)
- GNU Make

On Debian/Ubuntu, you can install prerequisites with:

```bash
sudo apt update
sudo apt install build-essential make gcc
```

### Build Instructions

1. Build all programs:
	```bash
	make
	```
2. Run a program:
	```bash
	./bin/locdiff
	./bin/prodval
	./bin/safecode
	```
3. Clean build files:
	```bash
	make clean
	```
