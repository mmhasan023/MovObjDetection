#include "EdgeSegmentMatch.h"





#define BWINDOW 20
#define SWINDOW 5

#define colorSimilarityDistance 200.0

/*
int minimum(int val1, int val2, int val3, int val4, int val5){
	int minVal = val1;
	
	if(val2<minVal)
		minVal = val2;
	if(val3<minVal)
		minVal = val3;
	if(val4<minVal)
		minVal = val4;
	if(val5<minVal)
		minVal = val5;
	return minVal;
}*/

int edgeFoundInCandidate(int edgeNo,int tCan, int *iniCanEdge)
{
	int i;
	for(i=0;i<tCan;i++)
	{
		if(edgeNo==iniCanEdge[i]) return 1;
	}

return 0;
}

int **getEdgeMap(EdgeSegmentType edge, int maxx,int maxy,int minx,int miny,int dx,int dy,int width){
/*
	float *array = new float [dx*dy];
	float **data = new float*[dy   ];
	for( register int i=0 ; i<dy ; i++ )
		data[i] = &array[i*dx];
	return data;	
*/	
int *row=new int [dx*dy];
int **data=new int *[dy];
int i,j;
for(i=0 ; i<dy ; i++ )
		data[i] = &row[i*dx];

for(i=0; i<dy;i++)
	for(j=0; j<dx; j++)
		{
			data[i][j]=1000;
		}

PointType p;
int size = edge.GetSize();
	for( i=0 ; i<size ; i++)
	{
		p=edge.GetPoint(i);
		data[p.GetY()-miny+width][p.GetX()-minx+width] = 0;
	}

	for(i=1; i<dy;i++)
		for(j=1; j<dx; j++)
		{
			data[i][j] = minimum(data[i-1][j-1]+4, data[i-1][j]+3, data[i][j-1]+3, data[i][j]);
		}
	
	for(i=dy-2; i>=0;i--)
		for(j=dx-2; j>=0; j--)
		{
			data[i][j] = minimum(data[i][j], data[i][j+1]+3, data[i+1][j]+3, data[i+1][j+1]+4);
		}
return data;
}


float computeCorrelationMatch(float *edgeCur,int edgeSize, float canEdgeMean, float *canEdgeCur,int canEdgeSize, int *pos, float *angle){
int i, j;
float maxScore, score, numinator, denuminator1,denuminator2; 
float edgeMean=0, edgeSum;
*pos=0;
maxScore=0;
for(i=0;i<=(edgeSize-canEdgeSize);i++)
{
	
	edgeSum=0;
	//canEdgeSum=0;
	for(j=0;j<canEdgeSize;j++)
	{
		edgeSum+=edgeCur[i+j];
	//	canEdgeSum+=canEdgeCur[j];
	}
	edgeMean=edgeSum/canEdgeSize;
	//canEdgeMean=canEdgeSum/canEdgeSize;

	numinator=0;
	denuminator1=0;
	denuminator2=0;
	for(j=0;j<canEdgeSize;j++)
	{
		numinator+=fabs((canEdgeCur[j]-canEdgeMean)*(edgeCur[i+j]-edgeMean));
		denuminator1+=(canEdgeCur[j]-canEdgeMean)*(canEdgeCur[j]-canEdgeMean);
		denuminator2+=(edgeCur[i+j]-edgeMean)*(edgeCur[i+j]-edgeMean);
	}

	if(sqrt(denuminator1*denuminator2)==0)
	{
		score=0;
	}
	else
	{
		score=numinator/sqrt(denuminator1*denuminator2);
	}
	
	if(maxScore<score)
	{
		maxScore=score;
		*pos=i;
		*angle=edgeMean-canEdgeMean;
	}
}
return maxScore;
}


