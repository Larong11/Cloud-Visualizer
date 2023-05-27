#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->visualizer->layout()->addWidget(&visualizer);
    visualizer.setGeometry(1, 1, visualizer.width(), visualizer.height());
    visualizer.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    visualizer.setFocusPolicy(Qt::StrongFocus);
    visualizer.addClouds(&cloudsXYZ, &cloudsXYZI, &cloudsXYZRGB);
    visualizer.update();

    ui->listClouds->setEditTriggers(QAbstractItemView::NoEditTriggers);

    selectAll = new QShortcut(QKeySequence(tr("Ctrl+A")), this);

    connect (selectAll, &QShortcut::activated, [this]()
    {
       for(auto &cloud : cloudsXYZ.clouds)
       {
           cloud.second = true;
       }
       for(auto &cloud : cloudsXYZI.clouds)
       {
           cloud.second = true;
       }
       for(auto &cloud : cloudsXYZRGB.clouds)
       {
           cloud.second = true;
       }
       MainWindow::updateClouds();
    });

    deleteS = new QShortcut(QKeySequence(tr("Ctrl+Backspace")), this);

    connect (deleteS, &QShortcut::activated, [this]()
    {
        for(int i = 0; i < cloudsXYZ.clouds.size(); i++)
        {
            if(cloudsXYZ.clouds.at(i).second)
            {
                cloudsXYZ.names.erase(cloudsXYZ.names.begin() + i);
                cloudsXYZ.clouds.erase(cloudsXYZ.clouds.begin() + i);
                i--;
            }
        }
        for(int i = 0; i < cloudsXYZI.clouds.size(); i++)
        {
            if(cloudsXYZI.clouds.at(i).second)
            {
                cloudsXYZI.names.erase(cloudsXYZI.names.begin() + i);
                cloudsXYZI.clouds.erase(cloudsXYZI.clouds.begin() + i);
                i--;
            }
        }
        for(int i = 0; i < cloudsXYZRGB.clouds.size(); i++)
        {
            if(cloudsXYZRGB.clouds.at(i).second)
            {
                cloudsXYZRGB.names.erase(cloudsXYZRGB.names.begin() + i);
                cloudsXYZRGB.clouds.erase(cloudsXYZRGB.clouds.begin() + i);
                i--;
            }
        }
        MainWindow::updateClouds();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changedStatus(QCheckBox *checkBox)
{
    QString name = "";
    QString type = "";
    QString obj_name = checkBox->objectName();
    int i = 0;
    while(obj_name[i] != '_')
    {
        i++;
    }
    i++;
    while(obj_name[i] != '_')
    {
        type += obj_name[i];
        i++;
    }
    i++;
    for(i; i < obj_name.size(); i++)
    {
        name += obj_name[i];
    }
    if(type == "xyz")
    {
        cloudsXYZ.clouds.at(std::distance(cloudsXYZ.names.begin(), std::find(cloudsXYZ.names.begin(), cloudsXYZ.names.end(), name))).second = checkBox->isChecked();
    }
    else if(type == "xyzi")
    {
        cloudsXYZI.clouds.at(std::distance(cloudsXYZI.names.begin(), std::find(cloudsXYZI.names.begin(), cloudsXYZI.names.end(), name))).second = checkBox->isChecked();
    }
    else if(type == "xyzrgb")
    {
        cloudsXYZRGB.clouds.at(std::distance(cloudsXYZRGB.names.begin(), std::find(cloudsXYZRGB.names.begin(), cloudsXYZRGB.names.end(), name))).second = checkBox->isChecked();
    }
    visualizer.update();
}

void MainWindow::deleteCloud(QPushButton *pushButton)
{
    QString name = "";
    QString type = "";
    QString obj_name = pushButton->objectName();
    int i = 0;
    while(obj_name[i] != '_')
    {
        i++;
    }
    i++;
    while(obj_name[i] != '_')
    {
        type += obj_name[i];
        i++;
    }
    i++;
    for(i; i < obj_name.size(); i++)
    {
        name += obj_name[i];
    }
    if(type == "xyz")
    {
        int del = std::distance(cloudsXYZ.names.begin(), std::find(cloudsXYZ.names.begin(), cloudsXYZ.names.end(), name));
        cloudsXYZ.clouds.erase(cloudsXYZ.clouds.begin() + del);
        cloudsXYZ.names.erase(cloudsXYZ.names.begin() + del);
        visualizer.deleteXYZ(del);
    }
    else if(type == "xyzi")
    {
        int del = std::distance(cloudsXYZI.names.begin(), std::find(cloudsXYZI.names.begin(), cloudsXYZI.names.end(), name));
        cloudsXYZI.clouds.erase(cloudsXYZI.clouds.begin() + del);
        cloudsXYZI.names.erase(cloudsXYZI.names.begin() + del);
        visualizer.deleteXYZI(del);
    }
    else if(type == "xyzrgb")
    {
        int del = std::distance(cloudsXYZRGB.names.begin(), std::find(cloudsXYZRGB.names.begin(), cloudsXYZRGB.names.end(), name));
        cloudsXYZRGB.clouds.erase(cloudsXYZRGB.clouds.begin() + del);
        cloudsXYZRGB.names.erase(cloudsXYZRGB.names.begin() + del);
        visualizer.deleteXYZRGB(del);
    }
    MainWindow::updateClouds();
    visualizer.update();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   ui->listClouds->setMaximumSize(this->width() * 0.25, ui->listClouds->maximumHeight());
   ui->listClouds->setMinimumSize(200, ui->listClouds->minimumHeight());
   ui->listClouds->resize(this->width() * 0.25, ui->listClouds->height());
   if(ui->listClouds->columnCount() > 0)
   {
       QTableWidget *table = ui->listClouds;
       table->setColumnWidth(0, 0.1 * table->width());
       table->setColumnWidth(1, 0.5 * table->width());
       table->setColumnWidth(2, 0.2 * table->width());
       table->setColumnWidth(3, 0.1 * table->width());
   }
}

void MainWindow::updateClouds()
{
    bool empty = (cloudsXYZ.clouds.empty() && cloudsXYZI.clouds.empty() && cloudsXYZRGB.clouds.empty());
    QTableWidget *table = ui->listClouds;
    if(!empty)
    {
        table->setColumnCount(4);
        table->setColumnWidth(0, 0.1 * table->width());
        table->setColumnWidth(1, 0.5 * table->width());
        table->setColumnWidth(2, 0.2 * table->width());
        table->setColumnWidth(3, 0.1 * table->width());
        QStringList header;
        header << "+" << "Name" << "Type" << "-";
        table->setHorizontalHeaderLabels(header);

        int colOfClouds = 1;

        for(int i = 0; i < cloudsXYZ.clouds.size(); i++)
        {
            table->setRowCount(colOfClouds);

            QCheckBox *check = new QCheckBox();
            QTableWidgetItem *name = new QTableWidgetItem(cloudsXYZ.names.at(i));
            QTableWidgetItem *type = new QTableWidgetItem(tr("XYZ"));
            QPushButton *del = new QPushButton(tr("-"));
            del->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
            check->setCheckState((cloudsXYZ.clouds.at(i).second? Qt::Checked : Qt::Unchecked));

            QString widgetSelectName = "opened_xyz_" + cloudsXYZ.names[i];
            QString widgetDeleteName = "delete_xyz_" + cloudsXYZ.names[i];

            check->setObjectName(widgetSelectName);
            del->setObjectName(widgetDeleteName);

            table->setCellWidget(colOfClouds - 1, 0, check);
            table->setItem(colOfClouds - 1, 1, name);
            table->setItem(colOfClouds - 1, 2, type);
            table->setCellWidget(colOfClouds - 1, 3, del);

            connect(check, &QCheckBox::toggled, [this, check](){
                MainWindow::changedStatus(check);
            });

            connect(del, &QPushButton::clicked, [this, del](){
                MainWindow::deleteCloud(del);
            });

            colOfClouds++;
        }

        for(int i = 0; i < cloudsXYZI.clouds.size(); i++)
        {
            table->setRowCount(colOfClouds);

            QCheckBox *check = new QCheckBox();
            QTableWidgetItem *name = new QTableWidgetItem(cloudsXYZI.names.at(i));
            QTableWidgetItem *type = new QTableWidgetItem(tr("I"));
            QPushButton *del = new QPushButton(tr("-"));
            del->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
            check->setCheckState((cloudsXYZI.clouds.at(i).second? Qt::Checked : Qt::Unchecked));

            QString widgetSelectName = "opened_xyzi_" + cloudsXYZI.names[i];
            QString widgetDeleteName = "delete_xyzi_" + cloudsXYZI.names[i];

            check->setObjectName(widgetSelectName);
            del->setObjectName(widgetDeleteName);

            table->setCellWidget(colOfClouds - 1, 0, check);
            table->setItem(colOfClouds - 1, 1, name);
            table->setItem(colOfClouds - 1, 2, type);
            table->setCellWidget(colOfClouds - 1, 3, del);

            connect(check, &QCheckBox::toggled, [this, check](){
                MainWindow::changedStatus(check);
            });

            connect(del, &QPushButton::clicked, [this, del](){
                MainWindow::deleteCloud(del);
            });

            colOfClouds++;
        }
        for(int i = 0; i < cloudsXYZRGB.clouds.size(); i++)
        {
            table->setRowCount(colOfClouds);

            QCheckBox *check = new QCheckBox();
            QTableWidgetItem *name = new QTableWidgetItem(cloudsXYZRGB.names.at(i));
            QTableWidgetItem *type = new QTableWidgetItem(tr("RGB"));
            QPushButton *del = new QPushButton(tr("-"));
            del->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
            check->setCheckState((cloudsXYZRGB.clouds.at(i).second? Qt::Checked : Qt::Unchecked));

            QString widgetSelectName = "opened_xyzrgb_" + cloudsXYZRGB.names[i];
            QString widgetDeleteName = "delete_xyzrgb_" + cloudsXYZRGB.names[i];

            check->setObjectName(widgetSelectName);
            del->setObjectName(widgetDeleteName);

            table->setCellWidget(colOfClouds - 1, 0, check);
            table->setItem(colOfClouds - 1, 1, name);
            table->setItem(colOfClouds - 1, 2, type);
            table->setCellWidget(colOfClouds - 1, 3, del);

            connect(check, &QCheckBox::toggled, [this, check](){
                MainWindow::changedStatus(check);
            });

            connect(del, &QPushButton::clicked, [this, del](){
                MainWindow::deleteCloud(del);
            });

            colOfClouds++;
        }
    }
    else
    {
        table->clear();
        table->setColumnCount(0);
        table->setRowCount(0);
    }
}

void MainWindow::on_actionOpenCloud_triggered()
{
    QFileDialog dialog(this);
    dialog.setDirectory("/Users/larong/Desktop");
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("PLY files (*.ply)"));
    QStringList fileNames;
    if (dialog.exec())
    {
        fileNames = dialog.selectedFiles();
//        std::cout << fileNames.size() << " file(s) openes: " << std::endl;
        for(auto &name : fileNames)
        {
            std::fstream fileToRead;
            fileToRead.open(name.toStdString());

            QString fileName;
            for(char &c : name.toStdString())
            {
                if(c == '/')
                {
                    fileName = "";
                }
                else if(c == '.')
                {
                    break;
                }
                else
                {
                    fileName.push_back(c);
                }
            }

//            std::cout << fileName.toStdString() << std::endl;

            std::string content;
            long long size;

            int type = 0;

            while(fileToRead>>content && content != "end_header")
            {
                if(content == "vertex")
                {
                    fileToRead>>content;
                    size = std::stoll(content);
                }
                if(content == "scalar_intensity")
                {
                    type = 1;
                }
                if(content == "r")
                {
                    type = 2;
                }
            }

            if(size != 0)
            {
                if(type == 0)
                {
                    pointCloud<point> openedCloud;

                    for(long long i = 0; i < size; i++)
                    {
                        float x, y, z;
                        fileToRead >> x >> y >> z;
                        openedCloud.push_back(point(x, y, z));
                    }
                    cloudsXYZ.clouds.push_back({openedCloud, 1});
                    int numberOfCopy = 0;
                    while(std::find(cloudsXYZ.names.begin(), cloudsXYZ.names.end(), fileName) != cloudsXYZ.names.end() ||
                          std::find(cloudsXYZI.names.begin(), cloudsXYZI.names.end(), fileName) != cloudsXYZI.names.end() ||
                          std::find(cloudsXYZRGB.names.begin(), cloudsXYZRGB.names.end(), fileName) != cloudsXYZRGB.names.end())
                    {
                        if(fileName.back() >= '0' && fileName.back() <= '9')
                        {
                            while(fileName.back() != '_')
                            {
                                fileName.resize(fileName.size() - 1);
                            }
                            fileName.resize(fileName.size() - 1);
                        }
                        fileName.push_back('_');
                        numberOfCopy++;
                        for(char c : std::to_string(numberOfCopy))
                        {
                            fileName.push_back(c);
                        }
                    }
//                    std::cout << fileName.toStdString() << std::endl;
                    cloudsXYZ.names.push_back(fileName);
                    visualizer.addCloudXYZ();
                }
                else if(type == 1)
                {
                    pointCloud<pointI> openedCloud;

                    for(long long i = 0; i < size; i++)
                    {
                        float x, y, z, intensity;
                        fileToRead >> x >> y >> z >> intensity;
                        openedCloud.push_back(pointI(x, y, z, intensity));
                    }

                    cloudsXYZI.clouds.push_back({openedCloud, 1});
                    int numberOfCopy = 0;
                    while(std::find(cloudsXYZ.names.begin(), cloudsXYZ.names.end(), fileName) != cloudsXYZ.names.end() ||
                          std::find(cloudsXYZI.names.begin(), cloudsXYZI.names.end(), fileName) != cloudsXYZI.names.end() ||
                          std::find(cloudsXYZRGB.names.begin(), cloudsXYZRGB.names.end(), fileName) != cloudsXYZRGB.names.end())
                    {
                        if(fileName.back() >= '0' && fileName.back() <= '9')
                        {
                            while(fileName.back() != '_')
                            {
                                fileName.resize(fileName.size() - 1);
                            }
                            fileName.resize(fileName.size() - 1);
                        }
                        fileName.push_back('_');
                        numberOfCopy++;
                        for(char c : std::to_string(numberOfCopy))
                        {
                            fileName.push_back(c);
                        }
                    }
                    cloudsXYZI.names.push_back(fileName);
                    visualizer.addCloudXYZI();
                }
                else if(type == 2)
                {
                    pointCloud<pointRGB> openedCloud;

                    for(long long i = 0; i < size; i++){
                        float x, y, z;
                        unsigned short r, g, b;
                        fileToRead >> x >> y >> z >> r >> g >> b;
                        openedCloud.push_back(pointRGB(x, y, z, r, g, b));
                    }

                    cloudsXYZRGB.clouds.push_back({openedCloud, 1});
                    int numberOfCopy = 0;
                    while(std::find(cloudsXYZ.names.begin(), cloudsXYZ.names.end(), fileName) != cloudsXYZ.names.end() ||
                          std::find(cloudsXYZI.names.begin(), cloudsXYZI.names.end(), fileName) != cloudsXYZI.names.end() ||
                          std::find(cloudsXYZRGB.names.begin(), cloudsXYZRGB.names.end(), fileName) != cloudsXYZRGB.names.end())
                    {
                        if(fileName.back() >= '0' && fileName.back() <= '9')
                        {
                            while(fileName.back() != '_')
                            {
                                fileName.resize(fileName.size() - 1);
                            }
                            fileName.resize(fileName.size() - 1);
                        }
                        fileName.push_back('_');
                        numberOfCopy++;
                        for(char c : std::to_string(numberOfCopy))
                        {
                            fileName.push_back(c);
                        }
                    }
                    cloudsXYZRGB.names.push_back(fileName);
                    visualizer.addCloudXYZRGB();
                }

            }
        }
        MainWindow::updateClouds();
    }
}

void MainWindow::on_actionSaveClouds_triggered()
{
    for(int i = 0; i < cloudsXYZ.names.size(); i++)
    {
        if(cloudsXYZ.clouds.at(i).second)
        {
            QFileDialog dialog;
            dialog.setFileMode(QFileDialog::AnyFile);
            QString dir = "/Users/larong/Desktop/" + cloudsXYZ.names.at(i);
            QString strFile = dialog.getSaveFileName(this, tr("Save point cloud"), dir , tr("PLY files (*.ply)"));
            QFile file(strFile);

            if(file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QTextStream out(&file);

                out << "ply" << '\n' << "format ascii 1.0" << '\n';
                out << "comment Created by Cloud Visualizer " << VERSION << " (Larong)" << '\n';

                QDateTime date = QDateTime::currentDateTime();
                QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss");
                QByteArray formattedTimeMsg = formattedTime.toLocal8Bit();
                out << "comment Created " << formattedTime << '\n';

                out << "obj_info Generated by Cloud Visualizer!" << '\n';

                out << "element vertex " << cloudsXYZ.clouds.at(i).first.size() << '\n';

                out << "property float x" << '\n';
                out << "property float y" << '\n';
                out << "property float z" << '\n';

                out << "end_header" << '\n';

                for(unsigned long long j = 0; j < cloudsXYZ.clouds.at(i).first.size(); j++)
                {
                    out << cloudsXYZ.clouds.at(i).first[j].x() << ' ' << cloudsXYZ.clouds.at(i).first[j].y() << ' ' << cloudsXYZ.clouds.at(i).first[j].z() << '\n';
                }
            }

            file.close();
        }
    }
    for(int i = 0; i < cloudsXYZI.names.size(); i++)
    {
        if(cloudsXYZI.clouds.at(i).second)
        {
            QFileDialog dialog;
            dialog.setFileMode(QFileDialog::AnyFile);
            QString dir = "/Users/larong/Desktop/" + cloudsXYZI.names.at(i);
            QString strFile = dialog.getSaveFileName(this, tr("Save point cloud"), dir , tr("PLY files (*.ply)"));
            QFile file(strFile);

            if(file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QTextStream out(&file);

                out << "ply" << '\n' << "format ascii 1.0" << '\n';
                out << "comment Created by Cloud Visualizer " << VERSION << " (Larong)" << '\n';

                QDateTime date = QDateTime::currentDateTime();
                QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss");
                QByteArray formattedTimeMsg = formattedTime.toLocal8Bit();
                out << "comment Created " << formattedTime << '\n';

                out << "obj_info Generated by Cloud Visualizer!" << '\n';

                out << "element vertex " << cloudsXYZI.clouds.at(i).first.size() << '\n';

                out << "property float x" << '\n';
                out << "property float y" << '\n';
                out << "property float z" << '\n';
                out << "property float scalar_intensity" << '\n';

                out << "end_header" << '\n';

                for(unsigned long long j = 0; j < cloudsXYZI.clouds.at(i).first.size(); j++)
                {
                    out << cloudsXYZI.clouds.at(i).first[j].x() << ' ' << cloudsXYZI.clouds.at(i).first[j].y() << ' ' << cloudsXYZI.clouds.at(i).first[j].z() << ' ' << cloudsXYZI.clouds.at(i).first[j].intensity() << '\n';
                }
            }

            file.close();
        }
    }
    for(int i = 0; i < cloudsXYZRGB.names.size(); i++)
    {
        if(cloudsXYZRGB.clouds.at(i).second)
        {
            QFileDialog dialog;
            dialog.setFileMode(QFileDialog::AnyFile);
            QString dir = "/Users/larong/Desktop/" + cloudsXYZRGB.names.at(i);
            QString strFile = dialog.getSaveFileName(this, tr("Save point cloud"), dir , tr("PLY files (*.ply)"));
            QFile file(strFile);

            if(file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QTextStream out(&file);

                out << "ply" << '\n' << "format ascii 1.0" << '\n';
                out << "comment Created by Cloud Visualizer " << VERSION << " (Larong)" << '\n';

                QDateTime date = QDateTime::currentDateTime();
                QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss");
                QByteArray formattedTimeMsg = formattedTime.toLocal8Bit();
                out << "comment Created " << formattedTime << '\n';

                out << "obj_info Generated by Cloud Visualizer!" << '\n';

                out << "element vertex " << cloudsXYZRGB.clouds.at(i).first.size() << '\n';

                out << "property float x" << '\n';
                out << "property float y" << '\n';
                out << "property float z" << '\n';
                out << "property unsigned short r" << '\n';
                out << "property unsigned short g" << '\n';
                out << "property unsigned short b" << '\n';

                out << "end_header" << '\n';

                for(unsigned long long j = 0; j < cloudsXYZRGB.clouds.at(i).first.size(); j++)
                {
                    out << cloudsXYZRGB.clouds.at(i).first[j].x() << ' ' << cloudsXYZRGB.clouds.at(i).first[j].y() << ' ' << cloudsXYZRGB.clouds.at(i).first[j].z() << ' ';
                    out << cloudsXYZRGB.clouds.at(i).first[j].r() << ' ' << cloudsXYZRGB.clouds.at(i).first[j].g() << ' ' << cloudsXYZRGB.clouds.at(i).first[j].b() << '\n';
                }
            }

            file.close();
        }
    }
}

void MainWindow::on_actionSaveAllClouds_triggered()
{
    for(int i = 0; i < cloudsXYZ.names.size(); i++)
    {
        QFileDialog dialog;
        dialog.setFileMode(QFileDialog::AnyFile);
        QString dir = "/Users/larong/Desktop/" + cloudsXYZ.names.at(i);
        QString strFile = dialog.getSaveFileName(this, tr("Save point cloud"), dir , tr("PLY files (*.ply)"));
        QFile file(strFile);

        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);

            out << "ply" << '\n' << "format ascii 1.0" << '\n';
            out << "comment Created by Cloud Visualizer " << VERSION << " (Larong)" << '\n';

            QDateTime date = QDateTime::currentDateTime();
            QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss");
            QByteArray formattedTimeMsg = formattedTime.toLocal8Bit();
            out << "comment Created " << formattedTime << '\n';

            out << "obj_info Generated by Cloud Visualizer!" << '\n';

            out << "element vertex " << cloudsXYZ.clouds.at(i).first.size() << '\n';

            out << "property float x" << '\n';
            out << "property float y" << '\n';
            out << "property float z" << '\n';

            out << "end_header" << '\n';

            for(unsigned long long j = 0; j < cloudsXYZ.clouds.at(i).first.size(); j++)
            {
                out << cloudsXYZ.clouds.at(i).first[j].x() << ' ' << cloudsXYZ.clouds.at(i).first[j].y() << ' ' << cloudsXYZ.clouds.at(i).first[j].z() << '\n';
            }
        }

        file.close();
    }
    for(int i = 0; i < cloudsXYZI.names.size(); i++)
    {
        QFileDialog dialog;
        dialog.setFileMode(QFileDialog::AnyFile);
        QString dir = "/Users/larong/Desktop/" + cloudsXYZI.names.at(i);
        QString strFile = dialog.getSaveFileName(this, tr("Save point cloud"), dir , tr("PLY files (*.ply)"));
        QFile file(strFile);

        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);

            out << "ply" << '\n' << "format ascii 1.0" << '\n';
            out << "comment Created by Cloud Visualizer " << VERSION << " (Larong)" << '\n';

            QDateTime date = QDateTime::currentDateTime();
            QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss");
            QByteArray formattedTimeMsg = formattedTime.toLocal8Bit();
            out << "comment Created " << formattedTime << '\n';

            out << "obj_info Generated by Cloud Visualizer!" << '\n';

            out << "element vertex " << cloudsXYZI.clouds.at(i).first.size() << '\n';

            out << "property float x" << '\n';
            out << "property float y" << '\n';
            out << "property float z" << '\n';
            out << "property float scalar_intensity" << '\n';

            out << "end_header" << '\n';

            for(unsigned long long j = 0; j < cloudsXYZI.clouds.at(i).first.size(); j++)
            {
                out << cloudsXYZI.clouds.at(i).first[j].x() << ' ' << cloudsXYZI.clouds.at(i).first[j].y() << ' ' << cloudsXYZI.clouds.at(i).first[j].z() << ' ' << cloudsXYZI.clouds.at(i).first[j].intensity() << '\n';
            }
        }

        file.close();
    }
    for(int i = 0; i < cloudsXYZRGB.names.size(); i++)
    {
        QFileDialog dialog;
        dialog.setFileMode(QFileDialog::AnyFile);
        QString dir = "/Users/larong/Desktop/" + cloudsXYZRGB.names.at(i);
        QString strFile = dialog.getSaveFileName(this, tr("Save point cloud"), dir , tr("PLY files (*.ply)"));
        QFile file(strFile);

        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);

            out << "ply" << '\n' << "format ascii 1.0" << '\n';
            out << "comment Created by Cloud Visualizer " << VERSION << " (Larong)" << '\n';

            QDateTime date = QDateTime::currentDateTime();
            QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss");
            QByteArray formattedTimeMsg = formattedTime.toLocal8Bit();
            out << "comment Created " << formattedTime << '\n';

            out << "obj_info Generated by Cloud Visualizer!" << '\n';

            out << "element vertex " << cloudsXYZRGB.clouds.at(i).first.size() << '\n';

            out << "property float x" << '\n';
            out << "property float y" << '\n';
            out << "property float z" << '\n';

            out << "end_header" << '\n';

            for(unsigned long long j = 0; j < cloudsXYZRGB.clouds.at(i).first.size(); j++)
            {
                out << cloudsXYZRGB.clouds.at(i).first[j].x() << ' ' << cloudsXYZRGB.clouds.at(i).first[j].y() << ' ' << cloudsXYZRGB.clouds.at(i).first[j].z() << ' ';
                out << cloudsXYZRGB.clouds.at(i).first[j].r() << ' ' << cloudsXYZRGB.clouds.at(i).first[j].g() << ' ' << cloudsXYZRGB.clouds.at(i).first[j].b() << '\n';
            }
        }

        file.close();
    }
}


//void MainWindow::on_showCloud_clicked()
//{
//    std::cout << "SELECTED: " << std::endl;
//    for(int i = 0; i < cloudsXYZ.clouds.size(); i++)
//    {
//        if(cloudsXYZ.clouds[i].second)
//        {
//            std::cout << cloudsXYZ.names[i].toStdString() << std::endl;
//        }
//    }
//    for(int i = 0; i < cloudsXYZI.clouds.size(); i++)
//    {
//        if(cloudsXYZI.clouds[i].second)
//        {
//            std::cout << cloudsXYZI.names[i].toStdString() << std::endl;
//        }
//    }
//    for(int i = 0; i < cloudsXYZRGB.clouds.size(); i++)
//    {
//        if(cloudsXYZRGB.clouds[i].second)
//        {
//            std::cout << cloudsXYZRGB.names[i].toStdString() << std::endl;
//        }
//    }
//}

