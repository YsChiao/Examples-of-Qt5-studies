#include <QtWidgets>
#include "emviewer.h"
#include "ui_emviewer.h"
#include "ui_tools.h"

EMviewer::EMviewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EMviewer)
{
    ui->setupUi(this);

    // create object for the label within statusbar
    statusLabel = new QLabel(this);
    ui->statusBar->addWidget(statusLabel);

    // create tool window
    toolsWidget = new Tools(this);

    // connection
    QObject::connect(ui->ExitAct, SIGNAL(triggered(bool)), this, SLOT(close()));
    QObject::connect(ui->OpenAct, SIGNAL(triggered(bool)), ui->object, SLOT(open()));
    QObject::connect(ui->object, SIGNAL(sendMessage(QString&)), this, SLOT(show_Message_toolWidget(QString&)));


    //connect glObject and glOperator
    QObject::connect(ui->object, SIGNAL(sendMaxMin(float,float)), this->toolsWidget, SLOT(MaxMin(float,float)));
    QObject::connect(ui->object, SIGNAL(sendFileName(QString&)),  this->toolsWidget, SLOT(FileName(QString&)));
    QObject::connect(this->toolsWidget, SIGNAL(sentLevelValue(int)), ui->object, SLOT(getLevel(int)));

}

EMviewer::~EMviewer()
{
    delete ui;
    delete statusLabel;
}


void EMviewer::show_Message_toolWidget(QString &Message)
{
    statusLabel->setText(Message);

    // toolWidget
    // obtain mainwindow position
    QRect window = this->geometry();
    // set the position of tools widget
    int height = toolsWidget->height();
    int width = toolsWidget->width();
    QRect toolsWindow(window.x()+window.width()+10, window.y()+height, width, height);
    toolsWidget->setGeometry(toolsWindow);
    toolsWidget->getUi()->slider->setSliderPosition(0);
    toolsWidget->show();
    if (!toolsWidget->isTopLevel())
    {
        std::cout << "not top level" << std::endl;
    }
}






