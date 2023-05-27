#include "pointcloud.h"
#include <cmath>
#include <iostream>

template<> pointCloud<point>::pointCloud()
{
    typePoint = "xyz";
    zeroTX = 0;
    zeroTY = 0;
    zeroTZ = 0;
    zeroRX = 0;
    zeroRY = 0;
    zeroRZ = 0;
}
template<> pointCloud<pointI>::pointCloud()
{
    typePoint = "i";
    zeroTX = 0;
    zeroTY = 0;
    zeroTZ = 0;
    zeroRX = 0;
    zeroRY = 0;
    zeroRZ = 0;
}
template<> pointCloud<pointRGB>::pointCloud()
{
    typePoint = "rgb";
    zeroTX = 0;
    zeroTY = 0;
    zeroTZ = 0;
    zeroRX = 0;
    zeroRY = 0;
    zeroRZ = 0;
}


void matMultVec(std::vector<std::vector<double>> &mat, point &dst)
{
    point src = dst;
    dst.set_x(mat[0][0] * src.x() + mat[0][1] * src.y() + mat[0][2] * src.z());
    dst.set_y(mat[1][0] * src.x() + mat[1][1] * src.y() + mat[1][2] * src.z());
    dst.set_z(mat[2][0] * src.x() + mat[2][1] * src.y() + mat[2][2] * src.z());
}
void rotate(point &p, float rX, float rY, float rZ)
{
    if(rX <= 0 && rY <= 0 && rZ <= 0)
    {
        std::vector<std::vector<double>> rotX = {{1,    0   ,    0    },
                                                 {0, cos(rX), -sin(rX)},
                                                 {0, sin(rX), cos(rX)}};
        matMultVec(rotX, p);

        std::vector<std::vector<double>> rotY = {{cos(rY) ,0, sin(rY)},
                                                 {    0   ,1,    0   },
                                                 {-sin(rY),0, cos(rY)}};
        matMultVec(rotY, p);

        std::vector<std::vector<double>> rotZ = {{cos(rZ), -sin(rZ),0},
                                                 {sin(rZ),  cos(rZ),0},
                                                 {   0   ,     0   ,1}};
        matMultVec(rotZ, p);
    }
    else
    {
        std::vector<std::vector<double>> rotZ = {{cos(rZ), -sin(rZ),0},
                                                 {sin(rZ),  cos(rZ),0},
                                                 {   0   ,     0   ,1}};
        matMultVec(rotZ, p);

        std::vector<std::vector<double>> rotY = {{cos(rY) ,0, sin(rY)},
                                                 {    0   ,1,    0   },
                                                 {-sin(rY),0, cos(rY)}};
        matMultVec(rotY, p);

        //rotation matrix X
        std::vector<std::vector<double>> rotX = {{1,    0   ,    0    },
                                                 {0, cos(rX), -sin(rX)},
                                                 {0, sin(rX), cos(rX)}};
        matMultVec(rotX, p);
    }
}
void translatePoint(point &p, float tX, float tY, float tZ)
{
    p.set_x(p.x() + tX);
    p.set_y(p.y() + tY);
    p.set_z(p.z() + tZ);
}
template<> void pointCloud<point>::transform(float tX, float tY, float tZ, float rX, float rY, float rZ, bool relative)
{
    for(int i = 0; i < this->size(); i++)
    {
        if(!relative)
        {
            translatePoint(this->points[i], -zeroTX, -zeroTY, -zeroTZ);
            rotate(this->points[i], -zeroRX / 180 * M_PI, -zeroRY / 180 * M_PI, -zeroRZ / 180 * M_PI);
        }
        rotate(this->points[i], rX / 180 * M_PI, rY / 180 * M_PI, rZ / 180 * M_PI);
        translatePoint(this->points[i], tX, tY, tZ);
    }
    this->transformHistory.push_back({zeroTX, zeroTY, zeroTZ, zeroRX, zeroRY, zeroRZ});
    if(!relative)
    {
        zeroTX = tX;
        zeroTY = tY;
        zeroTZ = tZ;
        zeroRX = rX;
        zeroRY = rY;
        zeroRZ = rZ;
    }
    else
    {
        zeroTX += tX;
        zeroTY += tY;
        zeroTZ += tZ;
        zeroRX += rX;
        zeroRY += rY;
        zeroRZ += rZ;
    }
}
template<> void pointCloud<point>::undo()
{
    if(transformHistory.empty()) return;
    auto tr = this->transformHistory.back();
    this->transformHistory.pop_back();
    float tX = tr[0];
    float tY = tr[1];
    float tZ = tr[2];
    float rX = tr[3];
    float rY = tr[4];
    float rZ = tr[5];
    for(int i = 0; i < this->size(); i++)
    {
        translatePoint(this->points[i], -zeroTX, -zeroTY, -zeroTZ);
        rotate(this->points[i], -zeroRX / 180 * M_PI, -zeroRY / 180 * M_PI, -zeroRZ / 180 * M_PI);
        rotate(this->points[i], rX / 180 * M_PI, rY / 180 * M_PI, rZ / 180 * M_PI);
        translatePoint(this->points[i], tX, tY, tZ);
    }
    zeroTX = tX;
    zeroTY = tY;
    zeroTZ = tZ;
    zeroRX = rX;
    zeroRY = rY;
    zeroRZ = rZ;
}


