#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QtGui"
#include <QMainWindow>
#include <iostream>
#include <fstream>
#include <QFileDialog>
#include <QLabel>
#include <QCheckBox>
#include <pointcloud.h>
#include <cloudsvisualiser.h>

#define VERSION "beta 0.4"

//template <typename T>
//struct insertedClouds
//{
//    std::vector<std::pair<pointCloud<T>, bool>> clouds;
//    std::vector<QString> names;
//};

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

    void resizeEvent(QResizeEvent* event);

    void changedStatus(QCheckBox *checkBox);
    void deleteCloud(QPushButton *pushButton);

//    void on_showCloud_clicked();

    void on_actionSaveClouds_triggered();

    void on_actionSaveAllClouds_triggered();

    void on_schemeXYZ_currentIndexChanged(int index);

    void on_schemeXYZI_currentIndexChanged(int index);

    void on_schemeRGB_currentIndexChanged(int index);

    void on_listClouds_cellActivated(int row, int column);

    void on_listClouds_itemSelectionChanged();

private:
    Ui::MainWindow *ui;
    cloudsVisualiser visualizer;

    void updateClouds();

    QShortcut *selectAll, *deleteS;

    //import clouds
    insertedClouds<point> cloudsXYZ;
    insertedClouds<pointI> cloudsXYZI;
    insertedClouds<pointRGB> cloudsXYZRGB;
};
#endif // MAINWINDOW_H
