#ifndef GIFDICTIONARY_H
#define GIFDICTIONARY_H

#include <iostream>
#include<vector>

using namespace std;

class GIFdictionary
{
private:
    vector<unsigned int>palette;
    vector<vector<unsigned int> >dictionary;
public:
    inline int addColor(unsigned int color)
    {
        this->palette.push_back(color);
        return this->palette.size() -1;
    }

    inline int isInPalette(unsigned int color)
    {
        unsigned int i = 0;
        for (vector<unsigned int>::iterator it = this->palette.begin();
             it != this->palette.end();
             i++, it++)
        {
            if (*it == color)
                return i;
        }
        return -1;
    }

    inline int find(vector<unsigned int> & record)
    {
        unsigned int i = 0;
        for (vector<vector<unsigned int> >::iterator it = this->dictionary.begin();
             it != this->dictionary.end();
             it ++, i++)
        {
            if (record == (*it))
                return i;
        }

        return -1;
    }

    inline int addRecord(vector<unsigned int> & record)
    {
        this->dictionary.push_back(vector<unsigned int>(record));
        return this->dictionary.size() -1;
    }

    inline unsigned int size()
    {
        return this->dictionary.size() + this->palette.size() + 2;
    }

    inline unsigned int recordLength()
    {
        unsigned int i = 0;
        unsigned int out = 2;
        for (; out < this->size(); i++)
            out *=2;

        return i;
    }

    inline unsigned int getClear()
    {
        return pow(2, this->getPaletteSize()+1);
    }

    inline unsigned int getEOI()
    {
        return pow(2, this->getPaletteSize()+1) + 1;
    }

    inline unsigned int getPaletteSize()
    {
        unsigned int i = 0;
        unsigned int out = 2;
        for (; out < this->palette.size(); i++)
            out *=2;

        return i;
    }

    inline vector<unsigned int> & getPalette()
    {
        return this->palette;
    }
};

#endif // GIFDICTIONARY_H
