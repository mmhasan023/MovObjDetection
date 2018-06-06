#include "EdgeCluster.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

EdgeClusterType:: EdgeClusterType()
{
	edgeList      = NULL ;
	numSegment = 0;
	clusterID=0;
	centerX=0;
	centerY=0;
	SumX=0;
	SumY=0;
	maxx=0;
	maxy=0;
	minx=0;
	miny=0;
	movX=0;
	movY=0;
	varX=0;
	varY=0;
	width=0; 
	height=0;
	oldx=0;oldy=0;olddx=0;olddy=0;
	learnRate=1;
	for(int i=0;i<4;i++)
	{
			for(int j=0;j<4;j++)
			{
				KalmanPar[i][j]=0.01;
			}
	}


	//curSegment	= NULL;
	edgeList	= new list<EdgeSegmentType> ;
}



EdgeClusterType::~EdgeClusterType()
{
	delete edgeList;
}

void EdgeClusterType::EdgeListClear()
{
	numSegment	= 0;
	centerX=0;
	centerY=0;
	edgeList->clear();
	edgeList=NULL;
}


void EdgeClusterType::AddNewSegment(EdgeSegmentType edge)
{
	if(edgeList==NULL){ 
		edgeList = new list<EdgeSegmentType> ;
		numSegment	= 0;		
	}
	edgeList->push_back(edge);
	numSegment++;
}

void EdgeClusterType::setOldMotion(int x, int y, float dx, float dy){
oldx=x; 
oldy=y;
olddx=dx;
olddy=dy;
}

void EdgeClusterType::getOldMotion(int &x, int &y, float &dx, float &dy){
x=oldx; 
y=oldy;
dx=olddx;
dy=olddy;
}

/*
void  EdgeClusterType::AddNewSegment(EdgeSegmentType *newSegment)
{
	if(edgeList==NULL){ 
		edgeList = new list<EdgeSegmentType> ;
		numSegment	= 0;
	}
	edgeList->push_back(*newSegment);
	numSegment++;
}
*/
/*
int EdgeClusterType::DeleteSegment(int index)
{ 
	EdgeSegmentType *tmpEdge;
	tmpEdge = new EdgeSegmentType;
	list<EdgeSegmentType>::iterator curEdge;
	for(curEdge=edgeList->begin();curEdge!=edgeList->end();curEdge++)
	{
		*tmpEdge = (*curEdge);
		int curIndex = (*tmpEdge).GetSegIndex();
		if(curIndex == index){
			edgeList->erase(curEdge);
			numSegment--;
			return 1;
		}
	}
	return 0;
}*/

int EdgeClusterType::getClusterIndex()
{
	return clusterID;
}

int EdgeClusterType::getTotalEdge()
{
	return numSegment;
}


void EdgeClusterType::setClusterIndex(int ID)
{
	clusterID=ID;
}


void EdgeClusterType::getClusterCenter(float *X, float *Y)
{
	*X=centerX;
	*Y=centerY;
}

void EdgeClusterType::setClusterCenter(float X, float Y)
{
	centerX=X;
	centerY=Y;
}



void EdgeClusterType::getMotion(float *X, float *Y)
{
	*X=movX;
	*Y=movY;
}

void EdgeClusterType::setMotion(float X, float Y)
{
	movX=X;
	movY=Y;
}


void EdgeClusterType::getVariance(float *X, float *Y)
{
	*X=varX;
	*Y=varY;
}

void EdgeClusterType::setVariance(float X, float Y)
{
	varX=X;
	varY=Y;
}



void EdgeClusterType::setSum(float X, float Y)
{
	SumX=SumX+X;
	SumY=SumY+Y;
}


void EdgeClusterType::getSum(float *X, float *Y)
{
	*X=SumX;
	*Y=SumY;
}

void EdgeClusterType::setBoundaryParam(int max, int may, int mix, int miy)
{
maxx=max;
maxy=may;
minx=mix;
miny=miy;
width=(maxx-minx)+1;
height=(maxy-miny)+1;
}

void EdgeClusterType::getWidthHeight(float *X, float *Y)
{
	*X=width;
	*Y=height;
}


void EdgeClusterType::setWidthHeight(float X, float Y)
{
	width=X;
	height=Y;
}


void EdgeClusterType::getBoundaryParam(int *max, int *may, int *mix, int *miy)
{
*max=maxx;
*may=maxy;
*mix=minx;
*miy=miny;
}

list<EdgeSegmentType>::iterator EdgeClusterType::DeleteSegment(list<EdgeSegmentType>::iterator segment)
{
	numSegment--;
	return edgeList->erase(segment);
}


list<EdgeSegmentType>::iterator EdgeClusterType::ListBegin()
{
	return edgeList->begin();
}

list<EdgeSegmentType>::iterator EdgeClusterType::ListEnd()
{
	return edgeList->end();
}

