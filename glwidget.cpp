#include "glwidget.h"

#include <QtGui>
#include <QtOpenGL>

#include <QSvgRenderer>

#include <stdlib.h>

#include <math.h>

#include "glwidget.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

//#include <string.h>
#include "gl2ps.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

//! [0]
glWidget::glWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    xRot = 120*16;
    yRot = 0;
    zRot = 0;

    qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
    qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);

    setAutoFillBackground(false);
    setMinimumSize(200, 200);
}

glWidget::~glWidget()
{
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void glWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot)
        xRot = angle;
}

void glWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot)
        yRot = angle;
}

void glWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot)
        zRot = angle;
}

void glWidget::initializeGL()
{
    glEnable(GL_MULTISAMPLE);

}

void glWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void glWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
        //updateGL();
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
        //updateGL();
    }
    update();
    lastPos = event->pos();
}

void glWidget::paintEvent(QPaintEvent *event)
{
    makeCurrent();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    qglClearColor(qtPurple.dark());
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    setupViewport(width(), height());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);

    //logo->draw();

    //glClearColor(0.3, 0.5, 0.8, 0.);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* draw a smooth-shaded torus */
    //glPushMatrix();
    //glRotatef(-60., 2., 0., 1.);
    glEnable(GL_LIGHTING);
    glutSolidTorus(0.1, 0.2, 30, 30);
    glDisable(GL_LIGHTING);
    //glPopMatrix();

    glShadeModel(GL_FLAT);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    QPainter painter(this);
    drawInstructions(&painter);
    drawSvgMath(&painter);

    //painter.drawEllipse(100,100,100,100);
    painter.end();
}

void glWidget::resizeGL(int width, int height)
{
    setupViewport(width, height);
}

void glWidget::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
}

QSize glWidget::sizeHint() const
{
    return QSize(400, 400);
}

void glWidget::setupViewport(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES
    glOrthof(-0.5, +0.5, -0.5, 0.5, 4.0, 15.0);
#else
    glOrtho(-0.5, +0.5, -0.5, 0.5, 4.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}

void glWidget::drawInstructions(QPainter *painter)
{
    QString text = tr("Click and drag with the left mouse button "
                      "to rotate the scene.");
    QFontMetrics metrics = QFontMetrics(font());
    int border = qMax(4, metrics.leading());

    QRect rect = metrics.boundingRect(0, 0, width() - 2*border, int(height()*0.125),
                                      Qt::AlignCenter | Qt::TextWordWrap, text);
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->fillRect(QRect(0, 0, width(), rect.height() + 2*border),
                     QColor(0, 0, 0, 127));
    painter->setPen(Qt::white);
    painter->fillRect(QRect(0, 0, width(), rect.height() + 2*border),
                      QColor(0, 0, 0, 127));
    painter->drawText((width() - rect.width())/2, border,
                      rect.width(), rect.height(),
                      Qt::AlignCenter | Qt::TextWordWrap, text);
}


void glWidget::drawSvgMath(QPainter* painter)
{
    QSvgRenderer R(QString(":/math.svg"));

    QSize sz = R.defaultSize();

    int fact = 15; // zoom factor
    int border = 10;
    int w = sz.width()*fact;
    int h = sz.height()*fact;

    painter->fillRect(QRect(0, height()-h-2*border, width(), h+2*border),
                      QColor(255, 255, 255, 160));

    R.render(painter,
             QRectF(width()/2 - w/2, height() - border - h,
                    w,h));
}
