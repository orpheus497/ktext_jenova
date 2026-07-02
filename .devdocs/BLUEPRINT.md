# Project Blueprint & Requirements

**Timestamp**: 2026-07-02 12:39

## System Requirements & Build Necessities

To successfully compile and install this plugin on a KF6-enabled Linux system (e.g. Arch Linux), the following build dependencies are strictly required:

* **CMake** (>= 3.16)
* **C++ Compiler** (GCC or Clang with C++20 support)
* **Qt 6 Base Development Packages** (`qt6-base`)
* **Extra CMake Modules** (`extra-cmake-modules`)
* **KDE Frameworks 6 Development Packages**:
  * `ktexteditor`
  * `kcoreaddons`
  * `ki18n`
  * `kxmlgui`
  * `syntax-highlighting`

**Build Command Execution**:
```bash
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=~/.local ..
make -j$(nproc)
make install
```

## Design Implementation Requests

### [ACTIVE] Code Documentation Formatting
* **Requirement**: The entire C++ codebase must be formatted to include `AGENTS.md` standard prefixes (e.g., `##Class purpose:`, `##Method purpose:`).
* **Questions**: Are there any specific formatting preferences for these comments (e.g., standard `//` vs `/* */`)?

### [BACKLOG] LLM Integration and Parsing
* **Requirement**: Ensure the `LlamaClient` perfectly parses the response JSON and manages context effectively without thread blocking.
* **Questions**: Should we implement local streaming for the responses, or is a complete JSON block sufficient for the MVP?

### [COMPLETED] 
*None currently.*
