#ifndef EMFILE_H
#define EMFILE_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <iterator>


class EmFile
{
public:

    EmFile();
    EmFile(const char* infile);
    ~EmFile();


    void Dims(int* dims);
    void Type(std::string&);

    std::vector<unsigned char> embytedata;
    std::vector<int> emintata;
    std::vector<float> emfloatdata;


private:
    EmFile(const EmFile&);
    EmFile& operator = (const EmFile&);

    unsigned char magic[1];
    char dummya[2];
    unsigned char type[1];
    int dims[3];
    char comment[80];
    int emdata[40];
    char dummyb[256];
};

#endif // EMFILE_H
