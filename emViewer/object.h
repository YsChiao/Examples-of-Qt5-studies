#ifndef OBJECT_H
#define OBJECT_H


#include <cmath>
#include <vector>
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL)
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL)
VTK_MODULE_INIT(vtkInteractionStyle)

#include <vtkSmartPointer.h>
#include <vtkImageActor.h>
#include <vtkActor.h>
#include <vtkCamera.h>
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
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>

#include "emfile.h"
#include "tools.h"
#include <QVTKWidget.h>
#include <QtAlgorithms>

class Object : public QVTKWidget
{
    Q_OBJECT

public:
    explicit Object(QWidget* parent = 0);
    ~Object();

public slots:
    void open();
    void getLevel(int value);

signals:
    void sendMessage(QString& Message);
    void sendMaxMin(float max, float min);
    void sendFileName(QString& name);

protected:
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

private:

    void readFile(const QString& fileName);

    void FileDataMaxMin(float& max, float& min);
    void FileDataToVtkImageData();
    //void FileDataToSliceVtkImageData(int&, vtkImageData*);

    void originPositin();
    void getRotation();


    void drawVolume();
    //void drawSlice(vtkImageData*);

    void Processing();
    void VolumeProcessing();
    //void SliceProcessing();
    void Information();


    EmFile emFile;
    QVector<unsigned char> fileDataByte;
    QVector<float> fileDataFloat;
    QVector<int> fileDataInt;
    int dims[3];
    float levelValue;


    vtkSmartPointer<vtkImageData> imageData;
    vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper;
    vtkSmartPointer<vtkPiecewiseFunction> opacityTransferFunction;
    vtkSmartPointer<vtkPiecewiseFunction> volumeGradientOpacity;
    vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction;
    vtkSmartPointer<vtkVolumeProperty> volumeProperty;
    vtkSmartPointer<vtkVolume> volume;
    vtkSmartPointer<vtkRenderer> renderer;

    vtkSmartPointer<vtkRenderWindowInteractor> interactor;
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style;

    vtkSmartPointer<vtkCamera> camera;
    vtkSmartPointer<vtkTransform> transform;



    double pos[3];
    double viewUp[3];
    double focalPoint[3];
    double orentation[3];
    vtkMatrix4x4 * 	transpose;

};

#endif


// OBJECT_H