template<> void pointCloud<point>::borderFilter(float minX, float maxX, float minY, float maxY, float minZ, float maxZ, int borderFilterType)
{
    std::vector<point> filtered, unfiltered;
    for(auto p : this->points)
    {
        float x = p.x(), y = p.y(), z = p.z();
        if(borderFilterType == 0)
        {
            if(x > minX && x < maxX)
            {
                filtered.push_back(p);
            }
            else
            {
                unfiltered.push_back(p);
            }
        }
        else if(borderFilterType == 1)
        {
            if(y > minY && y < maxY)
            {
                filtered.push_back(p);
            }
            else
            {
                unfiltered.push_back(p);
            }
        }
        else if(borderFilterType == 2)
        {
            if(z > minZ && z < maxZ)
            {
                filtered.push_back(p);
            }
            else
            {
                unfiltered.push_back(p);
            }
        }
        else
        {
            if(x > minX && x < maxX && y > minY && y < maxY && z > minZ && z < maxZ)
            {
                filtered.push_back(p);
            }
            else
            {
                unfiltered.push_back(p);
            }
        }
    }
    filterHistory.push_back(unfiltered);
    this->points = filtered;
}
template<> void pointCloud<point>::undoFilter()
{
    if(this->filterHistory.empty()) return;
    auto filterP = this->filterHistory.back();
    this->filterHistory.pop_back();
    for(auto &p : filterP)
    {
        this->points.push_back(p);
    }
}



void matMultVec(std::vector<std::vector<double>> &mat, pointI &dst)
{
    point src = dst;
    dst.set_x(mat[0][0] * src.x() + mat[0][1] * src.y() + mat[0][2] * src.z());
    dst.set_y(mat[1][0] * src.x() + mat[1][1] * src.y() + mat[1][2] * src.z());
    dst.set_z(mat[2][0] * src.x() + mat[2][1] * src.y() + mat[2][2] * src.z());
}
void rotate(pointI &p, float rX, float rY, float rZ)
{
    if(rX <= 0 && rY <= 0 && rZ <= 0)
    {
        std::vector<std::vector<double>> rotX = {{1,    0   ,    0    },
                                                 {0, cos(rX), -sin(rX)},
                                                 {0, sin(rX), cos(rX)}};
        matMultVec(rotX, p);

        std::vector<std::vector<double>> rotY = {{cos(rY) ,0, sin(rY)},
                                                 {    0   ,1,    0   },
                                                 {-sin(rY),0, cos(rY)}};
        matMultVec(rotY, p);

        std::vector<std::vector<double>> rotZ = {{cos(rZ), -sin(rZ),0},
                                                 {sin(rZ),  cos(rZ),0},
                                                 {   0   ,     0   ,1}};
        matMultVec(rotZ, p);
    }
    else
    {
        std::vector<std::vector<double>> rotZ = {{cos(rZ), -sin(rZ),0},
                                                 {sin(rZ),  cos(rZ),0},
                                                 {   0   ,     0   ,1}};
        matMultVec(rotZ, p);

        std::vector<std::vector<double>> rotY = {{cos(rY) ,0, sin(rY)},
                                                 {    0   ,1,    0   },
                                                 {-sin(rY),0, cos(rY)}};
        matMultVec(rotY, p);

        //rotation matrix X
        std::vector<std::vector<double>> rotX = {{1,    0   ,    0    },
                                                 {0, cos(rX), -sin(rX)},
                                                 {0, sin(rX), cos(rX)}};
        matMultVec(rotX, p);
    }
}
void translatePoint(pointI &p, float tX, float tY, float tZ)
{
    p.set_x(p.x() + tX);
    p.set_y(p.y() + tY);
    p.set_z(p.z() + tZ);
}
template<> void pointCloud<pointI>::transform(float tX, float tY, float tZ, float rX, float rY, float rZ, bool relative)
{
    for(int i = 0; i < this->size(); i++)
    {
        if(!relative)
        {
            translatePoint(this->points[i], -zeroTX, -zeroTY, -zeroTZ);
            rotate(this->points[i], -zeroRX / 180 * M_PI, -zeroRY / 180 * M_PI, -zeroRZ / 180 * M_PI);
        }
        rotate(this->points[i], rX / 180 * M_PI, rY / 180 * M_PI, rZ / 180 * M_PI);
        translatePoint(this->points[i], tX, tY, tZ);
    }
    this->transformHistory.push_back({zeroTX, zeroTY, zeroTZ, zeroRX, zeroRY, zeroRZ});
    if(!relative)
    {
        zeroTX = tX;
        zeroTY = tY;
        zeroTZ = tZ;
        zeroRX = rX;
        zeroRY = rY;
        zeroRZ = rZ;
    }
    else
    {
        zeroTX += tX;
        zeroTY += tY;
        zeroTZ += tZ;
        zeroRX += rX;
        zeroRY += rY;
        zeroRZ += rZ;
    }
}
template<> void pointCloud<pointI>::undo()
{
    if(transformHistory.empty()) return;
    auto tr = this->transformHistory.back();
    this->transformHistory.pop_back();
    float tX = tr[0];
    float tY = tr[1];
    float tZ = tr[2];
    float rX = tr[3];
    float rY = tr[4];
    float rZ = tr[5];
    for(int i = 0; i < this->size(); i++)
    {
        translatePoint(this->points[i], -zeroTX, -zeroTY, -zeroTZ);
        rotate(this->points[i], -zeroRX / 180 * M_PI, -zeroRY / 180 * M_PI, -zeroRZ / 180 * M_PI);
        rotate(this->points[i], rX / 180 * M_PI, rY / 180 * M_PI, rZ / 180 * M_PI);
        translatePoint(this->points[i], tX, tY, tZ);
    }
    zeroTX = tX;
    zeroTY = tY;
    zeroTZ = tZ;
    zeroRX = rX;
    zeroRY = rY;
    zeroRZ = rZ;
}

