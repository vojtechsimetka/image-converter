#ifndef SUBBLOCK_H
#define SUBBLOCK_H

#include <cv.h>

using namespace cv;
using namespace std;

class SubBlock
{
public:
    Mat data;
    unsigned int offset_x;
    unsigned int offset_y;
    unsigned int width;
    unsigned int height;

public:
    inline SubBlock(const Mat &data, unsigned int offset_x, unsigned int offset_y ,unsigned int width, unsigned int height)
    {
        this->data = Mat(data, Rect(offset_x, offset_y, width, height));
        this->offset_x = offset_x;
        this->offset_y = offset_y;
        this->width = width;
        this->height = height;
    }

    inline const Mat & getData() const
    {
        return this->data;
    }
};

#endif // SUBBLOCK_H
