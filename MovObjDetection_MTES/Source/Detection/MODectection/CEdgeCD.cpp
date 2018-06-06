#include "CEdgeCD.h"




#define MOTION_THRESHOLD 10
#define MAX_SEGMENT_SIZE 40

static EdgeListType *TGroupEdgeList;
static EdgeClusterList *clusterList;
static EdgeClusterList *tmpClusterList;
static EdgeListType *MatchedEdgeList;
static EdgeListType *tEdgeList;
static TorsoModel *TM;
static UpperArmModel *URAM;
static UpperArmModel *ULAM;
static LowerArmModel *LLAM;
static LowerArmModel *LRAM;
static HeadModel *HM;


void maskMovingRegion(KSdUint8 **curImage, KSdUint8 **pOutputImage, EdgeListType *movEdgeList, int dx, int dy)
{

	int i,j,k,l;
	list<EdgeSegmentType>::iterator curEdgeSeg;
	for(i=0;i<dx;i++)
	{
		for(j=0;j<dy;j++)
		{
			pOutputImage[j][i]=0;
		}
	}
	
	KScScalarImage2dUint8* dstImage = NULL;
	dstImage = new KScScalarImage2dUint8;
	if(dstImage == NULL)
	{
		::MessageBox(NULL, "memory allocation error.",
			  "iBurnsLineUint8", MB_OK);
			return;
	}
	dstImage->Alloc(dx,dy);
	KSdUint8 **pDestinationArray = dstImage->Get2dArray();

/*
//new 7x7 low pass filter
	double result;
	int ex,ey,flag,x,y;
	for (y = 3, ex = dx - 3, ey = dy - 3 ; y < ey ; y++)
		for (x = 3 ; x < ex ; x++)
		{	
			result = (   curImage[y-3][x-3] +
					 2 * curImage[y-3][x-2] +
					 4 * curImage[y-3][x-1] +
					 8 * curImage[y-3][x  ] +
					 4 * curImage[y-3][x+1] +
					 2 * curImage[y-3][x+2] +
						 curImage[y-3][x+3] +

					 2 * curImage[y-2][x-3] +
					 4 * curImage[y-2][x-2] +
					 8 * curImage[y-2][x-1] +
					16 * curImage[y-2][x  ] +
					 8 * curImage[y-2][x+1] +
					 4 * curImage[y-2][x+2] +
					 2 * curImage[y-2][x+3] +

					 4 * curImage[y-1][x-3] +
					 8 * curImage[y-1][x-2] +
					16 * curImage[y-1][x-1] +
					32 * curImage[y-1][x  ] +
					16 * curImage[y-1][x+1] +
					 8 * curImage[y-1][x+2] +
					 4 * curImage[y-1][x+3] +

					 8 * curImage[y][x-3] +
					16 * curImage[y][x-2] +
					32 * curImage[y][x-1] +
					64 * curImage[y][x  ] +
					32 * curImage[y][x+1] +
					16 * curImage[y][x+2] +
 					 8 * curImage[y][x+3] +

					 4 * curImage[y+1][x-3] +
					 8 * curImage[y+1][x-2] +
					16 * curImage[y+1][x-1] +
					32 * curImage[y+1][x  ] +
					16 * curImage[y+1][x+1] +
					 8 * curImage[y+1][x+2] +
					 4 * curImage[y+1][x+3] +

					 2 * curImage[y+2][x-3] +
					 4 * curImage[y+2][x-2] +
					 8 * curImage[y+2][x-1] +
					16 * curImage[y+2][x  ] +
					 8 * curImage[y+2][x+1] +
					 4 * curImage[y+2][x+2] +
					 2 * curImage[y+2][x+3] +

					     curImage[y+3][x-3] +
					 2 * curImage[y+3][x-2] +
					 4 * curImage[y+3][x-1] +
					 8 * curImage[y+3][x  ] +
					 4 * curImage[y+3][x+1] +
					 2 * curImage[y+3][x+2] +
					     curImage[y+3][x+3] ) / 484.0 + 0.5;

			pOutputImage[y][x] = (unsigned char)min((max (0, result)), 255);
		}*/


	double       temp;
	register int x = dx - 1, y = dy - 1;
	for (register int i = 1, j ; i < y ; i++)
		for (j = 1 ; j < x ; j++)
		{	
			temp = (4 * curImage[i    ][j    ] +
						curImage[i - 1][j + 1] +
					    curImage[i + 1][j + 1] +
						curImage[i - 1][j - 1] +
						curImage[i + 1][j - 1] +
					2 * curImage[i - 1][j    ] +
					2 * curImage[i    ][j - 1] +
					2 * curImage[i    ][j + 1] +
					2 * curImage[i + 1][j    ]) / 16.0 + 0.5;

			pOutputImage[i][j] = (unsigned char)min((max (0, temp)), 255);
		}



		for (y = 0; y < dy ; y++)
		for (x = 0 ; x < dx ; x++)
		{	
			pDestinationArray[y][x]=pOutputImage[y][x];
			pOutputImage[y][x]=0;
		}



	
	
	for(curEdgeSeg=movEdgeList->ListBegin();curEdgeSeg!=movEdgeList->ListEnd();curEdgeSeg++)
	{
		PointType pt;
			for(i=0;i<(*curEdgeSeg).GetSize();i++)
			{
				pt=(*curEdgeSeg).GetPoint(i);
			//	pOutputImage[pt.GetY()][pt.GetX()]=curImage[pt.GetY()][pt.GetX()];

				for(k=-5;k<=5;k++)
				{
					for(l=-5;l<=5;l++)
					{
						if(pt.GetY()+k<0||pt.GetX()+l<0||pt.GetY()+k>=dy||pt.GetX()+l>=dx) continue;
						pOutputImage[pt.GetY()+k][pt.GetX()+l]=pDestinationArray[pt.GetY()+k][pt.GetX()+l];
					}
				}
			}
	}
}

void labelSort(int *A, int n)
{
	int i,j, value, done=0;

    for (i=1;i<n-1;i++)
	{
        value = A[i];
        j = i - 1;
        done = 0;
        do
		{
            if (A[j] > value)
			{
                A[j + 1] = A[j];
                j = j - 1;
                if (j < 0) done = 1;
            }
            else
                done = 1;
		}
        while(done==0);
        A[j + 1] = value;
		}
	
}
int maxFrequency(int *lValues,int size,int *Labelcount){
	int i,curValue, MaxValue, count=1, MaxCount=0;

	curValue=MaxValue=lValues[0];
	for(i=1;i<size;i++)
	{
		if(curValue==lValues[i]) count++;
		else
		{
			if(MaxCount<count)
			{
				MaxCount=count;
				MaxValue=curValue;
			}

			curValue=lValues[i];
			count=1;
		}
	}

	if(MaxCount<count)
	{
		MaxCount=count;
		MaxValue=curValue;
	}

*Labelcount=MaxCount;
return MaxValue;
}

float matchShape(EdgeSegmentType refEdgeSeg, EdgeSegmentType curEdgeSeg, int *position){
float hscore=0;
return hscore;
}

void displayClusterList(EdgeClusterList *clusterList, KSdUint8 ** pOutputImage)
{
int size,i;
PointType p;
list<EdgeClusterType>::iterator curCluster;
list<EdgeSegmentType>::iterator curEdgeSeg;


for(curCluster=clusterList->ListBegin();curCluster!=clusterList->ListEnd();curCluster++)
	{
		if((*curCluster).getTotalEdge()>0)
		{
		for(curEdgeSeg=(*curCluster).ListBegin();curEdgeSeg!=(*curCluster).ListEnd();curEdgeSeg++) 
		{
		//	if(curEdgeSeg->getWeight()>=95)
			{
			size=(*curEdgeSeg).GetSize();
			for( i=0 ; i<size ; i++)
			{
				p=(*curEdgeSeg).GetPoint(i);
			    pOutputImage[p.GetY()][p.GetX()] = 255;
			}
			}
		}
		showClusterWindow(&(*curCluster), pOutputImage);
		}
	}
}


void displayArrayImage(KSdUint8 ** pOutputImage,int **pMovLabel, int dx,int dy)
{	
int i,j;
	
	for(i=0; i<dy ; i++ )
		for( j=0 ; j<dx ; j++ )
			pOutputImage[i][j] = pMovLabel[i][j];

}

void displayImagePoint(KSdUint8 ** pOutputImage,int dx,int dy, int Sx, int Sy)
{	
	if(Sx<=dx&&Sy<=dy) pOutputImage[Sy][Sx] = 255;

}

void drawMotionArrow(KSdUint8 **pOutputImage, EdgeListType *refEdgeList,int dx, int dy)
{
list<EdgeSegmentType>::iterator curEdgeSeg;
int maxx,maxy,minx,miny,Xc,Yc;
float mx,my;

	for(curEdgeSeg=refEdgeList->ListBegin();curEdgeSeg!=refEdgeList->ListEnd();curEdgeSeg++)
	{
		(*curEdgeSeg).getBoundaryParam(&maxx,&maxy,&minx,&miny);
		Xc=(maxx+minx)/2;
		Yc=(maxy+miny)/2;
		(*curEdgeSeg).getMotion(&mx,&my);
		drawArrow(pOutputImage,Xc,Yc,mx,my,dx,dy);
	}
}