float computeCorrelation(float *edgeCur,int edgeSize, float em, float *canEdgeCur,int canEdgeSize, float cem, int *pos){
int i, j;
float maxScore, score, numinator, denuminator1,denuminator2; 
*pos=3;
maxScore=0;
for(i=0;i<=(edgeSize-canEdgeSize);i++)
{
	numinator=0;
	denuminator1=0;
	denuminator2=0;
	for(j=0;j<canEdgeSize;j++)
	{
		numinator+=fabs((canEdgeCur[j]-cem)*(edgeCur[i+j]-em));
		denuminator1+=(canEdgeCur[j]-cem)*(canEdgeCur[j]-cem);
		denuminator2+=(edgeCur[i+j]-em)*(edgeCur[i+j]-em);
	}
	if(sqrt(denuminator1*denuminator2)==0)
	{
		score=0;
	}
	else
	{
		score=numinator/sqrt(denuminator1*denuminator2);
	}
	
	if(maxScore<score)
	{
		maxScore=score;
		*pos=i+3;
	}
}
return maxScore;
}
float AngleBasedMatch(KSdUint8 ** pOutputImage, EdgeSegmentType *edge, EdgeSegmentType *canEdge, int *distX, int *distY, float *angle){
int edgeSize=edge->GetSize();
int canEdgeSize=canEdge->GetSize();
*distX=0;
*distY=0;
//if((float)(edgeSize/canEdgeSize)>=3||(float)(edgeSize/canEdgeSize)<=0.33) return 0;
if(edgeSize>=12&&canEdgeSize>=12)
{
/*

int ii;
PointType pt;
for(ii=0;ii<canEdgeSize;ii++)
{	pt=canEdge->GetPoint(ii);
	pOutputImage[pt.GetY()][pt.GetX()]=255;


}
for(ii=0;ii<edgeSize;ii++)
{	pt=edge->GetPoint(ii);
	pOutputImage[pt.GetY()][pt.GetX()]=255;


}

*/

float *edgeCur = new float[edgeSize];
float *canEdgeCur = new float[canEdgeSize];
float score,sum,em=0, cem=0;
int i,pos,dx,dy;
PointType pt1, pt2;

sum=0;
pt1=edge->GetPoint(0);
for(i=0;i<edgeSize;i++)
{	pt2=edge->GetPoint(i);
	dx=pt1.GetX()-pt2.GetX();
	dy=pt1.GetY()-pt2.GetY();
//	dx=abs(dx);
//	dy=abs(dy);

    if(dx==0)
	{
		edgeCur[i]=atan2(dy,0.0001)*180/3.141592654+180;
		i=i;
	}
	else
	{
		edgeCur[i]=atan2((double)dy,(double)dx)*180/3.141592654+180;
		i=i;
	
	}
sum+=edgeCur[i];
}
em=sum/edgeSize;

sum=0;
pt1=canEdge->GetPoint(0);
for(i=0;i<canEdgeSize;i++)
{
	pt2=canEdge->GetPoint(i);
	dx=pt1.GetX()-pt2.GetX();
	dy=pt1.GetY()-pt2.GetY();
	
    if(dx==0)
	{
		canEdgeCur[i]=atan2(dy,0.0001)*180/3.141592654+180;
	}
	else
	{
		canEdgeCur[i]=atan2((double)dy,(double)dx)*180/3.141592654+180;
	
	}
sum+=canEdgeCur[i];
}
cem=sum/canEdgeSize;


if(edgeSize>=canEdgeSize)
{

//	score=computeCorrelationMatch(edgeCur,edgeSize, em, canEdgeCur,canEdgeSize,cem,&pos);
	score=computeCorrelationMatch(edgeCur,edgeSize, cem, canEdgeCur,canEdgeSize,&pos, angle);
	*distX=edge->GetPoint(pos).GetX()-canEdge->GetPoint(0).GetX();
	*distY=edge->GetPoint(pos).GetY()-canEdge->GetPoint(0).GetY();
	//score=score/(canEdgeSize-6);

}
else{
	score=computeCorrelationMatch(canEdgeCur,canEdgeSize, em, edgeCur,edgeSize,&pos, angle);
	*distX=edge->GetPoint(0).GetX()-canEdge->GetPoint(pos).GetX();
	*distY=edge->GetPoint(0).GetY()-canEdge->GetPoint(pos).GetY();
	//score=score/(edgeSize-6);
}

delete [] edgeCur;
delete [] canEdgeCur;
//return 0;
return score;
}
return 0;
}










