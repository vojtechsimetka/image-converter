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
 * @brief Main function
 * @param argc Argument counter
 * @param argv Argument vector
 * @return Exit code
 */
int main( int argc, const char* argv[] )
{
    // Loads image
    try
    {
        Arguments arg(argc, argv);
        ImageProcessing processor(arg.getInputFile());

        // Grayscale conversion
        processor.convertToGrayscale(arg.isGrayscale());

        // Image resize
        processor.resize(arg);

        // Display output
        processor.displayImage(arg.showOutput());

        // Saves output
        processor.save(arg.getOutputFile(), arg.getOutput());
    }
    catch(string e)
    {
        cerr << "Error: " << e << endl;
        return EXIT_FAILURE;
    }
    catch(const char * e)
    {
        cerr << "Error: " << e << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
