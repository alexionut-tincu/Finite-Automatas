# Finite Automatas

## Description

A C++ implementation of the three FA's: Determinstic Finite Automata, Nondeterministic Finite Automata and Lambda Nondeterministic Finite Automata.

These will take a series of inputs and, for each queried word, will return whether the word is or is not accepted by the provided language.

## Usage

The C++ files shall be compiled, then executed.

```bash
g++ -O3 -o file ./file.cpp
./file
```

By default, they will use an `input.txt` and an `output.txt` file for I/O, in the same directory.

For `nfa.cpp`, any command-line argument can be passed to get the first found path for the word, if accepted.

By default, `lnfa.cpp` will use the character '.' as lambda. This can be changed in the defines.

The input file should follow this structure:

* Line 1: the number of states (N), natural value
* Line 2: the states, N unique integer values
* Line 3: the number of transitions (M), natural value
* Lines 4 - M+3: the transitions, tuple of two valid states and a letter (or lambda)
* Line M+4: the initial state, valid state
* Line M+5: the number of final states (F), natural value
* Line M+6: the final states, F valid states
* Line M+7: the number of words to check
* Lines M+8 - F+M+7: the words to be checked, strings

## Contributing

Please leave a PR if you spot a bug or wish to add a feature.