float curvatureBasedMatch(EdgeSegmentType *edge, EdgeSegmentType *canEdge, int *distX, int *distY){
int edgeSize=edge->GetSize();
int canEdgeSize=canEdge->GetSize();
*distX=0;
*distY=0;
//if(float(edgeSize/canEdgeSize)>=0.25&&float(edgeSize/canEdgeSize)<=4)
{
if(edgeSize>=15&&canEdgeSize>=15)
{
float *edgeCur = new float[edgeSize-6];
float *canEdgeCur = new float[canEdgeSize-6];
float score,sum,em=0,cem=0;
int i,pos;
PointType pt1, pt2,pt3,pt4,pt5,pt6,pt7;

sum=0;
for(i=3;i<edgeSize-3;i++)
{
	pt1=edge->GetPoint(i-3);
	pt2=edge->GetPoint(i-2);
	pt3=edge->GetPoint(i-1);
	pt4=edge->GetPoint(i);
	pt5=edge->GetPoint(i+1);
	pt6=edge->GetPoint(i+2);
	pt7=edge->GetPoint(i+3);
	edgeCur[i-3]=fabs(curvature(pt1,pt2,pt3,pt4,pt5,pt6,pt7));
	sum+=edgeCur[i-3];
}
em=sum/(edgeSize-6);

sum=0;
for(i=3;i<canEdgeSize-3;i++)
{
	pt1=canEdge->GetPoint(i-3);
	pt2=canEdge->GetPoint(i-2);
	pt3=canEdge->GetPoint(i-1);
	pt4=canEdge->GetPoint(i);
	pt5=canEdge->GetPoint(i+1);
	pt6=canEdge->GetPoint(i+2);
	pt7=canEdge->GetPoint(i+3);
	canEdgeCur[i-3]=fabs(curvature(pt1,pt2,pt3,pt4,pt5,pt6,pt7));
	sum+=canEdgeCur[i-3];
}
cem=sum/(canEdgeSize-6);

if(edgeSize>=canEdgeSize)
{
	score=computeCorrelation(edgeCur,edgeSize-6,em, canEdgeCur,canEdgeSize-6,cem,&pos);
	*distX=edge->GetPoint(pos).GetX()-canEdge->GetPoint(3).GetX();
	*distY=edge->GetPoint(pos).GetY()-canEdge->GetPoint(3).GetY();
	//score=score/(canEdgeSize-6);

}
else{
	score=computeCorrelation(canEdgeCur,canEdgeSize-6,cem,edgeCur,edgeSize-6,em,&pos);
	*distX=edge->GetPoint(3).GetX()-canEdge->GetPoint(pos).GetX();
	*distY=edge->GetPoint(3).GetY()-canEdge->GetPoint(pos).GetY();
	//score=score/(edgeSize-6);
}

delete [] edgeCur;
delete [] canEdgeCur;


return score;
}
return 0;
}
	return 0;
}

/*

float HaughEdgeMatch(int ** edgeMap, int dx, int dy,int *distX, int *distY, EdgeSegmentType edgeC)
{
	int maxx, maxy, minx, miny;
	long distance, minDistance=32767;
	int posX=0, posY=0,k,l,i;
	edgeC.getBoundaryParam(&maxx, &maxy, &minx, &miny);
	int size = edgeC.GetSize();
	PointType p;
	
	for(k=2; k<dy-(maxy-miny+1)-2;k++)
	{
		for(l=2; l<dx-(maxx-minx+1)-2;l++)
		{ 
			distance=0;
			for( i=0 ; i<size ; i++)
			{
				p=edgeC.GetPoint(i);
				distance+=edgeMap[p.GetY()-miny+k][p.GetX()-minx+l]*edgeMap[p.GetY()-miny+k][p.GetX()-minx+l];
			}
			if(distance<minDistance)
			{
				minDistance=distance;
				posY=k;
				posX=l;

			}
		
		}
	}
	*distX=minx+posY-SWINDOW;
	*distY=miny+posX-SWINDOW;
	return float(sqrt(minDistance/size)/3.0);
}
*/

