#include "globject.h"
#include <cmath>
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL)
VTK_MODULE_INIT(vtkInteractionStyle)

#include <vtkSmartPointer.h>
#include <vtkImageActor.h>
#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkImageData.h>
#include <vtkImageMapper.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>

#include <vtkDataSetMapper.h>

#include <QFileDialog>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QVector3D>

glObject::glObject(QWidget* parent)
    : QVTKWidget(parent)
{

}

glObject::~glObject()
{

}

void glObject::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::information(this, tr("EM Viewer"), tr("Cannot load %1.").arg(fileName));
            return;
        }
        else
        {
            fileData = file.readAll();
            SliceProcessing();
            //VolumeProcessing();
        }
    }
    else
    {
        QMessageBox::information(this, tr("EM Viewer"), tr("No File!!!"));
        return ;
    }
}

QSize glObject::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize glObject::sizeHint() const
{
    return QSize(400,400);
}

void glObject::getFileSize(const QByteArray& fileData, QVector3D& fileDataSize)
{
    // concatenation the binary data and get the size of volumn
    unsigned int x, y, z;
    unsigned char a, b, c, d;
    for(int n = 4; n < 16; n += 4)
    {
        a = (unsigned char)fileData.at(n);
        b = (unsigned char)fileData.at(n+1);
        c = (unsigned char)fileData.at(n+2);
        d = (unsigned char)fileData.at(n+3);

        if(n == 4)
        {
            x = a | (b<<8) | (c<<16) | (d<<24);
        }
        if(n == 8)
        {
            y = a | (b<<8) | (c<<16) | (d<<24);
        }
        if(n == 12)
        {
            z = a | (b<<8) | (c<<16) | (d<<24);
        }
    }
    //    qDebug() << qPrintable(QString::number(x,16));
    //    qDebug() << qPrintable(QString::number(y,16));
    //    qDebug() << qPrintable(QString::number(z,16));

    //    int x_size = (int)x;
    //    int y_size = (int)y;
    //    int z_size = (int)z;

    //    qDebug() << qPrintable(QString::number(x_size,10));
    //    qDebug() << qPrintable(QString::number(y_size,10));
    //    qDebug() << qPrintable(QString::number(z_size,10));


    // concatenation of input volumn size by format [x,y,z] and show on the stautsbar.
    QString sQStringx = QString::number(x);
    QString sQStringy = QString::number(y);
    QString sQStringz = QString::number(z);
    QString Message = QString("[" + sQStringx +","+ sQStringy +","+ sQStringz+"]");
    emit sendMessage(Message);

    fileDataSize.setX(float(x));
    fileDataSize.setY(float(y));
    fileDataSize.setZ(float(z));

}


void glObject::FileDataBinaryToFloat(const QByteArray& fileData, QVector<float>& fileDataFloat)
{
    int fileHead = 512;
    int typeLength = 4;

    // concatenation the binary data
    unsigned int value;
    unsigned char a, b, c, d;
    for(int n = fileHead; n < fileData.length(); n = n + typeLength)
    {
        a = (unsigned char)fileData.at(n);
        b = (unsigned char)fileData.at(n+1);
        c = (unsigned char)fileData.at(n+2);
        d = (unsigned char)fileData.at(n+3);

        value = a | (b<<8) | (c<<16) | (d<<24);
        int sign = (value & 0x80000000) ? -1 : 1;
        float exponent = (int)((value >> 23)&0xff) - 127;
        float mantissa = 1 + (float)((value & 0x7fffff)) / (float)(0x7fffff);
        float valueFloat =  sign * mantissa * std::pow(2.0, exponent);
        fileDataFloat.push_back(valueFloat);
    }

    // normalize the data from -3sigma to 3sigma
    int length = fileDataFloat.length();
    float min = *std::min_element(fileDataFloat.constBegin(),fileDataFloat.constEnd());
    float max = *std::max_element(fileDataFloat.constBegin(),fileDataFloat.constEnd());

    float sum = 0;
    for(int i = 0; i < fileDataFloat.length(); i++)
    {
        sum += fileDataFloat[i];
    }

    float mean = 0;
    float sigma = 0;
    mean = sum / length;
    for(int i = 0; i < length; i++)
    {
        sigma += ((fileDataFloat[i] - mean) * (fileDataFloat[i] - mean));
    }
    sigma = std::sqrt(sigma/length);

    //    qDebug() << qPrintable(QString::number(min));
    //    qDebug() << qPrintable(QString::number(max));
    //    qDebug() << qPrintable(QString::number(length));
    //    qDebug() << qPrintable(QString::number(mean));
    //    qDebug() << qPrintable(QString::number(sum));
    //    qDebug() << qPrintable(QString::number(sigma));
    //    qDebug() << "Data.................................";

    // normalization
    for(int i = 0; i < length; i++)
    {
        fileDataFloat[i] = ((fileDataFloat[i] - min) / (max - min));
    }

    // smooth
    for(int i = 0; i < length; i++)
    {
        if(fileDataFloat[i] > (3*sigma + mean))
        {
            fileDataFloat[i] = 1;
        }
        if(fileDataFloat[i] < (-3*sigma + mean))
        {
            fileDataFloat[i] = 0;
        }
    }

    //    //check data value
    //    qDebug() << qPrintable(QString::number(fileDataFloat[0]));
    //    qDebug() << qPrintable(QString::number(fileDataFloat[1]));
    //    qDebug() << qPrintable(QString::number(fileDataFloat[2]));
    //    qDebug() << qPrintable(QString::number(fileDataFloat[3]));
    //    qDebug() << qPrintable(QString::number(fileDataFloat[4]));
    //    qDebug() << qPrintable(QString::number(fileDataFloat[5]));
    //    qDebug() << qPrintable(QString::number(fileDataFloat[6]));
    //    qDebug() << qPrintable(QString::number(fileDataFloat[7]));

}


