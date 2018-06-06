// LineSegmentType.cpp: implementation of the LineSegmentType class.
//
//////////////////////////////////////////////////////////////////////

#include "LineSegmentType.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LineSegmentType::LineSegmentType()
{
	x1=0;
	y1=0;
	x2=0;
	y2=0;
	xm=0;
	ym=0;
	length=0;
	slope=0.0; 
	sideColor1=0.0;
	sideColor2=0.0;
	lineCandidateType=0; //for grouping
	
}
/*LineSegmentType::LineSegmentType(int cx1,int cy1, int cx2, int cy2)
{
x1=cx1;
y1=cy1;
x2=cx2;
y2=cy2;
xm=(int)(cx1+cx2)/2;
ym=(int)(cy1+cy2)/2;
length=(int)sqrt((double)(x1-x2)*(x1-x2)+(double)(y1-y2)*(y1-y2));
slope=fabs((double)(y1-y2)/(x1-x2)); 

sideColor1=0.0;
sideColor2=0.0;
lineCandidateType=0; //for grouping
}
*/
LineSegmentType::LineSegmentType(const LineSegmentType &ob)
{
	x1=ob.x1;
	y1=ob.y1;
	x2=ob.x2;
	y2=ob.y2;
	xm=ob.xm;
	ym=ob.ym;
	length=ob.length;
	slope=ob.slope; 
	sideColor1=ob.sideColor1;
	sideColor2=ob.sideColor2;
	lineCandidateType=ob.lineCandidateType;
}

LineSegmentType::~LineSegmentType()
{

}

void LineSegmentType::setLineSegment(int cx1,int cy1, int cx2, int cy2, float cslope, float csideColor1, float csideColor2)
{
x1=cx1;
y1=cy1;
x2=cx2;
y2=cy2;
xm=(int)(cx1+cx2)/2;
ym=(int)(cy1+cy2)/2;
length=(int)sqrt((double)(x1-x2)*(x1-x2)+(double)(y1-y2)*(y1-y2));
slope=cslope; 
sideColor1=csideColor1;
sideColor2=csideColor2;
lineCandidateType=0; //for grouping
}

void LineSegmentType::getLine(int *cx1, int *cy1, int *cx2, int *cy2)
{
*cx1=x1;
*cy1=y1;
*cx2=x2;
*cy2=y2;
}

void LineSegmentType::getMidPoint(int *cx1, int *cy1)
{
*cx1=xm;
*cy1=ym;
}

void LineSegmentType::getSideColor(float *c1, float *c2)
{
*c1=sideColor1;
*c2=sideColor2;
}


int LineSegmentType::getLength()
{
return length;
}

float LineSegmentType::getSlope()
{
return slope;
}

int LineSegmentType::getLineCanType()
{
return lineCandidateType;
}
//void LineSegmentType::setSideColor(int i, float color);
	
void LineSegmentType::setLineCanType(int can)
{
lineCandidateType=can;
}
	
//void LineSegmentType::operator = ( LineSegmentType csg )
//{
//LineSegmentType(csg);
//}