int edgeSegmentMatch(KSdUint8 ** pOutputImage, EdgeClusterType *curCluster, EdgeSegmentType edge, EdgeListType *EdgeList, int **pMovDistance, int **pMovLabel, int dx, int dy, float *score, float *movX, float *movY, int *segI)
{
int windowL=0, WINDOW=0;
int windowR=0; 
int windowU=0;
int windowD=0;
float edgeCenterX=0;
float edgeCenterY=0;
int i,j,edgeNo=0,flag=0,k;
int tCan=0,tCanEColor=0;
int minx,miny,maxx,maxy;
float  maxScore=0,angle=0;
float  meanX, meanY,weightFactor;
int distX, distY;
float mX, mY;


//FILE *fp=fopen("d:\\out.cpp","w");

if((curCluster)==NULL)
{
	WINDOW=BWINDOW;
	edge.getEdgeCenterParam(&edgeCenterX,&edgeCenterY);
	edge.getBoundaryParam(&maxx,&maxy,&minx,&miny);
	edge.getMotion(&mX,&mY);
}
else
{
	WINDOW=SWINDOW;
	(*curCluster).getBoundaryParam(&maxx,&maxy,&minx,&miny);
	(*curCluster).getMotion(&mX, &mY);
}


list<EdgeSegmentType>::iterator curEdgeSeg; 


//perform boundary check
windowL=(miny+mY-WINDOW)>0? (miny+mY-WINDOW):0;
windowR=(maxy+mY+WINDOW)<dy? (maxy+mY+WINDOW):dy;
windowU=(minx+mX-WINDOW)>0? (minx+mX-WINDOW):0;
windowD=(maxx+mX+WINDOW)<dx? (maxx+mX+WINDOW):dx;
if(((windowR-windowL+1)<0)||(windowD-windowU+1)<0||(windowR-windowL+1)>dy-1||(windowD-windowU+1)>dx-1){
	return 2;
}




int *iniCanEdge = new int[(windowR-windowL+1)*(windowD-windowU+1)];
//for(i=0;i<windowR;i++)
for(i=windowL;i<windowR;i++)
{
	for(j=windowU;j<windowD;j++)
	//for(j=0;j<windowD;j++)
	{							//if(i==windowL||j==windowU||j==windowD-1||i==windowR-1)
								//pOutputImage[i][j]=255;

		if(pMovDistance[i][j]==0)
		{
			
			edgeNo=pMovLabel[i][j];
			/*fprintf(fp,"%d ",edgeNo);*/
			if(edgeNo>=1000) continue;
			if(edgeFoundInCandidate(edgeNo,tCan, iniCanEdge)==1) continue;
			else
			{
				iniCanEdge[tCan]=edgeNo;
				tCan++;
			}
		}
	}
/*	fprintf(fp,"\n");*/
}

	if(tCan>0)
	{
		int *colorCanEdge = new int[tCan];
		tCanEColor=0;
		
		for(i=0;i<tCan;i++)
		{
			for(curEdgeSeg=EdgeList->ListBegin();curEdgeSeg!=EdgeList->ListEnd();curEdgeSeg++)
			{
				flag=0;
								
				if((*curEdgeSeg).GetSegIndex()==iniCanEdge[i])
				{	/*		
					meanE=edge.getSideColor(1);
					variE=edge.getSideColorVar(1);

					mean=(*curEdgeSeg).getSideColor(1);
					vari=(*curEdgeSeg).getSideColorVar(1);

					variE=variE>vari? vari: variE;
					mahadist=fabs((meanE-mean)*(meanE-mean)/variE*variE);
				
					if(mahadist<colorSimilarityDistance) flag++;

					meanE2=edge.getSideColor(2);
					variE2=edge.getSideColorVar(2);
					mean2=(*curEdgeSeg).getSideColor(2);
					vari2=(*curEdgeSeg).getSideColorVar(2);

					vari2=vari2<variE2? vari2: variE2;
					mahadist2=fabs((mean2-meanE2)*(mean2-meanE2)/(vari2*vari2));
					if(mahadist2<colorSimilarityDistance) flag++;

					*/

					//variE=variE<variE2? variE: variE2;
					//mahadist=fabs((mean2-mean)*(mean2-mean)/(vari*vari));
					//mahadist2=fabs((meanE2-meanE)*(meanE2-meanE)/(variE*variE));
					
					//mahadistRatio=mahadist/mahadist2;
					//if(mahadistRatio>=0.7&&mahadistRatio<=1.3) flag=0;
					//else flag++;
					
					//shapeRatio=(*curEdgeSeg).getEdgeShape()/edge.getEdgeShape();
					//if(shapeRatio>0.7&&shapeRatio<1.3) flag++;
					
					//flag++;

					if(flag>=0)
					{
					colorCanEdge[tCanEColor++]=iniCanEdge[i];
					}
					break;
				}
			}
		}
		if(tCanEColor>0)
		{
			int segIndex;
			float *MatchScore = new float[tCanEColor];
			int *DistX = new int[tCanEColor];
			int *DistY = new int[tCanEColor];
			float *angleT = new float[tCanEColor];
		//	int *edgeWeight = new int[tCanEColor];

		//	int *SegIndex=new int[tCanEColor];
			for(i=0;i<tCanEColor;i++)
			{
				MatchScore[i]=0;
				DistX[i]=1000;
				DistY[i]=1000;
				angleT[i]=360;
			//	edgeWeight[i]=1;

				for(curEdgeSeg=EdgeList->ListBegin();curEdgeSeg!=EdgeList->ListEnd();curEdgeSeg++)
					{
						if((*curEdgeSeg).GetSegIndex()==colorCanEdge[i])
						{
							//(*curEdgeSeg).getBoundaryParam(&maxx,&maxy,&minx,&miny);
							//MatchScore[i]=AngleBasedMatch(&edge,&(*curEdgeSeg),&distX,&distY, &angle);//HaughEdgeMatch(edgeMap,dx,dy,&distX,&distY,(*curEdgeSeg)); 
							MatchScore[i]=AngleBasedMatch(pOutputImage, &edge,&(*curEdgeSeg),&distX,&distY, &angle);//HaughEdgeMatch(edgeMap,dx,dy,&distX,&distY,(*curEdgeSeg)); 
							DistX[i]=distX;
							DistY[i]=distY;
							angleT[i]=fabs(angle);
						//	edgeWeight[i]=(*curEdgeSeg).getWeight();
							break;
						}
					}
			}
			
		maxScore=MatchScore[0];
		distX=DistX[0];
		distY=DistY[0];
		angle=angleT[0];
//		weightFactor=edgeWeight[0];
		segIndex=colorCanEdge[0];
		edge.getMotion(&meanX, &meanY);
		
		for(k=0;k<tCanEColor;k++)
		{
			if(MatchScore[k]>0.8)
			{
				maxScore=MatchScore[k];
				distX=DistX[k];
				distY=DistY[k];
				angle=angleT[k];
//		weightFactor=edgeWeight[0];
				segIndex=colorCanEdge[k];
				break;
			}
		}
		
	//	curEdgeWeight=edge.getWeight();
	//	if(curEdgeWeight<=100)
	//	{
	//	weightFactor=(105-curEdgeWeight)/5;
	//	}
	//	else {
	//		weightFactor=1;
	//	}
	//	weightFactor=edge.getFrameIndex();
		
		weightFactor=1;
		for(i=k+1;i<tCanEColor;i++)
		{
			//if(maxScore<MatchScore[i]){
			//if((abs(distX)+abs(distY))>(abs(DistX[i])+abs(DistY[i])))
			if(MatchScore[i]>0.8&&(fabs(DistX[i]+meanX*weightFactor)+fabs(DistY[i]+meanY*weightFactor))<(fabs(distX+meanX*weightFactor)+fabs(distY+meanY*weightFactor)))
			{
				if(angleT[i]<=angle||angleT[i]<30){
				distX=DistX[i];//+(meanX*(weightFactor-1));
				distY=DistY[i];//+(meanY*(weightFactor-1));
				angle=angleT[i];
				maxScore=MatchScore[i];
				segIndex=colorCanEdge[i];
				}
			}
		}

		*segI=segIndex;
		*score=maxScore;
		*movX=distX;
		*movY=distY;

		delete [] MatchScore;
		delete [] DistX;
		delete [] DistY;
		delete [] angleT;


/*
		int * array = edgeMap[0];
		delete [] array;
		delete [] edgeMap;
	*/	
		}
	
	delete [] colorCanEdge;
	colorCanEdge=NULL;
	}

delete [] iniCanEdge;	
iniCanEdge=NULL;



if(maxScore>0.8) return 1;	
return 0;
}




