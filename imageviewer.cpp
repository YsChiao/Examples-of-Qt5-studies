#include "imageviewer.h"
#include "ui_imageviewer.h"


ImageViewer::ImageViewer(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::ImageViewer)
{
  ui->setupUi(this);

  ui->ImageLabel->setBackgroundRole(QPalette::Base);
  ui->ImageLabel->setScaledContents(true);
  ui->scrollArea->setBackgroundRole(QPalette::Dark);

  QObject::connect(ui->openAct, SIGNAL(triggered(bool)), this, SLOT(open()));
  QObject::connect(ui->exitAct, SIGNAL(triggered(bool)), this, SLOT(close()));

}

ImageViewer::~ImageViewer()
{
  delete ui;
}

void ImageViewer::open()
{
    qDebug() << "open()" ;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    QDir::currentPath());
    if(!fileName.isEmpty())
    {
        QImage image(fileName);
        if(image.isNull())
        {
            QMessageBox::information(this, tr("Image Viewer"),
                                     tr("Cannot load %1.").arg(fileName));
            return;
        }
        ui->ImageLabel->setPixmap(QPixmap::fromImage(image));
    }
    else
    {
        QMessageBox::information(this, tr("Image Viewer"),
                                 tr("Load file failed."));
        return;
    }
}






























