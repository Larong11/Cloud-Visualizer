#ifndef POINT_H
#define POINT_H


class point
{
public:
    point();
    point(float x, float y, float z);
    float x();
    float y();
    float z();
    void set_x(float x);
    void set_y(float y);
    void set_z(float z);

protected:
    float x_p;
    float y_p;
    float z_p;
};


class pointI : public point
{
public:
    pointI();
    pointI(float x, float y, float z, float intensity);
    float intensity();
    void set_intensity(float intensity);

private:
    float intensity_p;
};


class pointRGB : public point
{
public:
    pointRGB();
    pointRGB(float x, float y, float z, unsigned short r, unsigned short g, unsigned short b);
    unsigned short r();
    unsigned short g();
    unsigned short b();
    void set_r(float r);
    void set_g(float g);
    void set_b(float b);

private:
    unsigned short r_p;
    unsigned short g_p;
    unsigned short b_p;
};

#endif // POINT_H
