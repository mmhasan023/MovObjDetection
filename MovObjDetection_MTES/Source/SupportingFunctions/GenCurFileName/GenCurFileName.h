#include <windows.h>
#include "KScScalarImage2dUint8.h"
#include "KScString.h"
#include <stdlib.h>
#include <time.h>
#include "KSClass.h" 
// DLL 외부에서 호출하기 위해서 필요한 부분 ...
extern "C" __declspec( dllexport )
int CDECL GenCurFileName(int* par0, KScString* par1);

// 2 개 이상의 함수를 export 할 경우, 아래에 수동으로 삽입합니다.


