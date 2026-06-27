# Wave Function Collapse


A C++ application that visualizes the Wave Function Collapse algorithm using SFML3.

Requirements
CMake 3.16 or newer
A C++17-compatible compiler
On Windows: Visual Studio 2022 with the Desktop Development with C++ workload

SFML3 is expected to be located in:

external/SFML/

with the following structure:

external/SFML/
├── include/
├── lib/
└── bin/
Building

Clone the repository:

git clone <repository-url>
cd WaveFunctionCollapse

Generate the build files:

cmake -B build

Build the project:

cmake --build build --config Release

Running

After building, the executable can be found in the build directory (for Visual Studio):

build/Release/WaveFunctionCollapse.exe

The required SFML DLLs are automatically copied next to the executable during the build process.

