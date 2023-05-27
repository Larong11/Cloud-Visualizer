#ifndef POINTCLOUD_H
#define POINTCLOUD_H

//C++ libs
#include <vector>
#include <string>

//my libs
#include "points.h"

template <typename pointType>
class pointCloud
{
public:
    pointCloud();

    void resize(unsigned n)
    {
        points.resize(n);
    }
    void clear(){
        points.clear();
    }
    void push_back(pointType p){
        points.push_back(p);
    }
    std::string type()
    {
        return typePoint;
    }
    pointType &operator[](unsigned i)
    {
        return points[i];
    }
    unsigned size()
    {
        return points.size();
    }
    void transform(float tX, float tY, float tZ,
                   float rX, float rY, float rZ, bool relative = false);
    void borderFilter(float minX, float maxX, float minY, float maxY, float minZ, float maxZ, int borderFilterType);
    void undo();
    void undoFilter();

private:
    std::string typePoint;
    std::vector<pointType> points;
    float zeroTX, zeroTY, zeroTZ;
    float zeroRX, zeroRY, zeroRZ;
    std::vector<std::vector<float>> transformHistory;
    std::vector<std::vector<pointType>> filterHistory;
};

#endif // POINTCLOUD_H
