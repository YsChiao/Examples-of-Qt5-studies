#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include <QImage>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class ImageViewer;
}

class ImageViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = 0);
    ~ImageViewer();

public slots:
    void open();

private:
    Ui::ImageViewer *ui;
};

#endif // IMAGEVIEWER_H
