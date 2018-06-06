// PointType.cpp: implementation of the PointType class.
//
//////////////////////////////////////////////////////////////////////

#include "PointType.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PointType::PointType()
{
	x = 0;
	y = 0;
}

PointType::~PointType()
{

}

bool PointType::operator == (PointType p)
{
	if(x==p.x && y==p.y) 
		return true ;
	else
		return false;
}


void PointType::operator = (PointType p)
{
	x=p.x;
	y=p.y;
}

PointType PointType::GetEE(){PointType p;p.x=x+1;p.y=y  ; return p;};
PointType PointType::GetNE(){PointType p;p.x=x+1;p.y=y-1; return p;};
PointType PointType::GetNN(){PointType p;p.x=x  ;p.y=y-1; return p;};
PointType PointType::GetNW(){PointType p;p.x=x-1;p.y=y-1; return p;};
PointType PointType::GetWW(){PointType p;p.x=x-1;p.y=y  ; return p;};
PointType PointType::GetSW(){PointType p;p.x=x-1;p.y=y+1; return p;};
PointType PointType::GetSS(){PointType p;p.x=x  ;p.y=y+1; return p;};
PointType PointType::GetSE(){PointType p;p.x=x+1;p.y=y+1; return p;};

void PointType::SetX(int i)
{
	x = i;
}

void PointType::SetY(int i)
{
	y = i;
}

void PointType::SetPoint(int i, int j)
{
	x = i;
	y = j;
}

int PointType::GetX()
{
	return x;
}

int PointType::GetY()
{
	return y;
}

void PointType::SetValue(float x)
{
	value = x;
}

float PointType::GetValue()
{
	return value;
}
