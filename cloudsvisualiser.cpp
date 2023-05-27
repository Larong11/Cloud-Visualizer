#include "cloudsvisualiser.h"

cloudsVisualiser::cloudsVisualiser(QWidget *parent)
{
    dist = 5;
    angleX = M_PI / 3;
    angleY = M_PI / 4;

    zeroX = 0;
    zeroY = 0;
    zeroZ = 0;

    isLeft = false;
    isRight = false;
}

void cloudsVisualiser::addClouds(insertedClouds<point> *cloudsX, insertedClouds<pointI> *cloudsI, insertedClouds<pointRGB> *cloudsR)
{
    cloudsXYZ = cloudsX;
    cloudsXYZI = cloudsI;
    cloudsXYZRGB = cloudsR;
}

void cloudsVisualiser::updateCamPos()
{
    if(angleX > M_PI) angleX -= 2 * M_PI;
    else if(angleX < -M_PI) angleX += 2 * M_PI;
    if(angleY > M_PI) angleY -= 2 * M_PI;
    else if(angleY < -M_PI) angleY += 2 * M_PI;

    posX = dist * sin(angleX) * cos(angleY) + zeroX;
    posY = dist * sin(angleX) * sin(angleY) + zeroY;
    posZ = dist * cos(angleX) + zeroZ;

    dirX = -posX + zeroX;
    dirY = -posY + zeroY;
    dirZ = -posZ + zeroZ;
}

void cloudsVisualiser::addCloudXYZ(int scheme)
{
    std::pair<pointCloud<point>, bool> &cloud = cloudsXYZ->clouds.back();
    pointCloudData data;
    data.size = cloud.first.size();
    data.scheme = scheme;
    data.vertexData = (GLfloat*)malloc(sizeof(GLfloat) * POINT_DIM * cloud.first.size());
    data.colorData = (GLubyte*)malloc(sizeof(GLubyte) * POINT_DIM * cloud.first.size());
    for(int i = 0; i < cloud.first.size(); i++)
    {
        float x = cloud.first[i].x();
        float y = cloud.first[i].y();
        float z = cloud.first[i].z();
        if(x > data.m.maxX)
        {
            data.m.maxX = x;
        }
        if(x < data.m.minX)
        {
            data.m.minX = x;
        }
        if(y > data.m.maxY)
        {
            data.m.maxY = y;
        }
        if(y < data.m.minY)
        {
            data.m.minY = y;
        }
        if(z > data.m.maxZ)
        {
            data.m.maxZ = z;
        }
        if(z < data.m.minZ)
        {
            data.m.minZ = z;
        }
    }
    for(int i = 0; i < cloud.first.size(); i++)
    {
        data.vertexData[i * POINT_DIM + 0] = cloud.first[i].x();
        data.vertexData[i * POINT_DIM + 1] = cloud.first[i].y();
        data.vertexData[i * POINT_DIM + 2] = cloud.first[i].z();

        float a;
        int r, g, b;

        if(data.scheme == 0)
        {
            a = (cloud.first[i].x() - data.m.minX) / (data.m.maxX - data.m.minX) * M_PI * 2 + M_PI / 10 * 3;
        }
        else if(data.scheme == 1)
        {
            a = (cloud.first[i].y() - data.m.minY) / (data.m.maxY - data.m.minY) * M_PI * 2 + M_PI / 10 * 3;
        }
        else if(data.scheme == 2)
        {
            a = (cloud.first[i].z() - data.m.minZ) / (data.m.maxZ - data.m.minZ) * M_PI * 2 + M_PI / 10 * 3;
        }

        r = sin(a) * 192 + 128;
        g = sin(a - 2 * M_PI / 3.0) * 192 + 128;
        b = sin(a - 4 * M_PI / 3.0) * 192 + 128;

        if(r > 255) r = 255;
        else if(r < 0) r = 0;
        if(g > 255) g = 255;
        else if(g < 0) g = 0;
        if(b > 255) b = 255;
        else if(b < 0) b = 0;

        data.colorData[i * POINT_DIM + 0] = r;
        data.colorData[i * POINT_DIM + 1] = g;
        data.colorData[i * POINT_DIM + 2] = b;
    }
    dataXYZ.push_back(data);
    update();
}