template<> void pointCloud<pointI>::borderFilter(float minX, float maxX, float minY, float maxY, float minZ, float maxZ, int borderFilterType)
{
    std::vector<pointI> filtered, unfiltered;
    for(auto &p : this->points)
    {
        float x = p.x(), y = p.y(), z = p.z();
        if(borderFilterType == 0)
        {
            if(x > minX && x < maxX)
            {
                filtered.push_back(p);
            }
            else
            {
                unfiltered.push_back(p);
            }
        }
        else if(borderFilterType == 1)
        {
            if(y > minY && y < maxY)
            {
                filtered.push_back(p);
            }
            else
            {
                unfiltered.push_back(p);
            }
        }
        else if(borderFilterType == 2)
        {
            if(z > minZ && z < maxZ)
            {
                filtered.push_back(p);
            }
            else
            {
                unfiltered.push_back(p);
            }
        }
        else
        {
            if(x > minX && x < maxX && y > minY && y < maxY && z > minZ && z < maxZ)
            {
                filtered.push_back(p);
            }
            else
            {
                unfiltered.push_back(p);
            }
        }
    }
    filterHistory.push_back(unfiltered);
    this->points = filtered;
}
template<> void pointCloud<pointI>::undoFilter()
{
    if(this->filterHistory.empty()) return;
    auto filterP = this->filterHistory.back();
    this->filterHistory.pop_back();
    for(auto &p : filterP)
    {
        this->points.push_back(p);
    }
}



