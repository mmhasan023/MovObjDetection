#include "EdgeListType.h"

class TorsoModel
{
	public:
		TorsoModel(int, int, float, int);
		virtual ~TorsoModel();

protected:	
		//information about edge segment
		EdgeSegmentType *VLtorsoEdge;	
		EdgeSegmentType *VRtorsoEdge;	
		EdgeSegmentType *UtorsoEdge;
		EdgeSegmentType *CVLtorsoEdge;	
		EdgeSegmentType *CVRtorsoEdge;	
		EdgeSegmentType *CUtorsoEdge;
	//	EdgeSegmentType *LtorsoEdge;
		PointType *pl,*pr,*pu, p_temp;

		float scale;
		int Vsize, Hsize;
		int Rx,Ry,theta;
		int width;
		int URArmX,URArmY;
		int ULArmX,ULArmY;
		int HX,HY;
		int CRx, CRy;

		float Cscale;
		int CVsize, CHsize;
		int Ctheta;
		int Cwidth;
		int CURArmX,CURArmY;
		int CULArmX,CULArmY;
		int CHX,CHY;

	public:
		void TorsoModel::displayTorsoModel(KSdUint8 ** pOutputImage, int dx,int dy);
		void TorsoModel::rotateTorsoModel(int rx, int ry, int th);
		void TorsoModel::rotateEdge(int rx, int ry,  EdgeSegmentType *edge,EdgeSegmentType *edge1, int th);
		void TorsoModel::rotatePoint(int rx, int ry, int&, int&, int,int,int th);
		void TorsoModel::getTorsoEdgeList(EdgeListType* modelEdgeList);
		void TorsoModel::getRightHandReferencePoint(int &, int&);
		void TorsoModel::getLeftHandReferencePoint(int &x, int&y);
		void TorsoModel::getHeadReferencePoint(int &x, int&y);
	
		//// member function---------------------------------------------
		





};


class UpperArmModel
{
	public:
		UpperArmModel(int, int, float, int);
		virtual ~UpperArmModel();

protected:	
		//information about edge segment
		EdgeSegmentType *VLtorsoEdge;	
		EdgeSegmentType *VRtorsoEdge;	
		EdgeSegmentType *CVLtorsoEdge;	
		EdgeSegmentType *CVRtorsoEdge;	
	
		PointType *pl,*pr,p_temp;

		float scale;
		int Vsize;
		int Rx,Ry,theta,CRx,CRy;
		int width;
		int LArmRX,LArmRY;
		
		float Cscale;
		int CVsize;
		int Ctheta;
		int Cwidth;
		int CLArmRX,CLArmRY;
				

	public:
		void UpperArmModel::displayUpperArmModel(KSdUint8 ** pOutputImage, int dx,int dy);
		void UpperArmModel::rotateUpperArmModel(int rx, int ry, int th);
		void UpperArmModel::rotateEdge(int rx, int ry,  EdgeSegmentType *edge,EdgeSegmentType *edge1, int th);
		void UpperArmModel::rotatePoint(int rx, int ry, int&, int&, int,int,int th);
		void UpperArmModel::getUpperArmEdgeList(EdgeListType* modelEdgeList);
		void UpperArmModel::getLowerArmReferencePoint(int &, int&);
		
		//// member function---------------------------------------------
		
};



class LowerArmModel
{
	public:
		LowerArmModel(int, int, float, int);
		virtual ~LowerArmModel();

protected:	
		//information about edge segment
		EdgeSegmentType *VLtorsoEdge;	
		EdgeSegmentType *VRtorsoEdge;	
		EdgeSegmentType *CVLtorsoEdge;	
		EdgeSegmentType *CVRtorsoEdge;	

		EdgeSegmentType *HtorsoEdge;	
		EdgeSegmentType *CHtorsoEdge;	
	
		PointType *pl,*pr,p_temp,*pu;

		float scale;
		int Vsize;
		int Rx,Ry,theta,CRx,CRy;
		int width;
		int LArmRX,LArmRY;
		
		float Cscale;
		int CVsize;
		int Ctheta;
		int Cwidth;
		int CLArmRX,CLArmRY;
				

	public:
		void LowerArmModel::displayLowerArmModel(KSdUint8 ** pOutputImage, int dx,int dy);
		void LowerArmModel::rotateLowerArmModel(int rx, int ry, int th);
		void LowerArmModel::rotateEdge(int rx, int ry,  EdgeSegmentType *edge,EdgeSegmentType *edge1, int th);
		void LowerArmModel::rotatePoint(int rx, int ry, int&, int&, int,int,int th);
		void LowerArmModel::getLowerArmEdgeList(EdgeListType* modelEdgeList);
		void LowerArmModel::getLowerArmEndPoint(int &, int&);
		
		//// member function---------------------------------------------
		
};


class HeadModel
{
	public:
		HeadModel(int, int, float, int);
		virtual ~HeadModel();

protected:	
		//information about edge segment
		EdgeSegmentType *VLtorsoEdge;	
		EdgeSegmentType *VRtorsoEdge;	
		EdgeSegmentType *CVLtorsoEdge;	
		EdgeSegmentType *CVRtorsoEdge;	

		EdgeSegmentType *HLtorsoEdge;	
		EdgeSegmentType *HRtorsoEdge;	
		EdgeSegmentType *CHLtorsoEdge;	
		EdgeSegmentType *CHRtorsoEdge;	

	
		PointType *pl,*pr,p_temp,*pu,*pd;

		float scale;
		int Vsize;
		int Hsize;
		int Rx,Ry,theta,CRx,CRy;
			
		float Cscale;
		int CVsize;
		int CHsize;
		int Ctheta;
		int Cwidth;
				

	public:
		void HeadModel::displayHeadModel(KSdUint8 ** pOutputImage, int dx,int dy);
		void HeadModel::rotateHeadModel(int rx, int ry, int th);
		void HeadModel::rotateEdge(int rx, int ry,  EdgeSegmentType *edge,EdgeSegmentType *edge1, int th);
		void HeadModel::rotatePoint(int rx, int ry, int&, int&, int,int,int th);
		void HeadModel::getHeadEdgeList(EdgeListType* modelEdgeList);		
		//// member function---------------------------------------------
		
};


