#include <QFileDialog>
#include <QString>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QVector>
#include <QTime>


#include "globject.h"

glObject::glObject(QWidget* parent)
    : QVTKWidget(parent)
{
    imageData = vtkSmartPointer<vtkImageData>::New();
    volumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
    opacityTransferFunction = vtkSmartPointer<vtkPiecewiseFunction>::New();
    volumeGradientOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
    colorTransferFunction =  vtkSmartPointer<vtkColorTransferFunction>::New();
    volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
    volume = vtkSmartPointer<vtkVolume>::New();
    renderer = vtkSmartPointer<vtkRenderer>::New();
}

glObject::~glObject()
{
}

void glObject::open()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());

    // get file name
    QFileInfo fileInfo(fileName);
    QString name = fileInfo.fileName();
    emit sendFileName(name);

    // read file and check
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
            // first value of level set
            readFile(fileName);
            Processing();
            emit renderDone();
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

    // default value of level
    levelValue = 0;

    // create em image object
    EmFile inemdata(infile);
    emFile = inemdata;


}

void glObject::FileDataMaxMin()
{
    // sent max and min value to QDialog "tools"
    float min = *std::min_element(fileDataFloat.constBegin(),fileDataFloat.constEnd());
    float max = *std::max_element(fileDataFloat.constBegin(),fileDataFloat.constEnd());
    emit sendMaxMin(max, min);
}

void glObject::getLevel(int value)
{
    // as same as magnifier in class glOperator
    levelValue = float(value)/10 ;
    //std::cout << "levelValue : " << levelValue << std::endl;
    Processing();
}

void glObject::Processing()
{
    // get type
    unsigned char fileType;
    emFile.Type(fileType);

    // load image data
    switch(fileType)
    {
    case 1:
    {
        std::vector<unsigned char> fileByte;
        emFile.ByteData(fileByte);
        // copy data form std to qvector
        QVector<unsigned char> temp = QVector<unsigned char>::fromStdVector(fileByte);
        fileDataByte = temp;
    }
    case 2:
    {
        std::vector<int> fileInt;
        emFile.IntData(fileInt);
        // copy data form std to qvector
        QVector<int> temp = QVector<int>::fromStdVector(fileInt);
        fileDataInt = temp;
    }
    case 5:
    {
        std::vector<float> fileFloat;
        emFile.FloatData(fileFloat);
        // copy data form std to qvector
        QVector<float> temp = QVector<float>::fromStdVector(fileFloat);
        fileDataFloat = temp;
    }
    }

    // get size
    emFile.Dims(dims);

    //SliceProcessing();
    VolumeProcessing();
}

QSize glObject::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize glObject::sizeHint() const
{
    return QSize(400,400);
}

//void glObject::FileDataToSliceVtkImageData(int& zAix, vtkImageData* imageData)
//{
//    // get size of origin data
//    int xLength = dims[0];
//    int yLength = dims[1];
//    int length = fileDataFloat.length();
//    int offset  = xLength*yLength*zAix;

//    // Specify the size of the image data
//    imageData->SetDimensions(xLength, yLength, 1);

//    // read file data as unsigned char from the original data format
//    imageData->AllocateScalars(VTK_FLOAT, 1);

//    // remove noise as the value less than the threshold in the draw data
//    // as within the draw data, there are so many noise inside
//    for(int i = 0; i < length; i++)
//    {
//        if(fileDataFloat[i] < 2)
//        {
//            fileDataFloat[i] = 0;
//        }
//    }

//    // as the data between 0 to 1
//    //==========================================================================================
//    // normalize the data from -3sigma to 3sigma
//    float min = *std::min_element(fileDataFloat.constBegin(),fileDataFloat.constEnd());
//    float max = *std::max_element(fileDataFloat.constBegin(),fileDataFloat.constEnd());

//    float sum = 0;
//    for(int i = 0; i < fileDataFloat.length(); i++)
//    {
//        sum += fileDataFloat[i];
//    }

//    float mean = 0;
//    float sigma = 0;
//    mean = sum / length;
//    for(int i = 0; i < length; i++)
//    {
//        sigma += ((fileDataFloat[i] - mean) * (fileDataFloat[i] - mean));
//    }
//    sigma = std::sqrt(sigma/length);

//    for(int i = 0; i < length; i++)
//    {
//        fileDataFloat[i] = ((fileDataFloat[i] - min) / (max - min));
//    }

//    // smooth
//    for(int i = 0; i < length; i++)
//    {
//        if(fileDataFloat[i] > (3*sigma + mean))
//        {
//            fileDataFloat[i] = 1;
//        }
//        if(fileDataFloat[i] < (-3*sigma + mean))
//        {
//            fileDataFloat[i] = 0;
//        }
//    }
//    //================================================================================
//    // as convert the data format from original to unsigned char
//    // fill every entry of the image with data
//    int temp = 0;
//    for(int y = 0; y < yLength; y++)
//    {
//        for(int x=0; x < xLength; x++)
//        {
//            float* pixel = static_cast<float*>(imageData->GetScalarPointer(x, y, 0));
//            pixel[0] = fileDataFloat[temp+offset];
//            temp++;
//        }
//    }
//    imageData->Modified();
//}


