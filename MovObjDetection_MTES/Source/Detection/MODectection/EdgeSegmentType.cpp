// EdgeSegmentType.cpp: implementation of the EdgeSegmentType class.
//
//////////////////////////////////////////////////////////////////////

#include "EdgeSegmentType.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

EdgeSegmentType::EdgeSegmentType()
{
	segSize		= 0;
	segIndex	= 0;
	curPointIndex	= 0;
	edgePoints	= NULL;


	//new
	curvaturePoints=NULL;
	//end

	sideColor1=0;
	sideColorVar1=0;
	sideColor2=0;
	sideColorVar2=0;
	avgCurvature=0;
	edgeShape=0;
	weight=0;
	edgeFrameNo=1;
	clusterMemberID=0;

	maxx=0;
	maxy=0;
	minx=0;
	miny=0;
	centerX=0;
	centerY=0;
	motionX=0;
	motionY=0;

}

EdgeSegmentType::EdgeSegmentType(const EdgeSegmentType &ob)
{
	segSize		= ob.segSize;
	segIndex	= ob.segIndex;
	curPointIndex	= ob.curPointIndex;
	edgePoints	= new PointType[ob.segSize];
	edgeGrads = new float[ob.segSize];
	edgeTheta = new float[ob.segSize];

	//new
	s1IntensityMean=new float[ob.segSize/SAMPLE_LENGTH];
	s1IntensityVar=new float[ob.segSize/SAMPLE_LENGTH];
	s2IntensityMean=new float[ob.segSize/SAMPLE_LENGTH];
	s2IntensityVar=new float[ob.segSize/SAMPLE_LENGTH];
	curvatureValue=new float[ob.segSize/SAMPLE_LENGTH];
	curvaturePoints=new PointType[ob.segSize/SAMPLE_LENGTH];
	for(int i=0; i<(ob.segSize/SAMPLE_LENGTH); i++)
	{	
		s1IntensityMean[i]=ob.s1IntensityMean[i];
		s1IntensityVar[i]=ob.s1IntensityVar[i];
		s2IntensityMean[i]=ob.s2IntensityMean[i];
		s2IntensityVar[i]=ob.s2IntensityVar[i];
		curvatureValue[i]=ob.curvatureValue[i];
		curvaturePoints[i]=ob.curvaturePoints[i];
	}
	//end new

	
	sideColor1=ob.sideColor1;
	sideColorVar1=ob.sideColorVar1;
	sideColor2=ob.sideColor2;
	sideColorVar2=ob.sideColorVar2;
	avgCurvature=ob.avgCurvature;
	edgeShape=ob.edgeShape;
	weight=ob.weight;
	edgeFrameNo=ob.edgeFrameNo;
	maxx=ob.maxx;
	maxy=ob.maxy;
	minx=ob.minx;
	miny=ob.miny;
	centerX=ob.centerX;
	centerY=ob.centerY;
	motionX=ob.motionX;
	motionY=ob.motionY;
	clusterMemberID=ob. clusterMemberID;

	
	for(int i=0; i<ob.segSize ; i++)
	{	
		edgePoints[i]=ob.edgePoints[i];
		edgeGrads[i]=ob.edgeGrads[i];
		edgeTheta[i]=ob.edgeTheta[i];
	}
}


EdgeSegmentType::~EdgeSegmentType()
{
if(edgePoints != NULL) 	delete [] edgePoints;
if(edgeGrads!=NULL) delete [] edgeGrads;
if(edgeTheta!=NULL) delete[] edgeTheta;

//new
if(s1IntensityMean!=NULL) delete [] s1IntensityMean;
if(s1IntensityVar!=NULL) delete [] s1IntensityVar;
if(s2IntensityMean!=NULL) delete [] s2IntensityMean;
if(s2IntensityVar!=NULL) delete [] s2IntensityVar;
if(curvatureValue!=NULL) delete [] curvatureValue;
if(curvaturePoints!=NULL) delete []curvaturePoints;
//end new

segSize		= 0;
segIndex	= 0;
curPointIndex	= 0;
weight=0;
edgeFrameNo=1;
}


