/**
 * ZPO project: GIF encoding and decoding
 * @date 20.4.2014
 * @file arguments.cpp
 * @author xsimet00
 * @author xsirok07
 * @author xskota05
 */

#include "arguments.h"

/**
 * @brief Arguments constructor
 * @param argc Arguments count
 * @param argv Arguments vector
 */
Arguments::Arguments(int argc, const char *argv[])
{
    this->state = HELP;
    this->out = "";
    this->rsz = NONE;
    this->grayscale = false;
    this->display = false;
    this->out = "";

    if (argc < 3)
        return;

    // Saves input file
    this->input_file = string(argv[1]);

    // Parses arguments
    for (int i = 2; i < argc; i++)
    {
        // Parameter -s N.N%
        if (strcmp(argv[i], "-s") == 0)
        {
            this->rsz = PERCENT;

            // Parameter -s must be followed by two double values
            if (i >= argc+1)
                return;

            this->resize_percent_x = atof(argv[++i])/100;
            this->resize_percent_y = atof(argv[++i])/100;
        }

        // Parameter -r N N
        else if (strcmp(argv[i], "-r") == 0)
        {
            this->rsz = DIMENSION;

            // Parameter -d must be followed by two integer values
            if (i >= argc+1)
                return;

            this->width = atoi(argv[++i]);
            this->height = atoi(argv[++i]);
        }

        // Parameter -o
        else if (strcmp(argv[i], "-o") == 0)
        {
            // Parameter -o must be followed by path
            if (i >= argc)
                return;

            this->out = argv[++i];

            if (this->out[this->out.size()-1] != '/')
                this->out += "/";
        }

        // Parameter -d
        else if (strcmp(argv[i], "-d") == 0)
            this->display = true;

        // Parameter -g
        else if (strcmp(argv[i], "-g") == 0)
            this->grayscale = true;

        // Output file formats
        else
        {
            if (strcmp(argv[i], "png") == 0)
                this->output.insert(PNG);

            else if (strcmp(argv[i], "jpeg") == 0)
                this->output.insert(JPEG);

            else if (strcmp(argv[i], "gif") == 0)
                this->output.insert(GIF);

            else if (strcmp(argv[i], "tiff") == 0)
                this->output.insert(TIFF);

            else if (strcmp(argv[i], "bmp") == 0)
                this->output.insert(BMP);

            else if (strcmp(argv[i], "dib") == 0)
                this->output.insert(DIB);

            else if (strcmp(argv[i], "jpg") == 0)
                this->output.insert(JPG);

            else if (strcmp(argv[i], "jp2") == 0)
                this->output.insert(JP2);

            else if (strcmp(argv[i], "pbm") == 0)
                this->output.insert(PBM);

            else if (strcmp(argv[i], "pgm") == 0)
                this->output.insert(PGM);

            else if (strcmp(argv[i], "ppm") == 0)
                this->output.insert(PPM);

            else if (strcmp(argv[i], "sr") == 0)
                this->output.insert(SR);

            else if (strcmp(argv[i], "ras") == 0)
                this->output.insert(RAS);

            else if (strcmp(argv[i], "tif") == 0)
                this->output.insert(TIF);

            else
                return;
        }
    }

    // Determines output path
    this->out += input_file.substr(0,input_file.find_last_of("."));

    // Parameters were succesfully parsed
    this->state = OK;
}
