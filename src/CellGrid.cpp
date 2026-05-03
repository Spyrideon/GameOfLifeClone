#include "CellGrid.h"

CellGrid::CellGrid(const int width,const int height)
    : width(width), height(height), cells(width * height, 0){
    initTexture();
}

void CellGrid::update() {
    if (cells[0] == 0) {
        cells = std::vector<uint8_t>(width * height, 1);
    }
    else {
        cells = std::vector<uint8_t>(width * height, 0);
    }
    uploadTexToGPU();
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

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture);
}