void glObject::FileDataToVtkImageData()
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

    //    //==========================================================================================
    //    // normalize the data from -3sigma to 3sigma
    //    min = 0;
    //    float sum = 0;
    //    for(int i = 0; i < fileDataFloat.length(); i++)
    //    {
    //        sum += fileDataFloat[i];
    //    }

    //    float mean = 0;
    //    float sigma = 0;
    //    mean = sum / length;
    //    for(int i = 0; i < length; i++)
    //    {
    //        sigma += ((fileDataFloat[i] - mean) * (fileDataFloat[i] - mean));
    //    }
    //    sigma = std::sqrt(sigma/length);


    //    //  normalize and smooth
    //    for(int i = 0; i < length; i++)
    //    {
    //        fileDataFloat[i] = ((fileDataFloat[i] - min) / (max - min));
    //        if (fileDataFloat[i] > (3*sigma + mean))
    //        {
    //            fileDataFloat[i] = 1;
    //        }
    //        else if (fileDataFloat[i] < (-3*sigma + mean))
    //        {
    //            fileDataFloat[i] = 0;
    //        }
    //    }
    //    //================================================================================

    const int numComponents = imageData->GetNumberOfScalarComponents();
    const int numElements = xLength*yLength*zLength*numComponents;
    unsigned char* pixel = static_cast<unsigned char*>(imageData->GetScalarPointer());
    for (int i = 0; i < numElements; i++)
    {
        // filter  data with level
        if (fileDataFloat[i] < levelValue)
        {
            pixel[i] = 0;
        }
        else
        {
            pixel[i] = fileDataFloat[i] * 255;
        }
    }
    imageData->Modified();

}


void glObject::drawVolume()
{
    // disable the warning windows pop.
    vtkObject::GlobalWarningDisplayOff();

    // The volume will be displayed by smart volume rendering compositing.
    volumeMapper->SetInputData(imageData);

    // The opacity transfer function
    opacityTransferFunction->AddPoint(0, 0.0);// intensity and opacity
    opacityTransferFunction->AddPoint(16, 1.0);

    // The gradient opacity function is used to decrease the opacity in the "flat" regions
    // of the volume while maintaining the opacity
    // at the boundaries between tissue types.  The gradient is measured
    // as the amount by which the intensity changes over unit distance.
    // For most medical data, the unit distance is 1mm.
    volumeGradientOpacity->AddPoint(0, 0.0);
    volumeGradientOpacity->AddPoint(90, 0.5);
    volumeGradientOpacity->AddPoint(100, 1.0);

    // color transfer function
    colorTransferFunction->AddRGBPoint(0.0,  0, 0, 0);
    colorTransferFunction->AddRGBPoint(16.0, 1, 1, 1);

    // set volume propoperties
    volumeProperty->SetScalarOpacity(opacityTransferFunction);
    volumeProperty->SetGradientOpacity(volumeGradientOpacity);
    volumeProperty->SetColor(colorTransferFunction);
    volumeProperty->SetInterpolationType(VTK_LINEAR_INTERPOLATION);
    volumeProperty->ShadeOn();

    // The vtkVolume is a vtkProp3D (like a vtkActor) and controls the position
    // and orientation of the volume in world coordinates.
    volume->SetMapper(volumeMapper);
    volume->SetProperty(volumeProperty);

    // VTK Renderer
    renderer->AddVolume(volume);

    // VTK/Qtwidget
    GetRenderWindow()->AddRenderer(renderer);

}

//void glObject::drawSlice(vtkImageData* imageData)
//{
//    vtkSmartPointer<vtkDataSetMapper> imageMapper = vtkSmartPointer<vtkDataSetMapper>::New();
//    imageMapper->SetInputData(imageData);

//    vtkSmartPointer<vtkActor> imageActor = vtkSmartPointer<vtkActor>::New();
//    imageActor->SetMapper(imageMapper);

//    // VTK Render
//    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
//    renderer->AddActor(imageActor);

//    // VTK/Qtwidget
//    GetRenderWindow()->AddRenderer(renderer);
//}

//void glObject::SliceProcessing()
//{
//    vtkImageData *fileDataSliceVtkImageData = vtkImageData::New();

//    // get middle slice
//    int zAix  = (int)(dims[2]/2);
//    // convert float volume data to vtkImageData
//    FileDataToSliceVtkImageData(zAix, fileDataSliceVtkImageData);

//    // display 2D volumn
//    drawSlice(fileDataSliceVtkImageData);

//    // status bar information
//    Information();
//}

void glObject::VolumeProcessing()
{
    // compute time consume
    QTime time_read;
    time_read.start();
    // get max min float data
    FileDataMaxMin();
    // convert float volume data to vtkImageData
    FileDataToVtkImageData(); // loading data;
    int readtime = time_read.elapsed();
    std::cout << "read and compute time : " << readtime << " ms" << std::endl;

    // compute time consume
    QTime time_rendering;
    time_rendering.start();
    // display 3D image
    drawVolume(); // drawing 3D volume;
    int renderingtime = time_rendering.elapsed();
    std::cout << "rendering : " << renderingtime << " ms" << std::endl;


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


















