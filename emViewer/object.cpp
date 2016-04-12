#include <QFileDialog>
#include <QString>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QVector>
#include <QTime>


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

    outlineSource = vtkSmartPointer<vtkOutlineSource>::New();
    outlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    outlineActor = vtkSmartPointer<vtkActor>::New();

    style_actor = vtkSmartPointer<vtkInteractorStyleTrackballActor>::New();
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
            std::cout << "First time of data visulization : " << time << " ms" << std::endl << std::endl;
        }
    }
    else
    {
        QMessageBox::information(this, tr("EM Viewer"), tr("No File!!!"));
        return ;
    }
}

void Object::getVolumeInformation()
{
    std::cout << "current volume origin, position, center, orientation, scale information : " << std::endl;

    double *origin = volume->GetOrigin();
    std::cout << origin[0] << " " << origin[1] << " " << origin[2] << std::endl;

    double *pos = volume->GetPosition();
    std::cout << pos[0] << " " << pos[1] << " " << pos[2] << std::endl;

    double *center = volume->GetCenter();
    std::cout << center[0] << " " << center[1] << " " << center[2] << std::endl;

    double *wxyz = volume->GetOrientationWXYZ();
    std::cout << wxyz[0] << " " << wxyz[1] << " " << wxyz[2] << " " << wxyz[3] << std::endl;

    double *scale = volume->GetScale();
    std::cout << scale[0] << " " << scale[1] << " " << scale[2] << std::endl;


    std::cout << "current outlineActor origin, position, center, orientation, scale information : " << std::endl;

    double *origin2 = outlineActor->GetOrigin();
    std::cout << origin2[0] << " " << origin2[1] << " " << origin2[2] << std::endl;

    double *pos2 = outlineActor->GetPosition();
    std::cout << pos2[0] << " " << pos2[1] << " " << pos2[2] << std::endl;

    double *center2 = outlineActor->GetCenter();
    std::cout << center2[0] << " " << center2[1] << " " << center2[2] << std::endl;

    double *wxyz2 = outlineActor->GetOrientationWXYZ();
    std::cout << wxyz2[0] << " " << wxyz2[1] << " " << wxyz2[2] << " " << wxyz2[3] << std::endl;

    double *scale2 = outlineActor->GetScale();
    std::cout << scale2[0] << " " << scale2[1] << " " << scale2[2] << std::endl;
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

    // set the vlume with origin, position, orientation, scale with default value.
    volume->SetOrigin(0,0,0);
    volume->SetPosition(0,0,0);
    volume->SetOrientation(0,0,0);
    volume->SetScale(1,1,1);

    // set the outline with origin, position, orientation, scale with defualt value.
    outlineActor->SetOrigin(0,0,0);
    outlineActor->SetPosition(0,0,0);
    outlineActor->SetOrientation(0,0,0);
    outlineActor->SetScale(1,1,1);

    // reset the camera parameters with the bounding box
    renderer->ResetCamera(0,dims[0],0,dims[1],0,dims[2]);
    std::cout << "camera reset " << std::endl;
    std::cout << dims[0] << " " << dims[1] << " " << dims[2] << std::endl;
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
    std::cout << "Total time of data visulization : " << time << " ms" << std::endl << std::endl;
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

    // draw the outline, bounds
    outlineSource->SetBounds(imageData->GetBounds());
    outlineMapper->SetInputConnection(outlineSource->GetOutputPort());
    outlineActor->SetMapper(outlineMapper);
    outlineActor->GetProperty()->SetLineStipplePattern(0xf0f0); //dotted line
    outlineActor->GetProperty()->SetColor(0,1,0); // with green color



    // VTK Renderer
    renderer->AddVolume(volume);
    renderer->AddActor(outlineActor);

    // VTK/Qtwidget
    GetRenderWindow()->AddRenderer(renderer);
    GetRenderWindow()->Render();

    // VTK Interactor Style : Actor Mode
    GetRenderWindow()->GetInteractor()->SetInteractorStyle(style_actor);
    GetRenderWindow()->GetInteractor()->Start();
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

    // get volume information, origin, center, scale ...
    getVolumeInformation();
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


















