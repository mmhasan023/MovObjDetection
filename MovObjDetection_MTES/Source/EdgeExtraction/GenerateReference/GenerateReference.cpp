
#include <windows.h>
#include "GenerateReference.h"

// DLL을 만들기 위한 부분
BOOL WINAPI DllMain (HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
		case DLL_PROCESS_ATTACH: break;
		case DLL_THREAD_ATTACH:  break;
		case DLL_THREAD_DETACH:  break;
		case DLL_PROCESS_DETACH: break;
	}

	return TRUE;
}

int frameNumber = 0;
KScScalarImage2dFloat *accArray;
KScScalarImage2dFloat *accImage;
//ReferenceGen *refGen;



float firstDerivative(float x , float y)
{
return fabs(x-y);
}

float secondDerivative(float x1, float x2, float x3, float x4)
{
return fabs(firstDerivative(x1,x2)-firstDerivative(x3,x4));
}

float curvature(PointType Pt1,PointType Pt2,PointType Pt3,PointType Pt4,PointType Pt5,PointType Pt6,PointType Pt7)
{
float x_y__=firstDerivative(Pt3.GetX(),Pt5.GetX())*secondDerivative(Pt1.GetY(),Pt2.GetY(),Pt6.GetY(),Pt7.GetY());
float y_x__=firstDerivative(Pt3.GetY(),Pt5.GetY())*secondDerivative(Pt1.GetX(),Pt2.GetY(),Pt6.GetX(),Pt7.GetY());
float x_2=firstDerivative(Pt3.GetX(),Pt5.GetX())*firstDerivative(Pt3.GetX(),Pt5.GetX());
float y_2=firstDerivative(Pt3.GetY(),Pt5.GetY())*firstDerivative(Pt3.GetY(),Pt5.GetY());

if(powf(x_2+y_2,1.5)>0.01)
return fabs((float)(x_y__-y_x__)/powf(x_2+y_2,1.5));
else
return 0;
}




