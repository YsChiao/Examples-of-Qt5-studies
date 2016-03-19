#include "globject.h"
#include <cmath>
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL)
VTK_MODULE_INIT(vtkInteractionStyle)

#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkImageActor.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkInteractorStyleImage.h>

#include <vtkImageImport.h>

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
            processing();
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

}

void glObject::processing()
{
    QVector3D fileDataSize;
    QVector<float> fileDataFloat;

    // get size of volumn
    getFileSize(fileData, fileDataSize);
    // convert volumn data from binary to float
    FileDataBinaryToFloat(fileData, fileDataFloat);

//    //check data value
//    qDebug() << qPrintable(QString::number(fileDataFloat[0]));
//    qDebug() << qPrintable(QString::number(fileDataFloat[1]));
//    qDebug() << qPrintable(QString::number(fileDataFloat[2]));
//    qDebug() << qPrintable(QString::number(fileDataFloat[3]));
//    qDebug() << qPrintable(QString::number(fileDataFloat[4]));
//    qDebug() << qPrintable(QString::number(fileDataFloat[5]));
//    qDebug() << qPrintable(QString::number(fileDataFloat[6]));
//    qDebug() << qPrintable(QString::number(fileDataFloat[7]));

    draw(fileDataFloat, fileDataSize);
}


void glObject::draw(QVector<float>& fileDataFloat, QVector3D& fileDataSize )
{
    // size of image
    int x = fileDataSize.x();
    int y = fileDataSize.y();
    int z = fileDataSize.z();

    // data of image
    std::vector<float> data = fileDataFloat.toStdVector();
    float *Image = &data[0];


    // Convert the c-tyle image to a vtkImageData
    vtkSmartPointer<vtkImageImport> imageImport =
            vtkSmartPointer<vtkImageImport>::New();
    imageImport->SetDataSpacing(1,1,1);
    imageImport->SetDataOrigin(0,0,0);
    imageImport->SetWholeExtent(0, x-1, 0, y-1, 0, z-1);
    imageImport->SetDataExtentToWholeExtent();
    imageImport->SetDataScalarTypeToUnsignedChar();
    imageImport->SetNumberOfScalarComponents(1);
    imageImport->SetImportVoidPointer(Image);
    imageImport->Update();

    // Create an actor
    vtkSmartPointer<vtkImageActor> actor =
            vtkSmartPointer<vtkImageActor>::New();
#if VTK_MAJOR_VERSION <= 5
    actor->SetInput(imageImport->GetOutput());
#else
    actor->SetInputData(imageImport->GetOutput());
#endif

    // Setup renderer
    vtkSmartPointer<vtkRenderer> renderer =
            vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);
    renderer->ResetCamera();

    // Setup render window
    vtkSmartPointer<vtkRenderWindow> renderWindow =
            vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);

    // Setup render window interactor
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtkSmartPointer<vtkInteractorStyleImage> style =
            vtkSmartPointer<vtkInteractorStyleImage>::New();

    renderWindowInteractor->SetInteractorStyle(style);

    // Render and start interaction
    renderWindowInteractor->SetRenderWindow(renderWindow);
    renderWindowInteractor->Initialize();

    renderWindowInteractor->Start();
}














