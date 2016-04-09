// transplanted from C

#include "emfile.h"
#include <unistd.h>

EmFile::EmFile()
{

}

EmFile::EmFile(const char *infile)
{
    FILE* input = NULL;
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
        int* intdata;
        if (!(intdata = new int[size*2]))
        {
            std::cout<< "could not allocate memory" << std::endl;
            exit (1);
        }
        fread (intdata, 2, size, input);
        for (int i = 0; i < size; i++)
        {
            emintdata.push_back(intdata[i]);
        }
        delete [] intdata;
        break;

    case 5:
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

    default:
        std::cout<< "file data type dosn't support, error!" << std::endl;
    }

    fclose (input);
}

EmFile::EmFile(const EmFile& emFile)
{
    // copy arrays
    std::copy(emFile.magic, emFile.magic+1, magic);
    std::copy(emFile.dummya, emFile.dummya+2, dummya);
    std::copy(emFile.type, emFile.type+1, type);
    std::copy(emFile.dims, emFile.dims+3, dims);
    std::copy(emFile.comment, emFile.comment+80, comment);
    std::copy(emFile.emdata, emFile.emdata+40, emdata);
    std::copy(emFile.dummyb, emFile.dummyb+256, dummyb);

    embytedata = emFile.embytedata;
    emintdata = emFile.emintdata;
    emfloatdata = emFile.emfloatdata;
}

EmFile& EmFile::operator = (const EmFile& emFile)
{
    // check for self-assignment
    if(this == &emFile)
    {
        return *this;
    }

    std::copy(emFile.magic, emFile.magic+1, magic);
    std::copy(emFile.dummya, emFile.dummya+2, dummya);
    std::copy(emFile.type, emFile.type+1, type);
    std::copy(emFile.dims, emFile.dims+3, dims);
    std::copy(emFile.comment, emFile.comment+80, comment);
    std::copy(emFile.emdata, emFile.emdata+40, emdata);
    std::copy(emFile.dummyb, emFile.dummyb+256, dummyb);

    embytedata = emFile.embytedata;
    emintdata = emFile.emintdata;
    emfloatdata = emFile.emfloatdata;

    return *this;
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

void EmFile::Type(unsigned char& vtype)
{
    switch(type[0])
    {
    case 1:
    {
        vtype = 1;
    }
    case 2:
    {
        vtype = 2;
    }
    case 5:
    {
        vtype = 5;
    }
    }
}

void EmFile::ByteData(std::vector<unsigned char>& fileDataByte)
{
    fileDataByte = embytedata;
}

void EmFile::IntData(std::vector<int>& fileDataInt)
{
    fileDataInt = emintdata;
}

void EmFile::FloatData(std::vector<float>& fileDataFloat)
{
    fileDataFloat = emfloatdata;
}












