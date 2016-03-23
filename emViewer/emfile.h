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

    // copy constructor
    EmFile(const EmFile& emFile);
    // assignment operator
    EmFile& operator = (const EmFile& emFile);

    void Dims(int* dims);
    void Type(unsigned char& type);
    void ByteData(std::vector<unsigned char>& fileDataByte);
    void IntData(std::vector<int>& fileDataInt);
    void FloatData(std::vector<float>& fileDataFloat);

private:

    unsigned char magic[1];
    char dummya[2];
    unsigned char type[1];
    int dims[3];
    char comment[80];
    int emdata[40];
    char dummyb[256];

    std::vector<unsigned char> embytedata;
    std::vector<int> emintdata;
    std::vector<float> emfloatdata;
};

#endif // EMFILE_H
