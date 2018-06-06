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

#include "iBurnsLineExDLLUint8.h"

typedef unsigned char	KSdUint8;


int x_1[] = {1,1,1,0,-1,-1,-1,0,1,1};//?
int y_1[] = {-1,0,1,1,1,0,-1,-1,-1,0};
int x_2[] = {2,2,2,2,1,0,-1,-2,-2,-2,-2,-2,-1,0,1,2,2,2};
int y_2[] = {-1,0,1,2,2,2,2,2,1,0,-1,-2,-2,-2,-2,-2,-1,0};

//#include <atlstr.h>

// DLL을 만들기 위한 부분

// 함수의 정의 부분
int iBurnsLineExDLLUint8(LineSegmentType lineOb[int(TOTAL_DEGREE/GROUP_ANGLE)][100], 
						 int lineIndex[int(TOTAL_DEGREE/GROUP_ANGLE)],
						 KScScalarImage2dUint8 *srcImg,
							   KScScalarImage2dUint8 *dstImg,
							//   KScScalarImage2dInt32 *dstImg2,
							   int* threshold,
							   int* option,
							   int* split_on,
							   int* length_on,
							   int* len_thd)
{
// 전처리 /////////////////////////////////////////////////////////////////////

    // 입력 버퍼 자료 형태 검사.
	if (srcImg->GetId() != KS_SCALAR_IMAGE_2D_UINT8)
	{
		::MessageBox(NULL, "input buffer type not suitable 1.",
			  "iBurnsLineUint8", MB_OK);
			return FALSE;
	}

    // 출력 버퍼 자료 형태 검사.
	if (dstImg->GetId() != KS_SCALAR_IMAGE_2D_UINT8)
	{
		::MessageBox(NULL, "output buffer type not suitable 2.",
			  "iBurnsLineUint8", MB_OK);
			return FALSE;
	}
/*	if (dstImg2->GetId() != KS_SCALAR_IMAGE_2D_INT32)
	{
		::MessageBox(NULL, "output buffer type not suitable 3.",
			  "iBurnsLineUint8", MB_OK);
			return FALSE;
	}*/

    // 입력 버퍼 할당 여부 검사.
	int dx =srcImg->GetMainXSize();
	int dy =srcImg->GetMainYSize();
	if (!dx || !dy)
	{
		::MessageBox(NULL, "Input buffer not allocated4.",
			"iBurnsLineUint8", MB_OK);
        return FALSE;
	}

	// 출력 버퍼 할당 여부 검사.
	dstImg->Free();
	if (dstImg->Alloc(dx, dy))
	{
		::MessageBox(NULL, "Fail to allocate output buffer.",
			"iBurnsLineUint8", MB_OK);
		return FALSE;
	}
/*	dstImg2->Free();
	if (dstImg2->Alloc(dx, dy))
	{
		::MessageBox(NULL, "Fail to allocate output buffer.",
			"iBurnsLineUint8", MB_OK);
		return FALSE;
	}*/

// 실제 처리 부분 /////////////////////////////////////////////////////////////

	//sobel 연산 -> 시작
	KScScalarImage2dUint8* mag = NULL;
	mag = new KScScalarImage2dUint8;
	if(mag == NULL)
	{
		::MessageBox(NULL, "memory allocation error.",
			  "iBurnsLineUint8", MB_OK);
			return FALSE;
	}
	mag->Alloc(dx,dy);

	KScScalarImage2dInt32* phase=NULL;
	phase = new KScScalarImage2dInt32;
	if(phase == NULL)
	{
		::MessageBox(NULL, "memory allocation error.",
			  "iBurnsLineUint8", MB_OK);
			return FALSE;
	}
	phase->Alloc(dx,dy);

    SobelMagPhaseForScalar(srcImg, mag, phase, threshold, option); 
	
	//compute gradient magnitude and phase angle....
	
	//sobel 연산 -> 끝



//testing module start
//	KSdUint8 **pMag      = mag   ->Get2dArray();
//    KSdInt32 **pPhase    = phase ->Get2dArray();
//	KSdUint8 **pOutputImage = dstImg->Get2dArray();
//	for(int i=0,j ; i<dy ; i++ )
//		for( j=0 ; j<dx ; j++ )
//			pOutputImage[i][j] = pMag[i][j];
//
//	KSdInt32 **pOutputImage2 = dstImg2->Get2dArray();
//	for(int i=0,j ; i<dy ; i++ )
//		for( j=0 ; j<dx ; j++ )
//			pOutputImage2[i][j] = pPhase[i][j];
//testing module end
	
	int dirNum=8; //8 principle directions

	if(*split_on) Line_SplitForScalar(mag, phase,dirNum); 

//testing module start
/*	KSdUint8 **pMag      = mag   ->Get2dArray();
    KSdInt32 **pPhase    = phase ->Get2dArray();
	KSdUint8 **pOutputImage = dstImg->Get2dArray();
	for(int i=0,j ; i<dy ; i++ )
		for( j=0 ; j<dx ; j++ )
			pOutputImage[i][j] = pMag[i][j];

	KSdInt32 **pOutputImage2 = dstImg2->Get2dArray();
	for(int i=0,j ; i<dy ; i++ )
		for( j=0 ; j<dx ; j++ )
			pOutputImage2[i][j] = pPhase[i][j];*/
//testing module end


	//segmentation -> 시작
	KScScalarImage2dUint8* seg1 = NULL;
	seg1 = new KScScalarImage2dUint8;
	if(seg1 == NULL)
	{
		::MessageBox(NULL, "memory allocation error.",
			  "iBurnsLineUint8", MB_OK);
			return FALSE;
	}
	seg1->Alloc(dx,dy);
	
	KScScalarImage2dUint8* seg2 = NULL;
	seg2 = new KScScalarImage2dUint8;
	if(seg2 == NULL)
	{
		::MessageBox(NULL, "memory allocation error.",
			  "iBurnsLineUint8", MB_OK);
			return FALSE;
	}
	seg2->Alloc(dx,dy);


    PhaseSegmentForScalar(phase, seg1, dirNum, 0);
    PhaseSegmentForScalar(phase, seg2, dirNum, 1); //22.5도 만큼 회전
	
	
	//segmentation -> 끝


//testing module start
/*	KSdUint8 **pMag      = seg1   ->Get2dArray();
    KSdInt32 **pPhase    = phase ->Get2dArray();
	KSdUint8 **pOutputImage = dstImg->Get2dArray();
	for(int i=0,j ; i<dy ; i++ )
		for( j=0 ; j<dx ; j++ )
			pOutputImage[i][j] = pMag[i][j];

	KSdInt32 **pOutputImage2 = dstImg2->Get2dArray();
	for(int i=0,j ; i<dy ; i++ )
		for( j=0 ; j<dx ; j++ )
			pOutputImage2[i][j] = pPhase[i][j];*/
//testing module end

if(*length_on) Length_FilterForScalar(seg1, seg2, mag); // 각도필터링





	////////////////////
	KScScalarImage2dInt32* label1=NULL;
	label1 = new KScScalarImage2dInt32;
	if(label1 == NULL)
	{
		::MessageBox(NULL, "memory allocation error.",
			  "iBurnsLineUint8", MB_OK);
			return FALSE;
	}
	label1->Alloc(dx,dy);
 
	KScScalarImage2dInt32* label2=NULL;
	label2 = new KScScalarImage2dInt32;
	if(label2 == NULL)
	{
		::MessageBox(NULL, "memory allocation error.",
			  "iBurnsLineUint8", MB_OK);
			return FALSE;
	}
	label2->Alloc(dx,dy);

    int nmbr_rgns = 1; //라벨링 갯수
  
	Coloring(seg1, label1, &nmbr_rgns); // 라벨링
	Coloring(seg2, label2, &nmbr_rgns);
	/////////////////////////////////////



//testing module start
/*	KSdInt32 **pMag      = label1   ->Get2dArray();
    KSdInt32 **pPhase    = label2 ->Get2dArray();
	KSdUint8 **pOutputImage = dstImg->Get2dArray();
	for(int i=0,j ; i<dy ; i++ )
		for( j=0 ; j<dx ; j++ )
			pOutputImage[i][j] = pMag[i][j];

	KSdInt32 **pOutputImage2 = dstImg2->Get2dArray();
	for(int i=0,j ; i<dy ; i++ )
		for( j=0 ; j<dx ; j++ )
			pOutputImage2[i][j] = pPhase[i][j];*/
//testing module end


	mstr_type *mstr; 
	int i;
    mstr = (mstr_type *)malloc((long)(nmbr_rgns + 2) * sizeof(mstr_type));
    for(i = 1; i <= nmbr_rgns; i++) {
        mstr[i].grdntang = PHASE_BCKGRND; // Initailize Gradient Angle
    }


	Length_Compute(seg1, seg2, label1, label2, mstr, nmbr_rgns);



//testing module start
/*	KSdInt32 **pMag      = label1   ->Get2dArray();
    KSdInt32 **pPhase    = label2 ->Get2dArray();
	KSdUint8 **pOutputImage = dstImg->Get2dArray();
	for(int i=0,j ; i<dy ; i++ )
		for( j=0 ; j<dx ; j++ )
			pOutputImage[i][j] = pMag[i][j];

	KSdInt32 **pOutputImage2 = dstImg2->Get2dArray();
	for(int i=0,j ; i<dy ; i++ )
		for( j=0 ; j<dx ; j++ )
			pOutputImage2[i][j] = pPhase[i][j];*/
//testing module end




	int nwnmbr_rgns; //?
    Voting((vmstr_type *)mstr, nmbr_rgns, label1, label2);
    Selection((vmstr_type *)mstr, nmbr_rgns, label1, label2, &nwnmbr_rgns, *len_thd); 

	/////////////////////////////출력
//	KSdInt32 **pDstArray2 = dstImg2->Get2dArray();
	KSdInt32 **pLabelArray = label1->Get2dArray();
	dstImg->InitTo(0);


/*
	for (int i2 = 0, j ; i2 < dy ; i2++)
		for (j = 0 ; j < dx ; j++)
			pDstArray2[i2][j] = pLabelArray[i2][j];*/
	////////////////////////////////

	line_list_type LINELIST;
    LINELIST.master_line = (master_type *)malloc(long(nwnmbr_rgns + 2) * sizeof(master_type));
    LINELIST.numoflines = nwnmbr_rgns;
    for(i = 0; i <= nwnmbr_rgns + 1; i++) LINELIST.master_line[i].tag = 0;

	// copy gradient direction from mstr to master_line
    Copy_Length((vmstr_type *)mstr, nmbr_rgns, LINELIST.master_line, nwnmbr_rgns);  

	// Compute Moments
	mmstr_type *mmstr;
    mmstr = (mmstr_type *)malloc((long)(nwnmbr_rgns + 2) * sizeof(mmstr_type));

    Moment_Compute(label1, mag, mmstr, nwnmbr_rgns);
    Orient_Compute(mmstr, nwnmbr_rgns, LINELIST.master_line);    
    Tdefine_Ends(LINELIST.master_line, nwnmbr_rgns);  

/*	CString Sec;
	for(int a=1; a<nwnmbr_rgns; a++)
	{
		Sec.Format("%d%s%d%s%d%s%d%s",(int)(LINELIST.master_line[a].xstart)," ",
			(int)LINELIST.master_line[a].xend," ",
			(int)LINELIST.master_line[a].ystart, " ",
			(int)LINELIST.master_line[a].yend," ");

		::MessageBox(NULL, Sec,"iBurnsLineUint8", MB_OK);
	}*/

	// 출력
	
	master_type *master = LINELIST.master_line;
	int linenum = nwnmbr_rgns;
	int x1,y1,x2,y2,angleIndex;
	float slope;
	int totalLine=int(TOTAL_DEGREE/GROUP_ANGLE);
	
	KSdUint8 **pDstArray = dstImg->Get2dArray();
	dstImg->InitTo(0);

	for(int k=1; k<=linenum; ++k)
	{
		x1=(int)LINELIST.master_line[k].xstart;
		y1=(int)LINELIST.master_line[k].ystart;
		x2=(int)LINELIST.master_line[k].xend;
		y2= (int)LINELIST.master_line[k].yend;
		//LineTo(x1,y1,x2,y2,dstImg,255); 
			
		if(TOTAL_DEGREE==360)
		{
		slope=atan2((float)(x2-x1),(float)(y2-y1))*180/3.141592654+180;
		}
		else
		{
		slope=atan((float)(x2-x1)/((float)(y2-y1)+0.0000001))*180/3.141592654+90;
		}

		angleIndex=(int(slope/GROUP_ANGLE)%totalLine);
		lineOb[angleIndex][lineIndex[angleIndex]].setLineSegment(x1,y1,x2,y2,slope,0.0,0.0);
		lineIndex[angleIndex]=(lineIndex[angleIndex]+1)%100;
	
	}
	
	
// 후처리 /////////////////////////////////////////////////////////////////////
	
	seg1->Free();
	seg2->Free();
	label1->Free();
	label2->Free();
	
	mag->Free();
	phase->Free();
	return TRUE;
}

