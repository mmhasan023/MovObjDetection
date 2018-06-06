// EdgeSegmentType.h: interface for the EdgeSegmentType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDGESEGMENTTYPE_H__496E3567_1002_4E49_BC92_DD5914A7096C__INCLUDED_)
#define AFX_EDGESEGMENTTYPE_H__496E3567_1002_4E49_BC92_DD5914A7096C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "PointType.h"
#include "List"

using namespace std;

class EdgeSegmentType  
{
private:
	int		segSize;   // number of pixels in the segment
	int		segIndex;  // Index/label of Current Segment
	int 	curPointIndex; 	 // While accessing the points of a segment this can be used
					 		 // for sequential visit of the points 
	PointType *edgePoints;  //segment of all points data
	float *edgeGrads;
	float *edgeTheta;

public:
	EdgeSegmentType();
	virtual ~EdgeSegmentType();
	EdgeSegmentType (int size); //Constructor
	bool AllocPoint(int size); //Allocation of points (size is number of points)
	bool AllocGrad(int size);
	bool AllocTheta(int size);
	
	bool SetPoint(int size, PointType *point); //Assignment of points
	bool SetPoint(list<PointType> *point, float **thin, float **theta); //Assignment of points
	PointType *GetStartPoint(void); //Get the start point of a segment
	PointType *GetEndPoint(void); //Get the end point of a segment
	void SetCurPointIndex(int i);
	PointType GetCurPoint(); //Get the points of a segment
   	PointType GetPoint(int i);
	float GetGrad(int i);
	int  GetSize(); //Get the size of the segment
	void SetSegIndex(int x);
	int	GetSegIndex();
	void operator = ( EdgeSegmentType sg ); //Overloading the assignment of for segment
};

class EdgeWeightType
{
	private:
		int weight;
		int segIndex;
	public:
		void SetWeight(int x);
		void SetSegIndex(int x);
		int GetWeight();
		int GetSegIndex();
};		


#endif // !defined(AFX_EDGESEGMENTTYPE_H__496E3567_1002_4E49_BC92_DD5914A7096C__INCLUDED_)
