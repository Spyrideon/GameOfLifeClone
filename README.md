# Conway's Game of Life — OpenGL Clone


## About the Project
 
Conway's Game of Life is a classic cellular automaton devised by mathematician John Horton Conway in 1970. The simulation takes place on an infinite 2D grid where each cell is either **alive** or **dead**, evolving each generation according to four simple rules:
 
1. A live cell with fewer than 2 live neighbours dies (underpopulation)
2. A live cell with 2 or 3 live neighbours survives
3. A live cell with more than 3 live neighbours dies (overpopulation)
4. A dead cell with exactly 3 live neighbours becomes alive (reproduction)
Despite the simplicity of these rules, extraordinarily complex patterns emerge.
 
---
 
## Built With
 
- **C++20**
- **OpenGL 3.3 Core**
- **GLFW 3.4** — window creation and input handling
- **glad** — OpenGL function loader
- **CMake** — build system
- **CLion** — IDE
---

### Building from Source
 
```bash
git clone https://github.com/yourusername/GameOfLifeClone.git
cd GameOfLifeClone
mkdir build && cd build
cmake ..
cmake --build .
```
 
The executable will be at `build/GameOfLifeClone.exe`.