/*EdgeSegmentType::~EdgeSegmentType()
{
	segSize		= 0;
	segIndex	= 0;
	curPointIndex	= 0;

	if(edgePoints != NULL)
		delete[] edgePoints;
}
*/


int EdgeSegmentType::findMinimum(int val1, int val2, int val3){
	int minVal = val1;
	if(val2<minVal)
		minVal = val2;
	if(val3<minVal)
		minVal = val3;

	return minVal;
}


//new 
bool EdgeSegmentType::AllocCurvatureIntensity(int size)
{

	s1IntensityMean=new float[int(size/SAMPLE_LENGTH)];
	s1IntensityVar=new float[int(size/SAMPLE_LENGTH)];
	s2IntensityMean=new float[int(size/SAMPLE_LENGTH)];
	s2IntensityVar=new float[int(size/SAMPLE_LENGTH)];
	curvatureValue=new float[int(size/SAMPLE_LENGTH)];
	curvaturePoints=new PointType[int(size/SAMPLE_LENGTH)];

	if(!s1IntensityMean) return false;
	else if(!s1IntensityVar)return false;
	else if(!s2IntensityMean) return false;
	else if(!s2IntensityVar)return false;
	else if(!curvatureValue)return false;
	else if(!curvaturePoints)return false;

	else    
		return true ;
}




EdgeSegmentType::EdgeSegmentType (int size)
{
	segSize		= size;
	AllocPoint(size);
	AllocGrad(size);
	AllocTheta(size);
	
	//new
	AllocCurvatureIntensity(size);
}

EdgeSegmentType::EdgeSegmentType (const EdgeSegmentType &ob, int size)
{
	segSize		= size;
	AllocPoint(size);
	AllocGrad(size);
	AllocTheta(size);

	//new
	AllocCurvatureIntensity(size);
}


float EdgeSegmentType::getSideColor(int i){
	if(i==1) return sideColor1;
	return sideColor2;
}

float EdgeSegmentType::getSideColorVar(int i){
	if(i==1) return sideColorVar1;
	return sideColorVar2;
}


float EdgeSegmentType::getAvgCurvature(){
	return avgCurvature;
}

float EdgeSegmentType::getEdgeShape(){
return edgeShape;
}

float EdgeSegmentType::getWeight()
{
	return weight;
}

int EdgeSegmentType::getFrameIndex()
{
	return edgeFrameNo;
}


void EdgeSegmentType::getMotion(float *i, float *j)
{
*i=motionX;
*j=motionY;	
}

void EdgeSegmentType::setSideColor(int i, float color){

	if(i==1) sideColor1=color;
	else sideColor2=color;
}

void EdgeSegmentType::setSideColorVar(int i, float color){

	if(i==1) sideColorVar1=color;
	else sideColorVar2=color;
}

void EdgeSegmentType::setAvgCurvature(float avgCur)
{
	avgCurvature=avgCur;	
	
}

void EdgeSegmentType::setEdgeShape(float shape)
{
	edgeShape=shape;
}
void EdgeSegmentType::setWeight(int i)
{
weight=i;
}
void EdgeSegmentType::setFrameIndex(int i)
{
	edgeFrameNo=i;
}


void EdgeSegmentType::setMotion(float i, float j)
{
motionX=i;
motionY=j;	
}



bool EdgeSegmentType::AllocPoint(int size)
{
//	this->segSize=size;
	edgePoints   = new PointType[size];
	memset(edgePoints,0,sizeof(PointType)*size);

	if(!edgePoints) 
		return false;
	else         
		return true ;
}

bool EdgeSegmentType::AllocGrad(int size)
{
	edgeGrads   = new float[size];
	memset(edgeGrads,0.0,sizeof(float)*size);

	if(!edgeGrads) 
		return false;
	else         
		return true ;
}

bool EdgeSegmentType::AllocTheta(int size)
{
	edgeTheta   = new float[size];
	memset(edgeTheta,0.0,sizeof(float)*size);

	if(!edgeTheta) 
		return false;
	else         
		return true ;
}

//new 

