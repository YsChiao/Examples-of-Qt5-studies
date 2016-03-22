#include "globject.h"
#include <cmath>
#include <vector>
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL)
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL)
VTK_MODULE_INIT(vtkInteractionStyle)

#include <vtkSmartPointer.h>
#include <vtkImageActor.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkImageData.h>
#include <vtkImageMapper.h>
#include <vtkImageProperty.h>
#include <vtkImageLuminance.h>
#include <vtkImageIdealHighPass.h>
#include <vtkDataSetMapper.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <vtkSmartVolumeMapper.h>


#include <QFileDialog>
#include <QString>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QVector>
#include <QVector3D>
#include <QVectorIterator>

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
            readFile(fileName);
            //SliceProcessing();
            VolumeProcessing();
        }
    }
    else
    {
        QMessageBox::information(this, tr("EM Viewer"), tr("No File!!!"));
        return ;
    }
}


void glObject::readFile(const QString& fileName)
{
    // read image path
    const char* infile = fileName.toStdString().c_str();

    // create em image object
    EmFile inemdata(infile);

    // load image data
    std::vector<float> data = inemdata.emfloatdata;
    fileDataFloat = QVector<float>::fromStdVector(data);

    // get size
    inemdata.Dims(dims);

    // get type
    std::string stdType;
    inemdata.Type(stdType);
    fileDataType = QString::fromStdString(stdType);
}

QSize glObject::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize glObject::sizeHint() const
{
    return QSize(400,400);
}

void glObject::FileDataToSliceVtkImageData(int& zAix, vtkImageData* imageData)
{
    // get size of origin data
    int xLength = dims[0];
    int yLength = dims[1];
    int length = fileDataFloat.length();
    int offset  = xLength*yLength*zAix;
    int threshold = 0;

    // Specify the size of the image data
    imageData->SetDimensions(xLength, yLength, 1);

    // read file data as unsigned char from the original data format
    imageData->AllocateScalars(VTK_FLOAT, 1);

    // remove noise as the value less than the threshold in the draw data
    // as within the draw data, there are so many noise inside
    for(int i = 0; i < length; i++)
    {
        if(fileDataFloat[i] < threshold)
        {
            fileDataFloat[i] = 0;
        }
    }

    // as the data between 0 to 1
    //==========================================================================================
    // normalize the data from -3sigma to 3sigma
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
    //================================================================================
    // as convert the data format from original to unsigned char
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


void glObject::FileDataToVtkImageData(vtkImageData* imageData)
{    
    // get size of origin data
    int xLength = dims[0];
    int yLength = dims[1];
    int zLength = dims[2];

    // Specify the size of the image data
    imageData->SetDimensions(xLength, yLength, zLength);
    imageData->SetSpacing(1.0, 1.0, 1.0);
    imageData->SetOrigin(0.0, 0.0, 0.0);

    // read file data as unsigned char from the original data format
    imageData->AllocateScalars(VTK_UNSIGNED_CHAR, 1);


    // remove noise as the value less than 2 in the draw data
    int length = fileDataFloat.length();
    for(int i = 0; i < length; i++)
    {
        if(fileDataFloat[i] < 2)
        {
            fileDataFloat[i] = 0;
        }
    }

    //==========================================================================================
    // normalize the data from -3sigma to 3sigma
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
    //================================================================================

    // fill every entry of the image with data
    int temp = 0;
    for(int z = 0; z < zLength; z++)
    {
        for(int y = 0; y < yLength; y++)
        {
            for(int x=0; x < xLength; x++)
            {
                unsigned char* pixel = static_cast<unsigned char*>(imageData->GetScalarPointer(x, y, z));
                // cast to 0 to 255
                pixel[0] = fileDataFloat[temp] * 255;
                temp++;
            }
        }
    }
    imageData->Modified();
}


void glObject::drawVolume(vtkImageData* imageData)
{
    // disable the warning windows pop.
    vtkObject::GlobalWarningDisplayOff();

    // The volume will be displayed by smart volume rendering compositing.
    vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
    volumeMapper->SetInputData(imageData);

    // The opacity transfer function
    vtkSmartPointer<vtkPiecewiseFunction> opacityTransferFunction = vtkSmartPointer<vtkPiecewiseFunction>::New();
    opacityTransferFunction->AddPoint(0, 0.0);// intensity and opacity
    opacityTransferFunction->AddPoint(16, 1.0);

    // The gradient opacity function is used to decrease the opacity in the "flat" regions
    // of the volume while maintaining the opacity
    // at the boundaries between tissue types.  The gradient is measured
    // as the amount by which the intensity changes over unit distance.
    // For most medical data, the unit distance is 1mm.
    vtkSmartPointer<vtkPiecewiseFunction> volumeGradientOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
    volumeGradientOpacity->AddPoint(0, 0.0);
    volumeGradientOpacity->AddPoint(90, 0.5);
    volumeGradientOpacity->AddPoint(100, 1.0);

    // color transfer function
    vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction =  vtkSmartPointer<vtkColorTransferFunction>::New();
    colorTransferFunction->AddRGBPoint(0.0,  0, 0, 0);
    colorTransferFunction->AddRGBPoint(16.0, 1, 1, 1);

    // set volume propoperties
    vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
    volumeProperty->SetScalarOpacity(opacityTransferFunction);
    volumeProperty->SetGradientOpacity(volumeGradientOpacity);
    volumeProperty->SetColor(colorTransferFunction);
    volumeProperty->SetInterpolationType(VTK_LINEAR_INTERPOLATION);
    volumeProperty->ShadeOn();


    // The vtkVolume is a vtkProp3D (like a vtkActor) and controls the position
    // and orientation of the volume in world coordinates.
    vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
    volume->SetMapper(volumeMapper);
    volume->SetProperty(volumeProperty);

    // VTK Render
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    //renderer->AddActor(imageActor);
    renderer->AddVolume(volume);

    // VTK/Qtwidget
    GetRenderWindow()->AddRenderer(renderer);


}

void glObject::drawSlice(vtkImageData* imageData)
{
    vtkSmartPointer<vtkDataSetMapper> imageMapper = vtkSmartPointer<vtkDataSetMapper>::New();
    imageMapper->SetInputData(imageData);

    vtkSmartPointer<vtkActor> imageActor = vtkSmartPointer<vtkActor>::New();
    imageActor->SetMapper(imageMapper);

    // VTK Render
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(imageActor);

    // VTK/Qtwidget
    GetRenderWindow()->AddRenderer(renderer);
}

void glObject::SliceProcessing()
{
    vtkImageData *fileDataSliceVtkImageData = vtkImageData::New();

    // get middle slice
    int zAix  = (int)(dims[2]/2);
    // convert float volume data to vtkImageData
    FileDataToSliceVtkImageData(zAix, fileDataSliceVtkImageData);

    // display 2D volumn
    drawSlice(fileDataSliceVtkImageData);

    // status bar information
    Information();
}

void glObject::VolumeProcessing()
{

    vtkImageData *fileDataVtkImageData = vtkImageData::New();

    // convert float volume data to vtkImageData
    FileDataToVtkImageData(fileDataVtkImageData);

    // display 3D image
    drawVolume(fileDataVtkImageData);

    // status bar information
    Information();
}

void glObject::Information()
{
    // get size information
    QString sQStringx = QString::number(dims[0]);
    QString sQStringy = QString::number(dims[1]);
    QString sQStringz = QString::number(dims[2]);
    QString Message = QString("[" +sQStringx +"," +sQStringy +"," +sQStringz +"]" );
    emit sendMessage(Message);

}


















