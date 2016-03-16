#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QtOpenGL>
#include <GL/gl.h>
#include <GL/glu.h>


class glObject : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    glObject(QWidget* parent = 0);
    ~glObject();

    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

public slots:
    void open();

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;


    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

private:
    void draw();

    int xRot;
    int yRot;
    int zRot;

    QPoint lastPos;
    QByteArray fileData;
};

#endif















// GLOBJECT_H
