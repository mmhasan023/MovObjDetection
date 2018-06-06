
#include <windows.h>
#include "ShowRefList.h"

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
int CDECL ShowRefList(KScScalarImage2dFloat* sourceImage,KScUserDefineArray* edgeClass,
					  KScScalarImage2dUint8* outputImage)
{

		if (sourceImage->GetId() != KS_SCALAR_IMAGE_2D_FLOAT)
	{
		::MessageBox(NULL, "Input buffer type not suitable.",
			"iDisplayEdgeListUint8", MB_OK);
        return FALSE;
	}


    // Assinging the size of input image
    int dx = sourceImage->GetMainXSize();
    int dy = sourceImage->GetMainYSize();

    if (!dx || !dy)
	{
		::MessageBox(NULL, "Input buffer not allocated.",
			"iDisplayEdgeListUint8", MB_OK);        return FALSE;
	}

	
	if (outputImage->GetId() != KS_SCALAR_IMAGE_2D_UINT8)
	{
		::MessageBox(NULL, "Output buffer type not suitable.",
			"iDisplayEdgeListUint8", MB_OK);
        return FALSE;
	}

	outputImage->Free();
	if (outputImage->Alloc(dx, dy))
	{
		::MessageBox(NULL, "Fail to allocate output buffer.",
			"iDisplayEdgeListUint8", MB_OK);
		return FALSE;
	}
	
	outputImage->InitTo(0);

	class EdgeListType *refEdge;
	refEdge = (class EdgeListType*)edgeClass->pData->pBuffer;
	KSdUint8 **pOutputImage = outputImage->Get2dArray();
	for(int i=0,j ; i<dy ; i++ )
		for( j=0 ; j<dx ; j++ )
			pOutputImage[i][j] = 0;
		
	
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
	return TRUE;

}



