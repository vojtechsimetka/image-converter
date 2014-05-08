#ifndef GIFENCODER_H
#define GIFENCODER_H

#include <iostream>
#include <cv.h>
#include <vector>
#include "subblock.h"
#include "gifwriter.h"
#include "gifdictionary.h"

using namespace cv;
using namespace std;

enum source
{
    PALETTE,
    DICTIONARY
};

struct output_struct
{
    int index;
    unsigned int size;

    output_struct(int i, unsigned int size)
    {
        this->index = i;
        this->size = size;
    }
};

class GIFencoder
{
private:
    vector<SubBlock> subimages;
    GIFwriter writer;

    void createSubBlocks(const Mat &image);
    void writeHeader(const Mat &image);
    void writeSubBlock(SubBlock &block);
    vector<output_struct> LZW(SubBlock & block);
    void writeImageDescriptor(SubBlock &block);
    void writePalette(SubBlock &block);
    void writeData(vector<output_struct> &output);
public:
    GIFencoder(const string &filename, const Mat &image);
};

#endif // GIFENCODER_H
