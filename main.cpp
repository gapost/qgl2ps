#include "mainwindow.h"
#include <QApplication>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif


int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(qgl2ps);

    glutInit(&argc, argv);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