void cloudsVisualiser::changeSchemeXYZ(int scheme)
{
    for(int i = 0; i < cloudsXYZ->clouds.size(); i++)
    {
        if(dataXYZ[i].scheme != scheme && !dataXYZ[i].selected)
        {
            dataXYZ[i].scheme = scheme;
            for(int j = 0; j < dataXYZ[i].size; j++)
            {
                float a;
                int r, g, b;

                if(scheme == 0)
                {
                    a = (cloudsXYZ->clouds[i].first[j].x() - dataXYZ[i].m.minX) / (dataXYZ[i].m.maxX - dataXYZ[i].m.minX) * M_PI * 2 + M_PI / 10 * 3;
                }
                else if(scheme == 1)
                {
                    a = (cloudsXYZ->clouds[i].first[j].y() - dataXYZ[i].m.minY) / (dataXYZ[i].m.maxY - dataXYZ[i].m.minY) * M_PI * 2 + M_PI / 10 * 3;
                }
                else if(scheme == 2)
                {
                    a = (cloudsXYZ->clouds[i].first[j].z() - dataXYZ[i].m.minZ) / (dataXYZ[i].m.maxZ - dataXYZ[i].m.minZ) * M_PI * 2 + M_PI / 10 * 3;
                }

                r = sin(a) * 192 + 128;
                g = sin(a - 2 * M_PI / 3.0) * 192 + 128;
                b = sin(a - 4 * M_PI / 3.0) * 192 + 128;

                if(r > 255) r = 255;
                else if(r < 0) r = 0;
                if(g > 255) g = 255;
                else if(g < 0) g = 0;
                if(b > 255) b = 255;
                else if(b < 0) b = 0;

                dataXYZ[i].colorData[j * POINT_DIM + 0] = r;
                dataXYZ[i].colorData[j * POINT_DIM + 1] = g;
                dataXYZ[i].colorData[j * POINT_DIM + 2] = b;
            }
        }
    }
    update();
}

void cloudsVisualiser::updateCloudPosXYZ(int index)
{
    for(int i = 0; i < cloudsXYZ->clouds[index].first.size(); i++)
    {
        dataXYZ[index].vertexData[i * POINT_DIM + 0] = cloudsXYZ->clouds[index].first[i].x();
        dataXYZ[index].vertexData[i * POINT_DIM + 1] = cloudsXYZ->clouds[index].first[i].y();
        dataXYZ[index].vertexData[i * POINT_DIM + 2] = cloudsXYZ->clouds[index].first[i].z();
    }
    update();
}

void cloudsVisualiser::resizeCloudXYZ(int index, int scheme)
{
    pointCloudData data;
    data.size = cloudsXYZ->clouds[index].first.size();
    data.scheme = scheme;
    data.vertexData = (GLfloat*)malloc(sizeof(GLfloat) * POINT_DIM * cloudsXYZ->clouds[index].first.size());
    data.colorData = (GLubyte*)malloc(sizeof(GLubyte) * POINT_DIM * cloudsXYZ->clouds[index].first.size());
    data.selected = dataXYZ[index].selected;
    for(int i = 0; i < cloudsXYZ->clouds[index].first.size(); i++)
    {
        float x = cloudsXYZ->clouds[index].first[i].x();
        float y = cloudsXYZ->clouds[index].first[i].y();
        float z = cloudsXYZ->clouds[index].first[i].z();
        if(x > data.m.maxX)
        {
            data.m.maxX = x;
        }
        if(x < data.m.minX)
        {
            data.m.minX = x;
        }
        if(y > data.m.maxY)
        {
            data.m.maxY = y;
        }
        if(y < data.m.minY)
        {
            data.m.minY = y;
        }
        if(z > data.m.maxZ)
        {
            data.m.maxZ = z;
        }
        if(z < data.m.minZ)
        {
            data.m.minZ = z;
        }
    }
    for(int i = 0; i < cloudsXYZ->clouds[index].first.size(); i++)
    {
        data.vertexData[i * POINT_DIM + 0] = cloudsXYZ->clouds[index].first[i].x();
        data.vertexData[i * POINT_DIM + 1] = cloudsXYZ->clouds[index].first[i].y();
        data.vertexData[i * POINT_DIM + 2] = cloudsXYZ->clouds[index].first[i].z();

        float a;
        int r, g, b;

        if(data.scheme == 0)
        {
            a = (cloudsXYZ->clouds[index].first[i].x() - data.m.minX) / (data.m.maxX - data.m.minX) * M_PI * 2 + M_PI / 10 * 3;
        }
        else if(data.scheme == 1)
        {
            a = (cloudsXYZ->clouds[index].first[i].y() - data.m.minY) / (data.m.maxY - data.m.minY) * M_PI * 2 + M_PI / 10 * 3;
        }
        else if(data.scheme == 2)
        {
            a = (cloudsXYZ->clouds[index].first[i].z() - data.m.minZ) / (data.m.maxZ - data.m.minZ) * M_PI * 2 + M_PI / 10 * 3;
        }

        r = sin(a) * 192 + 128;
        g = sin(a - 2 * M_PI / 3.0) * 192 + 128;
        b = sin(a - 4 * M_PI / 3.0) * 192 + 128;

        if(r > 255) r = 255;
        else if(r < 0) r = 0;
        if(g > 255) g = 255;
        else if(g < 0) g = 0;
        if(b > 255) b = 255;
        else if(b < 0) b = 0;

        if(dataXYZ[index].selected)
        {
            r = 255;
            g = 255;
            b = 255;
        }
        data.colorData[i * POINT_DIM + 0] = r;
        data.colorData[i * POINT_DIM + 1] = g;
        data.colorData[i * POINT_DIM + 2] = b;
    }
    dataXYZ[index] = data;
    update();
}

