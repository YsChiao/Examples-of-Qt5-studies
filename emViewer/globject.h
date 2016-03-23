#ifndef GLOBJECT_H
#define GLOBJECT_H


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

#include "emfile.h"
#include "gloperator.h"
#include <QVTKWidget.h>
#include <QtAlgorithms>

class glObject : public QVTKWidget
{
    Q_OBJECT

public:
    explicit glObject(QWidget* parent = 0);
    ~glObject();

public slots:
    void open();
    void getLevel(int value);

signals:
    void sendMessage(QString& Message);
    void sendMaxMin(float max, float min);
    void sendFileName(QString& name);
    void renderDone();

protected:

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

private:

    void readFile(const QString& fileName);

    void FileDataToVtkImageData();
    //void FileDataToSliceVtkImageData(int&, vtkImageData*);

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

};

#endif















// GLOBJECT_H