// 2 개 이상의 함수를 정의할 경우, 아래에 수동으로 삽입합니다.

int SobelMagPhaseForScalar(KScScalarImage2dUint8* srcImg,
								 KScScalarImage2dUint8* mag,
								 KScScalarImage2dInt32* phase,
								 int* threshold,
								 int* option                 )
{
// 전처리 /////////////////////////////////////////////////////////////////////
	
	//입력 버퍼의 자료 형태 검사
	if(srcImg->GetId() != KS_SCALAR_IMAGE_2D_UINT8)
	{
		::MessageBox(NULL, "input buffer type not suitable.",
			  "iBurnsLineUint8", MB_OK);
			return FALSE;
	}
	if(mag->GetId() != KS_SCALAR_IMAGE_2D_UINT8)
	{
		::MessageBox(NULL, "Output buffer type not suitable.",
			"iBurnsLineUint8", MB_OK);
			return FALSE;
	}
	if(phase->GetId() != KS_SCALAR_IMAGE_2D_INT32)
	{
		::MessageBox(NULL, "Output buffer type not suitable.",
			"iBurnsLineUint8", MB_OK);
			return FALSE;
	}

	//입력 버퍼의 할당 여부 검사
	int dx =srcImg->GetMainXSize();
	int dy =srcImg->GetMainYSize();
	if (!dx || !dy  )
	{
		::MessageBox(NULL, "Input buffer not allocated5.",
			"iBurnsLineUint8", MB_OK);
			return FALSE;
	}

	if(dx != mag->GetXSize() || dy != mag->GetYSize())
	{
		// ROI가 설정되어 있으면 Free()에서 ROI도 해제됨.
		mag->Free();
		// Alloc()의 반환값 : 성공적이면 0, 실패이면 0이 아닌 에러 코드.
		if (mag->Alloc(dx, dy))
		{
			::MessageBox(NULL, "Fail to allocate output buffer.",
				"iBurnsLineUint8", MB_OK);
			return FALSE;
		}
	}
	if(dx != phase->GetXSize() || dy != phase->GetYSize())
	{
		// ROI가 설정되어 있으면 Free()에서 ROI도 해제됨.
		phase->Free();
		// Alloc()의 반환값 : 성공적이면 0, 실패이면 0이 아닌 에러 코드.
		if (phase->Alloc(dx, dy))
		{
			::MessageBox(NULL, "Fail to allocate output buffer.",
				"iBurnsLineUint8", MB_OK);
			return FALSE;
		}
	}

    int         x,  y ;
    int        Gx, Gy ;
    int        M_Value,					// Magnitude를 저장하는 변수
               P_Value;					// Phase를 저장하는 변수
    double     adjust = 180.0 / PI180;	//?
    int        height, width;

    KSdUint8 **pSrcArray = srcImg->Get2dArray();
	KSdUint8 **pMag      = mag   ->Get2dArray();
    KSdInt32 **pPhase    = phase ->Get2dArray();

    width  = srcImg->GetXSize();
    height = srcImg->GetYSize();

    for(y = 0; y < height; y++) {
        pMag[y][0]         = MAG_BCKGRND  ;
        pMag[y][1]         = MAG_BCKGRND  ;
        pMag[y][width-1]   = MAG_BCKGRND  ;
        pMag[y][width-2]   = MAG_BCKGRND  ;
        pPhase[y][0]       = PHASE_BCKGRND;
        pPhase[y][1]       = PHASE_BCKGRND;
        pPhase[y][width-1] = PHASE_BCKGRND;
        pPhase[y][width-2] = PHASE_BCKGRND;        
    }

    for(x = 0; x < width; x++) {
        pMag[0][x]          = MAG_BCKGRND  ;
        pMag[1][x]          = MAG_BCKGRND  ;
        pMag[height-1][x]   = MAG_BCKGRND  ;
        pMag[height-2][x]   = MAG_BCKGRND  ;
        pPhase[0][x]        = PHASE_BCKGRND;
        pPhase[1][x]        = PHASE_BCKGRND;
        pPhase[height-1][x] = PHASE_BCKGRND;
        pPhase[height-2][x] = PHASE_BCKGRND;
    }

    // 실제 루틴
    for(y = 2; y < height - 2; y++) {
       for(x = 2; x < width - 2; x++) {
		   if((int)pSrcArray[y + 1][x - 1]==0||(int)pSrcArray[y + 1][x    ]==0||(int)pSrcArray[y + 1][x + 1]==0||(int)pSrcArray[y - 1][x - 1]==0||(int)pSrcArray[y - 1][x    ]==0||(int)pSrcArray[y - 1][x + 1]==0||(int)pSrcArray[y + 1][x + 1]==0||(int)pSrcArray[y    ][x + 1]==0||(int)pSrcArray[y - 1][x + 1]==0||(int)pSrcArray[y + 1][x - 1]==0||(int)pSrcArray[y    ][x - 1]==0||(int)pSrcArray[y - 1][x - 1]==0)
		   {
			   pMag[y][x] = MAG_BCKGRND;
			   pPhase[y][x] = PHASE_BCKGRND;
			   continue;
		   }

           Gx = ((int)pSrcArray[y + 1][x - 1] + 
		         (int)pSrcArray[y + 1][x    ] * 2 +
				 (int)pSrcArray[y + 1][x + 1]) -
				((int)pSrcArray[y - 1][x - 1] +
				 (int)pSrcArray[y - 1][x    ] * 2 +
				 (int)pSrcArray[y - 1][x + 1]);

		   Gy = ((int)pSrcArray[y + 1][x + 1] + 
		         (int)pSrcArray[y    ][x + 1] * 2 +
				 (int)pSrcArray[y - 1][x + 1]) -
				((int)pSrcArray[y + 1][x - 1] +
				 (int)pSrcArray[y    ][x - 1] * 2 +
				 (int)pSrcArray[y - 1][x - 1]);
           
           M_Value = (abs(Gx) + abs(Gy)); // Magnitude!
           if(*option == 1) { 
               M_Value /= 4;
               if(M_Value < *threshold) { // Background Pixel
                   pMag[y][x] = 0;
                   pPhase[y][x] = PHASE_BCKGRND;
               }
               else {
                   if(M_Value > 255) pMag[y][x] = 255;
                   else pMag[y][x] = M_Value;
                   P_Value = (int)(atan2((float)Gy, (float)Gx) * adjust + 0.5);
                   if(P_Value > 0) pPhase[y][x] = P_Value;
                   else pPhase[y][x] = (P_Value + 360);
               }
           }
           else if(*option == 2) {
               if(M_Value < *threshold) { // Background Pixel
                   pMag[y][x] = 0;
                   pPhase[y][x] = PHASE_BCKGRND;
               }
               else {
                   pMag[y][x] = 255;                   
                   P_Value = (int)(atan2((float)Gy, (float)Gx) * adjust + 0.5);
                   if(P_Value > 0) pPhase[y][x] = P_Value;
                   else pPhase[y][x] = (P_Value + 360);
               }
           }
           else if(*option == 3) {               
               if(M_Value < *threshold) { // Background Pixel
                   pMag[y][x] = 0;
                   pPhase[y][x] = PHASE_BCKGRND;
               }
               else {     
                   if(M_Value > 255) M_Value = 255;
                   pMag[y][x] = M_Value;
                   P_Value = (int)(atan2((float)Gy, (float)Gx) * adjust + 0.5);
                   if(P_Value > 0) pPhase[y][x] = P_Value;
                   else pPhase[y][x] = (P_Value + 360);
               }
           }           
       }
    }

    mag->AppendComment("Get Sobel Magnitude");
    phase->AppendComment("Get Sobel Phase");
    return TRUE;
}

