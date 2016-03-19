#ifndef GLOBJECT_H
#define GLOBJECT_H

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
    void getFileSize(const QByteArray&, QVector3D&);
    void FileDataBinaryToFloat(const QByteArray&, QVector<float>&);
    void draw(QVector<float>&, QVector3D&);
    void processing();
    QByteArray fileData;
};

#endif















// GLOBJECT_H
