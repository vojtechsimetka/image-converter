#ifndef GIFWRITER_H
#define GIFWRITER_H

#include <fstream>

using namespace std;

class GIFwriter
{
private:
    unsigned int overflow;
    unsigned long long data;
    ofstream file;

public:
    GIFwriter(const string & filename);
    ~GIFwriter();
    void write(const int &data, unsigned int length);
    void write(const char *data, unsigned int length);
    void finish();
};

#endif // GIFWRITER_H