int Line_SplitForScalar(KScScalarImage2dUint8 *mag, KScScalarImage2dInt32 *phase, int dirnum)
{
    int i, j, base, tl, tr;
    double delta, adjust, sum = 0.0;
    int lbl, cnst;
    unsigned char hashtable[362];
    int k, x, y;
    int width, height;
    KSdUint8 **pMag;
    KSdInt32 **pPhase;
	int t1=0,t2=0;

    // For buffer manage attach ...
	mag->Attach();
    
	if(mag->GetId() != KS_SCALAR_IMAGE_2D_UINT8) return FALSE;
    if(phase->GetId() != KS_SCALAR_IMAGE_2D_INT32) return FALSE;

    pMag = mag->Get2dArray();
    pPhase = phase->Get2dArray();

    width  = mag->GetXSize();
    height = mag->GetYSize();

    delta = 360.0 / (double)dirnum; //45
    adjust = delta / 2.0; //22.5

    // generate hash table //?
    for(i = 0; i < 360; i++) {
        sum = i + adjust;
        if(sum >= 360) sum = sum - 360;
        cnst = (int)( sum / delta ) % dirnum;
        hashtable[i] = (unsigned char)cnst;
    }
    hashtable[360] = 0;
    hashtable[361] = 255;

    // Pass over original magnitude array and thin with a 3 * 3 operator
    for(i = 2; i < height - 2; i++) {
        for(j = 2; j < width -2; j++) {
            lbl = hashtable[pPhase[i][j]];
            if(lbl < 361) {
                base = pMag[i][j];
                switch(lbl) 
				{
                    case 2 :
                    case 6 : // horizontal direction edge
                        tl = pPhase[i-1][j];
                        tr = pPhase[i+1][j];
                        if(hashtable[tl] == lbl && hashtable[tr] == lbl) 
						{
                            if(pMag[i-1][j] > base) 
							{
                                if(pMag[i+1][j] > base) 
                                    pPhase[i][j] = 361;
                                else if(pMag[i+1][j] == base)
								{
									//v_srch_d(0, 1);
									t1=0;t2=1;
									x = j + 2 * t1;
									y = i + 2 * t2;
								    for( k = 2 ; hashtable[pPhase[y][x]]==lbl && pMag[y][x] == base ; k++ )
									{
										if( pMag[y][x] > base )
											for( x -= (t1), y -= (t2) ; k > 0 ; k--,  x -= (t1), y -= (t2) )
												pPhase[y][x] = 361;
										x += (t1);
										y += (t2);
									}
								}
                            }
                            else if(pMag[i - 1][j] == base && pMag[i + 1][j] > base) 
							{
                                //v_srch_d(0, -1);
									t1=0;t2=-1;
									x = j + 2 * t1;
									y = i + 2 * t2;
								    for( k = 2 ; hashtable[pPhase[y][x]]==lbl && pMag[y][x] == base ; k++ )
									{
										if( pMag[y][x] > base )
											for( x -= (t1), y -= (t2) ; k > 0 ; k--,  x -= (t1), y -= (t2) )
												pPhase[y][x] = 361;
										x += (t1);
										y += (t2);
									}

                            }
                        }
                        break;

                    case 0 :
                    case 4 : // vertical direction edge
                        tl = pPhase[i][j-1];
                        tr = pPhase[i][j+1];
                        if(hashtable[tl] == lbl && hashtable[tr] == lbl) 
						{
                            if(pMag[i][j-1] > base) 
							{
                                if(pMag[i][j+1] > base) 
                                    pPhase[i][j] = 361;
                                else if(pMag[i][j+1] == base) 
								{
									//v_srch_d(1, 0);
									t1=1;t2=0;
									x = j + 2 * t1;
									y = i + 2 * t2;
								    for( k = 2 ; hashtable[pPhase[y][x]]==lbl && pMag[y][x] == base ; k++ )
									{
										if( pMag[y][x] > base )
											for( x -= (t1), y -= (t2) ; k > 0 ; k--,  x -= (t1), y -= (t2) )
												pPhase[y][x] = 361;
										x += (t1);
										y += (t2);
									}
								}
                            }
                            else if(pMag[i][j-1] == base && pMag[i][j+1] > base) 
							{
								//v_srch_d(-1, 0);
									t1=-1;t2=0;
									x = j + 2 * t1;
									y = i + 2 * t2;
								    for( k = 2 ; hashtable[pPhase[y][x]]==lbl && pMag[y][x] == base ; k++ )
									{
										if( pMag[y][x] > base )
											for( x -= (t1), y -= (t2) ; k > 0 ; k--,  x -= (t1), y -= (t2) )
												pPhase[y][x] = 361;
										x += (t1);
										y += (t2);
									}
							}
                        }
                        break;

                    case 1 :
                    case 5 :
                        tl = pPhase[i-1][j-1];
                        tr = pPhase[i+1][j+1];
                        if(hashtable[tl] == lbl && hashtable[tr] == lbl) 
						{
                            if(pMag[i-1][j-1] > base) 
							{
                                if(pMag[i+1][j+1] > base) 
                                    pPhase[i][j] = 361;
                                else if(pMag[i+1][j+1] == base)
								{
									//v_srch_d(1, 1);
									t1=1;t2=1;
									x = j + 2 * t1;
									y = i + 2 * t2;
								    for( k = 2 ; hashtable[pPhase[y][x]]==lbl && pMag[y][x] == base ; k++ )
									{
										if( pMag[y][x] > base )
											for( x -= (t1), y -= (t2) ; k > 0 ; k--,  x -= (t1), y -= (t2) )
												pPhase[y][x] = 361;
										x += (t1);
										y += (t2);
									}
								}
                            }
                            else if(pMag[i-1][j-1] == base && pMag[i+1][j+1] > base)
							{
								//v_srch_d(-1, -1);
									t1=-1;t2=-1;
									x = j + 2 * t1;
									y = i + 2 * t2;
								    for( k = 2 ; hashtable[pPhase[y][x]]==lbl && pMag[y][x] == base ; k++ )
									{
										if( pMag[y][x] > base )
											for( x -= (t1), y -= (t2) ; k > 0 ; k--,  x -= (t1), y -= (t2) )
												pPhase[y][x] = 361;
										x += (t1);
										y += (t2);
									}
							}
                        }
                        break;

                    case 3 :
                    case 7 :
                        tl = pPhase[i+1][j-1];
                        tr = pPhase[i-1][j+1];
                        if(hashtable[tl] == lbl && hashtable[tr] == lbl) 
						{
                            if(pMag[i+1][j-1] > base) 
							{
                                if(pMag[i-1][j+1] > base) 
                                    pPhase[i][j] = 361;
                                else if(pMag[i-1][j+1] == base)
								{
									//v_srch_d(1, -1);
									t1=1;t2=-1;
									x = j + 2 * t1;
									y = i + 2 * t2;
								    for( k = 2 ; hashtable[pPhase[y][x]]==lbl && pMag[y][x] == base ; k++ )
									{
										if( pMag[y][x] > base )
											for( x -= (t1), y -= (t2) ; k > 0 ; k--,  x -= (t1), y -= (t2) )
												pPhase[y][x] = 361;
										x += (t1);
										y += (t2);
									}
								}
                            }
                            else if(pMag[i+1][j-1] == base && pMag[i-1][j+1] > base) 
							{
								//v_srch_d(-1, 1);
									t1=-1;t2=1;
									x = j + 2 * t1;
									y = i + 2 * t2;
								    for( k = 2 ; hashtable[pPhase[y][x]]==lbl && pMag[y][x] == base ; k++ )
									{
										if( pMag[y][x] > base )
											for( x -= (t1), y -= (t2) ; k > 0 ; k--,  x -= (t1), y -= (t2) )
												pPhase[y][x] = 361;
										x += (t1);
										y += (t2);
									}
							}
                        }
                        break;

                    default : pPhase[i][j] = 361;
                }
            }
        }
    }

    phase->AppendComment("Splitting");
	return TRUE;
}

