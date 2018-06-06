
#include "KSClass.h" // KSClass들에 대한 헤더
#include "Canny.h"
#include "ReferenceGen.h"

// DLL 
extern "C" __declspec( dllexport )
int CDECL GenerateReference(int *n,
							KScScalarImage2dUint8* par0, 
							KScScalarImage2dFloat *accArray, 
							KScUserDefineArray *edgeClass);



