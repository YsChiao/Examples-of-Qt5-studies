#ifndef OBJECT_H
#define OBJECT_H


#include <cmath>
#include <vector>
#include <algorithm>
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL)
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL)
VTK_MODULE_INIT(vtkInteractionStyle)

#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkImageData.h>
#include <vtkImageActor.h>
#include <vtkDataSetMapper.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkOutlineSource.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkLine.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkUnsignedCharArray.h>

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

    void Axes(double xMax = 1.0, double yMax = 1.0, double zMax = 1.0);

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

    // read data from file
    void readFile(const QString& fileName);

    // get data size
    void Information();

    // get the copy of data for manipulating
    void dataLoading();

    // get the max and min data
    void FileDataMaxMin(float& max, float& min);

    // covert data from vector to VtkImageData format
    void FileDataToVtkImageData();

    // rendering and display
    void drawVolume();

    // data flow : FileDataMaxMin, FileDataToVtkImageData, drawVolume
    void VolumeProcessing();

    // get volume origin, position, center, orientation, scale information
    void getVolumeInformation();

    // original data
    EmFile emFile;

    // copy data
    std::vector<unsigned char> byteData;
    std::vector<int> intData;
    std::vector<float> floatData;

    // copy data dimentions
    int dims[3];

    // coppy data type
    unsigned char fileType;

    // filter value, default 0
    float levelValue;

    // vtk rendering class
    vtkSmartPointer<vtkImageData> imageData;
    vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper;
    vtkSmartPointer<vtkPiecewiseFunction> opacityTransferFunction;
    vtkSmartPointer<vtkPiecewiseFunction> volumeGradientOpacity;
    vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction;
    vtkSmartPointer<vtkVolumeProperty> volumeProperty;
    vtkSmartPointer<vtkVolume> volume;
    vtkSmartPointer<vtkRenderer> renderer;

//    // vtk outline class
//    vtkSmartPointer<vtkOutlineSource> outlineSource;
//    vtkSmartPointer<vtkPolyDataMapper> outlineMapper;
//    vtkSmartPointer<vtkActor> outlineActor;

    // create a vtkPoints container and store the points in it
    vtkSmartPointer<vtkPoints> pts = vtkSmartPointer<vtkPoints>::New();
    // add the points to the polydata container
    vtkSmartPointer<vtkPolyData> linesPolyData = vtkSmartPointer<vtkPolyData>::New();
    // create lines
    vtkSmartPointer<vtkLine> line0 = vtkSmartPointer<vtkLine>::New();
    vtkSmartPointer<vtkLine> line1 = vtkSmartPointer<vtkLine>::New();
    vtkSmartPointer<vtkLine> line2 = vtkSmartPointer<vtkLine>::New();
    // Create a vtkCellArray container and store the lines in it
    vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
    // Create a vtkUnsignedCharArray container and store the colors in it
    vtkSmartPointer<vtkUnsignedCharArray> colors = vtkSmartPointer<vtkUnsignedCharArray>::New();
    // Setup the visualization pipeline
    vtkSmartPointer<vtkPolyDataMapper> linesMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkActor> linesActor = vtkSmartPointer<vtkActor>::New();


    // set interactor style, actor mode
    vtkSmartPointer<vtkInteractorStyleTrackballActor> style_actor;
};

#endif


// OBJECT_H
