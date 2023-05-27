#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <fstream>
#include <QFileDialog>
#include <QLabel>
#include <QCheckBox>
#include <pointcloud.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpenCloud_triggered();

private:
    Ui::MainWindow *ui;

    //import clouds
    std::vector<std::pair<pointCloud<point>, std::string>> cloudsPoint;
    std::vector<std::pair<pointCloud<pointI>, std::string>> cloudsPointI;
    std::vector<std::pair<pointCloud<pointRGB>, std::string>> cloudsPointRGB;
};
#endif // MAINWINDOW_H
