#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QBrush>
#include <QFont>
#include <QImage>
#include <QPen>
#include <QGLWidget>

class QPaintEvent;
class QWidget;

class glWidget : public QGLWidget
{
    Q_OBJECT

public:
    glWidget(QWidget *parent = 0);
    ~glWidget();

    QSize sizeHint() const;
    int xRotation() const { return xRot; }
    int yRotation() const { return yRot; }
    int zRotation() const { return zRot; }

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

protected:
    void initializeGL();
    void paintEvent(QPaintEvent *event);
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void showEvent(QShowEvent *event);

private:
    void drawInstructions(QPainter *painter);
    void drawSvgMath(QPainter *painter);
    void setupViewport(int width, int height);

    QColor qtGreen;
    QColor qtPurple;

    GLuint object;
    int xRot;
    int yRot;
    int zRot;
    QPoint lastPos;
};

#endif