void drawEdgeBoundary(KSdUint8 **pOutputImage, EdgeListType *refEdgeList,int color)
{
list<EdgeSegmentType>::iterator curEdgeSeg;
int maxx, maxy,minx,miny;
	
	for(curEdgeSeg=refEdgeList->ListBegin();curEdgeSeg!=refEdgeList->ListEnd();curEdgeSeg++)
	{
		(*curEdgeSeg).getBoundaryParam(&maxx,&maxy,&minx,&miny);
		drawrec(pOutputImage, minx, miny, maxx, maxy, color);
	}
}



void displayCurvaturePoints(KSdUint8 **pOutputImage, EdgeListType *refEdgeList,int dx,int dy)
{
	PointType p;
	float cValue;
	list<EdgeSegmentType>::iterator curEdgeSeg;
	int size;
	for(curEdgeSeg=refEdgeList->ListBegin();curEdgeSeg!=refEdgeList->ListEnd();curEdgeSeg++)
	{
		size=(*curEdgeSeg).GetSize();
		for(int i=0 ; i<(size/SAMPLE_LENGTH); i++)
		{
		p=(*curEdgeSeg).GetCurvaturePoint(i);
		cValue=(*curEdgeSeg).GetCurvatureValue(i);
		
			if(cValue*200>255) pOutputImage[p.GetY()][p.GetX()] = 255;
			else pOutputImage[p.GetY()][p.GetX()] = cValue*200;
		
		}
	}

}
void displayList(KSdUint8 **pOutputImage, EdgeListType *refEdgeList,int dx,int dy)
{	
	int ypos=0;
	int xpos=0,size,i;
	PointType p;
	list<EdgeSegmentType>::iterator curEdgeSeg;
	for(curEdgeSeg=refEdgeList->ListBegin();curEdgeSeg!=refEdgeList->ListEnd();curEdgeSeg++)
	{
		//Initializing distance, size and maximum matching label counter
		//size=(*curEdgeSeg).GetSize()-1;
		size=(*curEdgeSeg).GetSize();
		for(i=0 ; i<size ; i++)
		{
		//Extracting ith points of current edge segments
	//	p=(*curEdgeSeg).GetPoint(size/2);
		p=(*curEdgeSeg).GetPoint(i);
		//float tangentdirX=secondDerivative((*curEdgeSeg).GetPoint(size/2-1).GetX(),(*curEdgeSeg).GetPoint(size/2).GetX(),(*curEdgeSeg).GetPoint(size/2+1).GetX());
//		int x_inc=cos(atan(tangentdirX));
//		float tangentdirY=secondDerivative((*curEdgeSeg).GetPoint(size/2-1).GetY(),(*curEdgeSeg).GetPoint(size/2).GetY(),(*curEdgeSeg).GetPoint(size/2+1).GetY());
//		int y_inc=cos(atan(tangentdirY));
			

		//pOutputImage[p.GetY()+ypos][p.GetX()+xpos]= (*curEdgeSeg).getSideColor(1);
		//pOutputImage[p.GetY()-ypos][p.GetX()-xpos]= (*curEdgeSeg).getSideColor(2);
		pOutputImage[p.GetY()][p.GetX()] = 255;
		//pOutputImage[p.GetY()][p.GetX()] = 255-(*curEdgeSeg).GetSegIndex();
		}
	}
}



CEdgeCD::CEdgeCD(KScScalarImage2dUint8 *srcImg)
{
	accGradM   = NULL ;

	curImage=srcImg;
	refDistImage  = NULL ;
	refLabelImage=NULL;
	movDistImage  = NULL ;
	movLabelImage=NULL;
//	refEdgeList    = NULL ;
	curEdgeList    = NULL ;
	movEdgeList = NULL ; 
//	TGroupEdgeList =NULL;
	tempEdgeList    = NULL ;
}

CEdgeCD::~CEdgeCD()
{
	DeleteEdgeList  ();
//	DeleteCanny     ();
	DeleteInOutImage();
}

static int FrameNumber=0;
////////////////////////////////////////////////////
//Creating Edgelist
void CEdgeCD::CreateEdgeList()
{
//	refEdgeList    = new EdgeListType();
	
	curEdgeList    = new EdgeListType();
	
	movEdgeList	   = new EdgeListType();

//	TGroupEdgeList	 = new EdgeListType();
		
	tempEdgeList   = new EdgeListType();	

	modelEdgeList   = new EdgeListType();
}

//Removing Edge List
void CEdgeCD::DeleteEdgeList()
{
//	delete refEdgeList   ;
	delete curEdgeList;
	delete movEdgeList;
	delete tempEdgeList;
	delete modelEdgeList;
//	delete TGroupEdgeList;

//	refEdgeList   = NULL;
	curEdgeList   = NULL;
	movEdgeList= NULL;
	tempEdgeList   = NULL;
//	TGroupEdgeList=NULL;
}

/*
////////////////////////////////////////////////////
// Creating accumulation image
void CEdgeCD::CreateAccImage()
{
	accGradM = new KScScalarImage2dFloat(curImage->GetXSize(),curImage->GetYSize());

}

//Deleting Accumulation image
void CEdgeCD::DeleteAccImage()
{
	delete m_pcAccHx   ;
	delete m_pcAccHy   ;
	delete m_pcAccGradM;
}
*/
//Allocating the input output image
void CEdgeCD::AllocInOutImage()
{
//	m_pcSrcImg   = new KScScalarImage2dUint8(m_nX,m_nY);
	refDistImage= new KScScalarImage2dInt32(curImage->GetXSize(),curImage->GetYSize());
	refLabelImage= new KScScalarImage2dInt32(curImage->GetXSize(),curImage->GetYSize());
	movDistImage= new KScScalarImage2dInt32(curImage->GetXSize(),curImage->GetYSize());
	movLabelImage= new KScScalarImage2dInt32(curImage->GetXSize(),curImage->GetYSize());
	modelDistImage= new KScScalarImage2dInt32(curImage->GetXSize(),curImage->GetYSize());
	modelLabelImage= new KScScalarImage2dInt32(curImage->GetXSize(),curImage->GetYSize());
	maskedInputImage=new KScScalarImage2dUint8(curImage->GetXSize(),curImage->GetYSize());


}

//Deleting input output image
void CEdgeCD::DeleteInOutImage()
{
	delete refDistImage;
	delete refLabelImage;
	delete movDistImage;
	delete movLabelImage;
	delete modelDistImage;
	delete modelLabelImage;
	delete maskedInputImage;

}
//

//Class Method
void CEdgeCD::InitEdgeCD()
{

	CreateEdgeList  ();
	AllocInOutImage ();
}


void CEdgeCD::SetCurImg(KScScalarImage2dUint8 *img)
{
	*curImage  = *img;
}

//Main Method
//nFrame is the image number or frame number




void CEdgeCD::SubtractTemporaryEdge(KSdUint8 ** pOutputImage, int dx, int dy)
{
	list<EdgeSegmentType>::iterator curEdgeSeg, tmpEdgeSeg;

		tEdgeList->ListToDistanceImage(movDistImage,movLabelImage);
		int **pMovDistance = movDistImage->Get2dArray();
		int **pMovLabel = movLabelImage->Get2dArray();
		//displayList(pOutputImage,movEdgeList, dx,dy);
		int segI[100],n;
	//	displayArrayImage(pOutputImage,pMovDistance, dx,dy);
		int flag=1, flag2,i;
		for(curEdgeSeg=movEdgeList->ListBegin();curEdgeSeg!=movEdgeList->ListEnd();)
		{
			flag=1;
			
			if(edgeMatch((*curEdgeSeg),tEdgeList,pMovDistance,pMovLabel,dx,dy, segI,n)==1)//add matched edges to group and erase them from the list
			{
				for(i=0;i<n;i++)
				{
					for(tmpEdgeSeg=tEdgeList->ListBegin();tmpEdgeSeg!=tEdgeList->ListEnd();tmpEdgeSeg++)
					{
					if((*tmpEdgeSeg).GetSegIndex()==segI[i])
					{
							if((*tmpEdgeSeg).getFrameIndex()<600)
							(*tmpEdgeSeg).setFrameIndex(((*tmpEdgeSeg).getFrameIndex()+5));
							if((*tmpEdgeSeg).getFrameIndex()>400)
							{	
								if(flag==0) continue;
								curEdgeSeg=movEdgeList->DeleteSegment(curEdgeSeg);
							    flag=0;
							}
					break;
					}
					}
				}
			}
			else
			{
			(*curEdgeSeg).setFrameIndex(10);
			tEdgeList->AddNewSegment(&(*curEdgeSeg));
			}
			if(flag==1) curEdgeSeg++;
			flag=1;
		}


		for(curEdgeSeg=tEdgeList->ListBegin();curEdgeSeg!=tEdgeList->ListEnd();)
		{
			flag=1;
			(*curEdgeSeg).setFrameIndex((*curEdgeSeg).getFrameIndex()-4);
			if((*curEdgeSeg).getFrameIndex()<1){
				curEdgeSeg=tEdgeList->DeleteSegment(curEdgeSeg);
				flag=0;
			}
			if(flag==1) curEdgeSeg++;
		}
//displayList(pOutputImage,movEdgeList, dx,dy);
}

