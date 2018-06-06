// EdgeListType.h: interface for the EdgeListType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDGELISTTYPE_H__D49D5A20_6DB0_48D5_9B64_9E307952073B__INCLUDED_)
#define AFX_EDGELISTTYPE_H__D49D5A20_6DB0_48D5_9B64_9E307952073B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "EdgeSegmentType.h"
#include "Vertex.h"
#include "List"
#include "KSClass.h"
using namespace std;
int minimum(int val1, int val2, int val3, int val4);
class EdgeListType  
{
	public:
		EdgeListType();
		virtual ~EdgeListType();

protected:	
		//information about edge segment
		list<EdgeSegmentType> *edgeList;	// List of edge segment

		int numSegment;			        	// number of edge segment

		//link information of each segment 
		list<VertexType> *verticeList;//list of start and end edge pixel information of each segment
		int numVertice;			// number of vertex 
	    
		// Pointer to current segment                      	
		EdgeSegmentType *curSegment;
	
	public:
	
		//// member function---------------------------------------------
		void  EdgeListClear();	
		void  AddNewSegment(EdgeSegmentType edge);
		void  AddNewSegment(EdgeSegmentType *edge);
		void  AddNewSegment(int size, PointType* point,int label);//Add new segment
		void  AddNewSegment(int size, list<PointType> *point, int label, float **thin, float **theta);
		void  DeleteFirstNPoints(list<EdgeSegmentType>::iterator curEdgeSeg, int size);
		void  DeleteLastNPoints(list<EdgeSegmentType>::iterator curEdgeSeg, int size);
		list<EdgeSegmentType>::iterator ListBegin();
		list<EdgeSegmentType>::iterator ListEnd();
		//Add a new list
		void  AppendList(list<EdgeSegmentType> *edgeList); //Append a new list
	//	void  AddNewVertex(int x, int y, int *array);
	//	void  SetVerticesSegNum();
	//	void  DeleteVertices();

		int DeleteSegment(int index);
		list<EdgeSegmentType>::iterator DeleteSegment(list<EdgeSegmentType>::iterator segment);
		void ListToDistanceImage(KScScalarImage2dInt32 *image, KScScalarImage2dInt32 *Limage);
		void ListToLabelImage(KScScalarImage2dInt32 *image);
	//	int DeleteVertex(int vertexIndex); 

	//	void CpyEdge(EdgeListType *inEdge);
	//	void CpyEdgeSeg(list<EdgeSegmentType> *edgeSeg);
	//	void CpyVertices(list<VertexType> *vertex);
	
	private:	
		int SmallestFreeIndex(); //Finds the smallest available segment number for new segment
	//	int MaxSegmentNumber();

	
	public:
		//void ListToMarkedImage(KScScalarImage2dInt32* img);
		//void ListToDistanceImage(KScScalarImage2dInt32 *img); //Creating distance image
	//	int  GetSize(int SegmentIndex); //returns the size of given segment
	//	void operator = ( EdgeListType inedge );

};

#endif // !defined(AFX_EDGELISTTYPE_H__D49D5A20_6DB0_48D5_9B64_9E307952073B__INCLUDED_)
