//////////////////////////////////////////////////////////////////////////
//
// 이 화일은 함수 등록시 자동으로 생성된 프로젝트입니다.
//
// 사용자가 임으로 만든 경우, Project의 Setting...의 설정이 필요하므로,
//
// 이는 도움말의 '함수 작성법'을 참조 바랍니다.
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// KSClass 라이브러리의 링크
//
//////////////////////////////////////////////////////////////////////////
//
// 헤더 파일과 라이브러리 파일 검색을 위하여
// Visual Studio에서 메뉴 Tools - Options - Directories 탭에서
// Show Directories for: 부분의
//     Include files에는 KSClass의 Include의 디렉토리를
//     Library files에는 KSClass의 Lib       디렉토리를 설정하여야 한다.
//
//////////////////////////////////////////////////////////////////////////
//
// * 주의점
//
// Show Directories for: 부분의
//     Include files에 KSClass의 Include와 Lib 디렉토리를 모두 설정할 경우,
//     링크 에러 발생한다.
//
//////////////////////////////////////////////////////////////////////////

#ifndef iBurnsLineExDLLUint8_h
#define iBurnsLineExDLLUint8_h

#include "KSClass.h" // KSClass들에 대한 헤더
#include "burns.h"
#include "LineSegmentType.h"

# define GROUP_ANGLE 10 //4
# define TOTAL_DEGREE 180 //360

// DLL 외부에서 호출하기 위해서 필요한 부분 ...
int iBurnsLineExDLLUint8(LineSegmentType lineOb[int(TOTAL_DEGREE/GROUP_ANGLE)][100], int lineIndex[int(TOTAL_DEGREE/GROUP_ANGLE)], KScScalarImage2dUint8 *srcImg,
						 KScScalarImage2dUint8 *dstImg,
							//   KScScalarImage2dInt32 *dstImg2,
							   int* threshold,
							   int* option,
							   int* split_on,
							   int* length_on,
							   int* len_thd);

// 각 픽셀에서 Mag와 Phase를 구한다.(테스트용, 나중에 이부분은 입력으로 받는다.)
int SobelMagPhaseForScalar(KScScalarImage2dUint8* srcImg   ,
								 KScScalarImage2dUint8* mag      ,
								 KScScalarImage2dInt32* phase    ,
								 int*                   threshold,
								 int*                   option   );

// 주변 값들과 비교하여 Mag가 작으면 의미가 없다고 보고 제거(효율성을 위해)
int Line_SplitForScalar(KScScalarImage2dUint8 *mag, KScScalarImage2dInt32 *phase, int dirnum);


// (1) 45도씩 8개로 나누어진 마스크와 22.5도 회전한 마스크(?)를 이용하여 Phase에 따라 0~7의 범위로 정의
int PhaseSegmentForScalar(KScScalarImage2dInt32* phase ,
								KScScalarImage2dUint8* seg   ,
								int                    DirNum,
								int                    Start );

// 주변 값들과 비교하여 길이가 짧으면 의미가 없다고 보고 제거(효율성을 위해)
int Length_FilterForScalar(KScScalarImage2dUint8 *seg1, 
						   KScScalarImage2dUint8 *seg2, 
						   KScScalarImage2dUint8 *mag);

// 그냥 마스크
int filter1(KScScalarImage2dUint8 *seg    , 
			int s1, int e1, int s2, int e2, 
			int x, int y, int a           );

// 회전 마스크
int filter2(KScScalarImage2dUint8 *seg    , 
			int s1, int e1, int s2, int e2, 
			int x, int y, int a           );

// (2) 각각의 결과물에 대하여 같은 값(0~7)로 된 부분을 하나의 영역으로 보고 라벨링
int Coloring(KScScalarImage2dUint8* segmap  ,
				   KScScalarImage2dInt32* outmap  ,
				   int*                   cr_label);

//?
int Clear_Boundary_Byte(KScScalarImage2dUint8 *Img, 
						int n, int gray_level     );

//?
int Forward_Color(KScScalarImage2dUint8 *segmap, 
				  KScScalarImage2dInt32 *outmap, 
				  int *crnt_label              );

//?
int Backward_Color(KScScalarImage2dUint8 *segmap, 
				   KScScalarImage2dInt32 *outmap, 
				   int *crnt_label);

//?
int Clear_Boundary_Int(KScScalarImage2dInt32 *Img, 
					   int n                     , 
					   int gray_level            );

// (3) 각각의 결과물에 대하여 정의된 하나의 영역을 하나의 선으로 정의
void Length_Compute(KScScalarImage2dUint8 *phase1, 
					KScScalarImage2dUint8 *phase2, 
					KScScalarImage2dInt32 *label1, 
					KScScalarImage2dInt32 *label2, 
					mstr_type *mstr, int nmbr_rgns);

// 선의 양끝점의 위치를 저장
int Compute_Ends(KScScalarImage2dUint8 *phase, KScScalarImage2dInt32 *label, mstr_type *mstr);

// 선의 길이를 저장
void Compute_Length(mstr_type *mstr, int nmbr_rgns);

// (4) 각각의 결과물들은 겹쳐지는 부분이 존재하므로 그 부분에 대해서 voting을 통해 한쪽으로 정의한다.
int Voting(vmstr_type *mstr, int nmbr_rgns, KScScalarImage2dInt32 *color1, KScScalarImage2dInt32 *color2);

// (5) voting 결과 둘중에 하나로 선택
int Selection(vmstr_type *mstr, int nmbr_rgns, KScScalarImage2dInt32 *color1, KScScalarImage2dInt32 *color2, int *nwnmbr_rgns, int lthresh);

// 의미가 없는 선들을 제거하고 나머지 선들을 가지고 정리하여 재정의
void Copy_Length(vmstr_type *mstr, int nmbr, master_type *master, int nwnmbr);

//? (6) Mag를 가중치로 보고 2차 모멘트 값을 구하여 그 값이 가장 큰 부분이 최종적으로 선택되는 선이다.
// 모멘트 값을 계산???
int Moment_Compute(KScScalarImage2dInt32 *lsr, KScScalarImage2dUint8 *mag, mmstr_type *mmstr, int nwnmbr);

//? 선의 기울기를 계산한다.
void Orient_Compute(mmstr_type *mmstr, int nwnmbr, master_type *master);

// 최종적으로 선택된 선을 정의한다.
void Tdefine_Ends(master_type *master, int num_regions);

//void LineTo(int x0, int y0, int x1, int y1, KScRgbImage2d *ColorOutput, KSdUint8 r, KSdUint8 g, KSdUint8 b)
void LineTo(int x0, int y0, int x1, int y1, KScScalarImage2dUint8 *GrayOutput, KSdUint8 val);

// 2 개 이상의 함수를 export 할 경우, 아래에 수동으로 삽입합니다.

void sortLineByLength(LineSegmentType lineOb[int(TOTAL_DEGREE/GROUP_ANGLE)][100], int lineIndex[int(TOTAL_DEGREE/GROUP_ANGLE)]);
void mergeZeroDistanceParallelLines(LineSegmentType lineOb[int(TOTAL_DEGREE/GROUP_ANGLE)][100],int lineIndex[int(TOTAL_DEGREE/GROUP_ANGLE)]);
#endif