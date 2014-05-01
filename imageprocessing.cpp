/**
 * ZPO project: GIF encoding and decoding
 * @date 20.4.2014
 * @file imageprocessing.cpp
 * @author xsimet00
 * @author xsirok07
 * @author xskota05
 */

#include "imageprocessing.h"
#include "gif2bmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <inttypes.h>
#include "constant.h"
#include <fcntl.h>

/**
 * @brief Detects if input file is GIF
 * @param filename Filename
 * @return True if gif was recognized
 */
bool ImageProcessing::isGif(const string &filename)
{
	// Tries to open file
    FILE * fp = fopen(filename.c_str(), "rb");

    if (fp == 0)
        throw "Unable to open file: " + filename;

    char gif[3];
	
	// Reads frist three bytes
    int i = fread(&gif, sizeof(char), 3, fp);

    fclose(fp);

    if (i != 3)
        throw "Error reading head of file: " + filename;

	// Tests if its a GIF
    if (strcmp(gif, "GIF") == 0)
        return true;

    return false;
}

/**
 * @brief ImageProcessing constructor
 * @param filename Input file
 */
ImageProcessing::ImageProcessing(const string filename)
{
    // TODO: Call GIF load function
    if (this->isGif(filename))
        this->image = loadGif(filename);

    else
        this->image = imread(filename);

    // Failed to load data
    if (this->image.total() == 0)
        throw "No image data in file: " + filename;
}

/**
 * @brief Converts image to grayscale
 * @param convert True if image should be converted
 */
void ImageProcessing::convertToGrayscale(bool convert)
{
    if (convert)
        cvtColor(this->image, this->image, CV_BGR2GRAY);
}

/**
 * @brief Resizes image accordingly to specified arguments
 * @param arg Arguments reference
 */
void ImageProcessing::resize(Arguments &arg)
{
    if (arg.getResize() == PERCENT)
        cv::resize(this->image, this->image, Size(0,0), arg.getResizePercentX(), arg.getResizePercentY());

    else if (arg.getResize() == DIMENSION)
        cv::resize(this->image, this->image, Size(arg.getWidth(),arg.getHeight()));
}

/**
 * @brief Saves image to output
 * @param filename Filename with path
 * @param file_types Types of the saved image
 */
void ImageProcessing::save(const string & filename, set<enum img_type> & file_types)
{
    for (set<enum img_type>::iterator it = file_types.begin();
         it != file_types.end();
         it ++)
    {
        try
        {
            switch(*it)
            {
            case PNG:
                imwrite(filename + ".png", this->image);
                break;

            case JPEG:
                imwrite(filename + ".jpeg", this->image);
                break;

            case TIFF:
                imwrite(filename + ".tiff", this->image);
                break;

            case GIF:
                // TODO: Call GIF save function
                cerr << "IMPLEMENT GIF SAVE FUNCTION" << endl;
                // writegif(filename + ".gif", this->image);
                break;

            case BMP:
                imwrite(filename + ".bmp", this->image);
                break;

            case DIB:
                imwrite(filename + ".dib", this->image);
                break;

            case JPG:
                imwrite(filename + ".jpg", this->image);
                break;

            case JP2:
                imwrite(filename + ".jp2", this->image);
                break;

            case PBM:
                imwrite(filename + ".pbm", this->image);
                break;

            case PGM:
                imwrite(filename + ".pgm", this->image);
                break;

            case PPM:
                imwrite(filename + ".ppm", this->image);
                break;

            case SR:
                imwrite(filename + ".sr", this->image);
                break;

            case RAS:
                imwrite(filename + ".ras", this->image);
                break;

            case TIF:
                imwrite(filename + ".tif", this->image);
                break;
            }
        }
        catch (...)
        {
            throw "Unable to save one of output files.";
        }
    }
}

/**
 * @brief Displays image
 * @param display True if image should be displayed
 */
void ImageProcessing::displayImage(bool display)
{
    if (display)
    {
        namedWindow("Output", CV_WINDOW_AUTOSIZE);
        imshow("Output", this->image);
        waitKey(0);
    }
}