bool EdgeSegmentType::computeMeanVarianceAroundCurvaturePoint(int index, PointType *cPt, KSdUint8 ** inputImage, int dx, int dy)
{
int i,j,oldValue;
int rX,rY,size;
rX=(*cPt).GetX();
rY=(*cPt).GetY();
int pX,pY,maxLabel=1,flag=1;
PointType p;
int labelCurvaturePos[CRMX][CRMY];
int crmx=CRMX+2,crmy=CRMY+2;
int inc=-1;
int labelUsed[100]={0},count,k;
float m1,m2,v1,v2;
int count3,count4;



do
{

for(i=0;i<100;i++) labelUsed[i]=0;
crmx=crmx-2;
crmy=crmy-2;
inc++;

for(i=0;i<CRMX;i++)
{
	for(j=0;j<CRMY;j++)
	{
		labelCurvaturePos[i][j]=100;
	}
}

size=this->GetSize();
for( i=0 ; i<size ; i++)
{
	p=this->GetPoint(i);
	pX=p.GetX();
	pY=p.GetY();
	if(((pX-rX)<=(crmx/2))&&((pX-rX)>=-(crmx/2))&&((pY-rY)<=(crmy/2))&&((pY-rY)>=-(crmy/2)))
	{
		labelCurvaturePos[pX-rX+(crmx/2)+inc][pY-rY+(crmy/2)+inc]=1000;
	}
}


for(i=1;i<crmx;i++)
{
	for(j=1;j<crmy;j++)
	{
		if(labelCurvaturePos[i][j]==1000) continue;
		labelCurvaturePos[i][j] = findMinimum(labelCurvaturePos[i-1][j], labelCurvaturePos[i][j-1], labelCurvaturePos[i][j]);
		if(labelCurvaturePos[i][j]==100) labelCurvaturePos[i][j]=maxLabel++;
	}
}

for(i=crmx-2; i>=0;i--)
{
	for(j=crmy-2; j>=0; j--)
	{
		if(labelCurvaturePos[i][j]==1000) continue;
		labelCurvaturePos[i][j] = findMinimum(labelCurvaturePos[i+1][j], labelCurvaturePos[i][j+1], labelCurvaturePos[i][j]);
		if(labelCurvaturePos[i][j]==100) labelCurvaturePos[i][j]=maxLabel++;
	}
}


for(i=crmx-2;i>=0;i--)
{
	for(j=1;j<crmy;j++)
	{
		if(labelCurvaturePos[i][j]==1000) continue;
		labelCurvaturePos[i][j] = findMinimum(labelCurvaturePos[i][j-1], labelCurvaturePos[i+1][j], labelCurvaturePos[i][j]);
		if(labelCurvaturePos[i][j]==100) labelCurvaturePos[i][j]=maxLabel++;
	}
}

for(i=1; i<crmx;i++)
	{
	for(j=crmy-2; j>=0; j--)
	{
		if(labelCurvaturePos[i][j]==1000) continue;
		labelCurvaturePos[i][j] = findMinimum(labelCurvaturePos[i-1][j], labelCurvaturePos[i][j+1], labelCurvaturePos[i][j]);
		if(labelCurvaturePos[i][j]==100) labelCurvaturePos[i][j]=maxLabel++;
	}
}


count=0;
for(i=0;i<crmx;i++)
{
	for(j=0;j<crmy;j++)
	{
		if((labelCurvaturePos[i][j]!=1000)&&(CURVATURE_REGION_MASK[i][j]==1))
		{
			for(k=0;k<count;k++)
			if(labelUsed[k]==labelCurvaturePos[i][j]) break;

			if(k==count)
			{
			labelUsed[count]=labelCurvaturePos[i][j];
			count++;
			}
		}
	}
}

}while(count!=2&&crmx>=5);

if(count==2)
{
	m1=v1=m2=v2=0.0;
	count3=0;
	count4=0;
for(i=-crmx/2;i<(crmx/2)+1;i++)
{
	for(j=-crmy/2;j<(crmy/2)+1;j++)
	{
		if(((i+rX)>=0)&&((i+rX)<dx)&&((j+rY)>=0)&&((j+rY)<dy))
		{
			if(((labelCurvaturePos[i+(crmx/2)][j+(crmy/2)])!=1000)&&CURVATURE_REGION_MASK[i+(crmx/2)][j+(crmy/2)]==1)
			{
				if(labelCurvaturePos[i+(crmx/2)][j+(crmy/2)]==labelUsed[0])
				{
					m1=m1+inputImage[j+rY][i+rX];
					count3++;
				}
				else if (labelCurvaturePos[i+(crmx/2)][j+(crmy/2)]==labelUsed[1])
				{
					m2=m2+inputImage[j+rY][i+rX];
					count4++;
				}

			}
			//inputImage[j+rY][i+rX]=labelCurvaturePos[i+(crmx/2)][j+(crmy/2)]*100;
		}
	}
}

m1=m1/count3;
m2=m2/count4;

for(i=-crmx/2;i<(crmx/2)+1;i++)
{
	for(j=-crmy/2;j<(crmy/2)+1;j++)
	{
		if(((i+rX)>=0)&&((i+rX)<dx)&&((j+rY)>=0)&&((j+rY)<dy))
		{
			if(((labelCurvaturePos[i+(crmx/2)][j+(crmy/2)])!=1000)&&CURVATURE_REGION_MASK[i+(crmx/2)][j+(crmy/2)]!=0)
			{
				if(labelCurvaturePos[i+(crmx/2)][j+(crmy/2)]==labelUsed[0])
				{
					v1=v1+fabs(m1-inputImage[j+rY][i+rX]);
					//inputImage[j+rY][i+rX]=(int)m1;	
				//	inputImage[j+rY][i+rX]=(int)200;	 //uncomment to see chosen curvature average color
				}
				else if (labelCurvaturePos[i+(crmx/2)][j+(crmy/2)]==labelUsed[1])
				{
					v2=v2+fabs(m2-inputImage[j+rY][i+rX]);
					//inputImage[j+rY][i+rX]=(int)m2;
					//inputImage[j+rY][i+rX]=(int)50;  //uncomment to see chosen curvature average color
				}

			}
			//inputImage[j+rY][i+rX]=labelCurvaturePos[i+(crmx/2)][j+(crmy/2)]*100;
		}
	}
}

v1=v1/count3;
v2=v2/count4;
s1IntensityMean[index]=m1;
s1IntensityVar[index]=v1;
s2IntensityMean[index]=m2;
s2IntensityVar[index]=v2;

}
else
{
s1IntensityMean[index]=0;
s1IntensityVar[index]=0;
s2IntensityMean[index]=0;
s2IntensityVar[index]=0;
}

return true;
}



