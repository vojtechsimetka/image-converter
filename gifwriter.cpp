#include "gifwriter.h"

unsigned long long cnt = 0;

GIFwriter::GIFwriter(const string & filename)
{
    this->write_values = false;
    this->overflow = 0;
    this->data = 0;

    this->file.open (filename, ios::out | ios::binary);

    if (file.bad())
        throw "Unable to open output gif file";
}

GIFwriter::~GIFwriter()
{
    this->file.close();
}

void GIFwriter::writeOut(unsigned int i, unsigned int i2)
{
    cerr << dec<< "velikost bloku dat " << i<<"b="<< i2<<  "B je na adrese: "<< dec << cnt << endl;
    cout << "velikost bloku dat " << i<<"b="<< i2<<  "B je na adrese: "<< dec << cnt << endl;
}

void GIFwriter::write(const int &data, unsigned int length)
{
    this->data |= data << this->overflow;
    this->overflow += length;
    while (this->overflow >= 8)
    {
        if (this->write_values)
        {
            std::cout << std::hex;
            cout << setw(24) << static_cast<int>(this->data & 0xFF) << endl;
        }
        this->file << static_cast<char>(this->data & 0xFF);
        this->data = this->data >> 8;
        this->overflow -= 8;
        cnt ++;
    }
}

void GIFwriter::write(const char * data, unsigned int length)
{
    for (int i = 0; i < length/8; i++)
        this->write(static_cast<int>(data[i]), 8);
}

void GIFwriter::finish()
{
    if (this->overflow != 0)
        this->write(0, 8 - this->overflow);
}
