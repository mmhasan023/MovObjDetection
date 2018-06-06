// EdgeListType.cpp: implementation of the EdgeListType class.
//
//////////////////////////////////////////////////////////////////////

#include "EdgeListType.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

EdgeListType:: EdgeListType()
{
	edgeList      = NULL ;
	numSegment = 0;
	
	verticeList	= NULL;
	numVertice	= 0;

	curSegment	= NULL;

	verticeList	= new list<VertexType>; 
	edgeList	= new list<EdgeSegmentType> ;
}

EdgeListType::~EdgeListType()
{
	delete verticeList;
	delete edgeList;
}

void EdgeListType::EdgeListClear()
{
	
	numSegment	= 0;
	numVertice	= 0;
	curSegment	= NULL;
	verticeList->clear(); 
	edgeList->clear();
}

void EdgeListType::AddNewSegment(EdgeSegmentType edge)
{
	if(edgeList==NULL){ 
		edgeList = new list<EdgeSegmentType> ;
		numSegment	= 0;
	}
	edgeList->push_back(edge);
	numSegment++;
}


void  EdgeListType::AddNewSegment(int size, PointType* point,int label)
{
	EdgeSegmentType *tmpSeg;
	tmpSeg=new EdgeSegmentType(size);
	//tmpSeg->AllocPoint(size);
	//tmpSeg->AllocGrad(size);
	//tmpSeg->AllocTheta(size);
	tmpSeg->SetPoint(size,point);
	tmpSeg->SetSegIndex(label);
	
	if(edgeList==NULL){ 
		edgeList = new list<EdgeSegmentType> ;
		numSegment	= 0;
	}
	edgeList->push_back((*tmpSeg));
	numSegment++;
}

void  EdgeListType::AddNewSegment(int size, list<PointType> *point,int label, float **thin, float **theta)
{
	EdgeSegmentType *tmpSeg;
	tmpSeg=new EdgeSegmentType(size);
	//tmpSeg->AllocPoint(size);
	//tmpSeg->AllocGrad(size);
	//tmpSeg->AllocTheta(size);
	
	tmpSeg->SetPoint(point, thin, theta);
	
	tmpSeg->SetSegIndex(label);
	
/*	PointType p;
	list<PointType>::iterator curPoint;
	for(curPoint=point->ListBegin();curPoint!=point->ListEnd();curPoint++)
	{
//		*tmpEdge = (*curEdge);
		int size = (*curEdge).GetSize();
		for(int i=0 ; i<size ; i++)
		{
			p=(*curEdge).GetPoint(i);
			pOutImage[p.GetY()][p.GetX()] =255;

		}
	}
*/	
	if(edgeList==NULL){ 
		edgeList = new list<EdgeSegmentType> ;
		numSegment	= 0;
	}
	edgeList->push_back(*tmpSeg);
	numSegment++;
}


void  EdgeListType::AddNewSegment(EdgeSegmentType *newSegment)
{
/*	EdgeSegmentType *tmpEdge;
	tmpEdge = new EdgeSegmentType;

	if(edgeList==NULL){ 
		edgeList = new list<EdgeSegmentType> ;
		numSegment	= 0;
	}
	*tmpEdge = (*newSegment);
	int index = SmallestFreeIndex();
	(*tmpEdge).SetSegIndex(index);
	edgeList->push_back(*tmpEdge);
	numSegment++;
*/
	if(edgeList==NULL){ 
		edgeList = new list<EdgeSegmentType> ;
		numSegment	= 0;
	}
	edgeList->push_back(*newSegment);
	numSegment++;
}

void  EdgeListType::AppendList(list<EdgeSegmentType> *newList)
{
	EdgeSegmentType *tmpEdge;
	
	list<EdgeSegmentType>::iterator curEdge;
	for(curEdge=newList->begin();curEdge!=newList->end();curEdge++)
	{
		tmpEdge = new EdgeSegmentType;
		*tmpEdge = (*curEdge);
		int index = SmallestFreeIndex();
		(*tmpEdge).SetSegIndex(index);
		edgeList->push_back(*tmpEdge);
	}
	numSegment+=newList->size();
}

int EdgeListType::DeleteSegment(int index)
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
}

list<EdgeSegmentType>::iterator EdgeListType::DeleteSegment(list<EdgeSegmentType>::iterator segment)
{
	return edgeList->erase(segment);
	numSegment--;
}

void EdgeListType::DeleteFirstNPoints(list<EdgeSegmentType>::iterator curEdgeSeg, int size)
{
	(*curEdgeSeg).DeleteFirstNPoints(size);

}

void EdgeListType::DeleteLastNPoints(list<EdgeSegmentType>::iterator curEdgeSeg, int size)
{
	(*curEdgeSeg).DeleteLastNPoints(size);

}