bool EdgeSegmentType::setCurvaturePoint(int i,float *cValue, PointType *point)
{
int sz;
if(!curvaturePoints)
{
	AllocCurvatureIntensity(this->GetSize());
}
curvaturePoints[i].SetX(point->GetX());
curvaturePoints[i].SetY(point->GetY());
curvatureValue[i]=(*cValue);
sz=this->GetSize();

return true;
}


bool EdgeSegmentType::SetPoint(int size, PointType *point)
{
	PointType *pTmpP = point;
	if(!edgePoints)
	{
		AllocPoint(size);
	}

	PointType *pCurP = edgePoints;
	for(register int i=0; i<size ; i++ , pTmpP++, pCurP++)
	{	
		int x	= (*pTmpP).GetX();
		(*pCurP).SetX((*pTmpP).GetX());
		(*pCurP).SetY((*pTmpP).GetY());
	}
	return true;
}

bool EdgeSegmentType::SetPoint(list<PointType> *point, float **thin, float **theta)
{
	int i=0;
	int tmp=0;
	if(!edgePoints)
	{
		AllocPoint(point->size());
	}

	if(!edgeGrads)
	{
		AllocGrad(point->size());
	}
	
	if(!edgeTheta)
	{
		AllocTheta(point->size());
	}

	if(!s1IntensityMean)
	{
		AllocCurvatureIntensity(point->size());
	}

	list<PointType>::iterator curpoint;
	for(curpoint=point->begin(); curpoint!=point->end();curpoint++)
	{
		//tmp+=curpoint->value;
		edgePoints[i]=(*curpoint);
		edgeGrads[i] = thin[(*curpoint).GetY()][(*curpoint).GetX()];
		edgeTheta[i] = theta[(*curpoint).GetY()][(*curpoint).GetX()];
		i++;
	}
	
	return true;
}


