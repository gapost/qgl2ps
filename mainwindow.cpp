#include "mainwindow.h"
#include "glwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QFile>

#include "gl2ps.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    glw = new glWidget(this);
    QVBoxLayout* vl = new QVBoxLayout();
    vl->addWidget(glw);

    fileFormat = new QComboBox;
    fileFormat->addItem("EPS",GL2PS_EPS);
    fileFormat->addItem("PDF",GL2PS_PDF);
    fileFormat->addItem("SVG",GL2PS_SVG);

    QHBoxLayout* hl = new QHBoxLayout;
    hl->addWidget(new QLabel("GL2PS file format:"));
    hl->addWidget(fileFormat);
    vl->addLayout(hl);

    sort = new QComboBox;
    sort->addItem("Simple",GL2PS_SIMPLE_SORT);
    sort->addItem("BSP",GL2PS_BSP_SORT);
    hl = new QHBoxLayout;
    hl->addWidget(new QLabel("GL2PS sorting:"));
    hl->addWidget(sort);
    vl->addLayout(hl);

    save = new QPushButton("Save graphics file");
    hl = new QHBoxLayout;
    hl->addStretch();
    hl->addWidget(save);
    vl->addLayout(hl);

    setLayout(vl);

    connect(save,SIGNAL(clicked()),this,SLOT(doSave()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::doSave()
{
    GLint viewport[4];
    viewport[0] = 0;
    viewport[1] = 0;
    viewport[2] = glw->width();
    viewport[3] = glw->height();

    GLint fmt = fileFormat->itemData(fileFormat->currentIndex()).toInt();
    GLint srt = sort->itemData(sort->currentIndex()).toInt();
    GLint options = GL2PS_DRAW_BACKGROUND | GL2PS_USE_CURRENT_VIEWPORT;
    if (srt==GL2PS_BSP_SORT) options |= GL2PS_BEST_ROOT;

    QString fname("out.");
    fname += fileFormat->currentText();

    FILE* fp = fopen(fname.toLatin1().constData(), "wb");


    if (!fp)
    {
        return;
    }

    int state = GL2PS_OVERFLOW, buffsize = 0, ret;
    while(state == GL2PS_OVERFLOW){
      buffsize += 1024*1024;
      ret = gl2psBeginPage(fname.toLatin1().constData(), "qgl2ps", viewport,
                     fmt, srt, options,
                     GL_RGBA, 0, NULL, 0, 0, 0,
                     buffsize, fp, fname.toLatin1().constData());
      if (ret==GL2PS_ERROR) break;
      glw->repaint();
      state = gl2psEndPage();
    }
    fclose(fp);


}
