# Project_CS163: Data Structure Visualization

## Introduction

This CS163 group lab project, the Data Structure Visualizer, provides interactive animations that demonstrate how fundamental data structures work. It’s designed to help users explore and understand operations like insertion, deletion, and traversal in real time.
## Features
This program currently supports the following data structures:
AVL Tree
Heap
Linked List
Graph


## Main Features

- **Animated Data Structure Operations**  
  Real-time visualizations of insert, delete, search, and update actions on your data structure.

- **Playback Controls**  
  Adjust animation speed and use play, pause, or reverse to explore each step at your own pace.

- **Synchronized Code Highlighting**  
  Step-by-step highlighting of the corresponding source code alongside the animation.

- **Flexible Data Input**  
  Generate a random list or import one from a file to drive the visualization.

## Installation

### Prerequisites

- **Git** (to clone the repository)  
- **CMake** version 3.28 or higher  
- **C++ compiler**:  
  - GCC or Clang on Linux/macOS  
  - MSVC or MinGW on Windows  
- **Internet connection** (so FetchContent can automatically download SFML)  
- **Optional**: a system-wide SFML installation; see [SFML downloads](https://www.sfml-dev.org/download.php) if you prefer not to use FetchContent 

### Building

```bash
mkdir build

cmake -G "MinGW Makefiles" -S . -B build

cmake --build build

.\build\bin\main.exe
```  

### Demo Video

https://www.youtube.com/watch?v=ExtQr4t8RA8

### Credits

- **Authors:**
  - Trinh Vo Nam Kiet (Student ID: 24125013, GitHub: namkiet)
  - Nguyen Huu Phuoc (Student ID: 24125018, GitHub: AkiyaNguyen)
  - Le Quoc Vi (Student ID: 24125085, GitHub: wzyle)

- **Course:** CS163 – Data Structures  
- **Institution:** VNU University of Science