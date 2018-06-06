#include "Vertex.h"

void ConnectType::operator = ( ConnectType co )
{
	conPoint = co.conPoint; 
	dirCentre = co.dirCentre;
	segIndex = co.segIndex;
}
	
PointType ConnectType::getConPoint()
{
	return conPoint;
}

int ConnectType::getDirCentre()
{
	return dirCentre;
}

int ConnectType::getSegIndex()
{
	return segIndex;
}

void VertexType::operator = ( VertexType ve )
{
	verPoint = ve.verPoint;	 //location of vertex
   	memcpy(connected, ve.connected,sizeof(ConnectType)*8);	//linked segment. 
   	numSegment = ve.numSegment;
}

PointType VertexType::getPoint(){
	return verPoint;
}
		
ConnectType* VertexType::getConnected(){
	return connected;
}

int VertexType::getNumSeg()
{
	return numSegment;
}