void glObject::FileDataToSliceVtkImageData(QVector<float>& fileDataFloat, const QVector3D& fileDataSize,
                                           int& zAix, vtkImageData* imageData)
{
    // get size of origin data
    int xLength = fileDataSize.x();
    int yLength = fileDataSize.y();
    int offset  = xLength*yLength*zAix;

    // Specify the size of the image data
    imageData->SetDimensions(xLength, yLength, 1);
    imageData->AllocateScalars(VTK_FLOAT, 1);

    // fill every entry of the image with data
    int temp = 0;
    for(int y = 0; y < yLength; y++)
    {
        for(int x=0; x < xLength; x++)
        {
            float* pixel = static_cast<float*>(imageData->GetScalarPointer(x, y, 0));
            pixel[0] = fileDataFloat[temp+offset];
            temp++;
        }
    }
    imageData->Modified();
}


void glObject::FileDataToVtkImageData(QVector<float>& fileDataFloat, const QVector3D& fileDataSize,
                                      vtkImageData* imageData)
{
    int xLength = fileDataSize.x();
    int yLength = fileDataSize.y();
    int zLength = fileDataSize.z();
    // create an image data
    //vtkSmartPointer<vtkImageData> imageData = vtkSmartPointer<vtkImageData>::New();

    // Specify the size of the image data
    imageData->SetDimensions(xLength, yLength, zLength);
    imageData->SetSpacing(1.0, 1.0, 1.0);
    imageData->SetOrigin(0.0, 0.0, 0.0);
    imageData->AllocateScalars(VTK_FLOAT, 1);

    //    int numberOfPoints = imageData->GetNumberOfPoints();
    //    int numberOfCells  = imageData->GetNumberOfCells();
    //    qDebug() << "Number of points" << qPrintable(QString::number(numberOfPoints));
    //    qDebug() << "Number of Cells"  << qPrintable(QString::number(numberOfCells));

    // fill every entry of the image with data
    int temp = 0;
    for(int z = 0; z < zLength; z++)
    {
        for(int y = 0; y < yLength; y++)
        {
            for(int x=0; x < xLength; x++)
            {
                float* pixel = static_cast<float*>(imageData->GetScalarPointer(x, y, z));
                pixel[0] = fileDataFloat[temp];
                temp++;
            }
        }
    }
    imageData->Modified();
}


void glObject::drawVolume(vtkImageData* imageData)
{
    vtkSmartPointer<vtkImageData> imageSource = vtkSmartPointer<vtkImageData>::New();
    imageSource = imageData;

    vtkSmartPointer<vtkDataSetMapper> imageMapper = vtkSmartPointer<vtkDataSetMapper>::New();
    imageMapper->SetInputData(imageSource);

    vtkSmartPointer<vtkActor> imageActor = vtkSmartPointer<vtkActor>::New();
    imageActor->SetMapper(imageMapper);

    // VTK Render
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(imageActor);

    // VTK/Qtwidget
    GetRenderWindow()->AddRenderer(renderer);

}

void glObject::drawSlice(vtkImageData* imageData)
{
    vtkSmartPointer<vtkImageData> imageSource = vtkSmartPointer<vtkImageData>::New();
    imageSource = imageData;

    vtkSmartPointer<vtkImageMapper> imageMapper = vtkSmartPointer<vtkImageMapper>::New();
    imageMapper->SetInputData(imageSource);
    imageMapper->SetColorWindow(255);
    imageMapper->SetColorLevel(127.5);

    vtkSmartPointer<vtkActor2D> imageActor = vtkSmartPointer<vtkActor2D>::New();
    imageActor->SetMapper(imageMapper);

    // VTK Render
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(imageActor);

    // VTK/Qtwidget
    GetRenderWindow()->AddRenderer(renderer);

}

void glObject::SliceProcessing()
{
    QVector<float> fileDataFloat;
    QVector3D fileDataSize;
    vtkImageData *fileDataSliceVtkImageData = vtkImageData::New();

    // get size of volume
    getFileSize(fileData, fileDataSize);
    // convert volumn data from binary to float
    FileDataBinaryToFloat(fileData, fileDataFloat);
    // get middle slice
    int zAix = (int)(fileDataSize.z()/2);
    FileDataToSliceVtkImageData(fileDataFloat, fileDataSize, zAix, fileDataSliceVtkImageData);

    // display 2D volumn
    drawVolume(fileDataSliceVtkImageData);


}

void glObject::VolumeProcessing()
{
    QVector<float> fileDataFloat;
    QVector3D fileDataSize;
    vtkImageData *fileDataVtkImageData = vtkImageData::New();

    // get size of volume
    getFileSize(fileData, fileDataSize);
    // convert volumn data from binary to float
    FileDataBinaryToFloat(fileData, fileDataFloat);

    // convert float volume data to vtkImageData
    FileDataToVtkImageData(fileDataFloat, fileDataSize, fileDataVtkImageData);

    //    // check data
    //    float* pixel = static_cast<float*>(fileDataVtkImageData->GetScalarPointer(7,0,0));
    //    qDebug() << qPrintable(QString::number(pixel[0]));


    // display 3D image
    drawSlice(fileDataVtkImageData);
}