void EdgeSegmentType::DeleteFirstNPoints(int size)
{
	PointType *tempEdgePoints   = new PointType[segSize-size];
	float *tempEdgeGrads	 = new float[segSize - size];
	float *tempEdgeTheta	 = new float[segSize - size];


	//new	
	float *tmps1IntensityMean=new float[(segSize - size)/SAMPLE_LENGTH];
	float *tmps1IntensityVar=new float[(segSize - size)/SAMPLE_LENGTH];
	float *tmps2IntensityMean=new float[(segSize - size)/SAMPLE_LENGTH];
	float *tmps2IntensityVar=new float[(segSize - size)/SAMPLE_LENGTH];
	float *tmpcurvatureValue=new float[(segSize - size)/SAMPLE_LENGTH];
	PointType *tmpcurvaturePoints=new PointType[(segSize - size)/SAMPLE_LENGTH];
	
	for(int i=(size/SAMPLE_LENGTH); i<((segSize)/SAMPLE_LENGTH); i++)
	{	
		tmps1IntensityMean[i]=s1IntensityMean[i];
		tmps1IntensityVar[i]=s1IntensityVar[i];
		tmps2IntensityMean[i]=s2IntensityMean[i];
		tmps2IntensityVar[i]=s2IntensityVar[i];
		tmpcurvatureValue[i]=curvatureValue[i];
		tmpcurvaturePoints[i]=curvaturePoints[i];
	}
	
	PointType *tempcp;
	float *temps1M;
	float *temps1V;
	float *temps2M;
	float *temps2V;
	float *tempcv;

	tempcp =curvaturePoints;
	temps1M=s1IntensityMean;
	temps1V=s1IntensityVar;
	temps2M=s2IntensityMean;
	temps2V=s2IntensityVar;
	tempcv=curvatureValue;

	curvaturePoints=tmpcurvaturePoints;
	s1IntensityMean=tmps1IntensityMean;
	s1IntensityVar=tmps1IntensityVar;
	s2IntensityMean=tmps2IntensityMean;
	s2IntensityVar=tmps2IntensityVar;
	curvatureValue=tmpcurvatureValue;


	delete tempcp;
	delete temps1M;
	delete temps1V;
	delete temps2M;
	delete temps2V;
	delete tempcv;
	//end new


	int i;
	for(i = size; i<segSize; i++)
	{
		tempEdgePoints[i-size] = edgePoints[i];
		tempEdgeGrads[i-size] = edgeGrads[i];
		tempEdgeTheta[i-size] = edgeTheta[i];

	}
	
	PointType *tempPoints;
	float *tempGrads;
	float *tempTheta;
	tempPoints =edgePoints;
	tempGrads =edgeGrads;
	tempTheta =edgeTheta;

	edgePoints =tempEdgePoints;
	edgeGrads  =tempEdgeGrads;
	edgeTheta  =tempEdgeTheta;

	delete tempPoints;
	delete tempGrads;
	delete tempTheta;
	
	segSize = segSize - size;
}

