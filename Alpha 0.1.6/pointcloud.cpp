#include "pointcloud.h"

template<>
pointCloud<point>::pointCloud()
{
    typePoint = "xyz";
}

template<>
pointCloud<pointI>::pointCloud()
{
    typePoint = "i";
}

template<> pointCloud<pointRGB>::pointCloud()
{
    typePoint = "rgb";
}
