#include "ReferenceGen.h"

ReferenceGen::ReferenceGen()
{
//	m_pcAccHxImg     = NULL;
//	m_pcAccHyImg     = NULL;
	accGradMImg  = NULL;
	curImage       = NULL;
	m_pcCanny        = NULL;
	accImage	= NULL;
//	m_pcReferenceEdge= NULL;
//	m_pcCurrentEdge  = NULL;
}

ReferenceGen::ReferenceGen(KScScalarImage2dFloat *accGradM, KScScalarImage2dFloat *accImg,
								  KScScalarImage2dUint8 *srcImg
								  /*Canny                 *canny   ,*/
								  //EdgeListType     *referenceEdge,
								  //EdgeListType       *currentEdge
								  )
{
	accGradMImg  = accGradM    ;
	curImage       = srcImg      ;
	accImage	=accImg;
	//m_pcReferenceEdge=referenceEdge;
	//m_pcCurrentEdge  =currentEdge  ;
	//m_pcAccHxImg->Alloc(m_pcSrcImg->GetXSize(),m_pcSrcImg->GetYSize());
	//m_pcAccHyImg->Alloc(m_pcSrcImg->GetXSize(),m_pcSrcImg->GetYSize());

	//m_pcLabelImg = new KScScalarImage2dInt32(m_pcSrcImg->GetXSize(),m_pcSrcImg->GetYSize());
	m_pcCanny    = new Canny(curImage,float(1.4),(float)0.8,(float)0.9,0);
	//CannyBase cBase(sourceImage, 1., 6, 20, 0);
	//Canny canny(sourceImage, destinationImage,1.,6,20,0);
}

ReferenceGen::~ReferenceGen()
{
	//m_pcAccHxImg->Free();
	//m_pcAccHyImg->Free();
	delete m_pcCanny;
}
/*
void  ReferenceGen::Init        (KScScalarImage2dFloat *accGradM,
								  KScScalarImage2dUint8 *srcImg
								  //EdgeListType     *referenceEdge,
								  //EdgeListType       *currentEdge
								  )
{
	accGradMImg  = accGradM    ;
	curImage       = srcImg      ;
	
	m_pcCanny   = new Canny(curImage,(float)1.,(float)0.55,(float)0.75,0);
}
*/

//ReferenceGen *refGen;

void ReferenceGen::AccImg()
{
	float ** pAccArray = accGradMImg->Get2dArray();
	float **pAccImage =accImage->Get2dArray();
	KScScalarImage2dFloat *tmp;
	tmp = new KScScalarImage2dFloat(curImage->GetXSize(),curImage->GetYSize());
	float **pTmpArray = tmp->Get2dArray();
	KSdUint8 **pInImage = curImage->Get2dArray();	

	int q_table[7];

	//KSdUint8 **curBuffer = curImage->Get2dArray();
	m_pcCanny->SetSrcImg(curImage);
	m_pcCanny->InitImages();
	m_pcCanny->NoiseFiltering();
	m_pcCanny->ComputeXGrad();
	m_pcCanny->ComputeYGrad();
	m_pcCanny->getGrad(pTmpArray);

/*	
	ComputeQtable(q_table,pTmpArray);

	for(register int i=0,j ; i<curImage->GetYSize() ; i++ )
		for( j=0 ; j<curImage->GetXSize() ; j++ )
		{
			if     (pTmpArray[i][j]<=q_table[0]) pAccArray[i][j] += 0;
			else if(pTmpArray[i][j]<=q_table[1]) pAccArray[i][j] += 1;
			else if(pTmpArray[i][j]<=q_table[2]) pAccArray[i][j] += 2;
			else if(pTmpArray[i][j]<=q_table[3]) pAccArray[i][j] += 3;
			else if(pTmpArray[i][j]<=q_table[4]) pAccArray[i][j] += 4;
			else if(pTmpArray[i][j]<=q_table[5]) pAccArray[i][j] += 5;
			else if(pTmpArray[i][j]<=q_table[6]) pAccArray[i][j] += 6;
			else                                 pAccArray[i][j] += 7;

		}
*/

	for(register int i=0,j ; i<curImage->GetYSize() ; i++ )
		for( j=0 ; j<curImage->GetXSize() ; j++ )
		{
			pAccArray[i][j]+=(pTmpArray[i][j]); 
		    pAccImage[i][j]+=pInImage[i][j];
		}

}

//Gradient Magnitude
void ReferenceGen::ComputeQtable(int *q_table,float **img)
{
	register int i,j;
	int tmp[7];
	int size = curImage->GetXSize()*curImage->GetYSize();
	for( i=0 ; i<7 ; i++ )
		tmp[i]=int(double(size)*Q_LEVEL_T[i]);

	//float ** pAccArray = m_pcAccGradMImg->Get2dArray();
	int histo[256];
	int sum  [256]; 

	for( i=0 ; i<256 ; i++ ) histo[i]=sum[i]=0;
	
	for( i=0 ; i<curImage->GetYSize() ; i++ )
		for( j=0 ; j<curImage->GetXSize() ; j++ )
			histo[unsigned char(img[i][j])]++;

	sum[0]=histo[0];
	
	for( i=1,j=0 ; ( (i<256) && (j<7) ) ; i++ ) 
	{
		sum[i]=histo[i]+sum[i-1];
		if( sum[i] >= tmp[j] ){ 
			q_table[j] =i;
			j++;
		}
	}
}

void ReferenceGen::NormalizationAccImg(int frame)
{
	register int i,j;
	float ** pAccArray = accGradMImg->Get2dArray();
	float ** pAccImage = accImage->Get2dArray();


/*	float scaleFactor = 255./(7.*(float)frame);
	for ( i=0 ; i<curImage->GetYSize() ; i++ )
		for( j=0 ; j<curImage->GetXSize() ; j++ )
			pAccArray[i][j] *= scaleFactor; 
	//////////////////////////////////////////////////////////////
*/	

	for ( i=0 ; i<curImage->GetYSize() ; i++ )
		for( j=0 ; j<curImage->GetXSize() ; j++ )
		{
			pAccArray[i][j] /= frame;
			pAccImage[i][j] /= frame;
		}

}

void ReferenceGen::ExtractRefEdge(EdgeListType *refEdge)
{
	float ** pAccArray = accGradMImg->Get2dArray();
	float ** pImageArray = accImage->Get2dArray();
	//Do_Canny()를 세분화한 함수.
	//void InitImages();
	//void NoiseFiltering();
	//void ComputeXGrad();
	//void ComputeYGrad();
	//void getGrad(float** img);함수 사용
	//void ThinImage();
	//void EdgeExtraction(EdgeListType *);
	/*
	m_pcCanny->InitImages();
	m_pcCanny->NoiseFiltering();
	m_pcCanny->ComputeXGrad();
	m_pcCanny->ComputeYGrad();
	m_pcCanny->ThinImage();
	m_pcCanny->EdgeExtraction(refEdge);
	*/

//	refEdge->SetOption(1);
	m_pcCanny->setGrad(pAccArray);
	m_pcCanny->ThinImage();
	m_pcCanny->EdgeExtraction(refEdge);
	//m_pcCanny->EdgeColorShape(refEdge);

}

void ReferenceGen::SetCurImg(KScScalarImage2dUint8 *img)
{
	curImage = img;
}
