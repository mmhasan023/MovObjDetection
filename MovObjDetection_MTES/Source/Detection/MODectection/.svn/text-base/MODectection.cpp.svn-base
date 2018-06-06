
#include <windows.h>
#include "MODectection.h"


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


// 함수의 정의 부분
int CDECL MODectection(KScScalarImage2dUint8* curImage,
					   KScScalarImage2dFloat* accGradImage,
					   KScUserDefineArray* edgeClass,
					   KScScalarImage2dUint8* outputImage)
{

	int dx,dy;

	if (accGradImage->GetId() != KS_SCALAR_IMAGE_2D_FLOAT)
	{
		::MessageBox(NULL, "Input buffer type not suitable.",
			"iDisplayEdgeListUint8", MB_OK);
        return FALSE;
	}
	else{


    // Assinging the size of input image
    dx = accGradImage->GetMainXSize();
    dy = accGradImage->GetMainYSize();

    if (!dx || !dy)
	{
		::MessageBox(NULL, "Input buffer1 not allocated.",
			"iDisplayEdgeListUint8", MB_OK);        return FALSE;
	}
	}

	
	if (outputImage->GetId() != KS_SCALAR_IMAGE_2D_UINT8)
	{
		::MessageBox(NULL, "Output buffer2 type not suitable.",
			"iDisplayEdgeListUint8", MB_OK);
        return FALSE;
	}
	else
	{
	outputImage->Free();
	if (outputImage->Alloc(dx, dy))
	{
		::MessageBox(NULL, "Fail to allocate output buffer.",
			"iDisplayEdgeListUint8", MB_OK);
		return FALSE;
	}
	}
	
	outputImage->InitTo(0);

	class EdgeListType *refEdge;
	refEdge = (class EdgeListType*)edgeClass->pData->pBuffer;


	KSdUint8 **pOutputImage = outputImage->Get2dArray();
	for(int i=0,j ; i<dy ; i++ )
		for( j=0 ; j<dx ; j++ )
			pOutputImage[i][j] = 0;
		
	CEdgeCD *edgeCD = new CEdgeCD(curImage);
	edgeCD->DoEdgeCD(refEdge, outputImage);


	
	
	/*
	PointType p;
	list<EdgeSegmentType>::iterator curEdge;
	for(curEdge=refEdge->ListBegin();curEdge!=refEdge->ListEnd();curEdge++)
	{
//		*tmpEdge = (*curEdge);
		int size = (*curEdge).GetSize();
		for(int i=0 ; i<size ; i++)
		{
			p=(*curEdge).GetPoint(i);
			//pOutImage[p.GetY()][p.GetX()] =KSdUint8((*curEdge).GetGrad(i));
			pOutputImage[p.GetY()][p.GetX()] = 255;

		}
	}
	*/
	
	return TRUE;
}

// 2 개 이상의 함수를 정의할 경우, 아래에 수동으로 삽입합니다.


