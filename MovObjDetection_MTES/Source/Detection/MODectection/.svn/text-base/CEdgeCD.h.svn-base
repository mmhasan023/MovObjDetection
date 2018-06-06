#include "KSClass.h"
#include"Canny.h"
#include"EdgeListType.h"
#include "curvature.h"
//#include"EdgeSegmentMatch.h"
#include"InitializeCluster.h"
#include"HoughTransform.h"
#include "Model.h"
#include "iBurnsLineExDLLUint8.h"


//Manager class
class CEdgeCD
{
public:
	CEdgeCD(KScScalarImage2dUint8 *srcImg);
	~CEdgeCD();
	

	//Edge Lists
protected:
//	EdgeListType *refEdgeList;
	EdgeListType *curEdgeList;
	EdgeListType *movEdgeList;
//	EdgeListType *TGroupEdgeList;
	EdgeListType *tempEdgeList;
	EdgeListType *modelEdgeList;

protected:
	void CreateEdgeList();
	void DeleteEdgeList();
public:


// Accumulation array
protected:
	KScScalarImage2dFloat *accGradM;
	KScScalarImage2dInt32 *refDistImage;
	KScScalarImage2dInt32 *refLabelImage;
	KScScalarImage2dInt32 *movDistImage;
	KScScalarImage2dInt32 *movLabelImage;
	KScScalarImage2dInt32 *modelDistImage;
	KScScalarImage2dInt32 *modelLabelImage;
	KScScalarImage2dUint8 *maskedInputImage;



		
protected:
	KScScalarImage2dUint8 *curImage;
	
protected:
	void AllocInOutImage ();
	void DeleteInOutImage();
public:
//	KScScalarImage2dInt32* GetLabelImg(){return m_pcLabelImg;};

	//Class Method
public:
	void InitEdgeCD();

	//Current Image Update
	void SetCurImg(KScScalarImage2dUint8 *img);


	//Main Method 
	void DoEdgeCD(EdgeListType* refEdgeList, KScScalarImage2dUint8 *outputImage);
	void findSideColorAndBoundary(EdgeSegmentType *curEdge, KSdUint8 ** inputImage);
	void findCurvaturePointsAndSideIntensityDistribution(EdgeSegmentType *curEdge, KSdUint8 ** inputImage, int dx, int dy);
	void SubtractTemporaryEdge(KSdUint8 ** pOutputImage, int dx, int dy);
	void SubtractEdge(EdgeListType *refEdgeList);
	void BreakMoingEdge();
	void RemoveScattered(int maskSize);

//	EdgeListType* GetRefEdge(){return refEdgeList;};
	EdgeListType* GetCurEdge(){return curEdgeList;};
	EdgeListType* GetMovingEdge(){return movEdgeList;};
//	EdgeListType* GetTGroupEdge(){return TGroupEdgeList;};
	EdgeListType* GetTmpEdge(){return tempEdgeList;};
};

