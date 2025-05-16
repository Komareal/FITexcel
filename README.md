# FITexcel

> **Coursework Project**  
> Programming and Algorithmics 2 (PA2)  
> Faculty of Information Technology, Czech Technical University in Prague (FIT CTU)  
> Language: C++

## Project Description

**FITexcel** is a minimal spreadsheet processor implemented in C++ as an assignment for the PA2 course at FIT CTU. The main goal was to practice class design and implementation of basic spreadsheet features, with a focus on object-oriented principles and practical use of C++.

## Objectives

The project implements:

- **Cell management**: Setting and retrieving values (numbers, strings, or formulas) for table cells.
- **Formula support**: Parsing and evaluating formulas, including cell references, mathematical operations, and a subset of spreadsheet functions (`sum`, `min`, `max`, `count`, `countval`, `if`).
- **Reference handling**: Both absolute and relative cell references, updated correctly when copying cell blocks.
- **Cyclic dependency detection**: Detects and prevents cycles in cell references (cyclic formulas).
- **Persistence**: Ability to save and load spreadsheet contents, with error handling for corrupted files.

## Provided Parser Library

> **Note:**  
> The library responsible for formula parsing was provided as part of the course assignment.  
> Its interface is accessible through the included `lib/expression.h` header.  
> The implementation does **not** include a custom parser, but instead integrates this supplied library for converting formulas into an internal representation (AST).

## Project Structure

The project is organized for clarity and maintainability:

- **src/**  
  Main source files and core class implementations.
- **lib/**  
  Assignment-provided libraries and headers, including `expression.h` and the static parser library.
- **tests/**  
  Example tests and usage scenarios.

## Implementation Overview

- **CSpreadsheet**: Manages the spreadsheet’s cells, formulas, and file IO.
- **CPos**: Parses and validates cell identifiers (e.g., `A1`, `$B$2`), handling both absolute and relative addressing.
- **CExpressionBuilder**: Implements the builder interface required by the parser library, constructing an internal representation for formulas.
- **CValue**: Represents the value of a cell (number, string, or undefined).

Efficient evaluation is achieved by building an AST for each formula at input time, so cell values can be quickly recalculated as needed.

## Example Usage

```cpp
CSpreadsheet sheet;
sheet.setCell(CPos("A1"), "42");
sheet.setCell(CPos("B1"), "=A1+8");
auto val = sheet.getValue(CPos("B1")); // Yields 50
```

## What I Learned

Through this project, I practiced:

- Designing C++ class hierarchies and encapsulation
- Managing dependencies between data (cell references, formula graphs)
- Basic file I/O and error handling
- Integrating assignment-provided libraries into a larger codebase
- Using version control for incremental project development

## Building & Running

1. Clone the repository:
   ```sh
   git clone https://github.com/Komareal/FITexcel.git
   cd FITexcel
   ```
2. Build with your preferred C++ build system.  
   Make sure to include the provided static parser library and `lib/expression.h` header.
3. Run the resulting executable.

## Disclaimer

This project was created as coursework for educational purposes. Please respect academic integrity guidelines if referencing or reusing code.

## License

MIT License