void CEdgeCD::DoEdgeCD(EdgeListType* refEdgeList, KScScalarImage2dUint8 *outputImage)
{
	InitEdgeCD();
	//	refEdgeList = refList;
	refDistImage->InitTo(0);
	KSdUint8 **pOutputImage = outputImage->Get2dArray();
	

	//Generation of labeled initial reference image for edge subtraction
//	refEdgeList->ListToLabelImage(refLabelImage);   //not necessary
	//creates labeled distance image
	refEdgeList->ListToDistanceImage(refDistImage,refLabelImage);
	int **pRefMarker = refDistImage->Get2dArray();
	int **pRefLabel =refLabelImage->Get2dArray();
	int count,Cx,Cy;
	int *movX=0,*movY=0;
	int Trx,Try,theta;
	int RArx,RAry,LArx,LAry;
	int LLARX,LLARY,LRARX,LRARY;
	int HRX,HRY;

	float scale;
	
	float centerX, centerY;
	float sumX, sumY, meanX,meanY;
	//float varX, varY;
	float matchScore;
	

	int dx = curImage->GetXSize();
	int dy = curImage->GetYSize();
	
//	int maxx,maxy, minx, miny;
	list<EdgeClusterType>::iterator curCluster;
	list<EdgeClusterType>::iterator tmpCluster;
	list<EdgeSegmentType>::iterator curEdgeSeg, tmpEdgeSeg;

	curEdgeList->EdgeListClear();
	movEdgeList->EdgeListClear();
	modelEdgeList->EdgeListClear();
	

	//CEdgeSubtract edgeSub(refEdgeList,tempEdgeList);
	SubtractEdge(refEdgeList);
	//BreakMoingEdge();
	RemoveScattered(20);

///moving line detection starts from here.................

	KSdUint8 **pMaskedImage= maskedInputImage->Get2dArray();
    KSdUint8 **pInputImage= curImage->Get2dArray();
	maskMovingRegion(pInputImage,pMaskedImage,movEdgeList, dx,dy);

	 int threshold, option, split_on, length_on, len_thd, totalAngle,startAngle;
	 
	 threshold=1;
	 option=1;
	 split_on=1;
	 length_on=1;
	 len_thd=10;
	 totalAngle=int(TOTAL_DEGREE/GROUP_ANGLE);

	 LineSegmentType lineOb[int(TOTAL_DEGREE/GROUP_ANGLE)][100];
	 int lineIndex[int(TOTAL_DEGREE/GROUP_ANGLE)];
	 int x1,y1,x2,y2,i,j;

	 for(int i=0;i<totalAngle;i++)
		 lineIndex[i]=0;

	 iBurnsLineExDLLUint8(lineOb, lineIndex, maskedInputImage, outputImage, &threshold,&option,&split_on,&length_on,&len_thd);	
	 
	 sortLineByLength(lineOb, lineIndex);

	 mergeZeroDistanceParallelLines(lineOb, lineIndex);
	 
	 sortLineByLength(lineOb, lineIndex);

//	 showTorsoCandidates(lineOb, lineIndex);

	  startAngle=totalAngle/2-1;
	 //for(i=0;i<totalAngle;i++)
	 for(i=startAngle-2;i<startAngle+2;i++)
	 {		 
		 for(j=0;j<lineIndex[i%totalAngle];j++)
		 {
			// if(lineIndex[i%totalAngle]<2) continue;
			 lineOb[i%totalAngle][j].getLine(&x1,&y1,&x2,&y2);
			 LineTo(x1,y1,x2,y2,outputImage,255); 	
		 }
		if(TOTAL_DEGREE==360)
		 {
		 for(j=0;j<lineIndex[(i+180/GROUP_ANGLE)%totalAngle];j++)
		 {
			// if(lineIndex[i%totalAngle]<2) continue;
			 lineOb[(i+180/GROUP_ANGLE)%totalAngle][j].getLine(&x1,&y1,&x2,&y2);
			 LineTo(x1,y1,x2,y2,outputImage,255); 	
		 }
		}
	 }
	
	
//displayList(pOutputImage,movEdgeList, dx,dy);
	

///moving line detection ends here....

	Trx=dy/2;
	Try=dx/2;
	scale=1;
	theta=0;
	//frame no. 355
	/*rx=dx/2;
	ry=dy/2;
	scale=1;
	theta=0;*/

/* //uncomment to execute model
	if(FrameNumber==0)
	{
		int count=0;
		TGroupEdgeList = new EdgeListType();
		TGroupEdgeList->EdgeListClear();
		clusterList=new EdgeClusterList();

		for(curEdgeSeg=movEdgeList->ListBegin();curEdgeSeg!=movEdgeList->ListEnd();curEdgeSeg++)
		{
			curEdgeSeg->setMotion(0,0);
			curEdgeSeg->setWeight(100);
			TGroupEdgeList->AddNewSegment((*curEdgeSeg));	
		}
	FrameNumber=1;
	TM=new TorsoModel(Trx,Try,scale,theta);
	URAM=new UpperArmModel(Trx,Try,scale,theta);
	ULAM=new UpperArmModel(Trx,Try,scale,theta);
	LRAM=new LowerArmModel(Trx,Try,scale,theta);
	LLAM=new LowerArmModel(Trx,Try,scale,theta);
	HM=new HeadModel(Trx,Try,scale,theta); 
	}
	else{
		//displayList(pOutputImage,movEdgeList, dx,dy);
		displayList(pOutputImage,TGroupEdgeList, dx,dy);
		//displayList(pOutputImage,movEdgeList, dx,dy);
		//displayCurvaturePoints(pOutputImage,TGroupEdgeList, dx,dy);
		//drawEdgeBoundary(pOutputImage,TGroupEdgeList,150);
		
		TGroupEdgeList->ListToDistanceImage(movDistImage,movLabelImage);
		for(curEdgeSeg=TGroupEdgeList->ListBegin();curEdgeSeg!=TGroupEdgeList->ListEnd();curEdgeSeg++)
		{
		findHoughEdgeMotion(pOutputImage, &(*curEdgeSeg), movEdgeList, movDistImage, movLabelImage, *movX, *movY, dx, dy);
		//VerifyEdgeShapeAndColor(pOutputImage, &(*curEdgeSeg), movEdgeList, movDistImage, movLabelImage, *movX, *movY, dx, dy);
		}

		//drawMotionArrow(pOutputImage,TGroupEdgeList,dx,dy);
		//fitModel(pOutputImage,TGroupEdgeList,dx,dy);	
		
		Trx=dy/2+46;
		Try=dx/2+30;
		theta=0;
		TM->rotateTorsoModel(Trx, Try, theta);
		TM->displayTorsoModel(pOutputImage, dx,dy);

		theta=0;
		TM->displayTorsoModel(pOutputImage, dx,dy);
		//TM->getTorsoEdgeList(modelEdgeList);
		//displayList(pOutputImage,modelEdgeList,dx,dy);
		theta=-70+rand()%20;
		TM->getRightHandReferencePoint(RArx,RAry);
		URAM->rotateUpperArmModel(RArx,RAry,theta);
		URAM->displayUpperArmModel(pOutputImage, dx,dy);

		theta=70+rand()%20;
		TM->getLeftHandReferencePoint(LArx,LAry);
		ULAM->rotateUpperArmModel(LArx,LAry,theta);
		ULAM->displayUpperArmModel(pOutputImage, dx,dy);
		

		theta=-95+rand()%20;
		URAM->getLowerArmReferencePoint(LLARX,LLARY);
		LLAM->rotateLowerArmModel(LLARX,LLARY,theta);
		LLAM->displayLowerArmModel(pOutputImage, dx,dy);
		
				
		theta=95+rand()%20;
		ULAM->getLowerArmReferencePoint(LRARX,LRARY);
		LRAM->rotateLowerArmModel(LRARX,LRARY,theta);
		LRAM->displayLowerArmModel(pOutputImage, dx,dy);
		
		theta=rand()%10;
		TM->getHeadReferencePoint(HRX,HRY);
		HM->rotateHeadModel(HRX,HRY,theta);
		HM->displayHeadModel(pOutputImage, dx,dy);

		
		TGroupEdgeList->EdgeListClear();
		for(curEdgeSeg=movEdgeList->ListBegin();curEdgeSeg!=movEdgeList->ListEnd();curEdgeSeg++)
		{
			curEdgeSeg->setMotion(0,0);
			curEdgeSeg->setWeight(100);
			TGroupEdgeList->AddNewSegment((*curEdgeSeg));	
		}






	//	clusterList->ClusterListClear();
	//	for(curEdgeSeg=TGroupEdgeList->ListBegin();curEdgeSeg!=TGroupEdgeList->ListEnd();curEdgeSeg++) //selected matched edge list
	//	{
	//		clusterList->AddNewSegment(&(*curEdgeSeg));
	//	}
	//	displayClusterList(&(*clusterList),pOutputImage);

		
		
////////////setting group motion mean
for(curCluster=clusterList->ListBegin();curCluster!=clusterList->ListEnd();curCluster++)
	{	
		sumX=0;
		sumY=0;
		count=0;
		for(curEdgeSeg=(*curCluster).ListBegin();curEdgeSeg!=(*curCluster).ListEnd();curEdgeSeg++) 
		{/////////////for all segments in the cluster
			(*curEdgeSeg).getMotion(&movX,&movY);
			sumX+=movX;
			sumY+=movY;
			count++;
		}
	(*curCluster).setMotion(sumX/count,sumY/count);
	(*curCluster).getClusterCenter(&centerX,&centerY);
	(*curCluster).setOldMotion(centerX, centerY, sumX/count, sumY/count);	
}
/////end setting group motion

///////////setting group motion variance
for(curCluster=clusterList->ListBegin();curCluster!=clusterList->ListEnd();curCluster++)
	{	
	(*curCluster).getMotion(&meanX,&meanY);
		sumX=0;
		sumY=0;
		count=0;
		for(curEdgeSeg=(*curCluster).ListBegin();curEdgeSeg!=(*curCluster).ListEnd();curEdgeSeg++) 
		{/////////////for all segments in the cluster
			(*curEdgeSeg).getMotion(&movX,&movY);
			sumX+=(movX-meanX)*(movX-meanX);
			sumY+=(movY-meanY)*(movY-meanY);
			count++;
		}
	(*curCluster).setVariance((sumX/count),(sumY/count));
	}

//	displayList(pOutputImage,movEdgeList, dx,dy);
/////end setting group motion
	}
	*/
	
	/*
	if(FrameNumber==0){
	
	tEdgeList = new EdgeListType();
	MatchedEdgeList = new EdgeListType();
	MatchedEdgeList->EdgeListClear();
	}
	FrameNumber++;

	if(FrameNumber==1)
	{
		int count=0;
		TGroupEdgeList = new EdgeListType();
		TGroupEdgeList->EdgeListClear();
		for(curEdgeSeg=movEdgeList->ListBegin();curEdgeSeg!=movEdgeList->ListEnd();curEdgeSeg++)
		{
			curEdgeSeg->setMotion(0,0);
			curEdgeSeg->setWeight(100);
			TGroupEdgeList->AddNewSegment((*curEdgeSeg));	
		}
//	displayList(pOutputImage,movEdgeList, dx,dy);
//	displayList(pOutputImage,TGroupEdgeList, dx,dy);
	}
	else if(FrameNumber==2)
	{
	//	displayList(pOutputImage,movEdgeList, dx,dy);
		TGroupEdgeList->ListToDistanceImage(movDistImage,movLabelImage);
		int **pMovDistance = movDistImage->Get2dArray();
		int **pMovLabel = movLabelImage->Get2dArray();
	//	displayArrayImage(pOutputImage,pMovLabel, dx,dy);
		int flag=1, segI;
		clusterList=new EdgeClusterList();
		clusterList->ClusterListClear();
		tmpClusterList=new EdgeClusterList();
		tmpClusterList->ClusterListClear();


		for(curEdgeSeg=movEdgeList->ListBegin();curEdgeSeg!=movEdgeList->ListEnd();)
		{
			(*curEdgeSeg).setMotion(0,0);
			(*curEdgeSeg).setWeight(100);
			//edgeSegmentMatch((*curEdgeSeg),TGroupEdgeList,pMovDistance,pMovLabel,dx,dy, &matchScore, &movX, &movY, &segI);
			//if(edgeSegmentMatch(NULL,(*curEdgeSeg),TGroupEdgeList,pMovDistance,pMovLabel,dx,dy, &matchScore, &movX, &movY, &segI)==1)//add matched edges to group and erase them from the list
//		
//int ii;
//PointType pt;
//for(ii=0;ii<(*curEdgeSeg).GetSize();ii++)
//{	pt=(*curEdgeSeg).GetPoint(ii);
//	pOutputImage[pt.GetY()][pt.GetX()]=255;
//}
//displayList(pOutputImage,movEdgeList, dx,dy);
			if(edgeSegmentMatch(pOutputImage,NULL,(*curEdgeSeg),TGroupEdgeList,pMovDistance,pMovLabel,dx,dy, &matchScore, &movX, &movY, &segI)==1)//add matched edges to group and erase them from the list
			{
//
//int ii;
//PointType pt;
//for(ii=0;ii<(*curEdgeSeg).GetSize();ii++)
//{	pt=(*curEdgeSeg).GetPoint(ii);
//	pOutputImage[pt.GetY()][pt.GetX()]=255;
//}				
				(*curEdgeSeg).setMotion(movX,movY);
				MatchedEdgeList->AddNewSegment((*curEdgeSeg));
				curEdgeSeg=movEdgeList->DeleteSegment(curEdgeSeg);
				flag=0;
				for(tmpEdgeSeg=TGroupEdgeList->ListBegin();tmpEdgeSeg!=TGroupEdgeList->ListEnd();tmpEdgeSeg++)
					{
						if((*tmpEdgeSeg).GetSegIndex()==segI)
						{
					//	MatchedEdgeList->AddNewSegment((*tmpEdgeSeg));
						tmpEdgeSeg=TGroupEdgeList->DeleteSegment(tmpEdgeSeg);
						break;
						}
					}
			}
			if(flag==1) curEdgeSeg++;
			flag=1;
		}
		
		for(curEdgeSeg=movEdgeList->ListBegin();curEdgeSeg!=movEdgeList->ListEnd();curEdgeSeg++)
		{
			//(*curEdgeSeg).setMotion(0,0);
			//(*curEdgeSeg).setWeight(100);
			TGroupEdgeList->AddNewSegment((*curEdgeSeg));//add unmatched edges to TGgroup
			
		}


		clusterList->ClusterListClear();
		for(curEdgeSeg=MatchedEdgeList->ListBegin();curEdgeSeg!=MatchedEdgeList->ListEnd();curEdgeSeg++) //selected matched edge list
		{
		//assignSegmentToClusterList
	//	(*curEdgeSeg).getMotion(&movX, &movY);
		clusterList->AddNewSegment(&(*curEdgeSeg));
		}
//displayList(pOutputImage,TGroupEdgeList, dx,dy);
		
///////////setting group motion mean
for(curCluster=clusterList->ListBegin();curCluster!=clusterList->ListEnd();curCluster++)
	{	
		sumX=0;
		sumY=0;
		count=0;
		for(curEdgeSeg=(*curCluster).ListBegin();curEdgeSeg!=(*curCluster).ListEnd();curEdgeSeg++) 
		{/////////////for all segments in the cluster
			(*curEdgeSeg).getMotion(&movX,&movY);
			sumX+=movX;
			sumY+=movY;
			count++;
		}
	(*curCluster).setMotion(sumX/count,sumY/count);
	(*curCluster).getClusterCenter(&centerX,&centerY);
	(*curCluster).setOldMotion(centerX, centerY, sumX/count, sumY/count);	
}
/////end setting group motion

///////////setting group motion variance
for(curCluster=clusterList->ListBegin();curCluster!=clusterList->ListEnd();curCluster++)
	{	
	(*curCluster).getMotion(&meanX,&meanY);
		sumX=0;
		sumY=0;
		count=0;
		for(curEdgeSeg=(*curCluster).ListBegin();curEdgeSeg!=(*curCluster).ListEnd();curEdgeSeg++) 
		{/////////////for all segments in the cluster
			(*curEdgeSeg).getMotion(&movX,&movY);
			sumX+=(movX-meanX)*(movX-meanX);
			sumY+=(movY-meanY)*(movY-meanY);
			count++;
		}
	(*curCluster).setVariance((sumX/count),(sumY/count));
	}

//	displayList(pOutputImage,movEdgeList, dx,dy);
/////end setting group motion
}
	
else ////Frame Number more than two perform tracking here
{
	SubtractTemporaryEdge(pOutputImage,dx,dy);	


	for(curEdgeSeg=movEdgeList->ListBegin();curEdgeSeg!=movEdgeList->ListEnd();curEdgeSeg++)
	{
		curEdgeSeg->setMotion(0,0);
		curEdgeSeg->setWeight(100);
	}

	MatchedEdgeList->EdgeListClear();
	for(curCluster=clusterList->ListBegin();curCluster!=clusterList->ListEnd(); curCluster++)
	{///////////for all cluster
		for(curEdgeSeg=(*curCluster).ListBegin();curEdgeSeg!=(*curCluster).ListEnd();curEdgeSeg++) 
		{/////////////for all segments in the cluster
			(*curEdgeSeg).setClusterMemberID((*curCluster).getClusterIndex());
			MatchedEdgeList->AddNewSegment(&(*curEdgeSeg));
		}
	}

	MatchedEdgeList->ListToDistanceImage(movDistImage,movLabelImage);
	int **pMovDistance = movDistImage->Get2dArray();
	int **pMovLabel = movLabelImage->Get2dArray();
	int flag=1, found, segI;

	//displayList(pOutputImage,MatchedEdgeList, dx,dy);
	//displayArrayImage(pOutputImage,pMovDistance, dx,dy);
	assignClusterIDtoMovingSegments(pOutputImage,movEdgeList, MatchedEdgeList, pMovDistance, pMovLabel, dx, dy);
//	for(curEdgeSeg=movEdgeList->ListBegin();curEdgeSeg!=movEdgeList->ListEnd();curEdgeSeg++)
//	{
//		int ii=(*curEdgeSeg).getClusterMemberID();
//		if((*curEdgeSeg).getClusterMemberID()==-1) {
//			PointType pt;
///			for(ii=0;ii<(*curEdgeSeg).GetSize();ii++)
//			{	pt=(*curEdgeSeg).GetPoint(ii);
//				pOutputImage[pt.GetY()][pt.GetX()]=255;
//			}
//		}
//
//	}

	movEdgeList->ListToDistanceImage(movDistImage,movLabelImage);
	pMovDistance = movDistImage->Get2dArray();
	pMovLabel = movLabelImage->Get2dArray();

	for(curCluster=clusterList->ListBegin();curCluster!=clusterList->ListEnd();)
	{///////////for all cluster
		flag=1;
		found=findSelectedHoughClusterMatch(pOutputImage,&(*curCluster), movEdgeList, pMovDistance, pMovLabel, dx, dy, &Cx, &Cy);
		updateSelectedClusterMemberSegments(pOutputImage, &(*clusterList), &(*curCluster), movEdgeList, pMovDistance, pMovLabel, dx, dy, Cx, Cy, found);
		
		if((*curCluster).getTotalEdge()==0)
		{
			curCluster=(*clusterList).DeleteCluster(curCluster);
			flag=0;
		}
		if(flag==1) curCluster++;

		
	}


	MatchedEdgeList->EdgeListClear();
	movEdgeList->ListToDistanceImage(movDistImage,movLabelImage);
	pMovDistance = movDistImage->Get2dArray();
	pMovLabel = movLabelImage->Get2dArray();
	
	for(curEdgeSeg=TGroupEdgeList->ListBegin();curEdgeSeg!=TGroupEdgeList->ListEnd();)
	{
		flag=1;
		if(edgeSegmentMatch(pOutputImage,NULL,(*curEdgeSeg),movEdgeList,pMovDistance,pMovLabel,dx,dy, &matchScore, &movX, &movY, &segI)==1)
			//add matched edges to group and erase them from the list
		{
			curEdgeSeg->setMotion(-movX,-movY);
			for(tmpEdgeSeg=movEdgeList->ListBegin();tmpEdgeSeg!=movEdgeList->ListEnd();tmpEdgeSeg++)
			{
				if((*tmpEdgeSeg).GetSegIndex()==segI)
				{
					(*curEdgeSeg).setWeight((*curEdgeSeg).getWeight()+5);			
					tmpEdgeSeg=movEdgeList->DeleteSegment(tmpEdgeSeg);
					MatchedEdgeList->AddNewSegment((*curEdgeSeg));	
					curEdgeSeg=TGroupEdgeList->DeleteSegment(curEdgeSeg);
					flag=0;
					break;
				}
			}
		}
		if(flag==1) curEdgeSeg++;
	}

	tmpClusterList->ClusterListClear();
	for(curEdgeSeg=MatchedEdgeList->ListBegin();curEdgeSeg!=MatchedEdgeList->ListEnd();) 
		//selected matched edge list
	{
		flag=1;
		if((*curEdgeSeg).getWeight()>=105) {
			//assignSegmentToClusterList
		//	clusterList->AddNewSegment((*curEdgeSeg));
			tmpClusterList->AddNewSegment(&(*curEdgeSeg));
			curEdgeSeg=MatchedEdgeList->DeleteSegment(curEdgeSeg);
			flag=0;
		}
		if(flag==1) curEdgeSeg++;
	}


	flag=1;
	for(tmpEdgeSeg=TGroupEdgeList->ListBegin();tmpEdgeSeg!=TGroupEdgeList->ListEnd();)
	{
		(*tmpEdgeSeg).setWeight((*tmpEdgeSeg).getWeight()-5);
		//(*curEdgeSeg).setMotion(100,100);
		if((*tmpEdgeSeg).getWeight()<90) {
		tmpEdgeSeg=TGroupEdgeList->DeleteSegment(tmpEdgeSeg);
		flag=0;			
		}
		if(flag==1) tmpEdgeSeg++;
		flag=1;
	}
	
	for(curEdgeSeg=MatchedEdgeList->ListBegin();curEdgeSeg!=MatchedEdgeList->ListEnd();curEdgeSeg++)
	{
		TGroupEdgeList->AddNewSegment((*curEdgeSeg));
	}

	for(curEdgeSeg=movEdgeList->ListBegin();curEdgeSeg!=movEdgeList->ListEnd();curEdgeSeg++)
	{
		(*curEdgeSeg).setMotion(0,0);
		TGroupEdgeList->AddNewSegment((*curEdgeSeg));//add unmatched edges to TGgroup
	}

	for(tmpCluster=tmpClusterList->ListBegin();tmpCluster!=tmpClusterList->ListEnd();tmpCluster++)
	{///////////for all cluster
		clusterList->AddNewFullCluster(&(*tmpCluster));
	}
	

displayClusterList(&(*clusterList),pOutputImage);
//displayClusterList(&(*tmpClusterList),pOutputImage);
//tmpClusterList->ClusterListClear();*/

	DeleteEdgeList();
	DeleteInOutImage();
}

