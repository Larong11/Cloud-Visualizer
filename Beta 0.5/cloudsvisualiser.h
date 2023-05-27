#ifndef CLOUDSVISUALISER_H
#define CLOUDSVISUALISER_H

#include <QtOpenGL>
#include <OpenGL/glu.h>
#include <glm.h>
#include <iostream>
#include <QOpenGLWidget>
#include <pointcloud.h>

#define POINT_DIM 3

template <typename T>
struct insertedClouds
{
    std::vector<std::pair<pointCloud<T>, bool>> clouds;
    std::vector<QString> names;
};

struct maxMin
{
    float maxX, minX;
    float maxY, minY;
    float maxZ, minZ;
    float maxI, minI;
    maxMin()
    {
        maxX = -1e9;
        minX = 1e9;
        maxY = -1e9;
        minY = 1e9;
        maxZ = -1e9;
        minZ = 1e9;
        maxI = -1e9;
        minI = 1e9;
    }
};

struct pointCloudData
{
    GLfloat *vertexData;
    GLubyte *colorData;
    bool selected;
    unsigned short scheme;
    unsigned int size;
    maxMin m;
    pointCloudData()
    {
        size = 0;
        selected = false;
    }
};

class cloudsVisualiser : public QOpenGLWidget
{
    Q_OBJECT
public:
    cloudsVisualiser(QWidget *parent = 0);

    void addClouds(insertedClouds<point> *cloudsX, insertedClouds<pointI> *cloudsI, insertedClouds<pointRGB> *cloudsR);

    void addCloudXYZ(int scheme);
    void changeSchemeXYZ(int scheme);
    void selectCloudXYZ(int index);
    void unselectCloudXYZ(int index, int scheme);
    void deleteXYZ(int index);

    void addCloudXYZI(int scheme);
    void changeSchemeXYZI(int scheme);
    void selectCloudXYZI(int index);
    void unselectCloudXYZI(int index, int scheme);
    void deleteXYZI(int index);

    void addCloudXYZRGB(int scheme);
    void changeSchemeXYZRGB(int scheme);
    void selectCloudXYZRGB(int index);
    void unselectCloudXYZRGB(int index, int scheme);
    void deleteXYZRGB(int index);

    virtual ~cloudsVisualiser() = default;

public slots:
    void initializeGL(); //init
    void paintGL(); //paint
    void resizeGL(int w, int h); //resize

    void mousePressEvent(QMouseEvent *event); //mouse press
    void mouseMoveEvent(QMouseEvent *event); //mouse move
    void mouseReleaseEvent(QMouseEvent *event); //mouse release
    void keyPressEvent(QKeyEvent *event); //keynoard press
    void keyReleaseEvent(QKeyEvent *event); //keyboard release
    void wheelEvent(QWheelEvent *event); //mouse wheel event

private:
    void updateCamPos();

    float posX, posY, posZ;
    float dirX, dirY, dirZ;
    double angleX, angleY;
    float zeroX, zeroY, zeroZ;
    float dist;

    QPoint mPrevPos;
    bool isLeft, isRight;
    const double MOUSE_SENS = 0.005;

    float kSens = 0.5;

    insertedClouds<point> *cloudsXYZ;
    insertedClouds<pointI> *cloudsXYZI;
    insertedClouds<pointRGB> *cloudsXYZRGB;

    std::vector<pointCloudData> dataXYZ, dataXYZI, dataXYZRGB;
};

#endif // CLOUDSVISUALISER_H