float AngleMatch(EdgeSegmentType *edge, EdgeSegmentType *canEdge, int *distX, int *distY, float *angle){
int edgeSize=edge->GetSize();
int canEdgeSize=canEdge->GetSize();
*distX=0;
*distY=0;
//if((float)(edgeSize/canEdgeSize)>=3||(float)(edgeSize/canEdgeSize)<=0.33) return 0;
if(edgeSize>=12&&canEdgeSize>=12)
{
/*

int ii;
PointType pt;
for(ii=0;ii<canEdgeSize;ii++)
{	pt=canEdge->GetPoint(ii);
	pOutputImage[pt.GetY()][pt.GetX()]=255;


}
for(ii=0;ii<edgeSize;ii++)
{	pt=edge->GetPoint(ii);
	pOutputImage[pt.GetY()][pt.GetX()]=255;


}

*/

float *edgeCur = new float[edgeSize];
float *canEdgeCur = new float[canEdgeSize];
float score,sum,em=0, cem=0;
int i,pos,dx,dy;
PointType pt1, pt2;

sum=0;
pt1=edge->GetPoint(0);
for(i=0;i<edgeSize;i++)
{	pt2=edge->GetPoint(i);
	dx=pt1.GetX()-pt2.GetX();
	dy=pt1.GetY()-pt2.GetY();
//	dx=abs(dx);
//	dy=abs(dy);

    if(dx==0)
	{
		edgeCur[i]=atan2(dy,0.0001)*180/3.141592654+180;
		i=i;
	}
	else
	{
		edgeCur[i]=atan2((double)dy,(double)dx)*180/3.141592654+180;
		i=i;
	
	}
sum+=edgeCur[i];
}
em=sum/edgeSize;

sum=0;
pt1=canEdge->GetPoint(0);
for(i=0;i<canEdgeSize;i++)
{
	pt2=canEdge->GetPoint(i);
	dx=pt1.GetX()-pt2.GetX();
	dy=pt1.GetY()-pt2.GetY();
	
    if(dx==0)
	{
		canEdgeCur[i]=atan2(dy,0.0001)*180/3.141592654+180;
	}
	else
	{
		canEdgeCur[i]=atan2((double)dy,(double)dx)*180/3.141592654+180;
	
	}
sum+=canEdgeCur[i];
}
cem=sum/canEdgeSize;


if(edgeSize>=canEdgeSize)
{

//	score=computeCorrelationMatch(edgeCur,edgeSize, em, canEdgeCur,canEdgeSize,cem,&pos);
	score=computeCorrelationMatch(edgeCur,edgeSize, cem, canEdgeCur,canEdgeSize,&pos, angle);
	*distX=edge->GetPoint(pos).GetX()-canEdge->GetPoint(0).GetX();
	*distY=edge->GetPoint(pos).GetY()-canEdge->GetPoint(0).GetY();
	//score=score/(canEdgeSize-6);

}
else{
	score=computeCorrelationMatch(canEdgeCur,canEdgeSize, em, edgeCur,edgeSize,&pos, angle);
	*distX=edge->GetPoint(0).GetX()-canEdge->GetPoint(pos).GetX();
	*distY=edge->GetPoint(0).GetY()-canEdge->GetPoint(pos).GetY();
	//score=score/(edgeSize-6);
}

delete [] edgeCur;
delete [] canEdgeCur;
//return 0;
return score;
}
return 0;
}

