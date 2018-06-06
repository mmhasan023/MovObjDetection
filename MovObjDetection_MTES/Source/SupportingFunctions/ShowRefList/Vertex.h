#include "PointType.h"
#include "memory.h"

class ConnectType
{
private:
		PointType		conPoint; //location linked Vertex
		int       dirCentre; //direction of center
	int       segIndex; //Index of linked Segment
	
	public:
		void operator = ( ConnectType co );
		PointType getConPoint();
		int getDirCentre();
		int getSegIndex();
};

class VertexType //BranchPointType
{
private:
   		PointType    verPoint;	 //location of vertex
   		ConnectType connected [8];	//linked segment. 
   		int          numSegment;	//number of linked segment 
	
	public:
   		void operator = ( VertexType ve);
   		PointType getPoint();
		ConnectType *getConnected();
		int getNumSeg();
};
