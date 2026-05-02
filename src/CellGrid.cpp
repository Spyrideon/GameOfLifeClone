#include "CellGrid.h"

CellGrid::CellGrid(int width, int height)
    : width(width), height(height), cells(width * height, 0){

}

void CellGrid::update() {

}

void CellGrid::uploadToGPU() {

}

// getter + setter
void CellGrid::set(const int x,const int y,const uint8_t val) {
    cells[x * y - 1] = val;
}
uint8_t CellGrid::get(const int x,const int y) {
    return cells[x * y - 1];
}