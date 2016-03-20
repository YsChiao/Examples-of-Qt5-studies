// transplanted from C

#include "emfile.h"
#include <unistd.h>

EmFile::EmFile()
{

}

EmFile::EmFile(const char *infile)
{
    FILE* input = 0;
    long size;

    if((input = fopen(infile, "rb")) == 0)
    {
        printf("could not open %s\n", infile);
        exit(1);
    }

    fread (magic,   1, 1,   input);
    fread (dummya,  1, 2,   input);
    fread (type,    1, 1,   input);
    fread (dims,    4, 3,   input);
    fread (comment, 1, 80,  input);
    fread (emdata,  4, 40,  input);
    fread (dummyb,  1, 256, input);

    size = dims[0] * dims[1] * dims[2];

    switch(type[0])
    {
    case 1:
        // std::cout <<"type : unsigned char "<< std::endl;
        unsigned char* bytedata;
        if (!(bytedata = new unsigned char[size]))
        {
            std::cout<< "could not allocate memory" << std::endl;
            exit (1);
        }
        fread (bytedata, 1, size, input);
        for (int i = 0; i < size; i++)
        {
            embytedata.push_back(bytedata[i]);
        }
        delete [] bytedata;
        break;

    case 2:
        // std::cout <<"type : int "<< std::endl;
        int* intdata;
        if (!(intdata = new int[size*2]))
        {
            std::cout<< "could not allocate memory" << std::endl;
            exit (1);
        }
        fread (intdata, 2, size, input);
        for (int i = 0; i < size; i++)
        {
            emintata.push_back(intdata[i]);
        }
        delete [] intdata;
        break;

    case 5:
        // std::cout <<"type : float "<< std::endl;
        float* floatdata;
        if (!(floatdata= new float[size*4]))
        {
            std::cout<< "could not allocate memory" << std::endl;
            exit (1);
        }
        fread (floatdata, 4, size, input);
        for (int i = 0; i < size; i++)
        {
            emfloatdata.push_back(floatdata[i]);
        }
        delete [] floatdata;
        break;
    }

    fclose (input);
}

EmFile::~EmFile()
{
}


void EmFile::Dims(int* vdims)
{
    vdims[0] = dims[0];
    vdims[1] = dims[1];
    vdims[2] = dims[2];
}

void EmFile::Type(std::string &vtype)
{
    switch(type[0])
    {
    case 1:
    {
        vtype = "unsigned char,8bit";
    }
    case 2:
    {
        vtype = "int,16bit";
    }
    case 5:
    {
        vtype = "float,32bit";
    }
    }
}


