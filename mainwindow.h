#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class glWidget;
class QComboBox;
class QPushButton;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void doSave();


private:
    glWidget* glw;
    QComboBox* fileFormat, *sort;
    QPushButton* save;

};

#endif // MAINWINDOW_H
