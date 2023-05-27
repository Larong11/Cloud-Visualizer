#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <fstream>
#include <QFileDialog>
#include <QLabel>
#include <QCheckBox>
#include <pointcloud.h>

#define VERSION "alpha 0.2.1"

template <typename T>
struct insertedClouds
{
    std::vector<std::pair<pointCloud<T>, bool>> clouds;
    std::vector<QString> names;
};

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

    void changedStatus(QCheckBox *checkBox);
    void deleteCloud(QPushButton *pushButton);

    void on_showCloud_clicked();

    void on_actionSaveClouds_triggered();

private:
    Ui::MainWindow *ui;

    void updateClouds();

    //import clouds
    insertedClouds<point> cloudsXYZ;
    insertedClouds<pointI> cloudsXYZI;
    insertedClouds<pointRGB> cloudsXYZRGB;
};
#endif // MAINWINDOW_H
