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

    writer.write(0x21, 8);
    writer.write(0xF9, 8);
    writer.write(0x04, 8);
    writer.write(0, 8);
    writer.write(0, 8);
    writer.write(0, 8);
    writer.write(0, 8);
    writer.write(0, 8);

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
    this->writer.write("GIF89a", 48);

    // Image width
    this->writer.write(image.cols, 8);
    this->writer.write(image.cols >> 8, 8);

    // Image height
    this->writer.write(image.rows, 8);
    this->writer.write(image.rows >> 8, 8);

    // Flags
    this->writer.write(0x77, 8);

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
    this->subimages.push_back(SubBlock(image, 0,0,image.cols, image.rows));
//    for (int y = 0; y < image.rows ; y += 16)
//    {
//        for (int x = 0; x < image.cols; x+= 16)
//        {
//            this->subimages.push_back(
//                SubBlock(image,
//                         x,
//                         y,
//                         (x < (int)(image.cols) - 16) ? 16 : (int)(image.cols) - x,
//                         (y < (int)(image.rows) - 16) ? 16 : (int)(image.rows) - y));
//        }
//    }
}

vector<output_struct> GIFencoder::LZW(SubBlock & block)
{
    vector<output_struct> output;
    vector<unsigned int> loaded_pixels;
    output_struct last_found(PALETTE, -1);

    // Pushes clear code
    output.push_back(output_struct(block.getDictionary().getClear(),
                                   block.getDictionary().getCurrentSize()));

    // Goes through whole image subblock
    for (unsigned int y = 0; y < block.getData().rows; y++)
    {
        for (unsigned int x = 0; x < block.getData().cols; x++)
        {
            // Adds current collor to sequence
            loaded_pixels.push_back(block.getData().at<Vec3b>(y, x).val[0] << 16 |
                                    block.getData().at<Vec3b>(y, x).val[1] << 8 |
                                    block.getData().at<Vec3b>(y, x).val[2]);

            if (block.dictionary.cleared)
            {
                cerr << endl << "Nacetl jsem barvu s indexem " << block.getDictionary().findColor(loaded_pixels.back()) << endl;
                for (int i = 0; i < loaded_pixels.size(); i++)
                    cerr << block.getDictionary().findColor(loaded_pixels[i]) << ",";
                cerr << endl;
            }

            // Just one color was loaded, load more
            if (loaded_pixels.size() == 1)
            {
                last_found = output_struct(block.getDictionary().findColor(loaded_pixels.back()),
                                           block.getDictionary().getCurrentSize());
                continue;
            }

            // Determines index of current sequence
            int record = block.getDictionary().find(loaded_pixels);

            // Record is in dictionary, continue loading input
            if (record != -1)
            {
                if (block.dictionary.cleared)
                {
                    cerr << "Nasel jsem zaznam ve slovniku s indexem " << record << " pro barvy: ";
                    for (int i = 0; i < loaded_pixels.size(); i++)
                        cerr << block.getDictionary().findColor(loaded_pixels[i]) << ",";
                    cerr << endl;
                }

                last_found = output_struct(record, block.getDictionary().getCurrentSize());
                continue;
            }

            // Stores record to output
            output.push_back(last_found);
            if (block.dictionary.cleared)
                cerr << "Zapsal jsem: " << last_found.index << endl;

            cout << setw(6) << last_found.index << setw(6) << last_found.size << setw(6) << block.dictionary.getLastIndex() << endl;

            // Adds new record to dicionary
            block.getDictionary().addRecord(loaded_pixels);

            // LZW is too big
            if (block.getDictionary().getCurrentSize() > 12)
            {
                output.push_back(output_struct(block.getDictionary().getClear(), 12));
                block.getDictionary().clear();
//                block.getDictionary().addRecord(loaded_pixels);
            }

            // Removes all pixels from loaded sequence except for last one
            unsigned int tmp = loaded_pixels.back();
            loaded_pixels.clear();
            loaded_pixels.push_back(tmp);

            // Determines color of last loaded pixel
            last_found = output_struct(block.getDictionary().findColor(tmp),
                                       block.getDictionary().getCurrentSize());
        }
    }

    // Stores last record to output
    if (!loaded_pixels.empty())
    {
        output.push_back(last_found);
        cout << setw(6) << last_found.index << setw(6) << last_found.size << setw(6) << block.dictionary.getLastIndex() << endl;
    }

    // Stores EOI
    output.push_back(output_struct(block.getDictionary().getEOI(), block.getDictionary().getCurrentSize()));
    return output;
}

