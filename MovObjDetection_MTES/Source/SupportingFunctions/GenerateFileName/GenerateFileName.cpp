//////////////////////////////////////////////////////////////////////////
//
// �� ȭ���� �Լ� ��Ͻ� �ڵ����� ������ ������Ʈ�Դϴ�.
//
// ����ڰ� ������ ���� ���, Project�� Setting...�� ������ �ʿ��ϹǷ�,
//
// �̴� ������ '�Լ� �ۼ���'�� ���� �ٶ��ϴ�.
//
//////////////////////////////////////////////////////////////////////////

//#include <windows.h>
#include "GenerateFileName.h"

// DLL�� ����� ���� �κ�
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

// �Լ��� ���� �κ�
int CDECL GenerateFileName(int* n,KScString* hString)
{
	char	temp[100];
	static	int		i =0;

	memset(temp, 0, 100);
//sprintf(temp, "Track\\Img (%d).BMP", (*n));//Img01(0).jpg
	//sprintf(temp, "Image\\outGiTae\\img%03d.BMP", (*n));

	//sprintf(temp, "DataSets\\img%03d.BMP", (*n)); //good data sets

//sprintf(temp, "DataSet1\\%0004d.jpg", (*n)+1); //medium dataset

//sprintf(temp, "\Det3\\img001 (%d).BMP", (*n)+1); //medium dataset
	
//sprintf(temp, "DataSet3\\%04d.jpg", (*n)+1);//Img01(0).jpg good dataset

//sprintf(temp, "Image\\ImageSequences\\ImageSequences\\img%04d.TIF", (*n));//Img01(0).jpg good dataset

//sprintf(temp, "Behaviour2\\Gestures_2_(C1)%04d.jpeg", (*n));//Img01(0).jpg good dataset
	
	
sprintf(temp, "HandMovement\\HandMovement%04d.jpeg", (*n));//Img01(0).jpg good dataset

//sprintf(temp, "3\\S5-T1-G.%05d.jpeg", (*n));

//	sprintf(temp, "NightCamera\\%04d.jpg", (*n)+1);
//	sprintf(temp, "PeopleWalk\\frame%04d.jpg", (*n)+1);

//sprintf(temp, "Test2\\ob%d.bmp", (*n));//Img01(0).jpg good dataset


//sprintf(temp, "C:\\Program Files\\MG\\MTES\\DataSet2\\Img (%d).jpg", (*n));//Img01(0).jpg good dataset



//	sprintf(temp, "Illumination Change\\%00000d.jpeg", (*n)+10000);//Img01(0).jpg

//sprintf(temp, "\CameraMovement\\img (%d).BMP", (*n));
//sprintf(temp, "\Sequence3\\img%04d.BMP", (*n));
	


//	sprintf(temp, "Det3\\img (%d).BMP", (*n)+1);  //good data set

	
	
																//sprintf(temp, "Det4\\%04d.jpg", (*n)+711);  //good data set













//	sprintf(temp,"Batabase\\sif_ras_edited_gray\\ed00%d.jpg",(*n)+1);

//	sprintf(temp,"Batabase\\Hall Monitor\\hall_cif_%d.bmp",(*n)+1);

	if (hString->GetBuffer())
		hString->Free();

	hString->Alloc(100);
	strcpy((char*)hString->GetBuffer(), temp);

	return TRUE;
}

// 2 �� �̻��� �Լ��� ������ ���, �Ʒ��� �������� �����մϴ�.


