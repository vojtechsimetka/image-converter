/**
 * ZPO project: GIF encoding and decoding
 * @date 20.4.2014
 * @file main.cpp
 * @author xsimet00
 * @author xsirok07
 * @author xskota05
 */

#include "imageprocessing.h"

/**
 * @brief Prints help
 */
void printHelp()
{
    cout << "Usage:" << endl
         << endl
         << "/imgConvertor [in_file] [options] [out_types]" << endl
         << endl
         << "[in_file]    Any of following images both grayscale and RGB:" << endl
         << "                 bmp, dib, jpeg, jpg, jpe, jp2, png, pbm, pgm," << endl
         << "                 ppm, sr, ras, tiff, tif, gif" << endl
         << endl
         << "[options]    -s x y            size of output in %" << endl
         << "             -r width height   width and height of the output image" << endl
         << "             -d                display output" << endl
         << "             -g                convert to grayscale" << endl
         << "             -o folder         output folder" << endl
         << endl
         << "[out_types] bmp, dib           Windows bitmaps" << endl
         << "            jpeg, jpg, jpe     JPEG format" << endl
         << "            jp2                JPEG 2000 format" << endl
         << "            png                Portable Network Graphics" << endl
         << "            pbm, pgm, ppm      Portable image format" << endl
         << "            sr, ras            Sun rasters" << endl
         << "            tiff, tif          TIFF format" << endl
         << "            gif                GIF format using custom implementation" << endl
         << endl
         << "NOTE: Some input and output files' formats may require additional codecs on" << endl
         << "Linux and BSD systems. Search for following libraries: libjpeg, libpng, libtiff" << endl
         << "and libjasper." << endl;
}

/**
 * @brief Main function
 * @param argc Argument counter
 * @param argv Argument vector
 * @return Exit code
 */
int main( int argc, const char* argv[] )
{
    Arguments arg(argc, argv);

    // Arguments were incorrect, print help and exit
    if (!arg.stateOK())
    {
        printHelp();
        return EXIT_FAILURE;
    }

    // Loads image
    ImageProcessing processor(arg.getInputFile());

    // Grayscale conversion
    processor.convertToGrayscale(arg.isGrayscale());

    // Image resize
    processor.resize(arg);

    // Display output
    processor.displayImage(arg.showOutput());

    // Saves output
    processor.save(arg.getOutputFile(), arg.getOutput());

    return EXIT_SUCCESS;
}