/*
void EdgeListType::ListToDistanceImage(KScScalarImage2dInt32 *image, KScScalarImage2dInt32 *Limage )
{
	list<EdgeSegmentType>::iterator curEdge;
	PointType p;
//	int tmp;
	register int i,j;
	// initializing the size
	int dx = image->GetXSize();
	int dy = image->GetYSize();
	int oldValue=0;


	int **pImage = image->Get2dArray();
	int **pLimage = Limage->Get2dArray();

	for(i=0; i<dy;i++)
		for(j=0; j<dx; j++)
		{
			pImage[i][j]=1000;
			pLimage[i][j]=1000;
		}

	for(curEdge=ListBegin();curEdge!=ListEnd();curEdge++)
	{
		int size = (*curEdge).GetSize();
		for( i=0 ; i<size ; i++)
		{
			p=(*curEdge).GetPoint(i);
			pImage[p.GetY()][p.GetX()] = 0;
			pLimage[p.GetY()][p.GetX()] = (*curEdge).GetSegIndex();
		}
	}

	for(i=1; i<dy;i++)
		for(j=1; j<dx; j++)
		{
			oldValue=pImage[i][j];
			pImage[i][j] = minimum(pImage[i-1][j-1]+4, pImage[i-1][j]+3, pImage[i-1][j+1]+4, pImage[i][j-1]+3, pImage[i][j]);
			if(oldValue!=pImage[i][j])
			{
				if(pImage[i][j]==pImage[i-1][j-1]+4)
				{
					pLimage[i][j]=pLimage[i-1][j-1];
				}
				else if(pImage[i][j]==pImage[i-1][j]+3)
				{
					pLimage[i][j]=pLimage[i-1][j];
				}
				else if(pImage[i][j]==pImage[i-1][j+1]+4)
				{
					pLimage[i][j]=pLimage[i-1][j+1];
				}
				else if(pImage[i][j]==pImage[i][j-1]+3)
				{
					pLimage[i][j]=pLimage[i][j-1];
				}
			}
		}
	
	for(i=dy-2; i>=0;i--)
		for(j=dx-2; j>=0; j--)
		{
			oldValue=pImage[i][j];
			pImage[i][j] = minimum(pImage[i][j], pImage[i][j+1]+3, pImage[i+1][j-1]+4, pImage[i+1][j]+3, pImage[i+1][j+1]+4);
			if(oldValue!=pImage[i][j])
			{
				if(pImage[i][j]==pImage[i][j+1]+3)
				{
					pLimage[i][j]=pLimage[i][j+1];
				}
				else if(pImage[i][j]==pImage[i+1][j-1]+4)
				{
					pLimage[i][j]=pLimage[i+1][j-1];
				}
				else if(pImage[i][j]==pImage[i+1][j]+3)
				{
					pLimage[i][j]=pLimage[i+1][j];
				}
				else if(pImage[i][j]==pImage[i+1][j+1]+4)
				{
					pLimage[i][j]=pLimage[i+1][j+1];
				}
			}
		}
}

*/





