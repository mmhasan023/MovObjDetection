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
//	verticeList->clear(); 
//	edgeList->clear();
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
	EdgeSegmentType tmpSeg(size);
	
	tmpSeg.SetPoint(size,point);
	
	tmpSeg.SetSegIndex(label);
	
	if(edgeList==NULL){ 
		edgeList = new list<EdgeSegmentType> ;
		numSegment	= 0;
	}
	edgeList->push_back(tmpSeg);
	numSegment++;
}

void  EdgeListType::AddNewSegment(int size, list<PointType> *point,int label, float **thin, float **theta)
{
	EdgeSegmentType tmpSeg(size);
	
	tmpSeg.SetPoint(point, thin, theta);
	
	tmpSeg.SetSegIndex(label);
	
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
	edgeList->push_back(tmpSeg);
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