float AngleMatchEdge(EdgeSegmentType *edge, EdgeSegmentType *canEdge, int *distX, int *distY, float *angle){
int edgeSize=edge->GetSize();
int canEdgeSize=canEdge->GetSize();
*distX=0;
*distY=0;
//if((float)(edgeSize/canEdgeSize)>=3||(float)(edgeSize/canEdgeSize)<=0.33) return 0;
if(edgeSize>=4&&canEdgeSize>=4)
{
/*

int ii;
PointType pt;
for(ii=0;ii<canEdgeSize;ii++)
{	pt=canEdge->GetPoint(ii);
	pOutputImage[pt.GetY()][pt.GetX()]=255;


}
for(ii=0;ii<edgeSize;ii++)
{	pt=edge->GetPoint(ii);
	pOutputImage[pt.GetY()][pt.GetX()]=255;


}

*/

float *edgeCur = new float[edgeSize];
float *canEdgeCur = new float[canEdgeSize];
float score,sum,em=0, cem=0;
int i,pos,dx,dy;
PointType pt1, pt2;

sum=0;
pt1=edge->GetPoint(0);
for(i=0;i<edgeSize;i++)
{	pt2=edge->GetPoint(i);
	dx=pt1.GetX()-pt2.GetX();
	dy=pt1.GetY()-pt2.GetY();
//	dx=abs(dx);
//	dy=abs(dy);

    if(dx==0)
	{
		edgeCur[i]=atan2(dy,0.0001)*180/3.141592654+180;
		i=i;
	}
	else
	{
		edgeCur[i]=atan2((double)dy,(double)dx)*180/3.141592654+180;
		i=i;
	
	}
sum+=edgeCur[i];
}
em=sum/edgeSize;

sum=0;
pt1=canEdge->GetPoint(0);
for(i=0;i<canEdgeSize;i++)
{
	pt2=canEdge->GetPoint(i);
	dx=pt1.GetX()-pt2.GetX();
	dy=pt1.GetY()-pt2.GetY();
	
    if(dx==0)
	{
		canEdgeCur[i]=atan2(dy,0.0001)*180/3.141592654+180;
	}
	else
	{
		canEdgeCur[i]=atan2((double)dy,(double)dx)*180/3.141592654+180;
	
	}
sum+=canEdgeCur[i];
}
cem=sum/canEdgeSize;


if(edgeSize>=canEdgeSize)
{

//	score=computeCorrelationMatch(edgeCur,edgeSize, em, canEdgeCur,canEdgeSize,cem,&pos);
	score=computeCorrelationMatch(edgeCur,edgeSize, cem, canEdgeCur,canEdgeSize,&pos, angle);
	*distX=edge->GetPoint(pos).GetX()-canEdge->GetPoint(0).GetX();
	*distY=edge->GetPoint(pos).GetY()-canEdge->GetPoint(0).GetY();
	//score=score/(canEdgeSize-6);

}
else{
	score=computeCorrelationMatch(canEdgeCur,canEdgeSize, em, edgeCur,edgeSize,&pos, angle);
	*distX=edge->GetPoint(0).GetX()-canEdge->GetPoint(pos).GetX();
	*distY=edge->GetPoint(0).GetY()-canEdge->GetPoint(pos).GetY();
	//score=score/(edgeSize-6);
}

delete [] edgeCur;
delete [] canEdgeCur;
//return 0;
return score;
}
return 0;
}