void CEdgeCD::findCurvaturePointsAndSideIntensityDistribution(EdgeSegmentType *curEdge, KSdUint8 ** inputImage,int dx, int dy)
{
int size = (*curEdge).GetSize();
int i;
float cur, localMaxCur;

	PointType *cPt;
	cPt= new PointType();
	localMaxCur=0.0;
	cPt->SetX((*curEdge).GetPoint(0).GetX());
	cPt->SetY((*curEdge).GetPoint(0).GetY());
	
	for(i=3 ; i<size-3 ; i++)
	{
		cur=curvature((*curEdge).GetPoint(i-3),(*curEdge).GetPoint(i-2),(*curEdge).GetPoint(i-1),(*curEdge).GetPoint(i),(*curEdge).GetPoint(i+1),(*curEdge).GetPoint(i+2),(*curEdge).GetPoint(i+3));
		if(cur>localMaxCur)
		{
			localMaxCur=cur;
			cPt->SetX((*curEdge).GetPoint(i).GetX());
			cPt->SetY((*curEdge).GetPoint(i).GetY());
		}
		if(((i+4)%SAMPLE_LENGTH)==0)
		{
			 (*curEdge).setCurvaturePoint(int(((i+4)/SAMPLE_LENGTH)-1),&localMaxCur,cPt);/////////////////////This line is creating error
			 (*curEdge).computeMeanVarianceAroundCurvaturePoint(int(((i+4)/SAMPLE_LENGTH)-1), cPt,inputImage,dx,dy);
			 
			 
			 localMaxCur=0.0;
			 cPt->SetX((*curEdge).GetPoint(i+1).GetX());
			 cPt->SetY((*curEdge).GetPoint(i+1).GetY());
			 
		}
	}
}








	





