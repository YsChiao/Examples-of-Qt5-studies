#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include <QImage>
#include <QDebug>
#include <QVector>
#include <QFileDialog>
#include <QMessageBox>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace Ui {
class ImageViewer;
}

class ImageViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = 0);
    ~ImageViewer();

signals:
    void GaussianBlurAct();

public slots:
    void open();
    void GaussianBlur();

private:
    QImage  cvMatToQImage(const cv::Mat& mat);
    cv::Mat QImageTocvMat(const QImage& image);


    Ui::ImageViewer *ui;
    QVector<QImage> vectorQImage;
};

#endif // IMAGEVIEWER_H
