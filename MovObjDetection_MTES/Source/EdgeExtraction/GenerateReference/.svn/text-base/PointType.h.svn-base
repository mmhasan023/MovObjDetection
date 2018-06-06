// PointType.h: interface for the PointType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINTTYPE_H__F2FFB0AB_CAF2_4819_952A_1AF52EF1EF88__INCLUDED_)
#define AFX_POINTTYPE_H__F2FFB0AB_CAF2_4819_952A_1AF52EF1EF88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class PointType  
{
private:
	int x, y; //coordinates of a point
	float value;
public:
	PointType();
	virtual ~PointType();

	bool operator == (PointType p); //Comparison
	void operator = (PointType p) ; //Assingment
	void SetX(int i);
	void SetY(int i);
	void SetPoint(int i, int j);
	int	 GetX();
	int  GetY();
	void SetValue(float x);
	float GetValue();
	PointType GetEE(); //Access the point locating EE
	PointType GetNE();
   	PointType GetNN();
   	PointType GetNW();
   	PointType GetWW();
   	PointType GetSW();
   	PointType GetSS();
   	PointType GetSE();

};

#endif // !defined(AFX_POINTTYPE_H__F2FFB0AB_CAF2_4819_952A_1AF52EF1EF88__INCLUDED_)
