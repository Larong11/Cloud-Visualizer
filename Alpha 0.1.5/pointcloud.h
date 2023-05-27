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

private:
    std::string typePoint;
    std::vector<pointType> points;
};

#endif // POINTCLOUD_H
