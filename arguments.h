/**
 * ZPO project: GIF encoding and decoding
 * @date 20.4.2014
 * @file arguments.h
 * @author xsimet00
 * @author xsirok07
 * @author xskota05
 */

#ifndef ARGUMENTS_H
#define ARGUMENTS_H
#include <iostream>
#include <set>

using namespace std;

/**
 * @brief The argument parsing state enumerator
 */
enum state
{
    HELP,
    OK
};

/**
 * @brief The resize options enumerator
 */
enum resize
{
    NONE,
    PERCENT,
    DIMENSION
};

/**
 * @brief The output image types enumerator
 */
enum img_type
{
    PNG,
    JPEG,
    GIF,
    TIFF,
    BMP,
    DIB,
    JPG,
    JP2,
    PBM,
    PGM,
    PPM,
    SR,
    RAS,
    TIF
};

/**
 * @brief Arguments parsing
 */
class Arguments
{
    enum state state;
    string input_file;
    string out;
    enum resize rsz;
    double resize_percent_x;
    double resize_percent_y;
    int width;
    int height;
    bool grayscale;
    bool display;
    set<enum img_type> output;

public:
    Arguments(int argc, const char *argv[]);

    /**
     * @brief Tests if arguments are specified correctly
     * @return True if arguments are OK
     */
    inline bool stateOK() {return this->state == OK;}

    /**
     * @brief Gets input filename
     * @return Input filename
     */
    inline const string & getInputFile(){return this->input_file;}

    /**
     * @brief Tests if grayscale argument was toggled
     * @return True if grayscale option was toggled
     */
    inline bool isGrayscale(){return this->grayscale;}

    /**
     * @brief Returns resize type option
     * @return Resize option
     */
    inline enum resize getResize(){return this->rsz;}

    /**
     * @brief Gets x resize percentage
     * @return X resize percentage
     */
    inline double getResizePercentX(){return this->resize_percent_x;}

    /**
     * @brief Gets y resize percentage
     * @return Y resize percentage
     */
    inline double getResizePercentY(){return this->resize_percent_y;}

    /**
     * @brief Gets width of resized image
     * @return Width of resized image
     */
    inline int getWidth(){return this->width;}

    /**
     * @brief Gets height of resized image
     * @return Height of resized image
     */
    inline int getHeight(){return this->height;}

    /**
     * @brief Tests if output should be displayed first
     * @return True if output should be displayed first
     */
    inline bool showOutput(){return this->display;}

    /**
     * @brief Gets vector containing output file types
     * @return Vector containing output file types
     */
    inline set<enum img_type> & getOutput(){return this->output;}

    /**
     * @brief Gets output filename including file path without extention
     * @return Output path + filename
     */
    inline const string & getOutputFile(){return this->out;}
};

#endif // ARGUMENTS_H
