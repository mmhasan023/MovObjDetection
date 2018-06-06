
#include <windows.h>
#include "GenCurFileName.h"

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

// Generation of File Name from sequence number
int CDECL GenCurFileName(int* n,KScString* hString)
{
	char	temp[100];
	static	int		i =0;

	memset(temp, 0, 100);
	
	sprintf(temp, "DataSets\\JuneHeong\\img%03d.BMP", (*n));
	if (hString->GetBuffer())
		hString->Free();

	hString->Alloc(100);
	strcpy((char*)hString->GetBuffer(), temp);

	return TRUE;
}



