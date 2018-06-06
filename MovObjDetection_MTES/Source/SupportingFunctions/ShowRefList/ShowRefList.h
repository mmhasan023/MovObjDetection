#include"EdgeListType.h"

#include "KSClass.h" // KSClass들에 대한 헤더

// DLL 외부에서 호출하기 위해서 필요한 부분 ...
extern "C" __declspec( dllexport )
int CDECL ShowRefList(KScScalarImage2dFloat* par0, 
					  KScUserDefineArray* par1, 
					  KScScalarImage2dUint8* par2);


