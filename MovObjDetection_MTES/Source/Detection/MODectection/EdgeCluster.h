#include "EdgeSegmentType.h"
//#include "Vertex.h"
//#include "List"
//#include "KSClass.h"
//using namespace std;

class EdgeClusterType  
{
	public:
		EdgeClusterType();
		virtual ~EdgeClusterType();

protected:	
		//information about edge segment
		list<EdgeSegmentType> *edgeList;	// List of edge segment
		int numSegment;
		int clusterID;
		float centerX;
		float centerY;
		float SumX;
		float SumY;
		int maxx;
		int maxy;
		int minx;
		int miny;
		int oldx;
		int oldy;
		float olddx;
		float olddy;
		float movX;
		float movY;
		float varX;
		float varY;
		float width;
		float height;
		float learnRate;

		


		
		// Pointer to current segment                      	
		//EdgeSegmentType *curSegment;
	
	public:
	
		float KalmanPar[4][4];
		//// member function---------------------------------------------
		void  EdgeListClear();	
		void  AddNewSegment(EdgeSegmentType edge);
		//void  AddNewSegment(EdgeSegmentType *edge);
		list<EdgeSegmentType>::iterator ListBegin();
		list<EdgeSegmentType>::iterator ListEnd();
	//	int DeleteSegment(int index);
		list<EdgeSegmentType>::iterator DeleteSegment(list<EdgeSegmentType>::iterator segment);
		int  getClusterIndex();
		void setClusterIndex(int ID);
		void getClusterCenter(float *X, float *Y);
		void setClusterCenter(float X, float Y);
		void setSum(float X, float Y);
		void getSum(float *X, float *Y);
		void setMotion(float X, float Y);
		void getMotion(float *X, float *Y);
		void setVariance(float X, float Y);
		void getVariance(float *X, float *Y);
		void getWidthHeight(float *X, float *Y);
		void setWidthHeight(float X, float Y);
		int getTotalEdge();
		void setBoundaryParam(int max, int may, int mix, int miy);
		void getBoundaryParam(int *max, int *may, int *mix, int *miy);
		void setOldMotion(int x, int y, float dx, float dy);
		void getOldMotion(int &x, int &y, float &dx, float &dy);


		




		
};


