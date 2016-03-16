#include "imageviewer.h"
#include "ui_imageviewer.h"



ImageViewer::ImageViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageViewer)
{
    ui->setupUi(this);

    ui->ImageLabel->setBackgroundRole(QPalette::Base);
    ui->ImageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->ImageLabel->setAlignment(Qt::AlignCenter);
    ui->ImageLabel->setScaledContents(true);
    ui->scrollArea->setBackgroundRole(QPalette::Light);

    ui->nextButton->setArrowType(Qt::RightArrow);
    ui->previousButton->setArrowType(Qt::LeftArrow);

    QObject::connect(ui->openAct, SIGNAL(triggered(bool)), this, SLOT(open()));
    QObject::connect(ui->exitAct, SIGNAL(triggered(bool)), this, SLOT(close()));
    QObject::connect(ui->GaussianBlurAct, SIGNAL(triggered(bool)), this, SLOT(GaussianBlur()));

}

ImageViewer::~ImageViewer()
{
    delete ui;
}

void ImageViewer::open()
{
    qDebug() << "open()" ;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
    if(!fileName.isEmpty())
    {
        QImage image(fileName);
        vectorQImage.push_back(image);
        if(image.isNull())
        {
            QMessageBox::information(this, tr("Image Viewer"), tr("Cannot load %1.").arg(fileName));
            return;
        }
        ui->ImageLabel->setPixmap(QPixmap::fromImage(vectorQImage[0]));
    }
    else
    {
        QMessageBox::information(this, tr("Image Viewer"), tr("Load file failed."));
        return;
    }
}

void ImageViewer::GaussianBlur()
{
    if(vectorQImage.isEmpty())
    {
        return;
    }
    cv::Mat cvImage = QImageTocvMat(vectorQImage[0]);
    // share memory.
    cv::GaussianBlur(cvImage, cvImage, cv::Size(9,9), 0, 0);
    QImage image = cvMatToQImage(cvImage);
    ui->ImageLabel->setPixmap(QPixmap::fromImage(image));

}


// no copy, share memory, FORMAT_RCB32
cv::Mat ImageViewer::QImageTocvMat(const QImage& image)
{
    cv::Mat mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine());
    return mat;
}

// share memory, FORMAT_CV_8UC4
QImage ImageViewer::cvMatToQImage(const cv::Mat &mat)
{
    QImage image( mat.data, mat.cols, mat.rows, mat.step,QImage::Format_RGB32);
    return image;
}






