//
// PhaseSegment  : Phase Image Segmentation
//                 Quantize gradient angle to dirnum levels. 
//                 if stat=1 then start quantize from 360/dirnum/2.0 degree
// Argument List : DirNum : Number of Directions 4, 8, 16
//                 Start : 0 to start from 0, 1 to start from 360/(2*dirnum)
//
int PhaseSegmentForScalar(KScScalarImage2dInt32* phase,
								KScScalarImage2dUint8* seg,
								int DirNum,
								int Start)
{
	if (phase->GetId() != KS_SCALAR_IMAGE_2D_INT32)
	{
		::MessageBox(NULL, "Output buffer type not suitable.",
			"iBurnsLineUint8", MB_OK);
			return FALSE;
	}
	if (seg->GetId() != KS_SCALAR_IMAGE_2D_UINT8)
	{
		::MessageBox(NULL, "Output buffer type not suitable.",
			"iBurnsLineUint8", MB_OK);
			return FALSE;
	}

	int dx = phase->GetXSize();
	int dy = phase->GetYSize();
	if (!dx || !dy)
	{
		::MessageBox(NULL, "Input buffer not allocated6.",
			"iBurnsLineUint8", MB_OK);
			return FALSE;
	}
	if (dx != seg->GetXSize() ||
			dy != seg->GetYSize())
	{
		// ROI가 설정되어 있으면 Free()에서 ROI도 해제됨.
		seg->Free();
		// Alloc()의 반환값 : 성공적이면 0, 실패이면 0이 아닌 에러 코드.
		if (seg->Alloc(dx, dy))
		{
			::MessageBox(NULL, "Fail to allocate output buffer.",
				"iBurnsLineUint8", MB_OK);
			return FALSE;
		}
	}

    double Delta, Adjust;
    unsigned char hashtable[362];
    int i, j, cnst;
    double sum;    

    KSdInt32 **pPhase = phase->Get2dArray();    
    KSdUint8 **pSeg   = seg  ->Get2dArray();

    int width  = dx;
    int height = dy;

    Delta = 360.0 / (double) DirNum; //45
    if(Start == 1) Adjust = Delta / 2.0; //22.5도 만큼 회전하기 위하여...
    else Adjust = 0;

    // generate hash table
    for(i = 0; i < 360; i++) {
        sum = i + Adjust;
        if(sum >= 360) sum = sum - 360;
        cnst = (int)( sum / Delta );
        cnst = cnst % DirNum;
        hashtable[i] = (unsigned char)cnst;
    }
    hashtable[360] = 0;
    hashtable[361] = 255;
    
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            pSeg[i][j] = hashtable[pPhase[i][j]];     
        }
    }

    seg->AppendComment("Phase Segment");
    return TRUE;
}