void matMultVec(std::vector<std::vector<double>> &mat, pointRGB &dst)
{
    point src = dst;
    dst.set_x(mat[0][0] * src.x() + mat[0][1] * src.y() + mat[0][2] * src.z());
    dst.set_y(mat[1][0] * src.x() + mat[1][1] * src.y() + mat[1][2] * src.z());
    dst.set_z(mat[2][0] * src.x() + mat[2][1] * src.y() + mat[2][2] * src.z());
}
void rotate(pointRGB &p, float rX, float rY, float rZ)
{
    if(rX <= 0 && rY <= 0 && rZ <= 0)
    {
        std::vector<std::vector<double>> rotX = {{1,    0   ,    0    },
                                                 {0, cos(rX), -sin(rX)},
                                                 {0, sin(rX), cos(rX)}};
        matMultVec(rotX, p);

        std::vector<std::vector<double>> rotY = {{cos(rY) ,0, sin(rY)},
                                                 {    0   ,1,    0   },
                                                 {-sin(rY),0, cos(rY)}};
        matMultVec(rotY, p);

        std::vector<std::vector<double>> rotZ = {{cos(rZ), -sin(rZ),0},
                                                 {sin(rZ),  cos(rZ),0},
                                                 {   0   ,     0   ,1}};
        matMultVec(rotZ, p);
    }
    else
    {
        std::vector<std::vector<double>> rotZ = {{cos(rZ), -sin(rZ),0},
                                                 {sin(rZ),  cos(rZ),0},
                                                 {   0   ,     0   ,1}};
        matMultVec(rotZ, p);

        std::vector<std::vector<double>> rotY = {{cos(rY) ,0, sin(rY)},
                                                 {    0   ,1,    0   },
                                                 {-sin(rY),0, cos(rY)}};
        matMultVec(rotY, p);

        //rotation matrix X
        std::vector<std::vector<double>> rotX = {{1,    0   ,    0    },
                                                 {0, cos(rX), -sin(rX)},
                                                 {0, sin(rX), cos(rX)}};
        matMultVec(rotX, p);
    }
}
void translatePoint(pointRGB &p, float tX, float tY, float tZ)
{
    p.set_x(p.x() + tX);
    p.set_y(p.y() + tY);
    p.set_z(p.z() + tZ);
}
template<> void pointCloud<pointRGB>::transform(float tX, float tY, float tZ, float rX, float rY, float rZ, bool relative)
{
    for(int i = 0; i < this->size(); i++)
    {
        if(!relative)
        {
            translatePoint(this->points[i], -zeroTX, -zeroTY, -zeroTZ);
            rotate(this->points[i], -zeroRX / 180 * M_PI, -zeroRY / 180 * M_PI, -zeroRZ / 180 * M_PI);
        }
        rotate(this->points[i], rX / 180 * M_PI, rY / 180 * M_PI, rZ / 180 * M_PI);
        translatePoint(this->points[i], tX, tY, tZ);
    }
    this->transformHistory.push_back({zeroTX, zeroTY, zeroTZ, zeroRX, zeroRY, zeroRZ});
    if(!relative)
    {
        zeroTX = tX;
        zeroTY = tY;
        zeroTZ = tZ;
        zeroRX = rX;
        zeroRY = rY;
        zeroRZ = rZ;
    }
    else
    {
        zeroTX += tX;
        zeroTY += tY;
        zeroTZ += tZ;
        zeroRX += rX;
        zeroRY += rY;
        zeroRZ += rZ;
    }
}
template<> void pointCloud<pointRGB>::undo()
{
    if(transformHistory.empty()) return;
    auto tr = this->transformHistory.back();
    this->transformHistory.pop_back();
    float tX = tr[0];
    float tY = tr[1];
    float tZ = tr[2];
    float rX = tr[3];
    float rY = tr[4];
    float rZ = tr[5];
    for(int i = 0; i < this->size(); i++)
    {
        translatePoint(this->points[i], -zeroTX, -zeroTY, -zeroTZ);
        rotate(this->points[i], -zeroRX / 180 * M_PI, -zeroRY / 180 * M_PI, -zeroRZ / 180 * M_PI);
        rotate(this->points[i], rX / 180 * M_PI, rY / 180 * M_PI, rZ / 180 * M_PI);
        translatePoint(this->points[i], tX, tY, tZ);
    }
    zeroTX = tX;
    zeroTY = tY;
    zeroTZ = tZ;
    zeroRX = rX;
    zeroRY = rY;
    zeroRZ = rZ;
}

template<> void pointCloud<pointRGB>::borderFilter(float minX, float maxX, float minY, float maxY, float minZ, float maxZ, int borderFilterType)
{
    std::vector<pointRGB> filtered, unfiltered;
    for(auto &p : this->points)
    {
        float x = p.x(), y = p.y(), z = p.z();
        if(borderFilterType == 0)
        {
            if(x > minX && x < maxX)
            {
                filtered.push_back(p);
            }
            else
            {
                unfiltered.push_back(p);
            }
        }
        else if(borderFilterType == 1)
        {
            if(y > minY && y < maxY)
            {
                filtered.push_back(p);
            }
            else
            {
                unfiltered.push_back(p);
            }
        }
        else if(borderFilterType == 2)
        {
            if(z > minZ && z < maxZ)
            {
                filtered.push_back(p);
            }
            else
            {
                unfiltered.push_back(p);
            }
        }
        else
        {
            if(x > minX && x < maxX && y > minY && y < maxY && z > minZ && z < maxZ)
            {
                filtered.push_back(p);
            }
            else
            {
                unfiltered.push_back(p);
            }
        }
    }
    filterHistory.push_back(unfiltered);
    this->points = filtered;
}
template<> void pointCloud<pointRGB>::undoFilter()
{
    if(this->filterHistory.empty()) return;
    auto filterP = this->filterHistory.back();
    this->filterHistory.pop_back();
    for(auto &p : filterP)
    {
        this->points.push_back(p);
    }
}