void EdgeSegmentType::DeleteLastNPoints(int size)
{
	PointType *tempEdgePoints   = new PointType[size];
	float *tempEdgeGrads	 = new float[size];
	float *tempEdgeTheta	 = new float[size];


	//new	
	float *tmps1IntensityMean=new float[(size)/SAMPLE_LENGTH];
	float *tmps1IntensityVar=new float[(size)/SAMPLE_LENGTH];
	float *tmps2IntensityMean=new float[(size)/SAMPLE_LENGTH];
	float *tmps2IntensityVar=new float[(size)/SAMPLE_LENGTH];
	float *tmpcurvatureValue=new float[(size)/SAMPLE_LENGTH];
	PointType *tmpcurvaturePoints=new PointType[(size)/SAMPLE_LENGTH];
	
	for(int i=0; i<((size)/SAMPLE_LENGTH); i++)
	{	
		tmps1IntensityMean[i]=s1IntensityMean[i];
		tmps1IntensityVar[i]=s1IntensityVar[i];
		tmps2IntensityMean[i]=s2IntensityMean[i];
		tmps2IntensityVar[i]=s2IntensityVar[i];
		tmpcurvatureValue[i]=curvatureValue[i];
		tmpcurvaturePoints[i]=curvaturePoints[i];
	}
	
	PointType *tempcp;
	float *temps1M;
	float *temps1V;
	float *temps2M;
	float *temps2V;
	float *tempcv;

	tempcp =curvaturePoints;
	temps1M=s1IntensityMean;
	temps1V=s1IntensityVar;
	temps2M=s2IntensityMean;
	temps2V=s2IntensityVar;
	tempcv=curvatureValue;


	curvaturePoints=tmpcurvaturePoints;
	s1IntensityMean=tmps1IntensityMean;
	s1IntensityVar=tmps1IntensityVar;
	s2IntensityMean=tmps2IntensityMean;
	s2IntensityVar=tmps2IntensityVar;
	curvatureValue=tmpcurvatureValue;

	delete tempcp;
	delete temps1M;
	delete temps1V;
	delete temps2M;
	delete temps2V;
	delete tempcv;
	//end new

	
	int i;
	for(i = 0; i<size; i++)
	{
		tempEdgePoints[i] = edgePoints[i];
		tempEdgeGrads[i] = edgeGrads[i];
		tempEdgeTheta[i] = edgeTheta[i];

	}

	PointType *tempPoints;
	float *tempGrads;
	float *tempTheta;
	tempPoints =edgePoints;
	tempGrads =edgeGrads;
	tempTheta =edgeTheta;

	edgePoints =tempEdgePoints;
	edgeGrads  =tempEdgeGrads;
	edgeTheta  =tempEdgeTheta;
	
	delete tempPoints;
	delete tempGrads;
	delete tempTheta;
	
	segSize = size;
}

void EdgeSegmentType::CopyBeforeN(EdgeSegmentType curEdgeSeg, int size)
{
	edgePoints   = new PointType[size];
	edgeGrads	 = new float[size];
	edgeTheta	 = new float[size];

	//new	
	int sz=((size)/SAMPLE_LENGTH);
	s1IntensityMean=new float[sz];
	s1IntensityVar=new float[sz];
	s2IntensityMean=new float[sz];
	s2IntensityVar=new float[sz];
	curvatureValue=new float[sz];
	curvaturePoints=new PointType[sz];
	
	for(int i=0; i<sz; i++)
	{	
		s1IntensityMean[i]=curEdgeSeg.s1IntensityMean[i];
		s1IntensityVar[i]=curEdgeSeg.s1IntensityVar[i];
		s2IntensityMean[i]=curEdgeSeg.s2IntensityMean[i];
		s2IntensityVar[i]=curEdgeSeg.s2IntensityVar[i];
		curvatureValue[i]=curEdgeSeg.curvatureValue[i];
		curvaturePoints[i]=curEdgeSeg.curvaturePoints[i];
	}
	
	//end new
	
	int i;
	for(i = 0; i<size; i++)
	{
		edgePoints[i] = curEdgeSeg.GetPoint(i);
		edgeGrads[i] = curEdgeSeg.GetGrad(i);
		edgeTheta[i] =curEdgeSeg.GetTheta(i);
	}
	segSize = size;
	segIndex = curEdgeSeg.GetSegIndex();
}