int Length_FilterForScalar(KScScalarImage2dUint8 *seg1, 
						   KScScalarImage2dUint8 *seg2, 
						   KScScalarImage2dUint8 *mag)
{    
    int i, j, a;
    KSdUint8 **pSeg1, **pSeg2, **pMag;
    int width, height;

    // For buffer manage attach ...
	mag->Attach();    
	
    if(seg1->GetId() != KS_SCALAR_IMAGE_2D_UINT8) return FALSE;
    if(seg2->GetId() != KS_SCALAR_IMAGE_2D_UINT8) return FALSE;
    if(mag->GetId() != KS_SCALAR_IMAGE_2D_UINT8) return FALSE;
    
    pSeg1 = seg1->Get2dArray();
    pSeg2 = seg2->Get2dArray();
    pMag  = mag->Get2dArray();    

    width  = mag->GetXSize();
    height = mag->GetYSize();

    for(i = 2; i < height - 2; i++) {
        for(j = 2; j < width -2; j++) {

            if((a = pSeg1[i][j]) < 8) {

                switch (a) {

                    case 2 :
                    case 6 : 
                        filter1(seg1, 1, 5, 1, 9, j, i, a);
                        break;

                    case 3 :
                    case 7 :
                        filter1(seg1, 2, 6, 3, 11, j, i, a);
                        break;

                    case 0 :
                    case 4 :
                        filter1(seg1, 3, 7, 5, 13, j, i, a);
                        break;

                    case 1 :
                    case 5 :
                        filter1(seg1, 4, 8, 7, 15, j, i, a);
                        break;

                    default : pSeg1[i][j] = SPHASE_BCKGRND;
                }
                    
                a = pSeg2[i][j];
            
                switch(a) {

                    case 2 :
                    case 6 :
                        filter2(seg2, 1, 5, 0, 8, j, i, a);
                        break;

                    case 3 :
                    case 7 :
                        filter2(seg2, 2, 6, 2, 10, j, i, a);
                        break;

                    case 0 :
                    case 4 :
                        filter2(seg2, 3, 7, 4, 12, j, i, a);
                        break;

                    case 1 :
                    case 5 :
                        filter2(seg2, 4, 8, 6, 14, j, i, a);
                        break;

                    default : pSeg2[i][j] = SPHASE_BCKGRND;
                } // end of switch
            } // end of if
        } // end of for j
    } // end of for i
   
    return TRUE;
}

int filter1( KScScalarImage2dUint8 *seg, 
			int s1, int e1, 
			int s2, int e2, 
			int x , int y , 
			int a         )
{
  	int ur1_fnd = TRUE;
	int dl1_fnd = TRUE;
    KSdUint8 **pSeg;

    // For buffer manage attach ...
	seg->Attach();    
	
    if(seg->GetId() != KS_SCALAR_IMAGE_2D_UINT8) return FALSE;    
    
    pSeg = seg->Get2dArray();    

    if(pSeg[y + y_1[s1]][x + x_1[s1]] != a) {
        if(pSeg[y + y_1[s1 + 1]][x + x_1[s1 + 1]] != a) {
            ur1_fnd = FALSE;
        }
    }

    if(pSeg[y + y_1[e1]][x + x_1[e1]] != a) {
        if(pSeg[y + y_1[e1 + 1]][x + x_1[e1 + 1]] != a) {
            dl1_fnd = FALSE;
        }
    }

    if(dl1_fnd) {
        if(!ur1_fnd) {
            if(pSeg[y + y_2[e2]][x + x_2[e2]] != a) {
                if(pSeg[y + y_2[e2 + 1]][x + x_2[e2 + 1]] != a) {
                    if(pSeg[y + y_2[e2 + 2]][x + x_2[e2 + 2]] != a) {
                        pSeg[y][x] = SPHASE_BCKGRND;
                    }
                }
            }
        }
    }
    else {
        if(!ur1_fnd) {
            pSeg[y][x] = SPHASE_BCKGRND;
        }
        else {
            if(pSeg[y + y_2[s2]][x + x_2[s2]] != a) {
                if(pSeg[y + y_2[s2 + 1]][x + x_2[s2 + 1]] != a) {
                    if(pSeg[y + y_2[s2 +2]][x + x_2[s2 + 2]] != a) {
                        pSeg[y][x] = SPHASE_BCKGRND;
                    }
                }
            }
        }
    }

    return FALSE;
}

int filter2(KScScalarImage2dUint8 *seg, int s1, int e1, int s2, int e2, int x, int y, int a)
{
  	int ur1_fnd = TRUE;
	int dl1_fnd = TRUE;
    KSdUint8 **pSeg;

    // For buffer manage attach ...
	seg->Attach();    
	
    if(seg->GetId() != KS_SCALAR_IMAGE_2D_UINT8) return FALSE;    
    
    pSeg = seg->Get2dArray();    

    if(pSeg[y + y_1[s1]][x + x_1[s1]] != a) ur1_fnd = FALSE;        
    if(pSeg[y + y_1[e1]][x + x_1[e1]] != a) dl1_fnd = FALSE;

    if(dl1_fnd) {
        if(!ur1_fnd) {
            if(pSeg[y + y_2[e2]][x + x_2[e2]] != a) {
                if(pSeg[y + y_2[e2 + 1]][x + x_2[e2 + 1]] != a) {
                    if(pSeg[y + y_2[e2 + 2]][x + x_2[e2 + 2]] != a) {
                        pSeg[y][x] = SPHASE_BCKGRND;
                    }
                }
            }
        }
    }
    else {
        if(!ur1_fnd) pSeg[y][x] = SPHASE_BCKGRND;        
        else {
            if(pSeg[y + y_2[s2]][x + x_2[s2]] != a) {
                if(pSeg[y + y_2[s2 + 1]][x + x_2[s2 + 1]] != a) {
                    if(pSeg[y + y_2[s2 +2]][x + x_2[s2 + 2]] != a) {
                        pSeg[y][x] = SPHASE_BCKGRND;
                    }
                }
            }
        }
    }

    return FALSE;
}

int Coloring(KScScalarImage2dUint8* segmap  ,
				   KScScalarImage2dInt32* outmap  ,
				   int*                   cr_label)
{
	if (segmap->GetId() != KS_SCALAR_IMAGE_2D_UINT8)
	{
		::MessageBox(NULL, "Output buffer type not suitable.",
			"iBurnsLineUint8", MB_OK);
			return FALSE;
	}
	if (outmap->GetId() != KS_SCALAR_IMAGE_2D_INT32)
	{
		::MessageBox(NULL, "Output buffer type not suitable.",
			"iBurnsLineUint8", MB_OK);
			return FALSE;
	}

	int dx =segmap->GetXSize();
	int dy =segmap->GetYSize();
	if (!dx || !dy  )
	{
		::MessageBox(NULL, "Input buffer not allocated7.",
			"iBurnsLineUint8", MB_OK);
			return FALSE;
	}
	if (dx != outmap->GetXSize() ||
			dy != outmap->GetYSize())
	{
		// ROI가 설정되어 있으면 Free()에서 ROI도 해제됨.
		outmap->Free();
		// Alloc()의 반환값 : 성공적이면 0, 실패이면 0이 아닌 에러 코드.
		if (outmap->Alloc(dx, dy))
		{
			::MessageBox(NULL, "Fail to allocate output buffer.",
				"iBurnsLineUint8", MB_OK);
			return FALSE;
		}
	}

    Clear_Boundary_Byte(segmap, 1, SPHASE_BCKGRND);
    Forward_Color(segmap, outmap, cr_label);
    Backward_Color(segmap, outmap, cr_label);
    Clear_Boundary_Int(outmap, 1, COLOR_BCKGRND);   
    
    return TRUE;
}

int Clear_Boundary_Byte(KScScalarImage2dUint8 *Img, int n, int gray_level) 
{
    register int i,j,max;
    BYTE bgray_level;
    KSdUint8 **pImg;
    int width, height;
  
    // For buffer manage attach ...
	Img->Attach();    
	
    if(Img->GetId() != KS_SCALAR_IMAGE_2D_UINT8) return FALSE;    
    
    pImg = Img->Get2dArray();

    width  = Img->GetXSize();
    height = Img->GetYSize();
   
    bgray_level = (BYTE)gray_level; // 창범: 원래는 bgray_level이었음. 
	max = height - 1;
    for(i = 0; i < width; i++) {
        for(j = 0; j < n; j++) {
            pImg[j][i] = bgray_level; //255
	        pImg[max - j][i] = bgray_level;
        }
    }
	
    max = width - 1;
    for(i = 0; i < n; i++) {
        for(j = 0; j < height; j++) {
            pImg[j][i] = bgray_level;
	        pImg[j][max - i] = bgray_level;
        }
    }
 
    return TRUE;
}

