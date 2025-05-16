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

> **Note:**  
> The library responsible for formula parsing was provided as part of the course assignment.  
> Its interface is accessible through the included `lib/expression.h` header.  
> The implementation does **not** include a custom parser, but instead integrates this supplied library for converting formulas into an internal representation.

## Project Structure

The project is organized for clarity and maintainability:

- **src/**  
  Main source files and core class implementations.
- **lib/**  
  Assignment-provided libraries and headers, including `expression.h` and the static parser library.
- **tests/**  
  Basic test cases

## Implementation Overview

<details>
<summary><strong>Core Classes</strong></summary>

- **CSpreadsheet**  
  The main interface and data holder for the spreadsheet.   
  - Stores all cell data 
  - Provides methods to set, erase, copy, and retrieve cell values.
  - Handles spreadsheet persistence (`save`/`load` with checksums and custom text format).
  - Manages dependency tracking and triggers for value recalculation.

- **CCell**  
  Represents a single cell (number, string, or formula).
  - Stores the formula as a parsed AST (via provided parser).
  - Caches its computed value for efficiency and tracks when recalculation is needed.
  - Holds a reference manager for dependency tracking (`CRefManager`), and its own cycle detection state.
  - Exposes methods for value computation and copying.

- **CPos**  
  Abstraction of a cell’s location (column/row) with support for absolute and relative references.
  - Provides parsing, validation, and conversion (e.g., from `A1`/`$B$2` into indices).

- **CRefManager**  
  Internal utility to manage all references a cell holds to other cells.
  - Handles reference updates for relative/absolute addressing.
  - Supports moving references during range copy operations.

- **CExpressionBuilder (CBuilder)**  
  Implements the interface for the provided parser library.
  - Builds the AST for formulas, mapping parsed tokens to node objects.
  - Supports all implemented spreadsheet functions and operators.

- **CValue**  
  A variant type representing a cell’s evaluated value (number, string, or undefined).
  - Used throughout for value passing, comparison, and checks.

- **AST Node Classes (e.g., AASTNode, CASTValNode, CASTBinaryNode, etc.)**  
  - Internal hierarchy for the parsed formula AST.
  - Each type of operation or value (literal, reference, function call, binary/unary op) is a separate node class.
  - Used for efficient and recursive evaluation of formulas.

</details>

<details>
<summary><strong>Key Implementation Details</strong></summary>

- **Formula Parsing & Evaluation**  
  Formulas are parsed using a provided external library. When a formula is entered, it’s converted into an AST via the builder, enabling fast reevaluation on cell changes.

- **Dependency Management & Cycle Detection**  
  When cells refer to each other (directly or transitively), dependencies are tracked to ensure updates propagate correctly. The implementation actively detects cyclic dependencies—if a formula would introduce a cycle, the update is rejected, preventing infinite recursion.

- **Persistence & Error Handling**  
  The spreadsheet can be saved to and loaded from a custom text format. Each save includes a checksum to detect file corruption, and the load method validates this before reconstructing the spreadsheet. Robust error handling ensures that invalid or corrupt files do not crash the program.

- **Cell Reference Handling**  
  Both absolute and relative references are supported, and are updated dynamically when copying or filling cell ranges. The CPos utility parses, validates, and manipulates reference strings, supporting features like mixed reference types and range selection.

- **Performance Considerations**  
  Each cell caches its computed value and tracks when it was last valid. This avoids unnecessary recomputation and ensures that recalculation only occurs when dependencies change. The use of the AST structure, efficient hashing, and run-counters further optimizes performance for bulk operations.

</details>

### Formula Syntax & Capabilities

Cells in FITexcel can contain a number, a string, or a formula. Formulas allow you to compute values dynamically, referencing other cells and using a rich set of operators and functions. Below is a summary of what you can use within formulas:

<details>
<summary><strong>Supported Elements in Formulas</strong></summary>

- **Numeric Literals**  
  - Whole or decimal numbers, optionally in scientific notation.  
    *Examples:* `15`, `2.54`, `1e+8`, `1.23e-10`

- **String Literals**  
  - Text enclosed in double quotes. To include a quote inside a string, double it (`""`).  
    *Examples:* `"Hello"`, `"She said ""Hi"""`  
    - Strings may contain any character, including newlines.

- **Cell References**  
  - Standard spreadsheet notation: column (letters) + row (number).  
    *Examples:* `A7`, `ZXCV789456`  
    - **Case-insensitive** for columns.
    - **Absolute/Relative References:**  
      - Prefix column/row with `$` to make it absolute.  
      - E.g.: `A5`, `$A5`, `A$5`, `$A$5`
    - When formulas are copied, relative parts will adjust automatically.

- **Cell Ranges**  
  - Define a rectangle of cells with a colon (`:`) between top-left and bottom-right cell references.  
    *Examples:* `A5:X17`, `A$7:$X29`, `A7:$X$29`

- **Functions**  
  - The following spreadsheet functions are implemented:
    - `sum(range)` — Sum all numeric values in the range.
    - `count(range)` — Count all defined (non-empty) cells in the range.
    - `min(range)` — Minimum numeric value in the range.
    - `max(range)` — Maximum numeric value in the range.
    - `countval(value, range)` — Count cells in range equal to `value`.
    - `if(cond, ifTrue, ifFalse)` — If `cond` is a nonzero number, returns `ifTrue`; if zero, returns `ifFalse`; if `cond` is undefined or a string, result is undefined.

- **Parentheses**  
  - Use `()` to group sub-expressions and control evaluation order.

- **Operators**  
  - **Arithmetic:**  
    - `^` (power, left associative)
    - `*` (multiply, left associative)
    - `/` (divide, left associative)
    - `+` (add or concatenate, left associative)
    - `-` (subtract, left associative)
  - **Unary Minus:**  
    - `-` (change sign, right associative; only for numbers)
  - **Relational:**  
    - `<`, `<=`, `>`, `>=`, `=`, `<>`  
    - Return `1` (true) or `0` (false).  
    - Only valid for two numbers or two strings; other combinations yield undefined.

- **Operator Precedence (from highest to lowest):**
    1. Function calls
    2. Power (`^`)
    3. Unary minus (`-`)
    4. Multiplication and division (`*`, `/`)
    5. Addition/subtraction (`+`, `-`)
    6. Relational (`<`, `<=`, `>`, `>=`)
    7. Equality/inequality (`=`, `<>`)

- **Notes:**
  - Division by zero results in an undefined value.
  - Comparisons between different types (e.g., string vs number) are undefined.
  - String concatenation is possible with `+`.
  - All cell references (including in ranges) can be absolute, relative, or mixed.

</details>

<details>
<summary><strong>Example of Core Usage</strong></summary>

```cpp
CSpreadsheet sheet;
sheet.setCell(CPos("A1"), "42");
sheet.setCell(CPos("B1"), "=A1+8");
auto val = sheet.getValue(CPos("B1")); // Yields 50
```
</details>

## What I Learned

Through this project, I practiced:

- Designing C++ class hierarchies and encapsulation
- Managing dependencies between data (cell references, formula graphs)
- Basic file I/O and error handling
- Integrating assignment-provided libraries into a larger codebase
- Using version control for incremental project development

## Building & Running

1. **Clone the repository:**
   ```sh
   git clone https://github.com/Komareal/FITexcel.git
   cd FITexcel
   ```

2. **Configure the project with CMake:**
   ```sh
   cmake -B build
   ```

3. **Build the project:**
   ```sh
   cmake --build build
   ```

4. **Run the executable:**
   ```sh
   ./build/excel
   ```

> **Notes:**  
> This build just runs my test cases.
> 
> 
> The provided static parser library and `lib/expression.h` header are already integrated via CMake. No additional configuration is required.

## Disclaimer

This project was created as coursework for educational purposes. Please respect academic integrity guidelines if referencing or reusing code.

## License

MIT License
