#include "points.h"

//point
//-------------------------------
point::point() //zero constructor
{
    this->x_p = 0;
    this->y_p = 0;
    this->z_p = 0;
}
point::point(float x, float y, float z) //constructor with info
{
    this->x_p = x;
    this->y_p = y;
    this->z_p = z;
}

//get protected info
float point::x()
{
    return this->x_p;
}
float point::y()
{
    return this->y_p;
}
float point::z()
{
    return this->z_p;
}
//-------------------------------


//pointI
//-------------------------------
pointI::pointI() : point() //zero constructor
{
    this->intensity_p = 0;
}
pointI::pointI(float x, float y, float z, float intensity) : point(x, y, z) //constructor with info
{
    this->intensity_p = intensity;
}

//get protected info
float pointI::intensity()
{
    return this->intensity_p;
}
//-------------------------------


//pointRGB
//-------------------------------
pointRGB::pointRGB() : point() //zero constructor
{
    this->r_p = 0;
    this->g_p = 0;
    this->b_p = 0;
}
pointRGB::pointRGB(float x, float y, float z, unsigned short r, unsigned short g, unsigned short b) : point(x, y, z) //constructor with info
{
    this->r_p = r;
    this->g_p = g;
    this->b_p = b;
}

//get protected info
unsigned short pointRGB::r()
{
    return this->r_p;
}
unsigned short pointRGB::g()
{
    return this->g_p;
}
unsigned short pointRGB::b()
{
    return this->b_p;
}
//-------------------------------