void EdgeSegmentType::CopyAfterN(EdgeSegmentType curEdgeSeg, int size)
{
	int oldSegSize =curEdgeSeg.GetSize();
	edgePoints   = new PointType[oldSegSize-size];
	edgeGrads	 = new float[oldSegSize-size];
	edgeTheta	 = new float[oldSegSize-size];

	//new	
	int sz=int((oldSegSize-size)/SAMPLE_LENGTH);
	s1IntensityMean=new float[sz];
	s1IntensityVar=new float[sz];
	s2IntensityMean=new float[sz];
	s2IntensityVar=new float[sz];
	curvatureValue=new float[sz];
	curvaturePoints=new PointType[sz];

	sz=int(oldSegSize/SAMPLE_LENGTH);
	int start=int(size/SAMPLE_LENGTH);
	for(int i=start; i<sz; i++)
	{	
		s1IntensityMean[i-start]=curEdgeSeg.s1IntensityMean[i];
		s1IntensityVar[i-start]=curEdgeSeg.s1IntensityVar[i];
		s2IntensityMean[i-start]=curEdgeSeg.s2IntensityMean[i];
		s2IntensityVar[i-start]=curEdgeSeg.s2IntensityVar[i];
		curvatureValue[i-start]=curEdgeSeg.curvatureValue[i];
		curvaturePoints[i-start]=curEdgeSeg.curvaturePoints[i];
	}
	//end new
	
	int i;
	for(i = size; i<oldSegSize; i++)
	{
		edgePoints[i-size] = curEdgeSeg.GetPoint(i);
		edgeGrads[i-size] = curEdgeSeg.GetGrad(i);
		edgeTheta[i-size] =curEdgeSeg.GetTheta(i);
	}
	segSize = oldSegSize-size;
	segIndex = curEdgeSeg.GetSegIndex();
}
void EdgeSegmentType::operator = (EdgeSegmentType sg)
{
	segSize = sg.segSize;
	segIndex = sg.segIndex;

/*	if(edgePoints != NULL){
		delete edgePoints;
		edgePoints =NULL;
	}	
*/	
	edgePoints = new PointType[segSize];
	
	//memset(edgePoints, 0, sizeof(PointType) * segSize);
	memcpy(edgePoints, sg.edgePoints, sizeof(PointType) * segSize);
}

PointType *EdgeSegmentType::GetStartPoint(void )
{
	if(edgePoints)
		return edgePoints;
	else
		return NULL;
}

PointType *EdgeSegmentType::GetEndPoint(void )
{
	
	if(edgePoints)
		return &edgePoints[segSize-1];
	else
		return NULL;
	//return &edgePoints[segSize-1];
}

PointType EdgeSegmentType::GetCurPoint()
{
	return edgePoints[curPointIndex];
}

PointType EdgeSegmentType::GetCurvaturePoint(int i)
{
	return curvaturePoints[i];
}
float EdgeSegmentType::GetCurvatureValue(int i)
{
return curvatureValue[i];
}

PointType EdgeSegmentType::GetPoint(int i)
{
	return edgePoints[i];
}

float EdgeSegmentType::GetGrad(int i)
{
	return edgeGrads[i];
}

float EdgeSegmentType::GetTheta(int i)
{
	return edgeTheta[i];
}

int  EdgeSegmentType::GetSize(){
	return segSize;
}

void EdgeSegmentType::SetSegIndex(int x)
{
	segIndex	= x; 
}

int EdgeSegmentType::GetSegIndex()
{
	return segIndex; 
}
void EdgeSegmentType::setBoundaryParam(int max, int may, int mix, int miy)
{
maxx=max;
maxy=may;
minx=mix;
miny=miy;
}

void EdgeSegmentType::getBoundaryParam(int *max, int *may, int *mix, int *miy)
{
*max=maxx;
*may=maxy;
*mix=minx;
*miy=miny;
}

void EdgeSegmentType::setEdgeCenterParam(float centerx, float centery)
{
centerX=centerx;
centerY=centery;
}

void EdgeSegmentType::getEdgeCenterParam(float *centerx, float *centery)
{
*centerx=centerX;
*centery=centerY;
}

void EdgeSegmentType::setClusterMemberID(int ID)
{
 clusterMemberID=ID;
}

int EdgeSegmentType::getClusterMemberID()
{
return  clusterMemberID;
}


void EdgeWeightType::SetWeight(int x)
{
	weight = x;
}

void EdgeWeightType::SetSegIndex(int x)
{
	segIndex = x;
}

int EdgeWeightType::GetWeight()
{
	return weight;
}

int EdgeWeightType::GetSegIndex()
{
	return segIndex;
}

//new
void setCurvatureIntensityValues(PointType *point,float *s1IntensityMean, float *s1IntensityVar, float *s2IntensityMean, float *s2IntensityVar)
{



}





void getCurvatureIntensityValues(PointType *point,float *s1IntensityMean, float *s1IntensityVar, float *s2IntensityMean, float *s2IntensityVar){



}