void cloudsVisualiser::selectCloudXYZ(int index)
{
    for(int i = 0; i < cloudsXYZ->clouds[index].first.size(); i++)
    {
        dataXYZ[index].colorData[i * POINT_DIM + 0] = 255;
        dataXYZ[index].colorData[i * POINT_DIM + 1] = 255;
        dataXYZ[index].colorData[i * POINT_DIM + 2] = 255;
    }
    dataXYZ[index].selected = true;
    update();
}

void cloudsVisualiser::unselectCloudXYZ(int index, int scheme)
{
    dataXYZ[index].scheme = 4;
    dataXYZ[index].selected = false;
    cloudsVisualiser::changeSchemeXYZ(scheme);
}

void cloudsVisualiser::deleteXYZ(int index)
{
    dataXYZ.erase(dataXYZ.begin() + index);
    update();
}

void cloudsVisualiser::addCloudXYZI(int scheme)
{
    std::pair<pointCloud<pointI>, bool> &cloud = cloudsXYZI->clouds.back();
    pointCloudData data;
    data.size = cloud.first.size();
    data.scheme = scheme;
    data.vertexData = (GLfloat*)malloc(sizeof(GLfloat) * POINT_DIM * cloud.first.size());
    data.colorData = (GLubyte*)malloc(sizeof(GLubyte) * POINT_DIM * cloud.first.size());
    for(int i = 0; i < cloud.first.size(); i++)
    {
        float x = cloud.first[i].x();
        float y = cloud.first[i].y();
        float z = cloud.first[i].z();
        float intensity = cloud.first[i].intensity();
        if(x > data.m.maxX)
        {
            data.m.maxX = x;
        }
        if(x < data.m.minX)
        {
            data.m.minX = x;
        }
        if(y > data.m.maxY)
        {
            data.m.maxY = y;
        }
        if(y < data.m.minY)
        {
            data.m.minY = y;
        }
        if(z > data.m.maxZ)
        {
            data.m.maxZ = z;
        }
        if(z < data.m.minZ)
        {
            data.m.minZ = z;
        }
        if(intensity > data.m.maxI)
        {
            data.m.maxI = intensity;
        }
        if(intensity < data.m.minI)
        {
            data.m.minI = intensity;
        }
    }
    for(int i = 0; i < cloud.first.size(); i++)
    {
        data.vertexData[i * POINT_DIM + 0] = cloud.first[i].x();
        data.vertexData[i * POINT_DIM + 1] = cloud.first[i].y();
        data.vertexData[i * POINT_DIM + 2] = cloud.first[i].z();

        float a;
        int r, g, b;

        if(data.scheme == 0)
        {
            a = (cloud.first[i].x() - data.m.minX) / (data.m.maxX - data.m.minX) * M_PI * 2 + M_PI / 10 * 3;
        }
        else if(data.scheme == 1)
        {
            a = (cloud.first[i].y() - data.m.minY) / (data.m.maxY - data.m.minY) * M_PI * 2 + M_PI / 10 * 3;
        }
        else if(data.scheme == 2)
        {
            a = (cloud.first[i].z() - data.m.minZ) / (data.m.maxZ - data.m.minZ) * M_PI * 2 + M_PI / 10 * 3;
        }
        else if(data.scheme == 3)
        {
            a = (cloud.first[i].intensity() - data.m.minI) / (data.m.maxI - data.m.minI) * M_PI * 2 + M_PI / 6 * 3;
        }

        r = sin(a) * 192 + 128;
        g = sin(a - 2 * M_PI / 3.0) * 192 + 128;
        b = sin(a - 4 * M_PI / 3.0) * 192 + 128;

        if(r > 255) r = 255;
        else if(r < 0) r = 0;
        if(g > 255) g = 255;
        else if(g < 0) g = 0;
        if(b > 255) b = 255;
        else if(b < 0) b = 0;

        data.colorData[i * POINT_DIM + 0] = r;
        data.colorData[i * POINT_DIM + 1] = g;
        data.colorData[i * POINT_DIM + 2] = b;
    }
    dataXYZI.push_back(data);
    update();
}

