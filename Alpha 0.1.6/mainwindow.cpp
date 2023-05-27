#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>

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
//    std::cout << type.toStdString() << ' ' << name.toStdString() << std::endl;
    if(type == "xyz")
    {
        cloudsXYZ.clouds.at(std::distance(cloudsXYZ.names.begin(), std::find(cloudsXYZ.names.begin(), cloudsXYZ.names.end(), name))).second = checkBox->isChecked();
    }
    else if(type == "xyzi")
    {
        cloudsXYZI.clouds.at(std::distance(cloudsXYZI.names.begin(), std::find(cloudsXYZI.names.begin(), cloudsXYZI.names.end(), name))).second = checkBox->isChecked();
    }
    else
    {
        cloudsXYZRGB.clouds.at(std::distance(cloudsXYZRGB.names.begin(), std::find(cloudsXYZRGB.names.begin(), cloudsXYZRGB.names.end(), name))).second = checkBox->isChecked();
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
                    while(std::find(cloudsXYZ.names.begin(), cloudsXYZ.names.end(), fileName) != cloudsXYZ.names.end())
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
                    cloudsXYZ.names.push_back(fileName);
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
                    while(std::find(cloudsXYZI.names.begin(), cloudsXYZI.names.end(), fileName) != cloudsXYZI.names.end())
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
                    while(std::find(cloudsXYZRGB.names.begin(), cloudsXYZRGB.names.end(), fileName) != cloudsXYZRGB.names.end())
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
                }

            }
        }
        for(QHBoxLayout *l : ui->clouds->findChildren<QHBoxLayout*>())
        {
            delete l;
        }
        for(QLabel *label : ui->cloudsWidget->findChildren<QLabel*>())
        {
            label->setVisible(false);
            delete label;
        }
        for(QCheckBox *check : ui->cloudsWidget->findChildren<QCheckBox*>())
        {
            check->setVisible(false);
            delete check;
        }
        if(!cloudsXYZ.clouds.empty())
        {
            QHBoxLayout *XYZ = new QHBoxLayout(ui->cloudsWidget);
            XYZ->setObjectName("XYZ");
            QLabel *nameXYZ = new QLabel("Cloud XYZ points", ui->cloudsWidget);
            nameXYZ->setAlignment(Qt::AlignHCenter);
            XYZ->addWidget(nameXYZ);
            ui->clouds->addLayout(XYZ);

            for(int i = 0; i < cloudsXYZ.clouds.size(); i++)
            {
                QHBoxLayout *l = new QHBoxLayout(ui->cloudsWidget);
                QCheckBox *check = new QCheckBox(ui->cloudsWidget);
                check->setText(cloudsXYZ.names[i]);
                check->setCheckState((cloudsXYZ.clouds[i].second? Qt::Checked : Qt::Unchecked));

                QString widgetName = "opened_xyz_" + cloudsXYZ.names[i];
                QString layoutName = "layout_" + cloudsXYZ.names[i];
                check->setObjectName(widgetName);
                l->setObjectName(layoutName);

                l->addWidget(check);
                ui->clouds->addLayout(l);

                connect(check, &QCheckBox::toggled, [this, &check](){
                    MainWindow::changedStatus(check);
                });
            }
        }
        if(!cloudsXYZI.clouds.empty())
        {
            QHBoxLayout *XYZI = new QHBoxLayout(ui->cloudsWidget);
            XYZI->setObjectName("XYZI");
            QLabel *nameXYZI= new QLabel("Cloud XYZI points", ui->cloudsWidget);
            nameXYZI->setAlignment(Qt::AlignHCenter);
            XYZI->addWidget(nameXYZI);
            ui->clouds->addLayout(XYZI);

            for(int i = 0; i < cloudsXYZI.clouds.size(); i++)
            {
                QHBoxLayout *l = new QHBoxLayout(ui->cloudsWidget);
                QCheckBox *check = new QCheckBox(ui->cloudsWidget);
                check->setText(cloudsXYZI.names[i]);
                check->setCheckState((cloudsXYZI.clouds[i].second? Qt::Checked : Qt::Unchecked));

                QString widgetName = "opened_xyzi_" + cloudsXYZI.names[i];
                QString layoutName = "layout_" + cloudsXYZI.names[i];
                check->setObjectName(widgetName);
                l->setObjectName(layoutName);

                l->addWidget(check);
                ui->clouds->addLayout(l);

                connect(check, &QCheckBox::toggled, [this, &check](){
                    MainWindow::changedStatus(check);
                });
            }
        }
        if(!cloudsXYZRGB.clouds.empty())
        {
            QHBoxLayout *XYZRGB = new QHBoxLayout(ui->cloudsWidget);
            XYZRGB->setObjectName("XYZRGB");
            QLabel *nameXYZRGB= new QLabel("Cloud XYZRGB points", ui->cloudsWidget);
            nameXYZRGB->setAlignment(Qt::AlignHCenter);
            XYZRGB->addWidget(nameXYZRGB);
            ui->clouds->addLayout(XYZRGB);

            for(int i = 0; i < cloudsXYZRGB.clouds.size(); i++)
            {
                QHBoxLayout *l = new QHBoxLayout(ui->cloudsWidget);
                QCheckBox *check = new QCheckBox(ui->cloudsWidget);
                check->setText(cloudsXYZRGB.names[i]);
                check->setCheckState((cloudsXYZRGB.clouds[i].second? Qt::Checked : Qt::Unchecked));

                QString widgetName = "opened_xyzrgb_" + cloudsXYZRGB.names[i];
                QString layoutName = "layout_" + cloudsXYZRGB.names[i];
                check->setObjectName(widgetName);
                l->setObjectName(layoutName);

                l->addWidget(check);
                ui->clouds->addLayout(l);

                connect(check, &QCheckBox::toggled, [this, &check](){
                    MainWindow::changedStatus(check);
                });
            }
        }
    }
}

