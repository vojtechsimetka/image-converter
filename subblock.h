#ifndef SUBBLOCK_H
#define SUBBLOCK_H

#include <cv.h>
#include "gifdictionary.h"

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
    GIFdictionary dictionary;

public:
    inline SubBlock(const Mat &data, unsigned int offset_x, unsigned int offset_y ,unsigned int width, unsigned int height)
    {
        this->data = Mat(data, Rect(offset_x, offset_y, width, height));
        this->offset_x = offset_x;
        this->offset_y = offset_y;
        this->width = width;
        this->height = height;

        set<unsigned int>colors;

        for (unsigned int y = 0; y < this->data.rows; y++)
        {
            for (unsigned int x = 0; x < this->data.cols; x++)
            {
                // Creates color code
                colors.insert(this->data.at<Vec3b>(y, x).val[0] << 16 |
                              this->data.at<Vec3b>(y, x).val[1] << 8 |
                              this->data.at<Vec3b>(y, x).val[2]);
            }
        }

        this->dictionary.addColors(colors);
    }

    inline GIFdictionary & getDictionary()
    {
        return this->dictionary;
    }

    inline const Mat & getData() const
    {
        return this->data;
    }
};

#endif // SUBBLOCK_H
