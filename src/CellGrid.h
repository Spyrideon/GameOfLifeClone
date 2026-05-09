#ifndef GAMEOFLIFECLONE_CELLGRID_H
#define GAMEOFLIFECLONE_CELLGRID_H
#include <vector>
#include <random>
#include <glad/glad.h>


class CellGrid {
private:
    std::vector<uint8_t> cells;

    unsigned int texture{};
    const int height;
    const int width;

public:
    CellGrid(int width, int height);
    ~CellGrid();

    // update the cells array on call
    void update();

    // upload the current cells array to the gpu
    void uploadTexToGPU() const;
    void toggleCell(int x, int y);
    uint8_t countNeighbours(int row, int col) const;
    void initTexture();
};


#endif