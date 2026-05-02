#ifndef GAMEOFLIFECLONE_CELLGRID_H
#define GAMEOFLIFECLONE_CELLGRID_H
#include <vector>


class CellGrid {
private:
    std::vector<uint8_t> cells;

    unsigned int texture;
    int height;
    int width;

public:
    CellGrid(int width, int height);

    // update the cells array on call
    void update();

    // upload the current cells array to the gpu
    void uploadToGPU();

    // getter + setter
    void set(const int x,const int y,const uint8_t val);
    uint8_t get(const int x,const int y);
};


#endif