void cloudsVisualiser::changeSchemeXYZI(int scheme)
{
    for(int i = 0; i < cloudsXYZI->clouds.size(); i++)
    {
        if(dataXYZI[i].scheme != scheme && !dataXYZI[i].selected)
        {
            dataXYZI[i].scheme = scheme;
            for(int j = 0; j < dataXYZI[i].size; j++)
            {
                float a;
                int r, g, b;

                if(scheme == 0)
                {
                    a = (cloudsXYZI->clouds[i].first[j].x() - dataXYZI[i].m.minX) / (dataXYZI[i].m.maxX - dataXYZI[i].m.minX) * M_PI * 2 + M_PI / 10 * 3;
                }
                else if(scheme == 1)
                {
                    a = (cloudsXYZI->clouds[i].first[j].y() - dataXYZI[i].m.minY) / (dataXYZI[i].m.maxY - dataXYZI[i].m.minY) * M_PI * 2 + M_PI / 10 * 3;
                }
                else if(scheme == 2)
                {
                    a = (cloudsXYZI->clouds[i].first[j].z() - dataXYZI[i].m.minZ) / (dataXYZI[i].m.maxZ - dataXYZI[i].m.minZ) * M_PI * 2 + M_PI / 10 * 3;
                }
                else if(scheme == 3)
                {
                    a = (cloudsXYZI->clouds[i].first[j].intensity() - dataXYZI[i].m.minI) / (dataXYZI[i].m.maxI - dataXYZI[i].m.minI) * M_PI * 2 + M_PI / 6 * 3;
                }

                r = sin(a) * 192 + 128;
                g = sin(a - 2 * M_PI / 3.0) * 192 + 128;
                b = sin(a - 4 * M_PI / 3.0) * 192 + 128;

                if(r > 255) r = 255;
                else if(r < 0) r = 0;
                if(g > 255) g = 255;
                else if(g < 0) g = 0;
                if(b > 255) b = 255;
                else if(b < 0) b = 0;

                dataXYZI[i].colorData[j * POINT_DIM + 0] = r;
                dataXYZI[i].colorData[j * POINT_DIM + 1] = g;
                dataXYZI[i].colorData[j * POINT_DIM + 2] = b;
            }
        }
    }
    update();
}

void cloudsVisualiser::updateCloudPosXYZI(int index)
{
    for(int i = 0; i < cloudsXYZI->clouds[index].first.size(); i++)
    {
        dataXYZI[index].vertexData[i * POINT_DIM + 0] = cloudsXYZI->clouds[index].first[i].x();
        dataXYZI[index].vertexData[i * POINT_DIM + 1] = cloudsXYZI->clouds[index].first[i].y();
        dataXYZI[index].vertexData[i * POINT_DIM + 2] = cloudsXYZI->clouds[index].first[i].z();
    }
    update();
}

