#include "EdgeCluster.h"
//#include "EdgeSegmentType.h"
//#include "List"
#include "KSClass.h"
#include "Kalman.h"

//using namespace std;

void showClusterWindow(EdgeClusterType *clusterEdgeList, KSdUint8 **pOutputImage);

class EdgeClusterList  
{
	public:
		EdgeClusterList();
		virtual ~EdgeClusterList();

protected:	
		//information about edge segment
		list<EdgeClusterType> *clusterList;	// List of clusters
		int numCluster;
		long CID;
					
		// Pointer to current segment                      	
		//EdgeSegmentType *curSegment;
	
	public:
	
		//// member function---------------------------------------------
		int getTotalClusterNumber();
		void  ClusterListClear();
		void addNewCluster(EdgeSegmentType edge);
		void addNewCluster(EdgeSegmentType *edge);
		void AddNewSegment(EdgeSegmentType seg);
		void AddNewSegment(EdgeSegmentType *edge);
		//void  AddNewSegment(EdgeSegmentType *seg);
		void  AddNewFullCluster(EdgeClusterType cluster);
		void  AddNewFullCluster(EdgeClusterType *newCluster);
			//void  AddNewCluster(EdgeClusterType *cluster);
		list<EdgeClusterType>::iterator ListBegin();
		list<EdgeClusterType>::iterator ListEnd();
	//	int DeleteCluster(int index);
	//	int DeleteSegment(int index);
		
		list<EdgeClusterType>::iterator DeleteCluster(list<EdgeClusterType>::iterator segment);
		void AddSegmentToCluster(EdgeSegmentType edge, EdgeClusterType *ec);
		void deleteSegmentFromCluster(list<EdgeSegmentType>::iterator *edge, EdgeClusterType *ec);
	//	list<EdgeSegmentType>::iterator DeleteSegment(list<EdgeSegmentType>::iterator segment);

};


