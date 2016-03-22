#ifndef GLOBJECT_H
#define GLOBJECT_H

#include "emfile.h"
#include <QVTKWidget.h>

class glObject : public QVTKWidget
{
    Q_OBJECT

public:
    explicit glObject(QWidget* parent = 0);
    ~glObject();

public slots:
    void open();

signals:
    void sendMessage(QString& Message);

protected:

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

private:

    void readFile(const QString& fileName);

    void FileDataToVtkImageData(vtkImageData*);
    void FileDataToSliceVtkImageData(int&, vtkImageData*);

    void drawVolume(vtkImageData*);
    void drawSlice(vtkImageData*);

    void VolumeProcessing();
    void SliceProcessing();
    void Information();


    QVector<float> fileDataFloat;
    QString fileDataType;
    int dims[3];
};

#endif















// GLOBJECT_H
