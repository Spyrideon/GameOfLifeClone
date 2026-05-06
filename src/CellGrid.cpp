#include "CellGrid.h"

CellGrid::CellGrid(const int width,const int height)
    : cells(width * height, 0), height(height), width(width){

    initTexture();

    std::mt19937 rng(std::random_device{}());
    std::bernoulli_distribution dist(0.4);

    for (auto& cell : cells) {
        cell = dist(rng) ? 1 : 0;
    }
}
CellGrid::~CellGrid(){
    glDeleteTextures(1, &texture);    // good practice, not needed
}


void CellGrid::update() {
    std::vector<uint8_t> new_cells(width * height, 0);
    for (int row = 0; row < width; row++) {
        for (int col = 0; col < height; col++) {
            uint8_t neighbours = countNeighbours(row, col);
            if (cells[row * width + col] == 1) {
                if (neighbours < 2 || neighbours > 3)
                    new_cells[row * width + col] = 0;
                else
                    new_cells[row * width + col] = 1;
            }
            else {
                if (neighbours == 3) new_cells[row * width + col] = 1;
            }
        }
    }
    cells = new_cells;
}

void CellGrid::uploadTexToGPU() {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexSubImage2D(GL_TEXTURE_2D,
        0, // mipmap
        0, 0,
        width, height,
        GL_RED,
        GL_UNSIGNED_BYTE,
        cells.data());
}

// getter + setter
void CellGrid::set(const int x,const int y,const uint8_t val) {
    cells[x * y - 1] = val;
}
uint8_t CellGrid::get(const int x,const int y) {
    return cells[x * y - 1];
}
uint8_t CellGrid::countNeighbours(int row, int col) {
    uint8_t count = 0;
    const bool has_top = row > 0;
    const bool has_bottom = row < height - 1;
    const bool has_left = col > 0;
    const bool has_right = col < width - 1;
    if (has_top) {
        if (has_left)   count += cells[(row-1) * width + (col-1)];
                        count += cells[(row-1) * width + col];
        if (has_right)  count += cells[(row-1) * width + (col+1)];
    }
    if (has_left)       count += cells[row * width + (col-1)];
    if (has_right)      count += cells[row * width + (col+1)];
    if (has_bottom) {
        if (has_left)   count += cells[(row+1) * width + (col-1)];
                        count += cells[(row+1) * width +  col   ];
        if (has_right)  count += cells[(row+1) * width + (col+1)];
    }
    return count;
}
void CellGrid::initTexture() {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_R8,
        width, height,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        cells.data());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
}