int Forward_Color(KScScalarImage2dUint8 *segmap, 
				  KScScalarImage2dInt32 *outmap, 
				  int *crnt_label              )
{
    int          ix;
    register int iy,i,tmp_num;
    int          crsgmnt, Max_iy, Max_ix;
    int          tsy,tey, crnt_angle;
    int width, height;
    KSdUint8 **pSegmap;
    KSdInt32 **pOutmap;

    // For buffer manage attach ...
	segmap->Attach();    
	
    if(segmap->GetId() != KS_SCALAR_IMAGE_2D_UINT8) return FALSE;
    if(outmap->GetId() != KS_SCALAR_IMAGE_2D_INT32) return FALSE;
    
    pSegmap = segmap->Get2dArray();
    pOutmap = outmap->Get2dArray();

    width = segmap->GetXSize();
    height = segmap->GetYSize();

    // label the first row  
    Max_ix = width - WINDOWSIZE;
    Max_iy = height - WINDOWSIZE;    

    for(iy = WINDOWSIZE; iy < Max_iy; ) {
        // count the number of connected pixels
        i = iy;
        for(tmp_num = 1; iy < Max_iy - 1 && pSegmap[iy][WINDOWSIZE] == pSegmap[iy+1][WINDOWSIZE]; iy++, tmp_num++);
        if(pSegmap[i][WINDOWSIZE] == SPHASE_BCKGRND) crsgmnt = COLOR_BCKGRND;
        else {
            crsgmnt = *crnt_label; 
            (*crnt_label)++; 
        }

        // color connected pixels with "current label" 
        for (; i <= iy; pOutmap[i][WINDOWSIZE] = crsgmnt, i++);
        iy++;
    }

    // label connected regions from x=2 to x=Img[segmap].Xmax-1       
    for(ix = WINDOWSIZE + 1; ix < Max_ix; ix++) {
         for(iy = WINDOWSIZE; iy < Max_iy; ) {
             tmp_num = 1; 	  
             i = iy;
             for(; iy < Max_iy - 1 && pSegmap[iy][ix] == pSegmap[iy + 1][ix]; tmp_num++, iy++);
             
             // check previous row to determine connected region   
	         if(pSegmap[i][ix] == SPHASE_BCKGRND) crsgmnt = COLOR_BCKGRND;
             else {
                 crnt_angle = pSegmap[i][ix];
                 tsy = (i > WINDOWSIZE) ? (i - 1) : i;
	             tey = (iy < (height - WINDOWSIZE - 1)) ? (iy + 1) : iy;
	             for( ; tsy <= tey && pSegmap[tsy][ix - 1] != crnt_angle; tsy++);
                 if(tsy > tey) { // not found. Starting New region 
                     crsgmnt = *crnt_label; 
                     (*crnt_label)++; 
                 }		            
                 else crsgmnt = pOutmap[tsy][ix - 1];
             }
                    
             // recoloring  
	         for (; i <= iy; pOutmap[i][ix] = crsgmnt, i++);
	         iy++;
         } 
    }    

    return TRUE;
}

int Backward_Color(KScScalarImage2dUint8 *segmap, 
				   KScScalarImage2dInt32 *outmap, 
				   int *crnt_label)
{
    register int ix, iy, i, tmp_num;
    int crsgmnt, Max_iy;
    int tsy, tey, crnt_angle;
    int width, height;
    KSdUint8 **pSegmap;
    KSdInt32 **pOutmap;

    // For buffer manage attach ...
	segmap->Attach();    
	
    if(segmap->GetId() != KS_SCALAR_IMAGE_2D_UINT8) return FALSE;
    if(outmap->GetId() != KS_SCALAR_IMAGE_2D_INT32) return FALSE;
    
    pSegmap = segmap->Get2dArray();
    pOutmap = outmap->Get2dArray();

    width = segmap->GetXSize();
    height = segmap->GetYSize();

    Max_iy = height - WINDOWSIZE;

    // label connected regions from x=Img[segmap].Xmax-1 to 2
    for(ix = width - WINDOWSIZE - 2; ix >= WINDOWSIZE; ix--) {
        for(iy = WINDOWSIZE; iy < Max_iy; ) {
            tmp_num = 1;
            i = iy;
            for(; iy < Max_iy - 1 && pSegmap[iy][ix] == pSegmap[iy + 1][ix]; tmp_num++, iy++);
            
            // check previous row to determine connected region   
            if(pSegmap[i][ix] != SPHASE_BCKGRND) {
                crnt_angle = pSegmap[i][ix];
	            tsy = (i > WINDOWSIZE) ? (i-1) : i;
	            tey = (iy < height - WINDOWSIZE - 1) ? (iy + 1) : iy;
	            for( ; tsy <= tey && pSegmap[tsy][ix + 1] != crnt_angle; tsy++);
	            if(tsy <= tey && pOutmap[tsy][ix + 1] != pOutmap[i][ix]) {
                    crsgmnt = pOutmap[tsy][ix + 1];
		            for( ; i <= iy; pOutmap[i][ix] = crsgmnt, i++); // recoloring
		        }
            } 

            iy++;
        }
    }

    return TRUE;
}

int Clear_Boundary_Int(KScScalarImage2dInt32 *Img, 
					   int n                     , 
					   int gray_level            )
{
    register int i,j,max;
    int width, height;
    KSdInt32 **pImg;

    // For buffer manage attach ...
	Img->Attach();    
	
    if(Img->GetId() != KS_SCALAR_IMAGE_2D_INT32) return FALSE;
    
    pImg = Img->Get2dArray();    
    
    width  = Img->GetXSize();
    height = Img->GetYSize();

    max = height -1;
    for(i = 0; i < width; i++) {
        for(j = 0; j < n; j++) {
            pImg[j][i] = gray_level;
            pImg[max - j][i] = gray_level;
        }
    }

    max = width - 1;
    for(i = 0; i < n; i++) {
        for(j = 0; j < height; j++) {
            pImg[j][i] = gray_level;
            pImg[j][max - i] = gray_level;
        }
    }
    
    return TRUE;
}

//
// Length_Compute : Computer the Length of each line supporting regions
//
void Length_Compute(KScScalarImage2dUint8 *phase1   , 
					KScScalarImage2dUint8 *phase2   , 
					KScScalarImage2dInt32 *label1   , 
					KScScalarImage2dInt32 *label2   , 
					mstr_type             *mstr     , 
					int                    nmbr_rgns)
{
	// determine two end points :
    // if grad = (0, 2, 4, 6), determine two end points in x - direction
    // else determine two end points in y - direction
	
	int i;

    for(i=1; i<=nmbr_rgns; mstr[i++].grdntang=PHASE_BCKGRND);    
    Compute_Ends(phase1, label1, mstr);
    Compute_Ends(phase2, label2, mstr);
	Compute_Length(mstr, nmbr_rgns);
}

//
// Compute_Ends : Find the two extream points.
//                If gradient direction is 0, 3, 4, 7 find extream points in x direction.
//                Otherwise find in y direction
//
int Compute_Ends(KScScalarImage2dUint8 *phase, 
				 KScScalarImage2dInt32 *label, 
				 mstr_type             *mstr ) // 선의 위치 각 끝점
{
    int i, j, ang, clr;
    int width, height;
    KSdUint8 **pPhase;
    KSdInt32 **pLabel;
     
    // For buffer manage attach ...
	phase->Attach();    
	
    if(phase->GetId() != KS_SCALAR_IMAGE_2D_UINT8) return FALSE;
    if(label->GetId() != KS_SCALAR_IMAGE_2D_INT32) return FALSE;
    
    pPhase = phase->Get2dArray();
    pLabel = label->Get2dArray();

    width = phase->GetXSize();
    height = phase->GetYSize();

    for(i = 0; i < width; i++) {
        for(j = 0; j < height; j++) {
            if((clr = pLabel[j][i]) != COLOR_BCKGRND) { //0
                if(mstr[clr].grdntang == PHASE_BCKGRND) { //361
                    // initialize
                    ang = (int)pPhase[j][i];
                    if(ang == 0 || ang == 3 || ang == 4 || ang == 7) mstr[clr].grdntang = -ang;
                    else mstr[clr].grdntang = ang;
                    
					mstr[clr].ystrt = j;
                    mstr[clr].xstrt = i;
                    mstr[clr].length = j;
                    mstr[clr].xend = i;
                }
                else {
                    if(mstr[clr].grdntang > 0) { // y = ax + b
                        mstr[clr].length = j;
						mstr[clr].xend = i;
                    }
                    else {
                        if(mstr[clr].ystrt > j) {
                            mstr[clr].ystrt = j;
                            mstr[clr].xstrt = i;
                        }
                        if(mstr[clr].length < j) {
                           mstr[clr].length = j;
                           mstr[clr].xend   = i;
                        }
                    }
                }
            }
        }
    }
    
    return TRUE;
}

