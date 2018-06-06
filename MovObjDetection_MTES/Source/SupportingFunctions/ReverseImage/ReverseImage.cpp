//////////////////////////////////////////////////////////////////////////
//
// 이 화일은 함수 등록시 자동으로 생성된 프로젝트입니다.
//
// 사용자가 임으로 만든 경우, Project의 Setting...의 설정이 필요하므로,
//
// 이는 도움말의 '함수 작성법'을 참조 바랍니다.
//
//////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "ReverseImage.h"

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
int CDECL ReverseImage(KScScalarImage2dUint8* inputImage,KScScalarImage2dUint8* outputImage)
{
	// thresholdValue represents the value that would be used for thresholding
    
	// Check the type of input buffer with the type of given parameter
	if (inputImage->GetId() != KS_SCALAR_IMAGE_2D_UINT8)
	{
		::MessageBox(NULL, "Input buffer type not suitable.",
			"Thresholding", MB_OK);
        return FALSE;
	}

    
	// Check the type of output buffer with the type of given parameter 
    if (outputImage->GetId() != KS_SCALAR_IMAGE_2D_UINT8)
	{
		::MessageBox(NULL, "Output buffer type not suitable.",
			"iLowPass5x5", MB_OK);
        return FALSE;
	}

    //initialize dx and dy by the number column and row of input image respectively
    int dx = inputImage->GetMainXSize();
    int dy = inputImage->GetMainYSize();
    
	if (!dx || !dy)  // //Check whether anyone is zero
	{
		::MessageBox(NULL, "Input buffer not allocated.", "ThresholdUint8", MB_OK);
        return FALSE;
	}

	// Check whether source and destination buffer are of same size or not
	if (dx != outputImage->GetXSize() ||
		dy != outputImage->GetYSize())
	{
		// if not then free destination buffer
		outputImage->Free();
		//allocate buffer for destination image with same size of input
		if (outputImage->Alloc(dx, dy))
		{
			::MessageBox(NULL, "Fail to allocate output buffer.",
				"Thresholding", MB_OK);
			return FALSE;
		}
	}
	
	else
	{
		if (inputImage != outputImage)
			outputImage->InitTo(0);
	}

	// initialize input and output buffer pointer by value of input and output image respectively
	KSdUint8 **pSrcArray=inputImage->Get2dArray();	
	KSdUint8 **pDstArray=outputImage->Get2dArray();	

	int i, j;
	
	
	for(i=0; i<dy; i++){
		for(j=0; j<dx; j++){
			pDstArray[i][j]=255-pSrcArray[i][j]; //assign with the reverse gralevel
			
		}
	}
	
	// Add comment to the thresholded image
	char comment[1024];
	sprintf(comment, "After Reversing %s",
		outputImage->GetName());
	outputImage->AppendComment(comment);
	
	return TRUE;
}

// 2 개 이상의 함수를 정의할 경우, 아래에 수동으로 삽입합니다.


