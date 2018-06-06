// LineSegmentType.h: interface for the LineSegmentType class.
//
//////////////////////////////////////////////////////////////////////

#include <math.h>
#include "PointType.h"
#include "KSClass.h"
#include "List"

using namespace std;

class LineSegmentType  
{
private:
	int x1,y1,x2,y2,xm,ym;
	int		length;
	float	slope; 
	float  sideColor1;
	float  sideColor2;
	int lineCandidateType; //for grouping
public:
	LineSegmentType();
	//LineSegmentType(int x1,int y1, int x2, int y2);
	LineSegmentType(const LineSegmentType &ob);
	virtual ~LineSegmentType();
	
	//float getSideColor(int i);
	void setLineSegment(int x1,int y1, int x2, int y2, float slope, float sideColor1, float sideColor2);
	void getLine(int *cx1, int *cy1, int *cx2, int *cy2);
	void getMidPoint(int *cx1, int *cy1);
	int getLength();
	float getSlope();
	void getSideColor(float *c1, float *c2);
	int getLineCanType();
	//void setSideColor(int i, float color);
	void setLineCanType(int can);
	//void operator = ( LineSegmentType sg ); //Overloading the assignment of for segment
};