void CEdgeCD::findSideColorAndBoundary(EdgeSegmentType *curEdge, KSdUint8 ** inputImage)
{
int size = (*curEdge).GetSize();
int count=1;
float s1_color=0;
float s2_color=0;
PointType p;
int xpos,ypos,i;
float x_inc, y_inc, s1_avg, s2_avg, varside1, varside2;
long distance;

int minx=(*curEdge).GetStartPoint()->GetX();
int miny=(*curEdge).GetStartPoint()->GetY();
int maxx=(*curEdge).GetEndPoint()->GetX();
int maxy=(*curEdge).GetEndPoint()->GetY();;

for( i=0 ; i<3 ; i++)
{
	if(minx>(*curEdge).GetPoint(i).GetX()) minx=(*curEdge).GetPoint(i).GetX();
	if(miny>(*curEdge).GetPoint(i).GetY()) miny=(*curEdge).GetPoint(i).GetY();
	if(maxx<(*curEdge).GetPoint(i).GetX()) maxx=(*curEdge).GetPoint(i).GetX();
	if(maxy<(*curEdge).GetPoint(i).GetY()) maxy=(*curEdge).GetPoint(i).GetY();
}

for( i=size-3 ; i<size ; i++)
{
	if(minx>(*curEdge).GetPoint(i).GetX()) minx=(*curEdge).GetPoint(i).GetX();
	if(miny>(*curEdge).GetPoint(i).GetY()) miny=(*curEdge).GetPoint(i).GetY();
	if(maxx<(*curEdge).GetPoint(i).GetX()) maxx=(*curEdge).GetPoint(i).GetX();
	if(maxy<(*curEdge).GetPoint(i).GetY()) maxy=(*curEdge).GetPoint(i).GetY();
}


		for(i=3 ; i<size-3 ; i++)
		{
			p=(*curEdge).GetPoint(i);
			if(minx>p.GetX()) minx=p.GetX();
			if(miny>p.GetY()) miny=p.GetY();
			if(maxx<p.GetX()) maxx=p.GetX();
			if(maxy<p.GetY()) maxy=p.GetY();
			
			/*
			tangentdirX=secondDerivative((*curEdge).GetPoint(i-2).GetX(),(*curEdge).GetPoint(i-1).GetX(),(*curEdge).GetPoint(i+1).GetX(),(*curEdge).GetPoint(i+2).GetX());
			x_inc=cos(atan(tangentdirX));
			tangentdirY=secondDerivative((*curEdge).GetPoint(i-2).GetY(),(*curEdge).GetPoint(i-1).GetY(),(*curEdge).GetPoint(i+1).GetY(),(*curEdge).GetPoint(i+2).GetY());
			y_inc=cos(atan(tangentdirY));
			if(x_inc>0) xpos=2;
			else xpos=-2;
			if(y_inc>0) ypos=2;
			else ypos=-2;
			*/
			xpos=0;ypos=0;
			x_inc=2*(*curEdge).GetPoint(i).GetX()-(*curEdge).GetPoint(i-2).GetX()-(*curEdge).GetPoint(i+2).GetX();
			y_inc=2*(*curEdge).GetPoint(i).GetY()-(*curEdge).GetPoint(i-2).GetY()-(*curEdge).GetPoint(i+2).GetY();
			if(x_inc>0) xpos=2;
			else if (x_inc<0) xpos=-2;
			if(y_inc>0) ypos=2;
			else if (y_inc<0)ypos=-2;
			if(xpos==0&&ypos==0){ xpos=2; ypos=2;}
			//pOutImage[p.GetY()+ypos][p.GetX()+xpos]= 255;
			//pOutImage[p.GetY()-ypos][p.GetX()-xpos]= 255;

			s1_color+=inputImage[p.GetY()+ypos][p.GetX()+xpos];
			s2_color+=inputImage[p.GetY()-ypos][p.GetX()-xpos];
			
			//if(count%10==0)
			if(i==size-4)
			{
				s1_avg=s1_color/count;
				s2_avg=s2_color/count;

				varside1=0;
				varside2=0;
				for(i=3 ; i<size-3 ; i++)
					{
						p=(*curEdge).GetPoint(i);
						
						/*tangentdirX=secondDerivative((*curEdge).GetPoint(i-2).GetX(),(*curEdge).GetPoint(i-1).GetX(),(*curEdge).GetPoint(i+1).GetX(),(*curEdge).GetPoint(i+2).GetX());
						x_inc=cos(atan(tangentdirX));
						tangentdirY=secondDerivative((*curEdge).GetPoint(i-2).GetY(),(*curEdge).GetPoint(i-1).GetY(),(*curEdge).GetPoint(i+1).GetY(),(*curEdge).GetPoint(i+2).GetY());
						y_inc=cos(atan(tangentdirY));
						if(x_inc>0) xpos=2;
						else xpos=-2;
						if(y_inc>0) ypos=2;
						else ypos=-2;
						*/
						xpos=0;ypos=0;
						x_inc=2*(*curEdge).GetPoint(i).GetX()-(*curEdge).GetPoint(i-2).GetX()-(*curEdge).GetPoint(i+2).GetX();
						y_inc=2*(*curEdge).GetPoint(i).GetY()-(*curEdge).GetPoint(i-2).GetY()-(*curEdge).GetPoint(i+2).GetY();
						if(x_inc>0) xpos=2;
						else if (x_inc<0) xpos=-2;
						if(y_inc>0) ypos=2;
						else if (y_inc<0)ypos=-2;
						if(xpos==0&&ypos==0){ xpos=2; ypos=2;}
						//pOutImage[p.GetY()+ypos][p.GetX()+xpos]= 255;
						//pOutImage[p.GetY()-ypos][p.GetX()-xpos]= 255;

						varside1+=fabs(s1_avg-inputImage[p.GetY()+ypos][p.GetX()+xpos]);
						varside2+=fabs(s2_avg-inputImage[p.GetY()-ypos][p.GetX()-xpos]);
					}


						i=size-4;
						varside1=varside1/count;
						varside2=varside2/count;

				(*curEdge).setSideColor(1,s1_avg); 
				(*curEdge).setSideColor(2,s2_avg); 
				(*curEdge).setSideColorVar(1,varside1);
				(*curEdge).setSideColorVar(2,varside2);
				(*curEdge).setBoundaryParam(maxx, maxy, minx, miny);
				(*curEdge).setEdgeCenterParam(float(minx+maxx)/2, float(miny+maxy)/2);
				(*curEdge).setWeight(100);
					
				distance=((*curEdge).GetPoint(0).GetX()-(*curEdge).GetPoint(size-1).GetX())*((*curEdge).GetPoint(0).GetX()-(*curEdge).GetPoint(size-1).GetX())+((*curEdge).GetPoint(0).GetY()-(*curEdge).GetPoint(size-1).GetY())*((*curEdge).GetPoint(0).GetY()-(*curEdge).GetPoint(size-1).GetY());

				(*curEdge).setEdgeShape(float(sqrt((double)distance)/size));
				
				p=(*curEdge).GetPoint(size/2);
				/*
				tangentdirX=secondDerivative((*curEdge).GetPoint(size/2-2).GetX(),(*curEdge).GetPoint(size/2-1).GetX(),(*curEdge).GetPoint(size/2+1).GetX(),(*curEdge).GetPoint(size/2+2).GetX());
				x_inc=cos(atan(tangentdirX));
				tangentdirY=secondDerivative((*curEdge).GetPoint(size/2-2).GetY(),(*curEdge).GetPoint(size/2-1).GetY(),(*curEdge).GetPoint(size/2+1).GetY(),(*curEdge).GetPoint(size/2+2).GetY());
				y_inc=cos(atan(tangentdirY));

				if(x_inc>0) xpos=2;
				else xpos=-2;
				if(y_inc>0) ypos=2;
				else ypos=-2;
				*/
				xpos=0;ypos=0;
				x_inc=2*(*curEdge).GetPoint(size/2).GetX()-(*curEdge).GetPoint(size/2-2).GetX()-(*curEdge).GetPoint(size/2+2).GetX();
				y_inc=2*(*curEdge).GetPoint(size/2).GetY()-(*curEdge).GetPoint(size/2-2).GetY()-(*curEdge).GetPoint(size/2+2).GetY();
				if(x_inc>0) xpos=2;
				else if (x_inc<0) xpos=-2;
				if(y_inc>0) ypos=2;
				else if (y_inc<0)ypos=-2;
				if(xpos==0&&ypos==0){ xpos=2; ypos=2;}
				//pOutImage[p.GetY()+ypos][p.GetX()+xpos]= s1_avg;
				//pOutImage[p.GetY()-ypos][p.GetX()-xpos]= s2_avg;
				count=0;
				s1_color=0;
				s2_color=0;
				
			}
			count++;
		}
}

