#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpenCloud_triggered()
{
    std::cout << "OPEN FILES" << std::endl;
    QFileDialog dialog(this);
    dialog.setDirectory("/Users/larong/Desktop");
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("PLY files (*.ply)"));
    QStringList fileNames;
    if (dialog.exec())
    {
        fileNames = dialog.selectedFiles();
        std::cout << fileNames.size() << " file(s) openes: " << std::endl;
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

            std::cout << fileName.toStdString() << std::endl;

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
                QHBoxLayout *l = new QHBoxLayout();
                QLabel *name;
                QCheckBox *check = new QCheckBox(fileName);
                if(type == 0)
                {
                    pointCloud<point> openedCloud;

                    for(long long i = 0; i < size; i++)
                    {
                        float x, y, z;
                        fileToRead >> x >> y >> z;
                        openedCloud.push_back(point(x, y, z));
                    }

                    name = new QLabel("pointXYZ");

                    cloudsPoint.push_back({openedCloud, fileName.toStdString()});
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

                    name = new QLabel("pointXYZI");

                    cloudsPointI.push_back({openedCloud, fileName.toStdString()});
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

                    name = new QLabel("pointXYZRGB");

                    cloudsPointRGB.push_back({openedCloud, fileName.toStdString()});
                }
                QString widgetName = "cloud" + fileName;
                check->setObjectName(widgetName);
                name->setAlignment(Qt::AlignRight);

                l->addWidget(check);
                l->addWidget(name);

                ui->clouds->addLayout(l);
            }
        }
    }
}