void cloudsVisualiser::resizeCloudXYZI(int index, int scheme)
{
    pointCloudData data;
    data.size = cloudsXYZI->clouds[index].first.size();
    data.scheme = scheme;
    data.vertexData = (GLfloat*)malloc(sizeof(GLfloat) * POINT_DIM * cloudsXYZI->clouds[index].first.size());
    data.colorData = (GLubyte*)malloc(sizeof(GLubyte) * POINT_DIM * cloudsXYZI->clouds[index].first.size());
    data.selected = dataXYZI[index].selected;
    for(int i = 0; i < cloudsXYZI->clouds[index].first.size(); i++)
    {
        float x = cloudsXYZI->clouds[index].first[i].x();
        float y = cloudsXYZI->clouds[index].first[i].y();
        float z = cloudsXYZI->clouds[index].first[i].z();
        float intensity = cloudsXYZI->clouds[index].first[i].intensity();
        if(x > data.m.maxX)
        {
            data.m.maxX = x;
        }
        if(x < data.m.minX)
        {
            data.m.minX = x;
        }
        if(y > data.m.maxY)
        {
            data.m.maxY = y;
        }
        if(y < data.m.minY)
        {
            data.m.minY = y;
        }
        if(z > data.m.maxZ)
        {
            data.m.maxZ = z;
        }
        if(z < data.m.minZ)
        {
            data.m.minZ = z;
        }
        if(intensity > data.m.maxI)
        {
            data.m.maxI = intensity;
        }
        if(intensity < data.m.minI)
        {
            data.m.minI = intensity;
        }
    }
    for(int i = 0; i < cloudsXYZI->clouds[index].first.size(); i++)
    {
        data.vertexData[i * POINT_DIM + 0] = cloudsXYZI->clouds[index].first[i].x();
        data.vertexData[i * POINT_DIM + 1] = cloudsXYZI->clouds[index].first[i].y();
        data.vertexData[i * POINT_DIM + 2] = cloudsXYZI->clouds[index].first[i].z();

        float a;
        int r, g, b;

        if(data.scheme == 0)
        {
            a = (cloudsXYZI->clouds[index].first[i].x() - data.m.minX) / (data.m.maxX - data.m.minX) * M_PI * 2 + M_PI / 10 * 3;
        }
        else if(data.scheme == 1)
        {
            a = (cloudsXYZI->clouds[index].first[i].y() - data.m.minY) / (data.m.maxY - data.m.minY) * M_PI * 2 + M_PI / 10 * 3;
        }
        else if(data.scheme == 2)
        {
            a = (cloudsXYZI->clouds[index].first[i].z() - data.m.minZ) / (data.m.maxZ - data.m.minZ) * M_PI * 2 + M_PI / 10 * 3;
        }
        else if(data.scheme == 3)
        {
            a = (cloudsXYZI->clouds[index].first[i].intensity() - data.m.minI) / (data.m.maxI - data.m.minI) * M_PI * 2 + M_PI / 6 * 3;
        }

        r = sin(a) * 192 + 128;
        g = sin(a - 2 * M_PI / 3.0) * 192 + 128;
        b = sin(a - 4 * M_PI / 3.0) * 192 + 128;

        if(r > 255) r = 255;
        else if(r < 0) r = 0;
        if(g > 255) g = 255;
        else if(g < 0) g = 0;
        if(b > 255) b = 255;
        else if(b < 0) b = 0;

        if(data.selected)
        {
            r = 255;
            g = 255;
            b = 255;
        }
        data.colorData[i * POINT_DIM + 0] = r;
        data.colorData[i * POINT_DIM + 1] = g;
        data.colorData[i * POINT_DIM + 2] = b;
    }
    dataXYZI[index] = data;
    update();
}

void cloudsVisualiser::selectCloudXYZI(int index)
{
    for(int i = 0; i < cloudsXYZI->clouds[index].first.size(); i++)
    {
        dataXYZI[index].colorData[i * POINT_DIM + 0] = 255;
        dataXYZI[index].colorData[i * POINT_DIM + 1] = 255;
        dataXYZI[index].colorData[i * POINT_DIM + 2] = 255;
    }
    dataXYZI[index].selected = true;
    update();
}

void cloudsVisualiser::unselectCloudXYZI(int index, int scheme)
{
    dataXYZI[index].scheme = 4;
    dataXYZI[index].selected = false;
    cloudsVisualiser::changeSchemeXYZI(scheme);
}

void cloudsVisualiser::deleteXYZI(int index)
{
    dataXYZI.erase(dataXYZI.begin() + index);
    update();
}