void BreakLongEdges(EdgeListType* movEdgeList)
{
	int size1=MAX_SEGMENT_SIZE,flag=0,size,size2=SAMPLE_LENGTH;

list<EdgeSegmentType>::iterator curEdgeSeg;

for(curEdgeSeg=movEdgeList->ListBegin();curEdgeSeg!=movEdgeList->ListEnd(); )
{
	flag=0;
	size=(*curEdgeSeg).GetSize();
	if(size<size2) {
		curEdgeSeg=movEdgeList->DeleteSegment(curEdgeSeg);
		flag=1;
	}
	else if(size>size1)
	{

		EdgeSegmentType *tempSegment=new EdgeSegmentType();
		tempSegment->CopyAfterN(*curEdgeSeg, size1);	
			
		EdgeSegmentType *tempSegment1=new EdgeSegmentType();
		tempSegment1->CopyBeforeN(*curEdgeSeg, size1);
			

		movEdgeList->AddNewSegment(tempSegment1);
		movEdgeList->AddNewSegment(tempSegment);

		curEdgeSeg=movEdgeList->DeleteSegment(curEdgeSeg);
		flag = 1;
	}
if(flag == 0) curEdgeSeg++;
}

}

void CEdgeCD::SubtractEdge(EdgeListType* refEdgeList)
{
	
	//Initialize the size of reference labelled image
	int dx = curImage->GetXSize();
	int dy = curImage->GetYSize();

	//Extracting the edges of current image
	Canny *canny;
	canny   = new Canny(curImage,(float)1.,(float)0.85,(float)0.95, 0);		// L=0.69, H: 0.86 //.85 .95
	//EdgeListType *curEdge = currentEdge;
//	EdgeListType *curEdge = new EdgeListType(dx,dy);
	canny->Do_canny(curEdgeList,1);



    

/////////////////Newly added for side color and shape

	PointType p;
	int size;
	float tangentdirX=0,tangentdirY=0;
	int count=1;
	float s1_avg=0, s2_avg=0,varside1=0,varside2=0;
	KSdUint8 ** inputImage = curImage->Get2dArray();

	list<EdgeSegmentType>::iterator curEdge;
	for(curEdge=curEdgeList->ListBegin();curEdge!=curEdgeList->ListEnd();curEdge++)
	{
		findCurvaturePointsAndSideIntensityDistribution(&(*curEdge), inputImage, dx, dy);
	}

	BreakLongEdges(curEdgeList);

	for(curEdge=curEdgeList->ListBegin();curEdge!=curEdgeList->ListEnd();curEdge++)
	{
		findSideColorAndBoundary(&(*curEdge), inputImage);
	}




	//Get the value of reference marker image having the distance vector
	int **pRefMarker = refDistImage->Get2dArray();
	int **pRefLabel = refLabelImage->Get2dArray();

	//Edited by Julius
//	KScScalarImage2dInt32 *subWeight = new KScScalarImage2dInt32(dx,dy);
//	subWeight->InitTo(0);
//	int **pSubWeightTmp = subWeight->Get2dArray();

	//Initializing the parameters for current image subtraction
	float distance;
	int nLabel=0;
	register int i;
	int refLabel=0;
	int Labelcount=0;
	int *lValues;
	int flag=0;
	int colorthreshold=0;
	float shapethreshold=0.0;
	float mshape=0;



//	PointType p;
	list<EdgeSegmentType>::iterator curEdgeSeg, refEdgeSeg;
//	int size;
//	CGetMaxIndex getmaxindex;

	for(curEdgeSeg=curEdgeList->ListBegin();curEdgeSeg!=curEdgeList->ListEnd();curEdgeSeg++)
	{
		//Initialing the size and distance
		distance = 0.0;
		size=(*curEdgeSeg).GetSize();
		lValues = new int[size];
	//	getmaxindex.init();
		for( i=0 ; i<size ; i++)
		{
			p=(*curEdgeSeg).GetPoint(i);
			distance = distance + pRefMarker[p.GetY()][p.GetX()] * pRefMarker[p.GetY()][p.GetX()];
			lValues[i]=pRefLabel[p.GetY()][p.GetX()];
		}

		labelSort(lValues,size);
		refLabel=maxFrequency(lValues,size,&Labelcount);

		distance = sqrt(distance/size)/3.0;
		
		
		if( distance >= 2.5){										//th: 1.5
			//PointType* pPoint = (*curEdgeSeg).m_Point;
			//nLabel = movEdgeList->SmallestFreeIndex();
	
					flag=0;
					float similarityThreshold=20;
					float meanT=0, mean=0, vari=0, variT=0, mahadist=0,mahadist1=0;
					float meanT2=0, mean2=0, vari2=0, variT2=0, mahadist2=0; //mahadistRatio;
					int position=0;

						//shapethreshold=0.01;
					for(refEdgeSeg=refEdgeList->ListBegin();refEdgeSeg!=refEdgeList->ListEnd();refEdgeSeg++)
					{
						if((*refEdgeSeg).GetSegIndex()==refLabel)
						{		
					/*	meanT=(*refEdgeSeg).getSideColor(1);
						variT=(*refEdgeSeg).getSideColorVar(1);

						mean=(*refEdgeSeg).getSideColor(2);
						vari=(*refEdgeSeg).getSideColorVar(2);

						variT=variT>vari? vari: variT;
						mahadist1=fabs((meanT-mean)*(meanT-mean)/variT*variT);
						//if(mahadist<similarityThreshold) flag++;

						meanT=(*curEdgeSeg).getSideColor(1);
						variT=(*curEdgeSeg).getSideColorVar(1);	

						mean=(*curEdgeSeg).getSideColor(2);
						vari=(*curEdgeSeg).getSideColorVar(2);

						variT=variT>vari? vari: variT;
						mahadist2=fabs((meanT-mean)*(meanT-mean)/variT*variT);
						//if(mahadist<similarityThreshold) flag++;
						if(fabs(mahadist1-mahadist2)<similarityThreshold*(*refEdgeSeg).GetSize()) flag=1;
						else flag=2;*/




						meanT=(*refEdgeSeg).getSideColor(1);
						variT=(*refEdgeSeg).getSideColorVar(1);

						mean=(*curEdgeSeg).getSideColor(1);
						vari=(*curEdgeSeg).getSideColorVar(1);

						variT=variT>vari? vari: variT;

						mahadist=fabs((meanT-mean)*(meanT-mean)/variT*variT);
						if(mahadist<similarityThreshold) flag++;

						meanT=(*refEdgeSeg).getSideColor(2);
						variT=(*refEdgeSeg).getSideColorVar(2);

						mean=(*curEdgeSeg).getSideColor(2);
						vari=(*curEdgeSeg).getSideColorVar(2);

						variT=variT>vari? vari: variT;
						mahadist=fabs((meanT-mean)*(meanT-mean)/variT*variT);
						if(mahadist<similarityThreshold) flag++;



						
					/*	meanT=(*refEdgeSeg).getSideColor(1);
						variT=(*refEdgeSeg).getSideColorVar(1);

						mean=(*curEdgeSeg).getSideColor(1);
						vari=(*curEdgeSeg).getSideColorVar(1);
						
						mean2=(*curEdgeSeg).getSideColor(2);
						vari2=(*curEdgeSeg).getSideColorVar(2);

						meanT2=(*refEdgeSeg).getSideColor(2);
						variT2=(*refEdgeSeg).getSideColorVar(2);*/

						//variT=variT>vari? vari: variT;
						//mahadist=fabs((meanT-mean)*(meanT-mean)/variT*variT);
						//if(mahadist<similarityThreshold) flag++;
						
						/*variT=variT<variT2? variT: variT2;
						vari=vari<vari2? vari: vari2;

						mahadist=fabs((meanT-meanT2)*(meanT-meanT2)/(variT*variT));
						mahadist2=fabs((mean-mean2)*(mean-mean2)/(vari*vari));

						mahadistRatio=mahadist/mahadist2;
						if(mahadistRatio>=0.7&&mahadistRatio<=1.3) flag=0;
						else flag++;*/
						

						//variT=variT>vari? vari: variT;
						//mahadist=fabs((meanT-mean)*(meanT-mean)/variT*variT);
						//if(mahadist<similarityThreshold) flag++;
						

/*
					meanE=edge.getSideColor(1);
					variE=edge.getSideColorVar(1);

					mean=(*curEdgeSeg).getSideColor(1);
					vari=(*curEdgeSeg).getSideColorVar(1);

					//variE=variE>vari? vari: variE;
					//mahadist=fabs((meanE-mean)*(meanE-mean)/variE*variE);
				
					//if(mahadist<colorSimilarityDistance) flag++;

					meanE2=edge.getSideColor(2);
					variE2=edge.getSideColorVar(2);
					mean2=(*curEdgeSeg).getSideColor(2);
					vari2=(*curEdgeSeg).getSideColorVar(2);

					vari=vari<vari2? vari: vari2;
					variE=variE<variE2? variE: variE2;
					
					mahadist=fabs((mean2-mean)*(mean2-mean)/(vari*vari));
					mahadist2=fabs((meanE2-meanE)*(meanE2-meanE)/(variE*variE));
					
					mahadistRatio=mahadist/mahadist2;
					if(mahadistRatio>=0.7&&mahadistRatio<=1.3) flag=0;
					else flag++;
					*/

						mshape=matchShape((*refEdgeSeg),(*curEdgeSeg),&position);

						//if(((*refEdgeSeg).getEdgeShape()/(*refEdgeSeg).GetSize()-(*curEdgeSeg).getEdgeShape()/(*curEdgeSeg).GetSize())<shapethreshold)
						//flag=1;
						break;
						}
					}
						//	if((((*refEdgeSeg).getSideColor(1)-(*curEdgeSeg).getSideColor(1))<colorthreshold) && (((*refEdgeSeg).getSideColor(2)-(*curEdgeSeg).getSideColor(2))<colorthreshold))
					
	
	
///////////////////////////////////////////////////////////
		if(flag<2) movEdgeList->AddNewSegment(*curEdgeSeg);
		}
	
		
		delete [] lValues;
		lValues=NULL;

	}



//	CWeightUpdate weightup(movingEdge,m_pcTmpEdge,m_pcRefEdge);
//	weightup.SetEdgeList(movingEdge,m_pcTmpEdge,m_pcRefEdge);
//	weightup.DoWeightUpdate(dx, dy);

	delete canny;
}