//
// Compute_Length : Compute the length of the LSR based on two end points 
//
void Compute_Length(mstr_type *mstr, int nmbr_rgns) // 선의 길이
{
    int i;
    double dx, dy;

    for(i = 1; i <= nmbr_rgns; i++) {
        dx = (double)(mstr[i].xend - mstr[i].xstrt + 1);
        dy = (double)(mstr[i].length - mstr[i].ystrt + 1);
        mstr[i].length = (int)(sqrt(dx * dx + dy * dy));
    }
}

//
// Voting : To select longer line supporting regions, accumulate votes 
//          for each line supprointfg regions
//
int Voting(vmstr_type            *mstr     , 
		   int                    nmbr_rgns, 
		   KScScalarImage2dInt32 *color1   , 
		   KScScalarImage2dInt32 *color2   )
{
    int i, j;
    int width, height;
    KSdInt32 **pColor1, **pColor2;
    
    // For buffer manage attach ...
	color1->Attach();    
	
    if(color1->GetId() != KS_SCALAR_IMAGE_2D_INT32) return FALSE;
    if(color2->GetId() != KS_SCALAR_IMAGE_2D_INT32) return FALSE;
    
    pColor1 = color1->Get2dArray();
    pColor2 = color2->Get2dArray();

    width  = color1->GetXSize();
    height = color2->GetYSize();

    for(i = 1; i <= nmbr_rgns; i++) {
        mstr[i].area = 0;
        mstr[i].votes = 0;
    }

    for(i = 0; i < width; i++) {
        for(j = 0; j < height; j++) {
            if(pColor1[j][i] != COLOR_BCKGRND) {
                mstr[pColor1[j][i]].area++;
                mstr[pColor2[j][i]].area++;
                if(mstr[pColor1[j][i]].length >= mstr[pColor2[j][i]].length) {
                    mstr[pColor1[j][i]].votes++;
                }
                else mstr[pColor2[j][i]].votes++;                                
            }
        }
    }
    
    return TRUE;
}

//
// Selection : Select the longer line supporintg regions based on the # of votes
//             and generate a labeled line supporting region map.
//
int Selection(vmstr_type            *mstr       , 
			  int                    nmbr_rgns  , 
			  KScScalarImage2dInt32 *color1     , 
			  KScScalarImage2dInt32 *color2     , 
			  int                   *nwnmbr_rgns, 
			  int                    lthresh    )
{
    int i, j, numclr;
    int width, height;
    KSdInt32 **pColor1, **pColor2;
    
    // For buffer manage attach ...
	color1->Attach();    
	
    if(color1->GetId() != KS_SCALAR_IMAGE_2D_INT32) return FALSE;
    if(color2->GetId() != KS_SCALAR_IMAGE_2D_INT32) return FALSE;
    
    pColor1 = color1->Get2dArray();
    pColor2 = color2->Get2dArray();

    width  = color1->GetXSize();
    height = color1->GetYSize();
    
    numclr = 0; 
    mstr[0].nwcolor = COLOR_BCKGRND;
    for(i = 1; i <= nmbr_rgns; i++) {
        if(mstr[i].length >= lthresh && mstr[i].votes && mstr[i].votes >= (int)(mstr[i].area >> 1)) {
            mstr[i].nwcolor = ++numclr;
        }
        else mstr[i].nwcolor = COLOR_BCKGRND;
    }
    *nwnmbr_rgns = numclr;

    for(i = 0; i < width; i++) 
    {
        for(j = 0; j < height; j++) 
        {
            if(pColor1[j][i] != COLOR_BCKGRND) 
            {
                if(mstr[pColor1[j][i]].nwcolor != COLOR_BCKGRND) 
                {
                    if(mstr[pColor2[j][i]].nwcolor != COLOR_BCKGRND) 
                    {
                        if(mstr[pColor1[j][i]].length >= mstr[pColor2[j][i]].length) 
                        {
                            pColor1[j][i] = mstr[pColor1[j][i]].nwcolor;
                        }
                        else pColor1[j][i] = mstr[pColor2[j][i]].nwcolor;
                    }
                    else pColor1[j][i] = mstr[pColor1[j][i]].nwcolor;
                }
                else pColor1[j][i] = mstr[pColor2[j][i]].nwcolor;
            }
            else pColor1[j][i] = mstr[pColor2[j][i]].nwcolor;
        }
    }
    

	
    return TRUE;
}

void Copy_Length(vmstr_type *mstr, int nmbr, master_type *master, int nwnmbr) 
{    
    int i;

    for(i = 1; i <= nmbr; i++) {
        if(mstr[i].nwcolor != COLOR_BCKGRND) {
            master[mstr[i].nwcolor].grdntang = mstr[i].grdntang;            
            master[mstr[i].nwcolor].length = mstr[i].length;
            master[mstr[i].nwcolor].tag = 1;
        }
    }    
}

int Moment_Compute(KScScalarImage2dInt32 *lsr   , //?
				   KScScalarImage2dUint8 *mag   , 
				   mmstr_type            *mmstr , 
				   int                    nwnmbr)
{
    long ix, iy;
    long tmp, tmpmag;
    register int color;
    int width, height;
    KSdUint8 **pmag;
    KSdInt32 **plsr;
    
    // For buffer manage attach ...
	lsr->Attach();    
	
    if(mag->GetId() != KS_SCALAR_IMAGE_2D_UINT8) return FALSE;
    if(lsr->GetId() != KS_SCALAR_IMAGE_2D_INT32) return FALSE;
    
    plsr   = lsr->Get2dArray();
    pmag   = mag->Get2dArray();

    width  = lsr->GetXSize();
    height = lsr->GetYSize();

	//initialize monent    
    for(register int i = 1; i <= nwnmbr; i++) 
	{
        mmstr[i].m00 = mmstr[i].m10 = mmstr[i].m01 = 0;
        mmstr[i].m11 = mmstr[i].m20 = mmstr[i].m02 = 0;
    }

	//compute moment
    for(iy = 1; iy < height; iy++) {
        for(ix = 1; ix < width; ix++) {
            if((color = plsr[iy][ix]) != COLOR_BCKGRND) {                
                tmpmag = (long)pmag[iy][ix];
                mmstr[color].m00 = mmstr[color].m00 + tmpmag;
                tmp = ix * tmpmag;
                mmstr[color].m10 = mmstr[color].m10 + tmp;
                mmstr[color].m11 = mmstr[color].m11 + tmp * iy;
                mmstr[color].m20 = mmstr[color].m20 + tmp * ix;
                if(mmstr[color].m20<0)
					int bdjv=0; //?
				tmp = iy * tmpmag;
                mmstr[color].m01 = mmstr[color].m01 + tmp;
                mmstr[color].m02 = mmstr[color].m02 + tmp * iy;
                if(mmstr[color].m20<0)
					int hvcjhvu=0; //?
            }
        }
    }

    return TRUE;
}