void GIFencoder::writeImageDescriptor(SubBlock &block)
{
    // Image separator
    this->writer.write(0x2C, 8);
    // Image left
    this->writer.write(block.offset_x, 8);
    this->writer.write(block.offset_x >> 8, 8);
    // Image right
    this->writer.write(block.offset_y, 8);
    this->writer.write(block.offset_y >> 8, 8);
    // Image width
    this->writer.write(block.width, 8);
    this->writer.write(block.width >> 8, 8);
    // Image height
    this->writer.write(block.height, 8);
    this->writer.write(block.height >> 8, 8);
    // Packed field
    this->writer.write(128 | block.dictionary.getPaletteSize(), 8);
}

void GIFencoder::writePalette(SubBlock &block)
{
    // Write color palette
    for (vector<unsigned int>::iterator it = block.getDictionary().getPalette().begin();
         it != block.getDictionary().getPalette().end();
         it ++)
        this->writer.write(*it, 24);
}

void GIFencoder::writeData(vector<output_struct> &output)
{
    this->writer.writeVals();

    // LZW Min code size
    this->writer.write(output.front().size-1, 8);

    unsigned int rest_length = 0;
    unsigned int rest_of_data = 0;

    // While there is something to write
    while (!output.empty())
    {
        // Current length is 0 + rest_length
        unsigned int length = rest_length;

        // Determines number of bites that can be read
        bool broken = false;
        for (vector<output_struct>::iterator it = output.begin();
             it != output.end();
             it++)
        {
            if (length + (*it).size > 255 * 8)
            {
                broken = true;
                break;
            }

            length += (*it).size;
        }

        // Size of data in bytes
        unsigned int velikost = 0;
        if (broken)
        {
            velikost = 255;
        }
        else if ((length)%8 == 0)
        {
            velikost = (length)/8;
        }

        else
        {
            velikost = (length)/8+1;
        }
        writer.writeOut(length, velikost);
        writer.write(velikost, 8);
        cout << "za" << endl;

        int i = 0;
        if (rest_length != 0)
        {
            writer.write(rest_of_data, rest_length);
            i+= rest_length;
        }

        for (; i < length;)
        {
            if (output.empty())
                throw "Not all data was written";

            i += output.front().size;
            this->writer.write(output.front().index, output.front().size);
            output.erase(output.begin());
            cout << dec << setw(6) << output.front().index << setw(6) << output.front().size << endl;
        }

        // Splits last record to next frame
        if (output.size() != 0 && (2040 - length != 0))
        {
            unsigned int to_be_written = 2040 - length;
            i+= to_be_written;
            rest_length = output.front().size - to_be_written;
            length = rest_length;
            rest_of_data = output.front().index >> to_be_written;
            writer.write(output.front().index, to_be_written);
            output.erase(output.begin());
            cout << setw(6) << output.front().index << setw(6) << output.front().size <<  " Rozdeleno mezi framy" << endl;
        }

        else
        {
            rest_length = 0;
            rest_of_data = 0;
        }
        cerr << i << endl;
    }
    writer.finish();
    writer.write(0, 8);
}

void GIFencoder::writeSubBlock(SubBlock &block)
{
    // Encodes using LZW
    vector<output_struct> output = this->LZW(block);

    // No data
    if (output.empty())
        return;

    // Writes image Descriptor
    this->writeImageDescriptor(block);

    // Writes color palette
    this->writePalette(block);

    // Writes data
    this->writeData(output);
}