//This part will break moving edge based on distance value;
void CEdgeCD::BreakMoingEdge()
{


	//done but does not consider edge color while matching........
	list<EdgeSegmentType>::iterator curEdgeSeg;
	float distance1, distance2;
	int flag;
	int sLength, i,j, k, size,size1, size2;
	int **pRefMarker = refDistImage->Get2dArray();
	KSdUint8 ** inputImage = curImage->Get2dArray();
	
	for(curEdgeSeg=movEdgeList->ListBegin();curEdgeSeg!=movEdgeList->ListEnd(); )
	{
		//Initialing the size and distance
		size=(*curEdgeSeg).GetSize();

		i=0;
    	flag = 0;
		sLength = 30;
		while( i+sLength <size)
		{
			distance1 = 0.0;
			for(j = 0; j<i+sLength/2; j++){
				distance1 += pRefMarker[(*curEdgeSeg).GetPoint(j).GetY()][(*curEdgeSeg).GetPoint(j).GetX()] 
					* pRefMarker[(*curEdgeSeg).GetPoint(j).GetY()][(*curEdgeSeg).GetPoint(j).GetX()];
			}
			size1 = i+sLength/2;
			distance1 = sqrt(distance1/size1)/3.0;
				
			distance2 = 0.0;
			for(k = 0, j = i+sLength/2; j<size; j++, k++){
				distance2 += pRefMarker[(*curEdgeSeg).GetPoint(j).GetY()][(*curEdgeSeg).GetPoint(j).GetX()]
					* pRefMarker[(*curEdgeSeg).GetPoint(j).GetY()][(*curEdgeSeg).GetPoint(j).GetX()];
			}
			size2 = size-size1;
			distance2 = sqrt(distance2/size2)/3.0;
			
			if(distance1 <= 1.25){
				EdgeSegmentType *tempSegment=new EdgeSegmentType();
				tempSegment->CopyAfterN(*curEdgeSeg, size1);
				findSideColorAndBoundary(tempSegment, inputImage);


				movEdgeList->AddNewSegment(tempSegment);
				curEdgeSeg=movEdgeList->DeleteSegment(curEdgeSeg);
				flag = 1;
				break;
			}
			else if(distance2 <= 1.25){
				EdgeSegmentType *tempSegment=new EdgeSegmentType();
				tempSegment->CopyBeforeN(*curEdgeSeg, size1);
				findSideColorAndBoundary(tempSegment, inputImage);

				
				movEdgeList->AddNewSegment(tempSegment);
				curEdgeSeg=movEdgeList->DeleteSegment(curEdgeSeg);
				flag = 1;
				break;
			}
			i = i + 5;
		}

		if(flag == 0)
			curEdgeSeg++;
	}

}

void CEdgeCD::RemoveScattered(int maskLength)
{
	//Initialize the size of reference labelled image
	int dx = curImage->GetXSize();
	int dy = curImage->GetYSize();
	KScScalarImage2dInt32 *movDistImage=new KScScalarImage2dInt32(dx,dy);
	movDistImage->InitTo(0);
	//Generation of labeled initial reference image for edge subtraction
	//movEdgeList->ListToDistanceImage(movDistImage);
	movEdgeList->ListToLabelImage(movDistImage);

	//Get the value of reference marker image having the distance vector
	int **pMovDistImage = movDistImage->Get2dArray();

	//Initializing the parameters for current image subtraction
	int nLabel=0;
	register int i;

	PointType p;
	list<EdgeSegmentType>::iterator curEdgeSeg;
	int size;
	int x, y, count, j, segment[500], curSegIndex; 

	for(curEdgeSeg=movEdgeList->ListBegin();curEdgeSeg!=movEdgeList->ListEnd(); )
	{
		//Initialing size
		count=0;
		size=(*curEdgeSeg).GetSize();
		curSegIndex =(*curEdgeSeg).GetSegIndex();
		for( i=0 ; i<size ; i = i+(maskLength*2))
		{
			p=(*curEdgeSeg).GetPoint(i);
			//distance = distance + pRefMarker[p.GetY()][p.GetX()] * pRefMarker[p.GetY()][p.GetX()];
			for(y=p.GetY()-maskLength; y<p.GetY()+maskLength; y++)
			{
				for(x=p.GetX()-maskLength; x<p.GetX()+maskLength; x++)
				{
					if(y<0||y>=dy||x<0||x>=dx)
						continue;

					if(pMovDistImage[y][x] && pMovDistImage[y][x]!=curSegIndex)
					{
						for(j=0;j<count; j++)
						{
							if(segment[j]==pMovDistImage[y][x])
								break;
						}
						if(j==count)
						{
							segment[count]=pMovDistImage[y][x];
							count++;
						}
					}
				}
			}
			if(i!=size-1 && size-1-i<maskLength*2)
				i= size-1-maskLength*2;
				
		}
		if(count<=1&&(*curEdgeSeg).GetSize()<20||count>=1&&(*curEdgeSeg).GetSize()<8)
		{
			curEdgeSeg=movEdgeList->DeleteSegment(curEdgeSeg);
		}
		else
			curEdgeSeg++;
	}
	delete movDistImage;
}

/*
void CEdgeCD::AdditionEdge(int nFrame)
{
	//curImg->pData
	int dx = m_pcSrcImg->GetXSize();
	int dy = m_pcSrcImg->GetYSize();

	//Extraction of current edge segments
	Canny *canny;
	canny   = new Canny(m_pcSrcImg,(float)1.,(float)0.7,(float)0.9,0);
	EdgeListType *curEdge = new EdgeListType(dx,dy);
	canny->Do_canny(curEdge,1);


	KScScalarImage2dInt32 *refImage = new KScScalarImage2dInt32(dx,dy);
	int **pRefImage = refImage->Get2dArray();
	refEdgeList->ListToMarkedImage(refImage);


	//curEdgeSeg points edge segments of current input edge list
	float distance;
	int nLabel=0;
	PointType p;
	list<EdgeSegmentType>::iterator curEdgeSeg;
	register int i;
	int size;

	for(curEdgeSeg=curEdge->m_EdgeList->begin();curEdgeSeg!=curEdge->m_EdgeList->end();curEdgeSeg++)
	{
		//Initializing distance, size and maximum matching label counter
		distance =0.0;
		size=(*curEdgeSeg).GetSize()-1;
	//	getmaxindex.init();
		for( i=0 ; i<size ; i++)
		{
			p=(*curEdgeSeg).GetCurPoint(i);
			distance = distance + pRefImage[p.y][p.x] * pRefImage[p.y][p.x];

		}
		
		distance = sqrt(distance/size)/3.0;

		if( distance >= 3.0){
			PointType* pPoint = (*curEdgeSeg).m_Point;
			nLabel = refEdgeList->smallestFreeIndex();
			refEdgeList->AddNewSegment(size, pPoint, nLabel,0);
		}

	}
	//Deletion of the temporary variables
	delete canny;
	delete curEdge;
	delete refImage;
}
*/