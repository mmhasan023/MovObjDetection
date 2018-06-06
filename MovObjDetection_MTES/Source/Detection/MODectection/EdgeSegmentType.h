// EdgeSegmentType.h: interface for the EdgeSegmentType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDGESEGMENTTYPE_H__496E3567_1002_4E49_BC92_DD5914A7096C__INCLUDED_)
#define AFX_EDGESEGMENTTYPE_H__496E3567_1002_4E49_BC92_DD5914A7096C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <math.h>
#include "PointType.h"
#include "KSClass.h"
#include "List"

using namespace std;
static int SAMPLE_LENGTH=8;         // SAMPLE_LENGTH=1;
#define CRMX 11
#define CRMY 11
/*
static int CURVATURE_REGION_MASK[CRMX][CRMY]=  {
	{1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1}};
*/
static int CURVATURE_REGION_MASK[CRMX][CRMY]=  {
	{0,0,0,0,1,1,1,0,0,0,0},
	{0,0,0,1,1,1,1,1,0,0,0},
	{0,0,1,1,1,1,1,1,1,0,0},
	{0,1,1,1,1,1,1,1,1,1,0},
	{1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1},
	{0,1,1,1,1,1,1,1,1,1,0},
	{0,0,1,1,1,1,1,1,1,0,0},
	{0,0,0,1,1,1,1,1,0,0,0},
	{0,0,0,0,1,1,1,0,0,0,0}};

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
	
	//new
	float *s1IntensityMean;
	float *s1IntensityVar;
	float *s2IntensityMean;
	float *s2IntensityVar;
	float *curvatureValue;
	PointType *curvaturePoints;
	//end new




	float sideColor1;
	float sideColorVar1;
	float sideColor2;
	float sideColorVar2;
	float avgCurvature;
	float edgeShape;
	int weight, edgeFrameNo, clusterMemberID;
	int maxx, maxy, minx, miny;
	float centerX, centerY;
	float motionX, motionY;


public:
	EdgeSegmentType();
	EdgeSegmentType(const EdgeSegmentType &ob);
	virtual ~EdgeSegmentType();

	EdgeSegmentType (int size); //Constructor
	EdgeSegmentType (const EdgeSegmentType &ob, int size);

	bool AllocPoint(int size); //Allocation of points (size is number of points)
	bool AllocGrad(int size);
	bool AllocTheta(int size);

	//new
	bool setCurvaturePoint(int i, float *cValue, PointType *point);



	float getSideColor(int i);
	float getSideColorVar(int i);
	float getAvgCurvature();
	float getEdgeShape();
	float getWeight();
	int getFrameIndex();
	void getMotion(float *i, float *j);

	void setSideColor(int i, float color);
	void setSideColorVar(int i, float color);
	void setAvgCurvature(float avgCur);
	void setEdgeShape(float shape);
	void setWeight(int i);
	void setFrameIndex(int i);
	void setMotion(float i, float j);

	
	bool SetPoint(int size, PointType *point); //Assignment of points
	bool SetPoint(list<PointType> *point, float **thin, float **theta); //Assignment of points
	void DeleteFirstNPoints(int size);
	void DeleteLastNPoints(int size);
	void CopyBeforeN(EdgeSegmentType curEdgeSeg, int size);
	void CopyAfterN(EdgeSegmentType curEdgeSeg, int size);
	PointType *GetStartPoint(void); //Get the start point of a segment
	PointType *GetEndPoint(void); //Get the end point of a segment
	void SetCurPointIndex(int i);

	PointType GetCurvaturePoint(int i);
	float GetCurvatureValue(int i);
	PointType GetCurPoint(); //Get the points of a segment
   	PointType GetPoint(int i);
	float GetGrad(int i);
	float GetTheta(int i);
	int  GetSize(); //Get the size of the segment
	void SetSegIndex(int x);
	int	GetSegIndex();
	void operator = ( EdgeSegmentType sg ); //Overloading the assignment of for segment
	void setBoundaryParam(int max, int may, int mix, int miy);
	void setEdgeCenterParam(float centerX, float centerY);
	void getBoundaryParam(int *max, int *may, int *mix, int *miy);
	void getEdgeCenterParam(float *centerX, float *centerY);
	void setClusterMemberID(int ID);
	int getClusterMemberID();


	//new
	bool AllocCurvatureIntensity(int size);
	void setCurvatureIntensityValues(PointType *point,float *s1IntensityMean, float *s1IntensityVar, float *s2IntensityMean, float *s2IntensityVar);
	void getCurvatureIntensityValues(PointType *point,float *s1IntensityMean, float *s1IntensityVar, float *s2IntensityMean, float *s2IntensityVar);
	bool computeMeanVarianceAroundCurvaturePoint(int index, PointType *cPt, KSdUint8 ** inputImage, int dx, int dy);
	int findMinimum(int val1, int val2, int val3);

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