int edgeMatch(EdgeSegmentType edge, EdgeListType *EdgeList, int **pMovDistance, int **pMovLabel, int dx, int dy, int *segI, int &n)
{
int windowL=0, WINDOW;
int windowR=0; 
int windowU=0;
int windowD=0;
float edgeCenterX=0;
float edgeCenterY=0;
int i,j,edgeNo=0,flag=0,k;
int tCan=0,tCanEColor=0;
int minx,miny,maxx,maxy;
float  maxScore=0,angle=0;
float  meanX, meanY,curEdgeWeight,weightFactor;
int distX, distY;
float mX, mY;
int *colorCanEdge;


//FILE *fp=fopen("d:\\out.cpp","w");
	WINDOW=2;
	edge.getBoundaryParam(&maxx,&maxy,&minx,&miny);
	


list<EdgeSegmentType>::iterator curEdgeSeg; 


//perform boundary check
windowL=(miny-WINDOW)>0? (miny-WINDOW):0;
windowR=(maxy+WINDOW)<dy? (maxy+WINDOW):dy;
windowU=(minx-WINDOW)>0? (minx-WINDOW):0;
windowD=(maxx+WINDOW)<dx? (maxx+WINDOW):dx;

int *iniCanEdge = new int[(windowR-windowL+1)*(windowD-windowU+1)];
//for(i=0;i<windowR;i++)
for(i=windowL;i<windowR;i++)
{
	for(j=windowU;j<windowD;j++)
	//for(j=0;j<windowD;j++)
	{							//if(i==windowL||j==windowU||j==windowD-1||i==windowR-1)
								//pOutputImage[i][j]=255;

		if(pMovDistance[i][j]==0)
		{
			
			edgeNo=pMovLabel[i][j];
			/*fprintf(fp,"%d ",edgeNo);*/
			if(edgeNo>=1000) continue;
			if(edgeFoundInCandidate(edgeNo,tCan, iniCanEdge)==1) continue;
			else
			{
				iniCanEdge[tCan]=edgeNo;
				tCan++;
			}
		}
	}
/*	fprintf(fp,"\n");*/
}

	if(tCan>0)
	{
		colorCanEdge = new int[tCan];
		tCanEColor=0;
		
		for(i=0;i<tCan;i++)
		{
			for(curEdgeSeg=EdgeList->ListBegin();curEdgeSeg!=EdgeList->ListEnd();curEdgeSeg++)
			{
				flag=0;
								
				if((*curEdgeSeg).GetSegIndex()==iniCanEdge[i])
				{	/*		
					meanE=edge.getSideColor(1);
					variE=edge.getSideColorVar(1);

					mean=(*curEdgeSeg).getSideColor(1);
					vari=(*curEdgeSeg).getSideColorVar(1);

					variE=variE>vari? vari: variE;
					mahadist=fabs((meanE-mean)*(meanE-mean)/variE*variE);
				
					if(mahadist<colorSimilarityDistance) flag++;

					meanE2=edge.getSideColor(2);
					variE2=edge.getSideColorVar(2);
					mean2=(*curEdgeSeg).getSideColor(2);
					vari2=(*curEdgeSeg).getSideColorVar(2);

					vari2=vari2<variE2? vari2: variE2;
					mahadist2=fabs((mean2-meanE2)*(mean2-meanE2)/(vari2*vari2));
					if(mahadist2<colorSimilarityDistance) flag++;

					*/

					//variE=variE<variE2? variE: variE2;
					//mahadist=fabs((mean2-mean)*(mean2-mean)/(vari*vari));
					//mahadist2=fabs((meanE2-meanE)*(meanE2-meanE)/(variE*variE));
					
					//mahadistRatio=mahadist/mahadist2;
					//if(mahadistRatio>=0.7&&mahadistRatio<=1.3) flag=0;
					//else flag++;
					
					//shapeRatio=(*curEdgeSeg).getEdgeShape()/edge.getEdgeShape();
					//if(shapeRatio>0.7&&shapeRatio<1.3) flag++;
					
					//flag++;

					if(flag>=0)
					{
					colorCanEdge[tCanEColor++]=iniCanEdge[i];
					}
					break;
				}
			}
		}
		if(tCanEColor>0)
		{
			int segIndex;
			float *MatchScore = new float[tCanEColor];
			int *DistX = new int[tCanEColor];
			int *DistY = new int[tCanEColor];
			float *angleT = new float[tCanEColor];
		//	int *edgeWeight = new int[tCanEColor];

		//	int *SegIndex=new int[tCanEColor];
			maxScore=0;
			for(i=0;i<tCanEColor;i++)
			{
				MatchScore[i]=0;
				DistX[i]=1000;
				DistY[i]=1000;
				angleT[i]=360;
			//	edgeWeight[i]=1;

				for(curEdgeSeg=EdgeList->ListBegin();curEdgeSeg!=EdgeList->ListEnd();curEdgeSeg++)
					{
						if((*curEdgeSeg).GetSegIndex()==colorCanEdge[i])
						{
							segI[i]=colorCanEdge[i];
							MatchScore[i]=AngleMatchEdge(&edge,&(*curEdgeSeg),&distX,&distY, &angle);//HaughEdgeMatch(edgeMap,dx,dy,&distX,&distY,(*curEdgeSeg)); 
							DistX[i]=distX;
							DistY[i]=distY;
							angleT[i]=fabs(angle);
							if(MatchScore[i]<0.7){
							segI[i]=0;
							}
							if(maxScore<MatchScore[i]) { maxScore=MatchScore[i]; }
						//	edgeWeight[i]=(*curEdgeSeg).getWeight();
							break;
						}
					}
			}
	/*		
		maxScore=MatchScore[0];
		distX=DistX[0];
		distY=DistY[0];
		angle=angleT[0];
//		weightFactor=edgeWeight[0];
		segIndex=colorCanEdge[0];
		edge.getMotion(&meanX, &meanY);
		
		for(k=0;k<tCanEColor;k++)
		{
			if(MatchScore[k]>0.8)
			{
				maxScore=MatchScore[k];
				distX=DistX[k];
				distY=DistY[k];
				angle=angleT[k];
//		weightFactor=edgeWeight[0];
				segIndex=colorCanEdge[k];
				break;
			}
		}
		
	//	curEdgeWeight=edge.getWeight();
	//	if(curEdgeWeight<=100)
	//	{
	//	weightFactor=(105-curEdgeWeight)/5;
	//	}
	//	else {
	//		weightFactor=1;
	//	}
	//	weightFactor=edge.getFrameIndex();
		
		weightFactor=1;
		for(i=k+1;i<tCanEColor;i++)
		{
			//if(maxScore<MatchScore[i]){
			//if((abs(distX)+abs(distY))>(abs(DistX[i])+abs(DistY[i])))
			if(MatchScore[i]>0.8&&(fabs(DistX[i]+meanX*weightFactor)+fabs(DistY[i]+meanY*weightFactor))<(fabs(distX+meanX*weightFactor)+fabs(distY+meanY*weightFactor)))
			{
				if(angleT[i]<=angle||angleT[i]<30){
				distX=DistX[i];//+(meanX*(weightFactor-1));
				distY=DistY[i];//+(meanY*(weightFactor-1));
				angle=angleT[i];
				maxScore=MatchScore[i];
				segIndex=colorCanEdge[i];
				}
			}
		}

		*segI=segIndex;
//		*score=maxScore;
	//	*movX=distX;
	//	*movY=distY;
*/
		delete [] MatchScore;
		delete [] DistX;
		delete [] DistY;
		delete [] angleT;


/*
		int * array = edgeMap[0];
		delete [] array;
		delete [] edgeMap;
	*/	
		}
	
	delete [] colorCanEdge;
	colorCanEdge=NULL;
	}

delete [] iniCanEdge;	
iniCanEdge=NULL;

n=tCanEColor;
if(maxScore>=0.7) return 1;	
return 0;
}