void cloudsVisualiser::addCloudXYZRGB(int scheme)
{
    std::pair<pointCloud<pointRGB>, bool> &cloud = cloudsXYZRGB->clouds.back();
    pointCloudData data;
    data.size = cloud.first.size();
    data.scheme = scheme;
    data.vertexData = (GLfloat*)malloc(sizeof(GLfloat) * POINT_DIM * cloud.first.size());
    data.colorData = (GLubyte*)malloc(sizeof(GLubyte) * POINT_DIM * cloud.first.size());
    for(int i = 0; i < cloud.first.size(); i++)
    {
        float x = cloud.first[i].x();
        float y = cloud.first[i].y();
        float z = cloud.first[i].z();
        if(x > data.m.maxX)
        {
            data.m.maxX = x;
        }
        if(x < data.m.minX)
        {
            data.m.minX = x;
        }
        if(y > data.m.maxY)
        {
            data.m.maxY = y;
        }
        if(y < data.m.minY)
        {
            data.m.minY = y;
        }
        if(z > data.m.maxZ)
        {
            data.m.maxZ = z;
        }
        if(z < data.m.minZ)
        {
            data.m.minZ = z;
        }
    }
    for(int i = 0; i < cloud.first.size(); i++)
    {
        data.vertexData[i * POINT_DIM + 0] = cloud.first[i].x();
        data.vertexData[i * POINT_DIM + 1] = cloud.first[i].y();
        data.vertexData[i * POINT_DIM + 2] = cloud.first[i].z();

        float a;
        int r, g, b;

        if(data.scheme == 0)
        {
            a = (cloud.first[i].x() - data.m.minX) / (data.m.maxX - data.m.minX) * M_PI * 2 + M_PI / 10 * 3;
        }
        else if(data.scheme == 1)
        {
            a = (cloud.first[i].y() - data.m.minY) / (data.m.maxY - data.m.minY) * M_PI * 2 + M_PI / 10 * 3;
        }
        else if(data.scheme == 2)
        {
            a = (cloud.first[i].z() - data.m.minZ) / (data.m.maxZ - data.m.minZ) * M_PI * 2 + M_PI / 10 * 3;
        }

        if(data.scheme == 3)
        {
            r = cloud.first[i].r();
            g = cloud.first[i].g();
            b = cloud.first[i].b();
        }
        else
        {
            r = sin(a) * 192 + 128;
            g = sin(a - 2 * M_PI / 3.0) * 192 + 128;
            b = sin(a - 4 * M_PI / 3.0) * 192 + 128;

            if(r > 255) r = 255;
            else if(r < 0) r = 0;
            if(g > 255) g = 255;
            else if(g < 0) g = 0;
            if(b > 255) b = 255;
            else if(b < 0) b = 0;
        }

        data.colorData[i * POINT_DIM + 0] = r;
        data.colorData[i * POINT_DIM + 1] = g;
        data.colorData[i * POINT_DIM + 2] = b;
    }
    dataXYZRGB.push_back(data);
    update();
}

void cloudsVisualiser::changeSchemeXYZRGB(int scheme)
{
    for(int i = 0; i < cloudsXYZRGB->clouds.size(); i++)
    {
        if(dataXYZRGB[i].scheme != scheme && !dataXYZRGB[i].selected)
        {
            dataXYZRGB[i].scheme = scheme;
            for(int j = 0; j < dataXYZRGB[i].size; j++)
            {
                float a;
                int r, g, b;

                if(scheme == 0)
                {
                    a = (cloudsXYZRGB->clouds[i].first[j].x() - dataXYZRGB[i].m.minX) / (dataXYZRGB[i].m.maxX - dataXYZRGB[i].m.minX) * M_PI * 2 + M_PI / 10 * 3;
                }
                else if(scheme == 1)
                {
                    a = (cloudsXYZRGB->clouds[i].first[j].y() - dataXYZRGB[i].m.minY) / (dataXYZRGB[i].m.maxY - dataXYZRGB[i].m.minY) * M_PI * 2 + M_PI / 10 * 3;
                }
                else if(scheme == 2)
                {
                    a = (cloudsXYZRGB->clouds[i].first[j].z() - dataXYZRGB[i].m.minZ) / (dataXYZRGB[i].m.maxZ - dataXYZRGB[i].m.minZ) * M_PI * 2 + M_PI / 10 * 3;
                }
                if(scheme == 3)
                {
                    r = cloudsXYZRGB->clouds[i].first[j].r();
                    g = cloudsXYZRGB->clouds[i].first[j].g();
                    b = cloudsXYZRGB->clouds[i].first[j].b();
                }
                else{
                    r = sin(a) * 192 + 128;
                    g = sin(a - 2 * M_PI / 3.0) * 192 + 128;
                    b = sin(a - 4 * M_PI / 3.0) * 192 + 128;

                    if(r > 255) r = 255;
                    else if(r < 0) r = 0;
                    if(g > 255) g = 255;
                    else if(g < 0) g = 0;
                    if(b > 255) b = 255;
                    else if(b < 0) b = 0;
                }

                dataXYZRGB[i].colorData[j * POINT_DIM + 0] = r;
                dataXYZRGB[i].colorData[j * POINT_DIM + 1] = g;
                dataXYZRGB[i].colorData[j * POINT_DIM + 2] = b;
            }
        }
    }
    update();
}

void cloudsVisualiser::updateCloudPosXYZRGB(int index)
{
    for(int i = 0; i < cloudsXYZRGB->clouds[index].first.size(); i++)
    {
        dataXYZRGB[index].vertexData[i * POINT_DIM + 0] = cloudsXYZRGB->clouds[index].first[i].x();
        dataXYZRGB[index].vertexData[i * POINT_DIM + 1] = cloudsXYZRGB->clouds[index].first[i].y();
        dataXYZRGB[index].vertexData[i * POINT_DIM + 2] = cloudsXYZRGB->clouds[index].first[i].z();
    }
    update();
}

