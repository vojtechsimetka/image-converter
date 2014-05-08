#ifndef GIFDICTIONARY_H
#define GIFDICTIONARY_H

#include <iostream>
#include <set>

using namespace std;

class GIFdictionary
{
private:
    vector<unsigned int> palette;
    vector<vector<unsigned int> >dictionary;
    unsigned int sz;
    unsigned int palette_size;
    unsigned int last_record;

public:
    GIFdictionary()
    {
        this->sz = 0;
        this->palette_size = 0;
        this->last_record = -1;
    }

    inline void addColors(set<unsigned int> & colors)
    {
        // Saves colors
        for (set<unsigned int>::iterator it = colors.begin();
             it != colors.end();
             it++)
        {
            this->palette.push_back(*it);
            this->last_record++;

            if (this->last_record > pow(2, this->sz+1))
                this->sz++;
        }

        while (this->sz < 1)
            this->sz++;

        // Fills rest with black color
        for (;this->last_record < pow(2, this->sz+1)-1;)
        {
            this->last_record++;
            this->palette.push_back(0);
        }

        this->palette_size = this->sz;

        // Clear code and EOI
        this->last_record+=2;
        this->sz++;
    }

    inline int findColor(unsigned int color)
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
                return i + this->palette.size()+2;
        }

        return -1;
    }

    inline int addRecord(vector<unsigned int> & record)
    {
        this->dictionary.push_back(vector<unsigned int>(record));
        this->last_record++;

        if (this->last_record >= pow(2, this->sz+1))
            this->sz++;

        return this->last_record;
    }

    inline unsigned int getClear() const
    {
        return this->palette.size();
    }

    inline unsigned int getEOI() const
    {
        return this->palette.size()+1;
    }

    inline unsigned int getPaletteSize() const
    {
        return this->palette_size;
    }

    inline vector<unsigned int> & getPalette()
    {
        return this->palette;
    }

    inline unsigned int getCurrentSize()
    {
        return this->sz+1;
    }

    inline unsigned int getLastIndex()
    {
        return this->last_record;
    }

    inline void clear()
    {
        this->dictionary.clear();
        this->last_record = this->palette.size()+1;
        this->sz = palette_size + 1;
    }
};

#endif // GIFDICTIONARY_H