// Main Method
int CDECL GenerateReference(int *maxFrame,
							KScScalarImage2dUint8* inImage,
							KScScalarImage2dFloat *outImage,
							KScUserDefineArray *edgeClass)
{
		// Check the type of input buffer with the type of given parameter
	if (inImage->GetId() != KS_SCALAR_IMAGE_2D_UINT8)
	{
		::MessageBox(NULL, "Input buffer type not suitable.",
			"Generate Histogram", MB_OK);
        return FALSE;
	}

    // Check the type of output buffer with the type of given parameter 
    if (outImage->GetId() != KS_SCALAR_IMAGE_2D_FLOAT)
	{
		::MessageBox(NULL, "Output buffer type not suitable.",
			"Generate Histogram", MB_OK);
        return FALSE;
	}
    
	//initialize dx and dy by the number column and row of input image respectively
    int dx = inImage->GetMainXSize();
    int dy = inImage->GetMainYSize();
    
	if (!dx || !dy)  // //Check whether anyone is zero
	{
		::MessageBox(NULL, "Input buffer not allocated.", "Generate Histogram", MB_OK);
        return FALSE;
	}
	
	
	// initialize input and output buffer pointer by value of input and output image respectively
	KSdUint8 **pInImage = inImage->Get2dArray();	
//	float **pOutImage = outImage->Get2dArray();
//	outImage->InitTo(0);

	//canny   = new Canny(inImage,(float)1.,(float)0.7,(float)0.82, 0);
	


	//canny->Do_canny(inEdge,1);

	EdgeListType *refEdge; 
	ReferenceGen *refGen;
	
	
	if(frameNumber == 0){
		accArray = new KScScalarImage2dFloat(dx, dy);
		accImage =new KScScalarImage2dFloat(dx, dy);
		accArray->InitTo(0);
		accImage->InitTo(0);


		//refGen = new ReferenceGen(accArray, inImage);
	}
	
	refGen = new ReferenceGen(accArray, accImage, inImage);

	refGen->SetCurImg(inImage);
	
	refGen->AccImg();
	
	if(frameNumber == (*maxFrame))
	{
		edgeClass->pData->Free();
		edgeClass->pData->Alloc( sizeof (class EdgeListType) );

		refEdge = new EdgeListType();
		refEdge = (class EdgeListType *)edgeClass->pData->pBuffer;
		//_edges->SetImgBuf(dx,dy);

		refGen->NormalizationAccImg(frameNumber + 1);
		outImage->Free();		
		//allocate buffer for destination image with same size of input
		if (outImage->Alloc(dx, dy))
		{
			::MessageBox(NULL, "Fail to allocate output buffer.",
			"Thresholding", MB_OK);
			return FALSE;
		}
	refGen->ExtractRefEdge(refEdge);
	
	float **pOutImage = outImage->Get2dArray();
	float **accArr=accArray->Get2dArray();
	float **accImg=accImage->Get2dArray();
	for(int i=1;i<dx-1;i++)
	{
		for(int j=1 ; j<dy-1 ; j++)
		{
			pOutImage[j][i] = accArr[j][i];
	//	pOutImage[j][i] = accImg[j][i];
		}
	}
	/*//////////////////
	
//	EdgeSegmentType *tmpEdge;
//	tmpEdge = new EdgeSegmentType;
	PointType p;
	list<EdgeSegmentType>::iterator curEdge;
	for(curEdge=refEdge->ListBegin();curEdge!=refEdge->ListEnd();curEdge++)
	{
//		*tmpEdge = (*curEdge);
		int size = (*curEdge).GetSize();
		for(int i=0 ; i<size ; i++)
		{
			p=(*curEdge).GetPoint(i);
			pOutImage[p.GetY()][p.GetX()] = KSdUint8((*curEdge).GetGrad(i));
			//pOutImage[p.GetY()][p.GetX()] = 255;

		}
	}
	/////////////////////*/




/////////////////Newly adder for side color and shape

	PointType p;
	int xpos,ypos, size,i;
	float tangentdirX=0,tangentdirY=0;
	float x_inc, y_inc;
	int count=1, s1_color=0, s2_color;
	float s1_avg=0, s2_avg=0, varside1=0, varside2=0;

	list<EdgeSegmentType>::iterator curEdge;
	for(curEdge=refEdge->ListBegin();curEdge!=refEdge->ListEnd();curEdge++)
	{
		size = (*curEdge).GetSize();
		count=1;
		s1_color=0;
		s2_color=0;
		for(i=3 ; i<size-3 ; i++)
		{
			p=(*curEdge).GetPoint(i);
			/*
			tangentdirX=secondDerivative((*curEdge).GetPoint(i-2).GetX(),(*curEdge).GetPoint(i-1).GetX(),(*curEdge).GetPoint(i+1).GetX(),(*curEdge).GetPoint(i+2).GetX());
			x_inc=cos(atan(tangentdirX));
			tangentdirY=secondDerivative((*curEdge).GetPoint(i-2).GetY(),(*curEdge).GetPoint(i-1).GetY(),(*curEdge).GetPoint(i+1).GetY(),(*curEdge).GetPoint(i+1).GetY());
			y_inc=cos(atan(tangentdirY));
			
			if(x_inc>0) xpos=1;
			else xpos=-1;
			if(y_inc>0) ypos=1;
			else ypos=-1;*/

				xpos=0;ypos=0;
				x_inc=2*(*curEdge).GetPoint(i).GetX()-(*curEdge).GetPoint(i-2).GetX()-(*curEdge).GetPoint(i+2).GetX();
				y_inc=2*(*curEdge).GetPoint(i).GetY()-(*curEdge).GetPoint(i-2).GetY()-(*curEdge).GetPoint(i+2).GetY();
				if(x_inc>0) xpos=2;
				else if (x_inc<0) xpos=-2;
				if(y_inc>0) ypos=2;
				else if (y_inc<0)ypos=-2;
				if(xpos==0&&ypos==0){ xpos=2; ypos=2;}
				//pOutImage[p.GetY()+ypos][p.GetX()+xpos]= 255;
				//pOutImage[p.GetY()-ypos][p.GetX()-xpos]= 255;

			s1_color+=accImg[p.GetY()+ypos][p.GetX()+xpos];
			s2_color+=accImg[p.GetY()-ypos][p.GetX()-xpos];
			
			//if(count%10==0)
			if(i==size-4)
			{
				s1_avg=s1_color/count;
				s2_avg=s2_color/count;
				varside1=0;
				varside2=0;
				for(i=3 ; i<size-3 ; i++)
					{
						p=(*curEdge).GetPoint(i);

						/*
						tangentdirX=secondDerivative((*curEdge).GetPoint(i-2).GetX(),(*curEdge).GetPoint(i-1).GetX(),(*curEdge).GetPoint(i+1).GetX(),(*curEdge).GetPoint(i+2).GetX());
						x_inc=cos(atan(tangentdirX));
						tangentdirY=secondDerivative((*curEdge).GetPoint(i-2).GetY(),(*curEdge).GetPoint(i-1).GetY(),(*curEdge).GetPoint(i+1).GetY(),(*curEdge).GetPoint(i+2).GetY());
						y_inc=cos(atan(tangentdirY));
			
						if(x_inc>0) xpos=1;
						else xpos=-1;
						if(y_inc>0) ypos=1;
						else ypos=-1;
						*/
						xpos=0;ypos=0;
						x_inc=2*(*curEdge).GetPoint(i).GetX()-(*curEdge).GetPoint(i-2).GetX()-(*curEdge).GetPoint(i+2).GetX();
						y_inc=2*(*curEdge).GetPoint(i).GetY()-(*curEdge).GetPoint(i-2).GetY()-(*curEdge).GetPoint(i+2).GetY();
						if(x_inc>0) xpos=2;
						else if (x_inc<0) xpos=-2;
						if(y_inc>0) ypos=2;
						else if (y_inc<0)ypos=-2;
						if(xpos==0&&ypos==0){ xpos=2; ypos=2;}
						//pOutImage[p.GetY()+ypos][p.GetX()+xpos]= s1_avg;
						//pOutImage[p.GetY()-ypos][p.GetX()-xpos]= s2_avg;

						//pOutImage[p.GetY()][p.GetX()] = 255;
						varside1+=fabs(s1_avg-accImg[p.GetY()+ypos][p.GetX()+xpos]);
						varside2+=fabs(s2_avg-accImg[p.GetY()-ypos][p.GetX()-xpos]);
					}
						i=size-4;
						varside1=varside1/count;
						varside2=varside2/count;

			

				(*curEdge).setSideColor(1,s1_avg);
				(*curEdge).setSideColor(2,s2_avg);
				(*curEdge).setSideColorVar(1,varside1);
				(*curEdge).setSideColorVar(2,varside2);


				(*curEdge).setEdgeShape((((*curEdge).GetPoint(1).GetX()-(*curEdge).GetPoint(size-1).GetX())*((*curEdge).GetPoint(1).GetX()-(*curEdge).GetPoint(size-1).GetX()))+ (((*curEdge).GetPoint(1).GetY()-(*curEdge).GetPoint(size-1).GetY())*((*curEdge).GetPoint(1).GetY()-(*curEdge).GetPoint(size-1).GetY())));

				
				p=(*curEdge).GetPoint(size/2);
				xpos=0;ypos=0;
				
				/*
				tangentdirX=secondDerivative((*curEdge).GetPoint(size/2-2).GetX(),(*curEdge).GetPoint(size/2-1).GetX(),(*curEdge).GetPoint(size/2+1).GetX(),(*curEdge).GetPoint(size/2+2).GetX());
				x_inc=cos(atan(tangentdirX));
				tangentdirY=secondDerivative((*curEdge).GetPoint(size/2-2).GetY(),(*curEdge).GetPoint(size/2-1).GetY(),(*curEdge).GetPoint(size/2+1).GetY(),(*curEdge).GetPoint(size/2+2).GetY());
				y_inc=cos(atan(tangentdirY));
				
				if(x_inc>0) xpos=1;
				else xpos=-1;
				if(y_inc>0) ypos=1;
				else ypos=-1;
				*/

				//pOutImage[p.GetY()+ypos][p.GetX()+xpos]= s1_avg;
				//pOutImage[p.GetY()-ypos][p.GetX()-xpos]= s2_avg;


				xpos=0;ypos=0;
				x_inc=2*(*curEdge).GetPoint(size/2).GetX()-(*curEdge).GetPoint(size/2-2).GetX()-(*curEdge).GetPoint(size/2+2).GetX();
				y_inc=2*(*curEdge).GetPoint(size/2).GetY()-(*curEdge).GetPoint(size/2-2).GetY()-(*curEdge).GetPoint(size/2+2).GetY();
				if(x_inc>0) xpos=2;
				else if (x_inc<0) xpos=-2;
				if(y_inc>0) ypos=2;
				else if (y_inc<0)ypos=-2;
				if(xpos==0&&ypos==0){ xpos=2; ypos=2;}
				pOutImage[p.GetY()+ypos][p.GetX()+xpos]= s1_avg;
				pOutImage[p.GetY()-ypos][p.GetX()-xpos]= s2_avg;
				count=0;
				s1_color=0;
				s2_color=0;
				varside1=0;
				varside2=0;
				
			}
			count++;
		}
	}



	}
	frameNumber++;
	delete refGen;
	return TRUE;
}



