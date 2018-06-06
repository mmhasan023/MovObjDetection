#include "KSClass.h"
#include "EdgeList.h"
#include "canny.h"
#include <math.h>
//#include "EdgeSubtract.h"

const double Q_LEVEL_T[7] = {0.5, 0.75, 0.85, 0.90, 0.93, 0.95, 0.96};


class ReferenceGen
{
protected:
//	KScScalarImage2dFloat *m_pcAccHxImg;
//	KScScalarImage2dFloat *m_pcAccHyImg;
	KScScalarImage2dFloat *accGradMImg;
	KScScalarImage2dFloat *accImage;


	KScScalarImage2dUint8 *curImage;
	//KScScalarImage2dInt32 *m_pcLabelImg;

	Canny *m_pcCanny;

//protected:
//	EdgeListType *m_pcReferenceEdge;
//	EdgeListType *m_pcCurrentEdge;

public:
	ReferenceGen();
	ReferenceGen(KScScalarImage2dFloat *accGradM, KScScalarImage2dFloat *accImg, KScScalarImage2dUint8 *srcImg);
	~ReferenceGen();

	//초기화 함수
//	void  Init(KScScalarImage2dFloat *accGradM, KScScalarImage2dUint8 *srcImg);

	void SetCurImg(KScScalarImage2dUint8 *img);

	void AccImg();

	void NormalizationAccImg(int frame);

public:	
	void ExtractRefEdge( EdgeListType *refEdge);

protected:

	void ComputeQtable(int *q_table,float **img);

};
