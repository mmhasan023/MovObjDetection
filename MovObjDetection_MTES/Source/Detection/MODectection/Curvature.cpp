#include"Curvature.h"

/*float firstDerivative(float x , float y)
{
return x-y;
}

float secondDerivative(float x1, float x2, float x3, float x4, float x5)
{
return firstDerivative(x5,x3)-firstDerivative(x3,x1);
}

float curvature(PointType Pt1,PointType Pt2,PointType Pt3,PointType Pt4,PointType Pt5)
{
float x_y__=firstDerivative(Pt4.GetX(),Pt2.GetX())*secondDerivative(Pt1.GetY(),Pt2.GetY(),Pt3.GetY(),Pt4.GetY(),Pt5.GetY());
float y_x__=firstDerivative(Pt4.GetY(),Pt2.GetY())*secondDerivative(Pt1.GetX(),Pt2.GetY(),Pt3.GetX(),Pt4.GetY(),Pt5.GetX());
float x_2=firstDerivative(Pt4.GetX(),Pt2.GetX())*firstDerivative(Pt4.GetX(),Pt2.GetX());
float y_2=firstDerivative(Pt4.GetY(),Pt2.GetY())*firstDerivative(Pt4.GetY(),Pt2.GetY());

if(powf(x_2+y_2,1.5)>0.01)
return (x_y__-y_x__)/powf(x_2+y_2,1.5);
else
return 0;
}*/

float firstDerivative(float x , float y)
{
return fabs(x-y);
}

float secondDerivative(float x1, float x2, float x3, float x4)
{
return fabs(firstDerivative(x1,x2)-firstDerivative(x3,x4));
}

float curvature(PointType Pt1,PointType Pt2,PointType Pt3,PointType Pt4,PointType Pt5,PointType Pt6,PointType Pt7)
{
float x_y__=firstDerivative(Pt3.GetX(),Pt5.GetX())*secondDerivative(Pt1.GetY(),Pt2.GetY(),Pt6.GetY(),Pt7.GetY());
float y_x__=firstDerivative(Pt3.GetY(),Pt5.GetY())*secondDerivative(Pt1.GetX(),Pt2.GetY(),Pt6.GetX(),Pt7.GetY());
float x_2=firstDerivative(Pt3.GetX(),Pt5.GetX())*firstDerivative(Pt3.GetX(),Pt5.GetX());
float y_2=firstDerivative(Pt3.GetY(),Pt5.GetY())*firstDerivative(Pt3.GetY(),Pt5.GetY());

if(powf(x_2+y_2,1.5)>0.01)
return fabs((float)(x_y__-y_x__)/powf(x_2+y_2,1.5));
else
return 1;
}