void cloudsVisualiser::resizeCloudXYZRGB(int index, int scheme)
{
    pointCloudData data;
    data.size = cloudsXYZRGB->clouds[index].first.size();
    data.scheme = scheme;
    data.vertexData = (GLfloat*)malloc(sizeof(GLfloat) * POINT_DIM * cloudsXYZRGB->clouds[index].first.size());
    data.colorData = (GLubyte*)malloc(sizeof(GLubyte) * POINT_DIM * cloudsXYZRGB->clouds[index].first.size());
    data.selected = dataXYZRGB[index].selected;
    for(int i = 0; i < cloudsXYZRGB->clouds[index].first.size(); i++)
    {
        float x = cloudsXYZRGB->clouds[index].first[i].x();
        float y = cloudsXYZRGB->clouds[index].first[i].y();
        float z = cloudsXYZRGB->clouds[index].first[i].z();
        if(x > data.m.maxX)
        {
            data.m.maxX = x;
        }
        if(x < data.m.minX)
        {
            data.m.minX = x;
        }
        if(y > data.m.maxY)
        {
            data.m.maxY = y;
        }
        if(y < data.m.minY)
        {
            data.m.minY = y;
        }
        if(z > data.m.maxZ)
        {
            data.m.maxZ = z;
        }
        if(z < data.m.minZ)
        {
            data.m.minZ = z;
        }
    }
    for(int i = 0; i < cloudsXYZRGB->clouds[index].first.size(); i++)
    {
        data.vertexData[i * POINT_DIM + 0] = cloudsXYZRGB->clouds[index].first[i].x();
        data.vertexData[i * POINT_DIM + 1] = cloudsXYZRGB->clouds[index].first[i].y();
        data.vertexData[i * POINT_DIM + 2] = cloudsXYZRGB->clouds[index].first[i].z();

        float a;
        int r, g, b;

        if(data.scheme == 0)
        {
            a = (cloudsXYZRGB->clouds[index].first[i].x() - data.m.minX) / (data.m.maxX - data.m.minX) * M_PI * 2 + M_PI / 10 * 3;
        }
        else if(data.scheme == 1)
        {
            a = (cloudsXYZRGB->clouds[index].first[i].y() - data.m.minY) / (data.m.maxY - data.m.minY) * M_PI * 2 + M_PI / 10 * 3;
        }
        else if(data.scheme == 2)
        {
            a = (cloudsXYZRGB->clouds[index].first[i].z() - data.m.minZ) / (data.m.maxZ - data.m.minZ) * M_PI * 2 + M_PI / 10 * 3;
        }

        if(data.scheme == 3)
        {
            r = cloudsXYZRGB->clouds[index].first[i].r();
            g = cloudsXYZRGB->clouds[index].first[i].g();
            b = cloudsXYZRGB->clouds[index].first[i].b();
        }
        else if(dataXYZRGB[index].selected)
        {
            r = 255;
            g = 255;
            b = 255;
        }
        else
        {
            r = sin(a) * 192 + 128;
            g = sin(a - 2 * M_PI / 3.0) * 192 + 128;
            b = sin(a - 4 * M_PI / 3.0) * 192 + 128;

            if(r > 255) r = 255;
            else if(r < 0) r = 0;
            if(g > 255) g = 255;
            else if(g < 0) g = 0;
            if(b > 255) b = 255;
            else if(b < 0) b = 0;
        }

        data.colorData[i * POINT_DIM + 0] = r;
        data.colorData[i * POINT_DIM + 1] = g;
        data.colorData[i * POINT_DIM + 2] = b;
    }
    dataXYZRGB[index] = data;
    update();
}

void cloudsVisualiser::selectCloudXYZRGB(int index)
{
    for(int i = 0; i < cloudsXYZRGB->clouds[index].first.size(); i++)
    {
        dataXYZRGB[index].colorData[i * POINT_DIM + 0] = 255;
        dataXYZRGB[index].colorData[i * POINT_DIM + 1] = 255;
        dataXYZRGB[index].colorData[i * POINT_DIM + 2] = 255;
    }
    dataXYZRGB[index].selected = true;
    update();
}

void cloudsVisualiser::unselectCloudXYZRGB(int index, int scheme)
{
    dataXYZRGB[index].scheme = 4;
    dataXYZRGB[index].selected = false;
    cloudsVisualiser::changeSchemeXYZRGB(scheme);
}