int EdgeListType::SmallestFreeIndex()
{

	list<EdgeSegmentType>::iterator curEdge;
	
	int minIndex=1;
	int *flag;
	int n = edgeList->size();
	flag = new int[n+2];
	memset(flag,0,(sizeof(int) * n + 2));

	for(curEdge=edgeList->begin();curEdge!=edgeList->end();++curEdge)
	{
		int index = (*curEdge).GetSegIndex();
		if(index > 0 && index <=n)
			flag[index]=1;
	}
	int i;
	for(i =1; i<n+2; i++){
		if(flag[i]==0)
			break;
	}
	
	delete[] flag;
	return i;
}

list<EdgeSegmentType>::iterator EdgeListType::ListBegin()
{
	return edgeList->begin();
}

list<EdgeSegmentType>::iterator EdgeListType::ListEnd()
{
	return edgeList->end();
}

int minimum(int val1, int val2, int val3, int val4){
	int minVal = val1;
	
	if(val2<minVal)
		minVal = val2;
	if(val3<minVal)
		minVal = val3;
	if(val4<minVal)
		minVal = val4;
	return minVal;
}
//Generate Distance Image
void EdgeListType::ListToDistanceImage(KScScalarImage2dInt32 *image, KScScalarImage2dInt32 *Limage )
{
	list<EdgeSegmentType>::iterator curEdge;
	PointType p;
//	int tmp;
	register int i,j;
	// initializing the size
	int dx = image->GetXSize();
	int dy = image->GetYSize();
	int oldValue=0;


	int **pImage = image->Get2dArray();
	int **pLimage = Limage->Get2dArray();

	for(i=0; i<dy;i++)
		for(j=0; j<dx; j++)
		{
			pImage[i][j]=1000;
			pLimage[i][j]=1000;
		}

	for(curEdge=ListBegin();curEdge!=ListEnd();curEdge++)
	{
		int size = (*curEdge).GetSize();
		for( i=0 ; i<size ; i++)
		{
			p=(*curEdge).GetPoint(i);
			pImage[p.GetY()][p.GetX()] = 0;
			pLimage[p.GetY()][p.GetX()] = (*curEdge).GetSegIndex();
		}
	}

	for(i=1; i<dy;i++)
		for(j=1; j<dx; j++)
		{
			oldValue=pImage[i][j];
			pImage[i][j] = minimum(pImage[i-1][j-1]+4, pImage[i-1][j]+3, pImage[i][j-1]+3, pImage[i][j]);
			if(oldValue!=pImage[i][j])
			{
				if(pImage[i][j]==pImage[i-1][j-1]+4)
				{
					pLimage[i][j]=pLimage[i-1][j-1];
				}
				else if(pImage[i][j]==pImage[i-1][j]+3)
				{
					pLimage[i][j]=pLimage[i-1][j];
				}
				else if(pImage[i][j]==pImage[i][j-1]+3)
				{
					pLimage[i][j]=pLimage[i][j-1];
				}
			}
		}
	
	for(i=dy-2; i>=0;i--)
		for(j=dx-2; j>=0; j--)
		{
			oldValue=pImage[i][j];
			pImage[i][j] = minimum(pImage[i][j], pImage[i][j+1]+3, pImage[i+1][j]+3, pImage[i+1][j+1]+4);
			if(oldValue!=pImage[i][j])
			{
				if(pImage[i][j]==pImage[i][j+1]+3)
				{
					pLimage[i][j]=pLimage[i][j+1];
				}
				else if(pImage[i][j]==pImage[i+1][j]+3)
				{
					pLimage[i][j]=pLimage[i+1][j];
				}
				else if(pImage[i][j]==pImage[i+1][j+1]+4)
				{
					pLimage[i][j]=pLimage[i+1][j+1];
				}
			}
		}
}

//Generate Label Image
void EdgeListType::ListToLabelImage(KScScalarImage2dInt32 *image)
{
	list<EdgeSegmentType>::iterator curEdge;
	PointType p;
	register int i,j;
	// initializing the size
	int dx = image->GetXSize();
	int dy = image->GetYSize();

	int **pImage = image->Get2dArray();

	for(i=0; i<dy;i++)
		for(j=0; j<dx; j++)
			pImage[i][j]=0;

	for(curEdge=ListBegin();curEdge!=ListEnd();curEdge++)
	{
		int size = (*curEdge).GetSize();
		for( i=0 ; i<size ; i++)
		{
			p=(*curEdge).GetPoint(i);
			pImage[p.GetY()][p.GetX()] = (*curEdge).GetSegIndex();
		}
	}
}