#ifndef GIFWRITER_H
#define GIFWRITER_H

#include <sstream>
#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>

using namespace std;

class GIFwriter
{
private:
    ofstream file;

public:
    unsigned int overflow;
    unsigned long long data;
    GIFwriter(const string & filename);
    ~GIFwriter();
    void write(const int &data, unsigned int length);
    void write(const char *data, unsigned int length);
    void finish();
    void writeOut(unsigned int i, unsigned int i2);
};

#endif // GIFWRITER_H