void Orient_Compute(mmstr_type *mmstr, int nwnmbr, master_type *master) //?
{
    double ang, tang;
    double u11, u20, u02, mx, my;
    int i;
    double adjst = 4.0 / PI180;
	
//	CString Sec;//for debug

    for (i = 1; i<=nwnmbr; i++)
	{
		/* compute mx,my and centeral moments  */
		mx  = (double)mmstr[i].m10 /    (double)mmstr[i].m00;
		my  = (double)mmstr[i].m01 /    (double)mmstr[i].m00;
		u11 = (double)mmstr[i].m11 - my*(double)mmstr[i].m10;
		u20 = (double)mmstr[i].m20 - mx*(double)mmstr[i].m10;
		u02 = (double)mmstr[i].m02 - my*(double)mmstr[i].m01;
		master[i].xend = (int)mx; //save center of region
		master[i].yend = (int)my;
		
		if (u20 != u02)
			ang = 0.5*atan2( 2.0*u11, double(u20-u02) );
		else if (u11>0) ang = PI90;
		else ang = -PI90;
		
		ang = 0.5*atan2( 2.0*u11, double(u20-u02) );
		
		/* minor axis */
		if (mmstr[i].m20 < mmstr[i].m02 )  
			ang =  ang;
		if (ang<0) ang = PI180 + ang;
		master[i].ornt = ang;
		
		tang = abs(master[i].grdntang);
		if (tang>=2 && tang<=5)
			master[i].ornt = ang;
		else
			master[i].ornt = ang + PI180;	
	}			
}

void Tdefine_Ends(master_type *master, int num_regions)
{
    int i, dx, dy;
    double hlength;

    for(i = 1; i <= num_regions; i++) {
        if(master[i].length > 0) {
            hlength = (double)master[i].length / 2.0;
            dx = (int)(hlength * cos((double)master[i].ornt));
            dy = (int)(hlength * sin((double)master[i].ornt));
            master[i].xstart = master[i].xend - dx;
            master[i].ystart = master[i].yend - dy;
            master[i].xend   = master[i].xend + dx;
            master[i].yend   = master[i].yend + dy;
        }
        else { // 창범 : 이 부분은 내가 추가한 루틴
               // 초기화를 하지 않아서 이상한 값이 들어있는 부분을 0으로 만든다.
            master[i].xstart = 0;
            master[i].ystart = 0;
            master[i].xend   = 0;
            master[i].yend   = 0;
        }
    }
}

void LineTo(int x0, int y0, int x1, int y1, KScScalarImage2dUint8 *GrayOutput, KSdUint8 val)
{
	int dy = y1 - y0;
	int dx = x1 - x0;
	float t = (float) 0.5;
	int DX  = GrayOutput->GetXSize();
    int DY = GrayOutput->GetYSize();
 
	if(abs(dx) > abs(dy))
	{
		float m = (float) dy / dx;
		t += y0;
		dx = (dx < 0) ? -1 : 1;
		m *= dx;
		while(x0 != x1)
		{
			x0 += dx;
			t += m;
			if(x0<0||t<0||x0>=DX||t>=DY) continue;
			GrayOutput->SetPixel(val, (int)x0, (int)t);
//			GrayOutput->SetPixel(g, (int)x0, (int)t);
//			GrayOutput->SetPixel(b, (int)x0, (int)t);
		}
	}
	else
	{
		float m = (float) dx / dy;
		t += x0;
		dy = (dy < 0) ? -1 : 1;
		m *= dy;
		while(y0 != y1)
		{
			y0 += dy;
			t += m;
			if(y0<0||t<0||y0>=DY||t>=DX) continue;
			GrayOutput->SetPixel(val, (int)t, (int)y0);
//			GrayOutput->SetPixel(g, (int)t, (int)y0);
//			GrayOutput->SetPixel(b, (int)t, (int)y0);
		}
	}
}   


void sortLineByLength(LineSegmentType lineOb[int(TOTAL_DEGREE/GROUP_ANGLE)][100], int lineIndex[int(TOTAL_DEGREE/GROUP_ANGLE)])
{

	int i,j,k;
	LineSegmentType tobj;
	int totalLine=int(TOTAL_DEGREE/GROUP_ANGLE);

	for(i=0;i<totalLine;i++)
	{
		for(j=0;j<lineIndex[i]-1;j++)
		{
			for(k=j+1; k<lineIndex[i]; k++)
			{
				if(lineOb[i][j].getLength()<lineOb[i][k].getLength())
				{
					tobj=lineOb[i][j];
					lineOb[i][j]=lineOb[i][k];
					lineOb[i][k]=tobj;
				}
			}
		
		}
	
	}



}      


float parallelDistance(LineSegmentType ob1, LineSegmentType ob2)
{
	int xm,ym,x1,y1,x2,y2;
	float a,b,c,h,s, area;

	ob1.getMidPoint(&xm, &ym);
	ob2.getLine(&x1,&y1,&x2,&y2);

	a=sqrt((float)(x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)); //base
	b=sqrt((float)(x1-xm)*(x1-xm)+(y1-ym)*(y1-ym)); 
	c=sqrt((float)(x2-xm)*(x2-xm)+(y2-ym)*(y2-ym)); 

	s=(a+b+c)/2.0;

	area=sqrt(s*(s-a)*(s-b)*(s-c));

	h=(2.0*area/a); //parallel distance

	return h;
}

void mergeZeroDistanceParallelLines(LineSegmentType lineOb[int(TOTAL_DEGREE/GROUP_ANGLE)][100],int lineIndex[int(TOTAL_DEGREE/GROUP_ANGLE)])
{
	int i,j,k, flag=0,l;
	int x1,y1,x2,y2,x3,y3,x4,y4;
	float d1,d2,d3,d4,d5,d6, dmin,dmax,c1,c2,slope;

	LineSegmentType objMin, objMax;

	int totalLine=int(TOTAL_DEGREE/GROUP_ANGLE);
	
	for(i=0;i<totalLine;i++)
	{
		for(j=0;j<lineIndex[i]-1;j++)
		{
			for(k=j+1; k<lineIndex[i]; k++)
			{
				flag=0;
				if(parallelDistance(lineOb[i][j], lineOb[i][k])<=6)
				{
					//merge short lines to make a big line....
					//merge lineOb[i][j], lineOb[i][k] to lineOb[i][j]
					lineOb[i][j].getLine(&x1,&y1,&x2,&y2);
					lineOb[i][k].getLine(&x3,&y3,&x4,&y4);

					d1=sqrt((float)(x1-x3)*(x1-x3)+(y1-y3)*(y1-y3)); //base
					d2=sqrt((float)(x2-x3)*(x2-x3)+(y2-y3)*(y2-y3)); //base
					d3=sqrt((float)(x1-x4)*(x1-x4)+(y1-y4)*(y1-y4)); //base
					d4=sqrt((float)(x2-x4)*(x2-x4)+(y2-y4)*(y2-y4)); //base

					dmin=d1;
					if(dmin>d2) dmin=d2;
					if(dmin>d3) dmin=d3;
					if(dmin>d4) dmin=d4;
					
					if(dmin<30.0)
					{	
						flag=1;	
						dmax=d1;
						slope=lineOb[i][j].getSlope();
						lineOb[i][j].getSideColor(&c1,&c2);
						objMax.setLineSegment(x1,y1,x3,y3,slope,c1,c2);
						if(dmax<d2)
						{
						dmax=d2;
						objMax.setLineSegment(x2,y2,x3,y3,slope,c1,c2);
						}
						if(dmax<d3)
						{
						dmax=d3;
						objMax.setLineSegment(x1,y1,x4,y4,slope,c1,c2);
						}
						if(dmax<d4)
						{
						dmax=d4;
						objMax.setLineSegment(x2,y2,x4,y4,slope,c1,c2);
						}

						d5=sqrt((float)(x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)); //base
						d6=sqrt((float)(x3-x4)*(x3-x4)+(y3-y4)*(y3-y4)); //base

						if(dmax<d5)
						{
							dmax=d5;
							objMax=lineOb[i][j];
						}
						if(dmax<d6)
						{
							dmax=d6;
							objMax=lineOb[i][k];
						}
					}
					if(flag==1)
					{
						for(l=k;l<lineIndex[i];l++)
						{
							lineOb[i][l]=lineOb[i][l+1];
						}
						lineOb[i][j]=objMax;
						lineIndex[i]--;
						i=0;
						j=0;
						break;
					}
					
				}
			}
		
		}
	
	}

}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           