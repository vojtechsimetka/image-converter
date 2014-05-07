#include "gifencoder.h"

/**
 * @brief GIFencoder constructor
 * @param filename Output filename
 * @param image Mat containing image to be saved
 */
GIFencoder::GIFencoder(const string &filename, const Mat &image)
    : writer(filename)
{
    Mat tmp(image);

    // Converts mat to CV_8U
    tmp.convertTo(tmp, CV_8U);

    // Convert to 3 channels
    if (tmp.channels() != 3)
        cvtColor(tmp,tmp,CV_GRAY2RGB);

    // Checks if the output image is CV_8UC3
    if (tmp.type() != CV_8UC3)
        throw "Failed to convert internal reprezentaion of GIF image to type CV_8UC3";

    // Determines subblocks with maximum of 256 colors
    this->createSubBlocks(tmp);

    // Writes header of the gif file
    this->writeHeader(tmp);

    // Writes all subblocks
    for (vector<SubBlock>::iterator it = this->subimages.begin();
         it != this->subimages.end();
         it ++)
        this->writeSubBlock(*(it));

    // Writes termination block
    this->writer.write(0x3B, 8);
}

/**
 * @brief Writes GIF header to output file
 * @param image Image reference
 */
void GIFencoder::writeHeader(const Mat &image)
{
    // Writes header block
    this->writer.write("GIF89a", 64);

    // Image width
    this->writer.write(image.cols >> 8, 8);
    this->writer.write(image.cols, 8);

    // Image height
    this->writer.write(image.rows >> 8, 8);
    this->writer.write(image.rows, 8);

    // Flags
    this->writer.write(0, 8);

    // Background color index
    this->writer.write(0, 8);

    // Pixel aspect ratio
    this->writer.write(0, 8);
}

/**
 * @brief Creates SubBlocks from the original image that have less than 256 colors
 * @param image Source image
 */
void GIFencoder::createSubBlocks(const Mat &image)
{
    for (int y = 0; y < image.rows ; y += 16)
    {
        for (int x = 0; x < image.cols; x+= 16)
        {
            this->subimages.push_back(
                SubBlock(image,
                         y,
                         x,
                         (y < image.rows - 16) ? 16 : image.rows - y,
                         (x < image.cols - 16) ? 16 : image.cols - x));
        }
    }
}

void GIFencoder::writeSubBlock(const SubBlock &block)
{
    GIFdictionary dictionary;
    vector<output_struct> output;
    vector<unsigned int> loaded_pixels;
    int last_loaded_color = -1;
    int last_dict_record = -1;

    output.push_back(output_struct(PALETTE, 0));

    for (unsigned int y = 0; y < block.getData().rows; y++)
    {
        for (unsigned int x = 0; x < block.getData().cols; x++)
        {
            unsigned int current_color = block.getData().at<Vec3b>(y, x).val[2] << 16 |
                                         block.getData().at<Vec3b>(y, x).val[1] << 8 |
                                         block.getData().at<Vec3b>(y, x).val[0];

            int index = (dictionary.isInPalette(current_color));

            if (index == -1)
                index = dictionary.addColor(current_color);

            loaded_pixels.push_back(current_color);

            // Just one color was loaded, load more
            if (loaded_pixels.size() == 1)
            {
                last_loaded_color = index;
                continue;
            }

            int record = dictionary.find(loaded_pixels);
            if (record != -1)
            {
                last_dict_record = record;
                continue;
            }

            else if (loaded_pixels.size() == 2)
            {
                output.push_back(output_struct(PALETTE, last_loaded_color));
                dictionary.addRecord(loaded_pixels);
            }

            else
            {
                output.push_back(output_struct(DICTIONARY, last_dict_record));
                dictionary.addRecord(loaded_pixels);
            }

            unsigned int tmp = loaded_pixels.back();
            loaded_pixels.clear();
            loaded_pixels.push_back(tmp);
        }
    }

    if (loaded_pixels.size() == 1)
        output.push_back(output_struct(PALETTE, last_loaded_color));

    else if (!loaded_pixels.empty())
    {
        output.push_back(output_struct(DICTIONARY, last_dict_record));
        output.push_back(output_struct(PALETTE, last_loaded_color));
    }

    // Image Descriptor
    // Image separator
    this->writer.write(0x2C, 8);
    // Image left
    this->writer.write(block.offset_x >> 8, 8);
    this->writer.write(block.offset_x, 8);
    // Image right
    this->writer.write(block.offset_y >> 8, 8);
    this->writer.write(block.offset_y, 8);
    // Image width
    this->writer.write(block.width >> 8, 8);
    this->writer.write(block.width, 8);
    // Image height
    this->writer.write(block.height >> 8, 8);
    this->writer.write(block.height, 8);
    // Packed field
    this->writer.write(128 | dictionary.getPaletteSize(), 8);

    for (vector<unsigned int>::iterator it = dictionary.getPalette().begin();
         it != dictionary.getPalette().end();
         it ++)
        this->writer.write(*it, 24);

    for (int i = pow(2, dictionary.getPaletteSize()+1) - dictionary.getPalette().size(); i > 0 ; i--)
        this->writer.write(0, 24);

//    cout << dictionary.getPaletteSize() << " " << dictionary.getPalette().size() + pow(2, dictionary.getPaletteSize()+1) - dictionary.getPalette().size() << endl;

    // Data
    unsigned int record_size =  dictionary.recordLength();
    // LZW Min code size
    this->writer.write(record_size, 8);

    // Determines number of data blocks
    output.push_back(output_struct(PALETTE, dictionary.getEOI()));
    output.front() = output_struct(PALETTE, dictionary.getClear());

    unsigned int total_size = output.size()*record_size;

    while (total_size > 256)
    {
        writer.write((256%record_size == 0) ? 256 : 256/record_size * record_size + 1, 8);
        for (int i = 0; i < 255/record_size; i++)
        {
            output_struct out = output.front();
            output.erase(output.begin());
            this->writer.write((out.src == DICTIONARY) ? out.index + dictionary.getClear()+2: out.index, record_size);
        }
        total_size -= 256/record_size * record_size;
    }

    writer.write(total_size, 8);

//    cout << output.size() << " " << total_size/record_size << endl;

    writer.write(output.size()*record_size, 8);
    while (!output.empty())
    {
        output_struct out = output.front();
        output.erase(output.begin());
        this->writer.write((out.src == DICTIONARY) ? out.index + dictionary.getClear()+2: out.index, record_size);
    }
    writer.finish();
    writer.write(0, 8);
//    writer.write(dictionary.getEOI(), record_size);
//    writer.finish();
}
