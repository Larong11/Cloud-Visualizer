#ifndef CLOUDSVISUALISER_H
#define CLOUDSVISUALISER_H

#include <QtOpenGL>
#include <OpenGL/glu.h>
#include <glm.h>
#include <iostream>
#include <QOpenGLWidget>

class cloudsVisualiser : public QOpenGLWidget
{
    Q_OBJECT
public:
    cloudsVisualiser(QWidget *parent = 0);
    virtual ~cloudsVisualiser() = default;
};

#endif // CLOUDSVISUALISER_H