void cloudsVisualiser::deleteXYZRGB(int index)
{
    dataXYZRGB.erase(dataXYZRGB.begin() + index);
    update();
}

void cloudsVisualiser::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
}

void cloudsVisualiser::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(85.0f, (GLfloat)w/(GLfloat)h, 0.01f, 200.0f);
    glMatrixMode(GL_MODELVIEW);
}

void paintCoordSys(float zX, float zY, float zZ)
{
    glLineWidth(4);


    glBegin(GL_LINE_STRIP);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(zX, zY, zZ);
    glVertex3f(zX + 1, zY, zZ);
    glEnd();


    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(zX, zY, zZ);
    glVertex3f(zX, zY + 1, zZ);
    glEnd();


    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(zX, zY, zZ);
    glVertex3f(zX, zY, zZ + 1);
    glEnd();
}

void paintFloorCells(float leftSide, float downSide)
{
    for(int x = leftSide; x <= leftSide + 100; x++)
    {
        for(int y = downSide; y <= downSide + 100; y++)
        {
            glBegin(GL_LINE_LOOP);
            glColor3f(0.5f, 0.5f, 0.5f);
            glVertex3f(x, y, 0.0f);
            glVertex3f(x + 1.0f, y, 0.0f);
            glVertex3f(x + 1.0f, y + 1.0f, 0.0f);
            glVertex3f(x, y + 1.0f, 0.0f);
            glEnd();
        }
    }
}

void paintCloud(pointCloudData &cloudData)
{
    glPointSize(3.0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(POINT_DIM, GL_FLOAT, 0, cloudData.vertexData);
    glColorPointer(POINT_DIM, GL_UNSIGNED_BYTE, 0, cloudData.colorData);
    glDrawArrays(GL_POINTS, 0, cloudData.size);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

void cloudsVisualiser::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glLoadIdentity();

    cloudsVisualiser::updateCamPos();

    gluLookAt(posX, posY, posZ, posX + dirX, posY + dirY, posZ + dirZ, 0, 0, 1);

    paintCoordSys(zeroX, zeroY, zeroZ);
    paintFloorCells(-50 + int(zeroX), -50 + int(zeroY));

    for(int i = 0; i < cloudsXYZ->clouds.size(); i++)
    {
        if(cloudsXYZ->clouds[i].second)
        {
            paintCloud(dataXYZ[i]);
        }
    }

    for(int i = 0; i < cloudsXYZI->clouds.size(); i++)
    {
        if(cloudsXYZI->clouds[i].second)
        {
            paintCloud(dataXYZI[i]);
        }
    }

    for(int i = 0; i < cloudsXYZRGB->clouds.size(); i++)
    {
        if(cloudsXYZRGB->clouds[i].second )
        {
            paintCloud(dataXYZRGB[i]);
        }
    }
}

void cloudsVisualiser::mousePressEvent(QMouseEvent *event)
{
    mPrevPos = event->pos(); //change mouse previous position
    if(event->button() == Qt::LeftButton) isLeft = true;
    if(event->button() == Qt::RightButton) isRight = true;
    update();
}

void cloudsVisualiser::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) isLeft = false;
    if(event->button() == Qt::RightButton) isRight = false;
    update();
}

void cloudsVisualiser::mouseMoveEvent(QMouseEvent *event)
{
    if(isLeft){
        angleY -= (event->pos().x() - mPrevPos.x()) * MOUSE_SENS;
        angleX -= (event->pos().y() - mPrevPos.y()) * MOUSE_SENS;
    }
    if(isRight){
        float leftToRight = -(event->pos().x() - mPrevPos.x()) * MOUSE_SENS * dist;
        float upToDown = -(event->pos().y() - mPrevPos.y()) * MOUSE_SENS * dist;

        zeroX += upToDown * cos(angleY) + leftToRight * sin(-angleY);
        zeroY += upToDown * sin(angleY) + leftToRight * cos(-angleY);
    }
    mPrevPos = event->pos();

    update();
}


void cloudsVisualiser::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W) zeroZ += kSens; //up
    if(event->key() == Qt::Key_S) zeroZ -= kSens; //down
    kSens += 0.05; //increase move speed
    update();
}

void cloudsVisualiser::keyReleaseEvent(QKeyEvent *event)
{
    kSens = 0.05;
}

void cloudsVisualiser::wheelEvent(QWheelEvent *event)
{
    float numSteps = event->angleDelta().y(); //how many steps of moving
    dist -= numSteps * 0.005; //move relative zero position
    if(dist <= 0.07)
    {
        dist = 0.07;
    }

    update();
}
