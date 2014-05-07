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
    source src;
    int index;

    output_struct(source s, int i)
    {
        this->src = s;
        this->index = i;
    }
};

class GIFencoder
{
private:
    vector<SubBlock> subimages;
    GIFwriter writer;

    void createSubBlocks(const Mat &image);
    void writeHeader(const Mat &image);
    void writeSubBlock(const SubBlock &block);
public:
    GIFencoder(const string &filename, const Mat &image);
};

#endif // GIFENCODER_H
