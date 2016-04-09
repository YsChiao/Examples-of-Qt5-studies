#include <QFileDialog>
#include <QString>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QVector>
#include <QTime>

#include <vtkTransform.h>
#include <vtkMatrix4x4.h>
#include <vtkInteractorStyleTrackballCamera.h>




#include "object.h"

Object::Object(QWidget* parent)
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

    interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();

    camera = vtkSmartPointer<vtkCamera>::New();
    transform = vtkSmartPointer<vtkTransform>::New();

    //originPositin();
}

Object::~Object()
{
}

void Object::open()
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
            readFile(fileName);
            Information();

            // check the time
            QTime processing_time_first;
            processing_time_first.start();

            dataLoading();

            int time = processing_time_first.elapsed();
            std::cout << "First time of data visulization : " << time << " ms" << std::endl;
        }
    }
    else
    {
        QMessageBox::information(this, tr("EM Viewer"), tr("No File!!!"));
        return ;
    }
}


void Object::readFile(const QString& fileName)
{
    // read new file data at first time
    const char* infile = fileName.toStdString().c_str();
    emFile = EmFile(infile);

    // default value of level
    levelValue = 0;

    // get size
    emFile.Dims(dims);

    // get type
    emFile.Type(fileType);
}

void Object::FileDataMaxMin(float& max, float& min)
{
    // sent max and min value to QDialog "tools"
    if (fileType == 1)
    {
        min = static_cast<float>(*std::min_element(byteData.begin(), byteData.end()));
        max = static_cast<float>(*std::max_element(byteData.begin(), byteData.end()));
    }
    else if (fileType == 2)
    {
        min = static_cast<float>(*std::min_element(intData.begin(), intData.end()));
        max = static_cast<float>(*std::max_element(intData.begin(), intData.end()));
    }
    else if (fileType == 5)
    {
        min = *std::min_element(floatData.begin(), floatData.end());
        max = *std::max_element(floatData.begin(), floatData.end());
    }
    emit sendMaxMin(max, min);
}

void Object::getLevel(int value)
{
    // as same as magnifier in class glOperator
    levelValue = static_cast<float>(value)/10 ;

    // load new filtered data and show new result
    // check the time
    QTime processing_time;
    processing_time.start();

    dataLoading();

    int time = processing_time.elapsed();
    std::cout << "Total time of data visulization : " << time << " ms" << std::endl;
}

void Object::dataLoading()
{
    // load a copy of data, and manipulate the copy data later
    switch(fileType)
    {
    case 1:
    {
        emFile.ByteData(byteData);
    }
    case 2:
    {
        emFile.IntData(intData);
    }
    case 5:
    {
        emFile.FloatData(floatData);
    }
    }

    VolumeProcessing();
}

QSize Object::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize Object::sizeHint() const
{
    return QSize(400,400);
}


void Object::FileDataToVtkImageData()
{    

    // Specify the size of the image data
    imageData->SetDimensions(dims[0], dims[1], dims[2]);
    imageData->SetSpacing(1.0, 1.0, 1.0);
    imageData->SetOrigin(0.0, 0.0, 0.0);

    // read file data as unsigned char from the original data format
    imageData->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

    const int numComponents = imageData->GetNumberOfScalarComponents();
    const int numElements = dims[0] * dims[1] * dims[2] * numComponents;
    unsigned char* pixel = static_cast<unsigned char*>(imageData->GetScalarPointer());
    for (int i = 0; i < numElements; i++)
    {
        if (fileType == 1)
        {
            // filter  data with level, smoothness
            if (byteData.at(i) < levelValue)
            {
                pixel[i] = 0;
            }
            else
            {
                pixel[i] = byteData.at(i) * 255;
            }

        }
        else if (fileType == 2)
        {
            // filter  data with level, smoothness
            if (intData.at(i) < levelValue)
            {
                pixel[i] = 0;
            }
            else
            {
                pixel[i] = intData.at(i) * 255;
            }
        }
        else if (fileType == 5)
        {
            // filter  data with level, smoothness
            if (floatData.at(i) < levelValue)
            {
                pixel[i] = 0;
            }
            else
            {
                pixel[i] = floatData.at(i) * 255;
            }
        }
    }
    imageData->Modified();
}

void Object::originPositin()
{
    camera->GetPosition(pos);
    std::cout << pos[0] << " " << pos[1] << " " << pos[2] << std::endl;
    camera->GetViewUp(viewUp);
    std::cout << viewUp[0] << " " << viewUp[1] << " " << viewUp[2] << std::endl;
    camera->GetFocalPoint(focalPoint);
    std::cout << focalPoint[0] << " " << focalPoint[1] << " " << focalPoint[2] << std::endl;

    vtkMatrix4x4 *matrix = camera->GetModelTransformMatrix();
    std::cout << *matrix << std::endl;

}

void Object::getRotation()
{
    double current_pos[3], current_viewUp[3], current_focalPoint[3], current_orentation[3];
    camera->GetPosition(current_pos);
    std::cout << current_pos[0] << " " << current_pos[1] << " " << current_pos[2] << std::endl;
    camera->GetViewUp(current_viewUp);
    std::cout << current_viewUp[0] << " " << current_viewUp[1] << " " << current_viewUp[2] << std::endl;
    camera->GetFocalPoint(current_focalPoint);
    std::cout << current_focalPoint[0] << " " << current_focalPoint[1] << " " << current_focalPoint[2] << std::endl;
    volume->GetOrientation(orentation);
    std::cout << current_orentation[0] << " " << current_orentation[1] << " " << current_orentation[2] << std::endl;

    vtkMatrix4x4 *matrix = camera->GetModelTransformMatrix();
    std::cout << *matrix << std::endl;
}


void Object::drawVolume()
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
    GetRenderWindow()->Render();
}

void Object::VolumeProcessing()
{
    // get max min float data
    float max = 0;
    float min = 0;
    FileDataMaxMin(max, min);

    // convert float volume data to vtkImageData
    FileDataToVtkImageData();

    // display 3D image
    drawVolume();
}

void Object::Information()
{
    // show size informatio within status bar
    QString sQStringx = QString::number(dims[0]);
    QString sQStringy = QString::number(dims[1]);
    QString sQStringz = QString::number(dims[2]);
    QString Message = QString("[" +sQStringx +"," +sQStringy +"," +sQStringz +"]" );
    emit sendMessage(Message);
}


















