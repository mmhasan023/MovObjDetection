// EdgeSegmentType.cpp: implementation of the EdgeSegmentType class.
//
//////////////////////////////////////////////////////////////////////

#include "EdgeSegmentType.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define SAMPLE_LENGTH 10

EdgeSegmentType::EdgeSegmentType()
{
	segSize		= 0;
	segIndex	= 0;
	curPointIndex	= 0;
	edgePoints	= NULL;

	//new
	curvaturePoints=NULL;
	//end


	sideColor1=0;
	sideColorVar1=0;
	sideColor2=0;
	sideColorVar2=0;
	avgCurvature=0;
	edgeShape=0;
	weight=0;
	maxx=0;
	maxy=0;
	minx=0;
	miny=0;
	centerX=0;
	centerY=0;

}

EdgeSegmentType::~EdgeSegmentType()
{
//	segSize		= 0;
//	segIndex	= 0;
//	curPointIndex	= 0;
	
//	if(edgePoints != NULL)
//		delete[] edgePoints;
}

//new 
bool EdgeSegmentType::AllocCurvatureIntensity(int size)
{

	s1IntensityMean=new float[int(size/SAMPLE_LENGTH)];
	s1IntensityVar=new float[int(size/SAMPLE_LENGTH)];
	s2IntensityMean=new float[int(size/SAMPLE_LENGTH)];
	s2IntensityVar=new float[int(size/SAMPLE_LENGTH)];
	curvatureValue=new float[int(size/SAMPLE_LENGTH)];
	curvaturePoints=new PointType[int(size/SAMPLE_LENGTH)];

	if(!s1IntensityMean) return false;
	else if(!s1IntensityVar)return false;
	else if(!s2IntensityMean) return false;
	else if(!s2IntensityVar)return false;
	else if(!curvatureValue)return false;
	else if(!curvaturePoints)return false;

	else    
		return true ;
}

EdgeSegmentType::EdgeSegmentType (int size)
{
	segSize		= size;
	AllocPoint(size);
	AllocGrad(size);
	AllocTheta(size);

	//new
	AllocCurvatureIntensity(size);
}


float EdgeSegmentType::getSideColor(int i){
	if(i==1) return sideColor1;
	return sideColor2;
}


float EdgeSegmentType::getSideColorVar(int i){
	if(i==1) return sideColorVar1;
	return sideColorVar2;
}

float EdgeSegmentType::getAvgCurvature(){
	return avgCurvature;
}

float EdgeSegmentType::getEdgeShape(){
return edgeShape;
}

float EdgeSegmentType::getWeight()
{
	return weight;
}
void EdgeSegmentType::getMotion(float *i, float *j)
{
*i=motionX;
*j=motionY;	
}
void EdgeSegmentType::setMotion(float i, float j)
{
motionX=i;
motionY=j;	
}





void EdgeSegmentType::setSideColor(int i, float color){

	if(i==1) sideColor1=color;
	else sideColor2=color;
}

void EdgeSegmentType::setSideColorVar(int i, float color){

	if(i==1) sideColorVar1=color;
	else sideColorVar2=color;
}

void EdgeSegmentType::setAvgCurvature(float avgCur)
{
	avgCurvature=avgCur;	
	
}

void EdgeSegmentType::setEdgeShape(float shape)
{
	edgeShape=shape;
}
void EdgeSegmentType::setWeight(int i)
{
weight=i;
}






bool EdgeSegmentType::AllocPoint(int size)
{
	edgePoints   = new PointType[size];
	memset(edgePoints,0,sizeof(PointType)*size);

	if(!edgePoints) 
		return false;
	else         
		return true ;
}

bool EdgeSegmentType::AllocGrad(int size)
{
	edgeGrads   = new float[size];
	memset(edgeGrads,0.0,sizeof(float)*size);

	if(!edgeGrads) 
		return false;
	else         
		return true ;
}

bool EdgeSegmentType::AllocTheta(int size)
{
	edgeTheta   = new float[size];
	memset(edgeTheta,0.0,sizeof(float)*size);

	if(!edgeTheta) 
		return false;
	else         
		return true ;
}
bool EdgeSegmentType::SetPoint(int size, PointType *point)
{
	PointType *pTmpP = point;
	if(!edgePoints)
	{
		AllocPoint(size);
	}

	PointType *pCurP = edgePoints;
	for(register int i=0; i<size ; i++ , pTmpP++, pCurP++)
	{	
		int x	= (*pTmpP).GetX();
		(*pCurP).SetX((*pTmpP).GetX());
		(*pCurP).SetY((*pTmpP).GetY());
	}
	return true;
}

bool EdgeSegmentType::SetPoint(list<PointType> *point, float **thin, float **theta)
{
	int i=0;
	int tmp=0;
	if(!edgePoints)
	{
		AllocPoint(point->size());
	}

	if(!edgeGrads)
	{
		AllocGrad(point->size());
	}
	
	if(!edgeTheta)
	{
		AllocTheta(point->size());
	}

	if(!s1IntensityMean)
	{
		AllocCurvatureIntensity(point->size());
	}


	list<PointType>::iterator curpoint;
	for(curpoint=point->begin(); curpoint!=point->end();curpoint++)
	{
		//tmp+=curpoint->value;
		edgePoints[i]=(*curpoint);
		edgeGrads[i] = thin[(*curpoint).GetY()][(*curpoint).GetX()];
		edgeTheta[i] = theta[(*curpoint).GetY()][(*curpoint).GetX()];
		i++;
	}
	

	return true;
}


void EdgeSegmentType::operator = (EdgeSegmentType sg)
{
	segSize = sg.segSize;
	segIndex = sg.segIndex;

/*	if(edgePoints != NULL){
		delete edgePoints;
		edgePoints =NULL;
	}	
*/	
	edgePoints = new PointType[segSize];
	
	//memset(edgePoints, 0, sizeof(PointType) * segSize);
	memcpy(edgePoints, sg.edgePoints, sizeof(PointType) * segSize);
}

PointType *EdgeSegmentType::GetStartPoint(void )
{
	if(edgePoints)
		return edgePoints;
	else
		return NULL;
}

PointType *EdgeSegmentType::GetEndPoint(void )
{
	
	if(edgePoints)
		return &edgePoints[segSize-1];
	else
		return NULL;
	//return &edgePoints[segSize-1];
}

PointType EdgeSegmentType::GetCurPoint()
{
	return edgePoints[curPointIndex];
}

PointType EdgeSegmentType::GetPoint(int i)
{
	return edgePoints[i];
}

float EdgeSegmentType::GetGrad(int i)
{
	return edgeGrads[i];
}

int  EdgeSegmentType::GetSize(){
	return segSize;
}

void EdgeSegmentType::SetSegIndex(int x)
{
	segIndex	= x; 
}

int EdgeSegmentType::GetSegIndex()
{
	return segIndex; 
}

void EdgeWeightType::SetWeight(int x)
{
	weight = x;
}

void EdgeWeightType::SetSegIndex(int x)
{
	segIndex = x;
}

int EdgeWeightType::GetWeight()
{
	return weight;
}

int EdgeWeightType::GetSegIndex()
{
	return segIndex;
}
