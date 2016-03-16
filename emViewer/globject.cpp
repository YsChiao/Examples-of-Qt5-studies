#include "globject.h"

glObject::glObject(QWidget* parent)
    : QOpenGLWidget(parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;
}

glObject::~glObject()
{

}

void glObject::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
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
            fileData = file.readAll();
            processing();
        }
    }
    else
    {
        QMessageBox::information(this, tr("EM Viewer"), tr("No File!!!"));
        return ;
    }
}

QSize glObject::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize glObject::sizeHint() const
{
    return QSize(400,400);
}

static void qNormalizeAngle(int& angle)
{
    while(angle < 0)
        angle += 360 * 16;
    while(angle > 360)
        angle -= 360 * 16;
}

void glObject::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if(angle != xRot)
    {
        xRot = angle;
        update();
    }
}

void glObject::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if(angle != yRot)
    {
        yRot = angle;
        update();

    }
}

void glObject::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if(angle != zRot)
    {
        zRot = angle;
        update();
    }
}


void glObject::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();

}

void glObject::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if(event->buttons() & Qt::LeftButton)
    {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    }
    else if(event->buttons() & Qt::RightButton)
    {
        setXRotation(xRot + 8 * dy);
        setYRotation(zRot + 8 * dx);
    }
    lastPos = event->pos();
}


void glObject::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0,0,0,1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_SMOOTH);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    //glEnable(GL_COLOR_MATERIAL);

    static GLfloat lightPosition[4] = {0, 0, 10, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

}

void glObject::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    //    glBegin(GL_TRIANGLES);
    //    glColor3f(1.0, 0.0, 0.0);
    //    glVertex3f(-0.5, -0.5, 0);
    //    glColor3f(0.0, 1.0, 0.0);
    //    glVertex3f( 0.5, -0.5, 0);
    //    glColor3f(0.0, 0.0, 1.0);
    //    glVertex3f( 0.0,  0.5, 0);
    //    glEnd();



    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
    draw();

}

void glObject::resizeGL(int width, int height)
{
    //    glViewport(0,0,width,height);
    //    glMatrixMode(GL_PROJECTION);
    //    glLoadIdentity();
    //    gluPerspective(45, (float)width/height, 0.01, 100.0);
    //    glMatrixMode(GL_MODELVIEW);
    //    glLoadIdentity();
    //    gluLookAt(0,0,5,0,0,0,0,1,0);

    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-2, +2, -2, +2, 1.0, 15.0);
#else
    glOrtho(-2, +2, -2, +2, 1.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);

}


void glObject::processing()
{
    // get size
    unsigned int x, y, z;
    for(int n = 4; n < 16; n += 4)
    {
        unsigned char a = (unsigned char)fileData.at(n);
        unsigned char b = (unsigned char)fileData.at(n+1);
        unsigned char c = (unsigned char)fileData.at(n+2);
        unsigned char d = (unsigned char)fileData.at(n+3);

        if(n == 4)
        {
            x = a | (b<<8) | (c<<16) | (d<<24);
        }
        if(n == 8)
        {
            y = a | (b<<8) | (c<<16) | (d<<24);
        }
        if(n == 12)
        {
            z = a | (b<<8) | (c<<16) | (d<<24);
        }
    }
//    qDebug() << qPrintable(QString::number(x,16));
//    qDebug() << qPrintable(QString::number(y,16));
//    qDebug() << qPrintable(QString::number(z,16));

//    int x_size = (int)x;
//    int y_size = (int)y;
//    int z_size = (int)z;

//    qDebug() << qPrintable(QString::number(x_size,10));
//    qDebug() << qPrintable(QString::number(y_size,10));
//    qDebug() << qPrintable(QString::number(z_size,10));


    // concatenation of input volumn size by format [x,y,z] and show on the stautsbar.
    QString sQStringx = QString::number(x);
    QString sQStringy = QString::number(y);
    QString sQStringz = QString::number(z);
    QString Message = QString("[" + sQStringx +","+ sQStringy +","+ sQStringz+"]");
    emit sendMessage(Message);
}



void glObject::draw()
{

    glBegin(GL_QUADS);
    glNormal3f(0,0,-1);
    glVertex3f(-1,-1,0);
    glVertex3f(-1,1,0);
    glVertex3f(1,1,0);
    glVertex3f(1,-1,0);
    glEnd();
    glBegin(GL_TRIANGLES);
    glNormal3f(0,-1,0.707);
    glVertex3f(-1,-1,0);
    glVertex3f(1,-1,0);
    glVertex3f(0,0,1.414);
    glEnd();
    glBegin(GL_TRIANGLES);
    glNormal3f(1,0, 0.707);
    glVertex3f(1,-1,0);
    glVertex3f(1,1,0);
    glVertex3f(0,0,1.414);
    glEnd();
    glBegin(GL_TRIANGLES);
    glNormal3f(0,1,0.707);
    glVertex3f(1,1,0);
    glVertex3f(-1,1,0);
    glVertex3f(0,0,1.414);
    glEnd();
    glBegin(GL_TRIANGLES);
    glNormal3f(-1,0,0.707);
    glVertex3f(-1,1,0);
    glVertex3f(-1,-1,0);
    glVertex3f(0,0,1.414);
    glEnd();
}












