/**
 * ZPO project: GIF encoding and decoding
 * @date 20.4.2014
 * @file imageprocessing.cpp
 * @author xsimet00
 * @author xsirok07
 * @author xskota05
 */

#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "arguments.h"
#include "gifencoder.h"

using namespace cv;

/**
 * @brief Used for image manipulation
 */
class ImageProcessing
{
    Mat image;

    bool isGif(const string &filename);
public:
    ImageProcessing(const string str);
    void convertToGrayscale(bool convert = false);
    void resize(Arguments &arg);
    void save(const string & filename, set<enum img_type> & file_types);
    void displayImage(bool = false);
};

#endif // IMAGEPROCESSING_H
