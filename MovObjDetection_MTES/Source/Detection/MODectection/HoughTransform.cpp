#include "HoughTransform.h"



#define SEG_LENGTH 8
#define T_DEG 36
#define PI 3.141592654
#define WINDOW 5
#define MASK 5
#define dimX 800
#define dimY 800
int Accumulator[dimY][dimX];

void drawArrow(KSdUint8 **pOutputImage,int Xc,int Yc,int mx,int my,int dx,int dy)
{

int i;
int minx, maxx,miny,maxy;
minx=Xc<(Xc+mx)?Xc:(Xc+mx);
maxx=Xc>(Xc+mx)?Xc:(Xc+mx);

miny=Yc<(Yc+my)?Yc:(Yc+my);
maxy=Yc>(Yc+my)?Yc:(Yc+my);


if(minx<0) minx=0;
if(miny<0) miny=0;
if(maxx>=dx) maxx=dx-1;
if(maxy>=dy) maxy=dy-1;

for(i=minx; i<=maxx;i++)
{
pOutputImage[miny][i]=255;
pOutputImage[maxy][i]=255;
}

for(i=miny; i<=maxy;i++)
{
pOutputImage[i][minx]=255;
pOutputImage[i][maxx]=255;
}

}


int edgeInsideCluster(EdgeSegmentType edge, EdgeClusterType *curCluster, int Sx, int Sy)
{
int minx,miny,maxx,maxy,count,size,i;
(*curCluster).getBoundaryParam(&maxx,&maxy,&minx,&miny);
minx=(minx-WINDOW)>0? (minx-WINDOW):0;
maxx=(maxx+WINDOW)<Sx-1? (maxx+WINDOW):Sx-1;
miny=(miny-WINDOW)>0? (miny-WINDOW):0;
maxy=(maxy+WINDOW)<Sy-1? (maxy+WINDOW):Sy-1;

size=edge.GetSize();
PointType pt;

count=0;
for(i=0;i<size;i++)
{
pt=edge.GetPoint(i);
if(pt.GetX()>=minx&&pt.GetX()<=maxx&&pt.GetY()>=miny&&pt.GetY()<=maxy)
count++;
}

if(((float)count/size)>=0.7 ) return 1;
return 0;
}

void drawrec(KSdUint8 **pOutputImage, int minx, int miny, int maxx, int maxy, int color){
int i;

for(i=minx; i<maxx;i++)
{
pOutputImage[miny][i]=color;
pOutputImage[maxy-1][i]=color;
}

for(i=miny; i<maxy;i++)
{
pOutputImage[i][minx]=color;
pOutputImage[i][maxx-1]=color;
}
}

int edgeCandidateFound(int edgeNo,int tCan, int *iniCanEdge)
{
	int i;
	for(i=0;i<tCan;i++)
	{
		if(edgeNo==iniCanEdge[i]) return 1;
	}

return 0;
}


int findIndex(float theta)
{
	int i;
	//float degree[36];
	for(i=1;i<T_DEG;i++)
		if(((i-T_DEG/2)*(360/T_DEG))>=theta) break;
		//degree[i]=i*10;
return i-1;
}
int findTheta(int th)
{
return ((th-T_DEG/2+1)*(360/T_DEG));
}


void updateClusterMemberSegments(KSdUint8 ** pOutputImage, EdgeClusterList *clusterList, EdgeClusterType *curCluster, EdgeListType *EdgeList, int **pMovDistance, int **pMovLabel, int Sx, int Sy, int posX, int posY, int found)
{
	int i, j, tCan, edgeNo, minx,miny,maxx,maxy,windowL,windowR,windowU,windowD;
	PointType pt,pt1;
	float Xc,Yc,movX,movY;
	int Xpc, Ypc, dex,dey,flag,flag2;
	float angle;

	list<EdgeSegmentType>::iterator clusterEdgeSeg;
	list<EdgeSegmentType>::iterator curEdgeSeg;
	(*curCluster).getBoundaryParam(&maxx,&maxy,&minx,&miny);

	//drawrec(pOutputImage,minx,miny,maxx,maxy,150);

	//(*curCluster).getMotion(&movX, &movY);
	Xc=(maxx+minx)/2;
	Yc=(maxy+miny)/2;
	
//	pOutputImage[(int)Yc][(int)Xc]=255;

	
	Xpc=posX;
	Ypc=posY;
	//movX=Xpc-Xc;
	//movY=Ypc-Yc;
	movX=movY=0;

//	pOutputImage[(int)Ypc][(int)Xpc]=255;

	if(found==1)
	{
	movX=Xpc-Xc;
	movY=Ypc-Yc;
	 if(fabs(movX)>WINDOW*2||fabs(movY)>WINDOW*2)
	 {
		(*curCluster).getMotion(&movX, &movY);
		//movX=movY=0;
		//(*curCluster).getMotion(&movX,&movY);
	}
	}
	
	//else 
//	movX=movY=0;
//	(*curCluster).getWidthHeight(&width, &height);
	
//	windowL=(Xpc-width/2-MASK+movX)>0? (Xpc-width/2+MASK+movX):0;
//	windowR=(Xpc+width/2-MASK+movX)<Sy? (Xpc+width/2+MASK+movX):Sy;
//	windowU=(Ypc-height/2-MASK+movY)>0? (Ypc-height/2+MASK+movY):0;
//	windowD=(Ypc+height/2-MASK+movY)<Sx? (Ypc+height/2+MASK+movY):Sx;

//	windowL=(Xpc-width/2)>0? (Xpc-width/2):0;
//	windowR=(Xpc+width/2)<Sy? (Xpc+width/2):Sy;
//	windowU=(Ypc-height/2)>0? (Ypc-height/2):0;
//	windowD=(Ypc+height/2)<Sx? (Ypc+height/2):Sx;
	
	windowL=(minx+movX-WINDOW)>0? (minx+movX-WINDOW):0;
	windowR=(maxx+movX+WINDOW)<Sx-1? (maxx+movX+WINDOW):Sx-1;
	windowU=(miny+movY-WINDOW)>0? (miny+movY-WINDOW):0;
	windowD=(maxy+movY+WINDOW)<Sy-1? (maxy+movY+WINDOW):Sy-1;

//	windowL=(minx-WINDOW)>0? (minx-WINDOW):0;
//	windowR=(maxx+WINDOW)<Sx-1? (maxx+WINDOW):Sx-1;
//	windowU=(miny-WINDOW)>0? (miny-WINDOW):0;
//	windowD=(maxy+WINDOW)<Sy-1? (maxy+WINDOW):Sy-1;


	drawrec(pOutputImage,windowL,windowU,windowR,windowD,150);



	
	edgeNo=0;
	int *iniCanEdge = new int[(windowR-windowL+1)*(windowD-windowU+1)];
	tCan=0;
	for(i=windowL;i<windowR;i++)
	{
		for(j=windowU;j<windowD;j++)
		//for(j=0;j<windowD;j++)
		{
			if(pMovDistance[j][i]==0)
			{				
				edgeNo=pMovLabel[j][i];
				if(edgeNo>=1000) continue;
				if(edgeCandidateFound(edgeNo,tCan, iniCanEdge)==1) continue;
				else
				{	iniCanEdge[tCan]=edgeNo;
					tCan++;
				}
				
			}
		}
	}

	for(clusterEdgeSeg=(*curCluster).ListBegin();clusterEdgeSeg!=(*curCluster).ListEnd();) 
	{
		flag=1;
		(*clusterEdgeSeg).setWeight((*clusterEdgeSeg).getWeight()-5);
		if((*clusterEdgeSeg).getWeight()<=90)
		{
			(*clusterList).deleteSegmentFromCluster(&clusterEdgeSeg,&(*curCluster));
			flag=0;
		}
		if((*curCluster).getTotalEdge()==0)
			{
				return;
			}
	if(flag==1) clusterEdgeSeg++;
	}


//(*curCluster).EdgeListClear();


if(tCan>0)
		{
			for(i=0;i<tCan;i++)
			{
				flag2=1;
				for(curEdgeSeg=EdgeList->ListBegin();curEdgeSeg!=EdgeList->ListEnd();)
				{
						if((*curEdgeSeg).GetSegIndex()==iniCanEdge[i])
						{
							flag=1;
							for(clusterEdgeSeg=(*curCluster).ListBegin();clusterEdgeSeg!=(*curCluster).ListEnd();) 
							{/////////////for all segments in the cluster
								if(edgeInsideCluster((*curEdgeSeg),curCluster,Sx,Sy)==1)
								{
									if(AngleBasedMatch(pOutputImage,&(*curEdgeSeg), &(*clusterEdgeSeg), &dex, &dey, &angle)>=0.8)
									{
										if(fabs((double)dex)<=2*WINDOW&&fabs((double)dey)<=2*WINDOW&&fabs((double)angle)<=2*WINDOW)
										{
										clusterEdgeSeg=(*curCluster).DeleteSegment(clusterEdgeSeg);
										flag=0;
										//break;
										}
									}						
								//if(flag==0) break;
								
								(*curCluster).AddNewSegment(*curEdgeSeg);
								curEdgeSeg=EdgeList->DeleteSegment(curEdgeSeg);
								flag2=0;
								break;
								}
								if(flag==1) clusterEdgeSeg++;
							flag=1;
							}
						}//endif
				if(flag2==1) curEdgeSeg++;
				flag2=1;
				}
			}//endfor
	}//endif















/*
	if(tCan>0)
		{
			for(i=0;i<tCan;i++)
			{
				flag2=1;
				for(curEdgeSeg=EdgeList->ListBegin();curEdgeSeg!=EdgeList->ListEnd();)
				{
						if((*curEdgeSeg).GetSegIndex()==iniCanEdge[i])
						{
							flag=1;
							for(clusterEdgeSeg=(*curCluster).ListBegin();clusterEdgeSeg!=(*curCluster).ListEnd();) 
							{/////////////for all segments in the cluster
								if(edgeInsideCluster((*curEdgeSeg),curCluster,Sx,Sy)==1)
								{
									if(AngleBasedMatch(&(*curEdgeSeg), &(*clusterEdgeSeg), &dex, &dey, &angle)>=0.8)
									{
										if(fabs(dex)<=2*WINDOW&&fabs(dey)<=2*WINDOW&&fabs(angle)<=2*WINDOW)
										{
										clusterEdgeSeg=(*curCluster).DeleteSegment(clusterEdgeSeg);
										flag=0;
										(*curCluster).AddNewSegment(*curEdgeSeg);
										curEdgeSeg=EdgeList->DeleteSegment(curEdgeSeg);
										flag2=0;
										break;
										}
									}
								
								
								//if(flag==0) break;
								}
								if(flag==1) clusterEdgeSeg++;
							flag=1;
							}
						

						
						}//endif
				if(flag2==1) curEdgeSeg++;
				flag2=1;
				}
			}//endfor
	}//endif
*/

int cmaxx=0, cminx=Sx, cminy=Sy, cmaxy=0;

for(clusterEdgeSeg=(*curCluster).ListBegin();clusterEdgeSeg!=(*curCluster).ListEnd();clusterEdgeSeg++) 
{/////////////for all segments in the cluster
	(*clusterEdgeSeg).getBoundaryParam(&maxx, &maxy, &minx, &miny);
//	(*curCluster).getBoundaryParam(&cmaxx, &cmaxy, &cminx, &cminy);
	cmaxx=maxx>cmaxx? maxx:cmaxx;
	cmaxy=maxy>cmaxy? maxy:cmaxy;
	cminx=minx<cminx? minx:cminx;
	cminy=miny<cminy? miny:cminy;
	(*curCluster).setBoundaryParam(cmaxx, cmaxy, cminx, cminy);
}
						
	/*					
	windowL=(minx+movX)>0? (minx+movX):0;
	windowR=(maxx+movX)<Sx-1? (maxx+movX):Sx-1;
	windowU=(miny+movY)>0? (miny+movY):0;
	windowD=(maxy+movY)<Sy-1? (maxy+movY):Sy-1;
	(*curCluster).setBoundaryParam(windowR,windowD,windowL,windowU);
*/

				//	(*curCluster).setBoundaryParam(maxx,maxy,minx,miny);
	movX=movY=0;
	(*curCluster).setMotion(movX,movY);

//delete [] iniCanEdge;
}


void assignClusterIDtoMovingSegments(KSdUint8 ** pOutputImage,EdgeListType * movEdgeList, EdgeListType *MatchedEdgeList, int **pMovDistance, int **pMovLabel, int dx, int dy)
{
	int flag=1;
	list<EdgeSegmentType>::iterator curEdgeSeg,tmpEdgeSeg;
	float matchScore, movX,movY;
	int segI;
	
	for(curEdgeSeg=movEdgeList->ListBegin();curEdgeSeg!=movEdgeList->ListEnd();curEdgeSeg++)
	{// for all current frames moving segments
		(*curEdgeSeg).setClusterMemberID(-1);

		if(edgeSegmentMatch(pOutputImage,NULL,(*curEdgeSeg),MatchedEdgeList,pMovDistance,pMovLabel,dx,dy, &matchScore, &movX, &movY, &segI)==1)
			{
/*
int ii;
PointType pt;
for(ii=0;ii<(*curEdgeSeg).GetSize();ii++)
{	pt=(*curEdgeSeg).GetPoint(ii);
	pOutputImage[pt.GetY()][pt.GetX()]=255;
}*/				
				(*curEdgeSeg).setMotion(movX,movY);
				for(tmpEdgeSeg=MatchedEdgeList->ListBegin();tmpEdgeSeg!=MatchedEdgeList->ListEnd();tmpEdgeSeg++)
					{
						if((*tmpEdgeSeg).GetSegIndex()==segI)
						{
							//int ii=(*tmpEdgeSeg).getClusterMemberID();
							//ii=ii;
							(*curEdgeSeg).setClusterMemberID((*tmpEdgeSeg).getClusterMemberID());
							break;
						}
					}
			}
	}
}



//void findHoughClusterMatch(KSdUint8 ** pOutputImage, EdgeClusterType *curCluster, EdgeListType *EdgeList, int **pMovDistance, int **pMovLabel, int Sx, int Sy, int *posX, int *posY){
int findHoughClusterMatch(KSdUint8 ** pOutputImage, EdgeClusterType *curCluster, EdgeListType *EdgeList, int **pMovDistance, int **pMovLabel, int Sx, int Sy, int *posX, int *posY){
//(KSdUint8 **SrcImg, KSdUint8 **PreImg, KSdDouble **DstImg, int curRow, int curCol, int BSize){
//double meanSrc, meanPre, BlockValueSrc, BlockValuePre, BlockResult;
//	int Accumulator[50][50];
	//int i, j, p;
	//int code, locRow, locCol;
	//for(i = 0; i < 15; i++)
	//	TopCodeVectorCount[i] = 0;
	/* Generating R-Table*/

	int size,i, j, tCan, edgeNo, index,minx,miny,maxx,maxy,windowL,windowR,windowU,windowD, maxV;
	PointType pt,pt1;
	float theta,Xc,Yc,dx,dy,movX,movY;
	int R_Table[T_DEG][100][2]={0},tpoint;
	int curCount[T_DEG]={0},count=0,ix,iy,th,r,Xpc,Ypc,factor=0,sum,k,l;
	float cx,cy;
	list<EdgeSegmentType>::iterator curEdgeSeg;
	//int Accumulator[dimY][dimX]={0};
	for(i=0;i<dimX;i++)
	{
		for(j=0;j<dimY;j++)
		{
			Accumulator[j][i]=0;
		}
	}

	
	(*curCluster).getBoundaryParam(&maxx,&maxy,&minx,&miny);
	(*curCluster).getMotion(&movX, &movY);
	Xc=(maxx+minx)/2;
	Yc=(maxy+miny)/2;

	//Xpc=Xc+movX;
	//Ypc=Yc+movY;
	Xpc=Xc;
	Ypc=Yc;

	//pOutputImage[(int)Ypc][(int)Xpc] = 255;
	//pOutputImage[(int)Ypc+1][(int)Xpc+1] = 200;
	//pOutputImage[(int)Ypc-1][(int)Xpc-1] = 200;



	////////generating R table
	for(curEdgeSeg=(*curCluster).ListBegin();curEdgeSeg!=(*curCluster).ListEnd();curEdgeSeg++) 
	{/////////////for all segments in the cluster
		size=curEdgeSeg->GetSize();
			for(i=0;i<size-SEG_LENGTH;i++)
			{	pt=curEdgeSeg->GetPoint(i);
				pt1=curEdgeSeg->GetPoint(i+SEG_LENGTH-1);
				dx=pt.GetX()-pt1.GetX();
				dy=pt.GetY()-pt1.GetY();
				if(dx==0) theta=90;
				else theta=atan2(dy,dx)*180/PI;
				index=findIndex(theta);
			//	if(theta>120)
			//	{
			//	theta=theta;
			//	}
				dx=Xc-pt.GetX();
				dy=Yc-pt.GetY();
				if(dx==0) theta=90;
				else theta=atan2(dy,dx)*180/PI;
				r=(int)sqrt(dx*dx+dy*dy);
				th=findIndex(theta);
				//th=(int)theta;
				R_Table[index][curCount[index]][0]=r;
				R_Table[index][curCount[index]][1]=th;
			//	cx=pt.GetX()+r*cos(findTheta(th)*PI/180);
			//	cy=pt.GetY()+r*sin(findTheta(th)*PI/180);
			//	pOutputImage[(int)cy][(int)cx]=200;
				curCount[index]++;
				count++;
			}
	}

/////////////////////end generating R table


	windowL=(minx+movX-WINDOW)>0? (minx+movX-WINDOW):0;
	windowR=(maxx+movX+WINDOW)<Sx? (maxx+movX+WINDOW):Sx;
	windowU=(miny+movY-WINDOW)>0? (miny+movY-WINDOW):0;
	windowD=(maxy+movY+WINDOW)<Sy? (maxy+movY+WINDOW):Sy;
//	drawrec(pOutputImage,windowL,windowU,windowR,windowD,150);

	int *iniCanEdge = new int[(windowR-windowL+1)*(windowD-windowU+1)];

	tCan=0;
	for(i=windowL;i<windowR;i++)
	{
		for(j=windowU;j<windowD;j++)
		//for(j=0;j<windowD;j++)
		{
			if(pMovDistance[j][i]==0)
			{
				edgeNo=pMovLabel[j][i];
				if(edgeNo>=1000) continue;
				if(edgeCandidateFound(edgeNo,tCan, iniCanEdge)==1) continue;
				else
				{
					iniCanEdge[tCan]=edgeNo;
					tCan++;
				}
			}
		}
	}

tpoint=0;
	if(tCan>0)
		{
			for(k=0;k<tCan;k++)
			{
				for(curEdgeSeg=EdgeList->ListBegin();curEdgeSeg!=EdgeList->ListEnd();curEdgeSeg++)
				{
						if((*curEdgeSeg).GetSegIndex()==iniCanEdge[k])
						{
								size=(*curEdgeSeg).GetSize();
								tpoint+=size;
									for(i=0;i<size-SEG_LENGTH;i++)
									{	pt=curEdgeSeg->GetPoint(i);
										pt1=curEdgeSeg->GetPoint(i+SEG_LENGTH-1);
										dx=pt.GetX()-pt1.GetX();
										dy=pt.GetY()-pt1.GetY();
										if(dx==0) theta=90;
										else theta=atan2(dy,dx)*180/PI;
										index=findIndex(theta);
										
										for(j=0;j<curCount[index];j++)
										{
											r=R_Table[index][j][0];
											th=R_Table[index][j][1];
											cx=pt.GetX()+r*cos(findTheta(th)*PI/180);
											cy=pt.GetY()+r*sin(findTheta(th)*PI/180);
											//pOutputImage[(int)cy][(int)cx]=200;
											if(cx>=0&&cx<Sx&&cy>=0&&cy<Sy)
											{
												Accumulator[(int)cx][(int)cy]++;
												//pOutputImage[(int)cy][(int)cx]+=10;
											}
										}
										//R_Table[index][curCount[index]][0]=Xc-pt.GetX();
										//R_Table[index][curCount[index]][1]=Yc-pt.GetY();
									}
							}//endif
					}
			}//endfor
	}//endif


maxV=0;
ix=0;
iy=0;
for(i=MASK/2;i<dimX-MASK/2;i++)
{
	for(j=MASK/2;j<dimY-MASK/2;j++)
	{
			sum=0;
			for(k=-MASK/2;k<=MASK/2;k++)
			{
				for(l=-MASK/2;l<=MASK/2;l++)
				{
					sum+=Accumulator[i+k][j+l];
					if(Accumulator[i+k][j+l]>2)
					{
					i=i;
					}

				}
			}
		if(maxV<sum)
		{
			maxV=sum;
			ix=i;
			iy=j;
		}
	}
}
*posX=ix;
*posY=iy;
delete [] iniCanEdge;

if((float)maxV/tpoint>=0.3) return 1;
else return 0;

}


int findSelectedHoughClusterMatch(KSdUint8 ** pOutputImage, EdgeClusterType *curCluster, EdgeListType *EdgeList, int **pMovDistance, int **pMovLabel, int Sx, int Sy, int *posX, int *posY){
//(KSdUint8 **SrcImg, KSdUint8 **PreImg, KSdDouble **DstImg, int curRow, int curCol, int BSize){
//double meanSrc, meanPre, BlockValueSrc, BlockValuePre, BlockResult;
//	int Accumulator[50][50];
	//int i, j, p;
	//int code, locRow, locCol;
	//for(i = 0; i < 15; i++)
	//	TopCodeVectorCount[i] = 0;
	/* Generating R-Table*/

	int size,i, j, tCan, edgeNo, index,minx,miny,maxx,maxy,windowL,windowR,windowU,windowD, maxV;
	PointType pt,pt1;
	float theta,Xc,Yc,dx,dy,movX,movY;
	float Kxp,Kyp, movXp, movYp;

	int R_Table[T_DEG][200][2]={0},tpoint;
	int curCount[T_DEG]={0},count=0,ix,iy,th,r,Xpc,Ypc,factor=0,sum,k,l;
	float cx,cy;
	int oldx, oldy;
	float olddx,olddy;



	list<EdgeSegmentType>::iterator curEdgeSeg;
	//int Accumulator[dimY][dimX]={0};
	for(i=0;i<dimX;i++)
	{
		for(j=0;j<dimY;j++)
		{
			Accumulator[j][i]=0;
		}
	}
	(*curCluster).getBoundaryParam(&maxx,&maxy,&minx,&miny);
	(*curCluster).getMotion(&movX, &movY);
	Xc=(maxx+minx)/2;
	Yc=(maxy+miny)/2;

	//Xpc=Xc+movX;
	//Ypc=Yc+movY;
	Xpc=Xc;
	Ypc=Yc;

	//pOutputImage[(int)Ypc][(int)Xpc] = 255;
	//pOutputImage[(int)Ypc+1][(int)Xpc+1] = 200;
	//pOutputImage[(int)Ypc-1][(int)Xpc-1] = 200;



	////////generating R table
	for(curEdgeSeg=(*curCluster).ListBegin();curEdgeSeg!=(*curCluster).ListEnd();curEdgeSeg++) 
	{/////////////for all segments in the cluster
		size=curEdgeSeg->GetSize();
			for(i=0;i<size-SEG_LENGTH;i++)
			{	pt=curEdgeSeg->GetPoint(i);
				pt1=curEdgeSeg->GetPoint(i+SEG_LENGTH-1);
				dx=pt.GetX()-pt1.GetX();
				dy=pt.GetY()-pt1.GetY();
				if(dx==0) theta=90;
				else theta=atan2(dy,dx)*180/PI;
				index=findIndex(theta);
			//	if(theta>120)
			//	{
			//	theta=theta;
			//	}
				dx=Xc-pt.GetX();
				dy=Yc-pt.GetY();
				if(dx==0) theta=90;
				else theta=atan2(dy,dx)*180/PI;
				r=(int)sqrt(dx*dx+dy*dy);
				th=findIndex(theta);
				//th=(int)theta;
				R_Table[index][curCount[index]][0]=r;
				R_Table[index][curCount[index]][1]=th;
			//	cx=pt.GetX()+r*cos(findTheta(th)*PI/180);
			//	cy=pt.GetY()+r*sin(findTheta(th)*PI/180);
			//	pOutputImage[(int)cy][(int)cx]=200;
				curCount[index]++;
				count++;
			}
	}

/////////////////////end generating R table



	

	//windowL=(minx+movX-WINDOW)>0? (minx+movX-WINDOW):0;
	//windowR=(maxx+movX+WINDOW)<Sx? (maxx+movX+WINDOW):Sx;
	//windowU=(miny+movY-WINDOW)>0? (miny+movY-WINDOW):0;
	//windowD=(maxy+movY+WINDOW)<Sy? (maxy+movY+WINDOW):Sy;
	//drawrec(pOutputImage,windowL,windowU,windowR,windowD,150);



	(*curCluster).getOldMotion(oldx,oldy,olddx, olddy);
	kalman_update((*curCluster).KalmanPar, oldx,oldy,olddx,olddy, Xc, Yc, movX, movY, Kxp, Kyp, movXp, movYp);
	(*curCluster).setOldMotion(Kxp,Kyp,movXp,movYp);

	windowL=(minx+movXp-WINDOW)>0? (minx+movXp-WINDOW):0;
	windowR=(maxx+movXp+WINDOW)<Sx? (maxx+movXp+WINDOW):Sx;
	windowU=(miny+movYp-WINDOW)>0? (miny+movYp-WINDOW):0;
	windowD=(maxy+movYp+WINDOW)<Sy? (maxy+movYp+WINDOW):Sy;
	//drawrec(pOutputImage,windowL,windowU,windowR,windowD,255);

	int *iniCanEdge = new int[(windowR-windowL+1)*(windowD-windowU+1)];

	tCan=0;
	for(i=windowL;i<windowR;i++)
	{
		for(j=windowU;j<windowD;j++)
		//for(j=0;j<windowD;j++)
		{
			if(pMovDistance[j][i]==0)
			{
				edgeNo=pMovLabel[j][i];
				if(edgeNo>=1000) continue;
				if(edgeCandidateFound(edgeNo,tCan, iniCanEdge)==1) continue;
				else
				{
					iniCanEdge[tCan]=edgeNo;
					tCan++;
				}
			}
		}
	}

tpoint=0;
	if(tCan>0)
		{
			for(k=0;k<tCan;k++)
			{
				for(curEdgeSeg=EdgeList->ListBegin();curEdgeSeg!=EdgeList->ListEnd();curEdgeSeg++)
				{
						if((*curEdgeSeg).GetSegIndex()==iniCanEdge[k]&&(*curEdgeSeg).getClusterMemberID()==(*curCluster).getClusterIndex())
						{
								size=(*curEdgeSeg).GetSize();
								tpoint+=size;
									for(i=0;i<size-SEG_LENGTH;i++)
									{	pt=curEdgeSeg->GetPoint(i);
										pt1=curEdgeSeg->GetPoint(i+SEG_LENGTH-1);
										dx=pt.GetX()-pt1.GetX();
										dy=pt.GetY()-pt1.GetY();
										if(dx==0) theta=90;
										else theta=atan2(dy,dx)*180/PI;
										index=findIndex(theta);
										
										for(j=0;j<curCount[index];j++)
										{
											r=R_Table[index][j][0];
											th=R_Table[index][j][1];
											cx=pt.GetX()+r*cos(findTheta(th)*PI/180);
											cy=pt.GetY()+r*sin(findTheta(th)*PI/180);
											//pOutputImage[(int)cy][(int)cx]=200;
											if(cx>=0&&cx<Sx&&cy>=0&&cy<Sy)
											{
												Accumulator[(int)cx][(int)cy]++;
												pOutputImage[(int)cy][(int)cx]+=10;
											}
										}
										//R_Table[index][curCount[index]][0]=Xc-pt.GetX();
										//R_Table[index][curCount[index]][1]=Yc-pt.GetY();
									}
							}//endif
					}
			}//endfor
	}//endif


maxV=0;
ix=0;
iy=0;
for(i=MASK/2;i<dimX-MASK/2;i++)
{
	for(j=MASK/2;j<dimY-MASK/2;j++)
	{
			sum=0;
			for(k=-MASK/2;k<=MASK/2;k++)
			{
				for(l=-MASK/2;l<=MASK/2;l++)
				{
					sum+=Accumulator[i+k][j+l];
					if(Accumulator[i+k][j+l]>2)
					{
					i=i;
					}

				}
			}
		if(maxV<sum)
		{
			maxV=sum;
			ix=i;
			iy=j;
		}
	}
}
*posX=ix;
*posY=iy;
delete [] iniCanEdge;

if(tpoint==0) return 0;
if((float)maxV/tpoint>=0.3) return 1;
else return 0;

}

void updateSelectedClusterMemberSegments(KSdUint8 ** pOutputImage, EdgeClusterList *clusterList, EdgeClusterType *curCluster, EdgeListType *EdgeList, int **pMovDistance, int **pMovLabel, int Sx, int Sy, int posX, int posY, int found)
{
	int i, j, tCan, edgeNo, minx,miny,maxx,maxy,windowL,windowR,windowU,windowD;
	PointType pt,pt1;
	float Xc,Yc,movX,movY,cmovX,cmovY, movXp, movYp;
	int Xpc, Ypc, dex,dey,flag,flag2, Kxp,Kyp;
	float angle;
	int oldx, oldy;
	float olddx,olddy;
	list<EdgeSegmentType>::iterator clusterEdgeSeg;
	list<EdgeSegmentType>::iterator curEdgeSeg;

	
	if(found==1)
	{
	(*curCluster).getBoundaryParam(&maxx,&maxy,&minx,&miny);

	//drawrec(pOutputImage,minx,miny,maxx,maxy,150);

	//(*curCluster).getMotion(&movX, &movY);
	Xc=(maxx+minx)/2;
	Yc=(maxy+miny)/2;
	
//	pOutputImage[(int)Yc][(int)Xc]=255;

	
	Xpc=posX;
	Ypc=posY;
	movX=movY=0;

//	pOutputImage[(int)Ypc][(int)Xpc]=255;

	if(found==1)
	{
	movX=Xpc-Xc;
	movY=Ypc-Yc;
	}


	//(*curCluster).getOldMotion(oldx,oldy,olddx, olddy);
	//kalman_update((*curCluster).KalmanPar, oldx,oldy,olddx,olddy, Xc, Yc, movX, movY, Kxp, Kyp, movXp, movYp);
	//(*curCluster).setOldMotion(Kxp,Kyp,movXp,movYp);
	
	(*curCluster).getOldMotion(Kxp,Kyp,movXp,movYp);
	
	//pOutputImage[(int)Kyp][(int)Kxp]=255;



	windowL=(minx+movX-WINDOW)>0? (minx+movX-WINDOW):0;
	windowR=(maxx+movX+WINDOW)<Sx-1? (maxx+movX+WINDOW):Sx-1;
	windowU=(miny+movY-WINDOW)>0? (miny+movY-WINDOW):0;
	windowD=(maxy+movY+WINDOW)<Sy-1? (maxy+movY+WINDOW):Sy-1;
	drawrec(pOutputImage,windowL,windowU,windowR,windowD,100);

	windowL=(minx+movXp-WINDOW)>0? (minx+movXp-WINDOW):0;
	windowR=(maxx+movXp+WINDOW)<Sx-1? (maxx+movXp+WINDOW):Sx-1;
	windowU=(miny+movYp-WINDOW)>0? (miny+movYp-WINDOW):0;
	windowD=(maxy+movYp+WINDOW)<Sy-1? (maxy+movYp+WINDOW):Sy-1;
	drawrec(pOutputImage,windowL,windowU,windowR,windowD,255);

	edgeNo=0;
	int *iniCanEdge = new int[(windowR-windowL+1)*(windowD-windowU+1)];
	tCan=0;
	for(i=windowL;i<windowR;i++)
	{
		for(j=windowU;j<windowD;j++)
		//for(j=0;j<windowD;j++)
		{
			if(pMovDistance[j][i]==0)
			{				
				edgeNo=pMovLabel[j][i];
				if(edgeNo>=1000) continue;
				if(edgeCandidateFound(edgeNo,tCan, iniCanEdge)==1) continue;
				else
				{	iniCanEdge[tCan]=edgeNo;
					tCan++;
				}
				
			}
		}
	}
/*
	for(clusterEdgeSeg=(*curCluster).ListBegin();clusterEdgeSeg!=(*curCluster).ListEnd();) 
	{
		flag=1;
		if((*clusterEdgeSeg).getWeight()<=90)
		{
			(*clusterList).deleteSegmentFromCluster(&clusterEdgeSeg,&(*curCluster));
			flag=0;
		}
		else (*clusterEdgeSeg).setWeight((*clusterEdgeSeg).getWeight()-5);
			if((*curCluster).getTotalEdge()==0)
			{
				return;
			}
	if(flag==1) clusterEdgeSeg++;
	}
*/
if(tCan>0)
		{
			for(i=0;i<tCan;i++)
			{
				flag2=1;
				for(curEdgeSeg=EdgeList->ListBegin();curEdgeSeg!=EdgeList->ListEnd();)
				{
						if((*curEdgeSeg).GetSegIndex()==iniCanEdge[i])
						{
							flag=1;
							for(clusterEdgeSeg=(*curCluster).ListBegin();clusterEdgeSeg!=(*curCluster).ListEnd();) 
							{/////////////for all segments in the cluster
								if(edgeInsideCluster((*curEdgeSeg),curCluster,Sx,Sy)==1&&((*curEdgeSeg).getClusterMemberID()==(*curCluster).getClusterIndex()||(*curEdgeSeg).getClusterMemberID()==-1))
								{
									if(AngleBasedMatch(pOutputImage,&(*curEdgeSeg), &(*clusterEdgeSeg), &dex, &dey, &angle)>=0.8)
									{
										if(fabs((double)dex)<=2*WINDOW&&fabs((double)dey)<=2*WINDOW&&fabs((double)angle)<=2*WINDOW)
										{
										clusterEdgeSeg=(*curCluster).DeleteSegment(clusterEdgeSeg);
										flag=0;	
										}
									}
															
								//if(flag==0) break;
								(*curCluster).getMotion(&cmovX,&cmovY);
								(*curEdgeSeg).getMotion(&movX,&movY);
							//	(*curCluster).AddNewSegment(*curEdgeSeg);
							//	curEdgeSeg=EdgeList->DeleteSegment(curEdgeSeg);
							//	flag2=0;
							//	break;
						//	if(fabs(cmovX)+fabs(cmovY)>10)
						//	{
						//	cmovX=cmovX;
						//	}
								if(fabs(movX-movXp)<=20&&fabs(movY+movYp)<=20)
								{
									(*curEdgeSeg).setWeight(100);
									(*curCluster).AddNewSegment(*curEdgeSeg);
									curEdgeSeg=EdgeList->DeleteSegment(curEdgeSeg);
									flag2=0;
								}
								else
								{
									curEdgeSeg=EdgeList->DeleteSegment(curEdgeSeg);
									flag2=0;

								}
								
								break;
									
								}
								if(flag==1) clusterEdgeSeg++;
							flag=1;
							}
						}//endif
				if(flag2==1) curEdgeSeg++;
				flag2=1;
				}
			}//endfor
	}//endif

int cmaxx=0, cminx=Sx, cminy=Sy, cmaxy=0;
(*curCluster).getBoundaryParam(&cmaxx, &cmaxy, &cminx, &cminy);
Xc=(cmaxx+cminx)/2;
Yc=(cmaxy+cminy)/2;

for(clusterEdgeSeg=(*curCluster).ListBegin();clusterEdgeSeg!=(*curCluster).ListEnd();clusterEdgeSeg++) 
{/////////////for all segments in the cluster
	(*clusterEdgeSeg).getBoundaryParam(&maxx, &maxy, &minx, &miny);
	cmaxx=maxx>cmaxx? maxx:cmaxx;
	cmaxy=maxy>cmaxy? maxy:cmaxy;
	cminx=minx<cminx? minx:cminx;
	cminy=miny<cminy? miny:cminy;
}
(*curCluster).setBoundaryParam(cmaxx, cmaxy, cminx, cminy);
Xc=(cmaxx+cminx)/2;
Yc=(cmaxy+cminy)/2;
(*curCluster).setMotion(Xpc-Xc,Ypc-Yc);

//(*curCluster).setMotion(movX,movY);
//	(*curCluster).setMotion(posX,posY);

delete [] iniCanEdge;
}

for(clusterEdgeSeg=(*curCluster).ListBegin();clusterEdgeSeg!=(*curCluster).ListEnd();) 
	{
		flag=1;
		if((*clusterEdgeSeg).getWeight()<=85)
		{
			(*clusterList).deleteSegmentFromCluster(&clusterEdgeSeg,&(*curCluster));
			flag=0;
		}
		else (*clusterEdgeSeg).setWeight((*clusterEdgeSeg).getWeight()-5);
			if((*curCluster).getTotalEdge()==0)
			{
				return;
			}
	if(flag==1) clusterEdgeSeg++;
	}
}


int findHoughEdgeMotion(KSdUint8 ** pOutputImage, EdgeSegmentType *refEdgeSeg, EdgeListType *curEdgeList, KScScalarImage2dInt32 *movDistImage, KScScalarImage2dInt32 *movLabelImage, int &movX, int &movY, int Sx, int Sy){
	
	int size,i, j, tCan, edgeNo, index,minx,miny,maxx,maxy,windowL,windowR,windowU,windowD, maxV;
	PointType pt,pt1;
	float theta,dx,dy;
	int R_Table[T_DEG][100][2]={0},tpoint;
	int curCount[T_DEG]={0},count=0,ix,iy,th,r,Xpc,Ypc,factor=0,sum,k,l;
	float cx,cy;
	(*refEdgeSeg).getBoundaryParam(&maxx,&maxy,&minx,&miny);
	int Xc=(maxx+minx)/2;
	int Yc=(maxy+miny)/2;
	int **pMovDistance = movDistImage->Get2dArray();
	int **pMovLabel = movLabelImage->Get2dArray();
	int mx,my;


	list<EdgeSegmentType>::iterator curEdgeSeg;
	//int Accumulator[dimY][dimX]={0};
	for(i=0;i<dimX;i++)
	{
		for(j=0;j<dimY;j++)
		{
			Accumulator[j][i]=0;
		}
	}

		/////generating R table
		size=refEdgeSeg->GetSize();
		for(i=0;i<size-SEG_LENGTH;i++)
			{	pt=refEdgeSeg->GetPoint(i);
				pt1=refEdgeSeg->GetPoint(i+SEG_LENGTH-1);
				dx=pt.GetX()-pt1.GetX();
				dy=pt.GetY()-pt1.GetY();
				if(dx==0) theta=90;
				else theta=atan2(dy,dx)*180/PI;
				index=findIndex(theta);
			
				dx=Xc-pt.GetX();
				dy=Yc-pt.GetY();
				if(dx==0) theta=90;
				else theta=atan2(dy,dx)*180/PI;
				r=(int)sqrt(dx*dx+dy*dy);
				th=findIndex(theta);
				
				R_Table[index][curCount[index]][0]=r;
				R_Table[index][curCount[index]][1]=th;
			
				curCount[index]++;
				count++;
			}
/////////////////////end generating R table

	windowL=(minx-WINDOW)>0? (minx-WINDOW):0;
	windowR=(maxx+WINDOW)<Sx? (maxx+WINDOW):Sx;
	windowU=(miny-WINDOW)>0? (miny-WINDOW):0;
	windowD=(maxy+WINDOW)<Sy? (maxy+WINDOW):Sy;
	//drawrec(pOutputImage,windowL,windowU,windowR,windowD,150);

	int *iniCanEdge = new int[(windowR-windowL+1)*(windowD-windowU+1)];
	tCan=0;
	for(i=windowL;i<windowR;i++)
	{
		for(j=windowU;j<windowD;j++)
		{
			if(pMovDistance[j][i]==0)
			{
				edgeNo=pMovLabel[j][i];
				if(edgeNo>=1000) continue;
				if(edgeCandidateFound(edgeNo,tCan, iniCanEdge)==1) continue;
				else
				{
					iniCanEdge[tCan]=edgeNo;
					tCan++;
				}
			}
		}
	}

    tpoint=0;
	if(tCan>0)
		{
			for(k=0;k<tCan;k++)
			{
				for(curEdgeSeg=curEdgeList->ListBegin();curEdgeSeg!=curEdgeList->ListEnd();curEdgeSeg++)
				{
						if((*curEdgeSeg).GetSegIndex()==iniCanEdge[k])
						{
								size=(*curEdgeSeg).GetSize();
								tpoint+=size;
									for(i=0;i<size-SEG_LENGTH;i++)
									{	pt=curEdgeSeg->GetPoint(i);
										pt1=curEdgeSeg->GetPoint(i+SEG_LENGTH-1);
										dx=pt.GetX()-pt1.GetX();
										dy=pt.GetY()-pt1.GetY();
										if(dx==0) theta=90;
										else theta=atan2(dy,dx)*180/PI;
										index=findIndex(theta);
										
										for(j=0;j<curCount[index];j++)
										{
											r=R_Table[index][j][0];
											th=R_Table[index][j][1];
											cx=pt.GetX()+r*cos(findTheta(th)*PI/180);
											cy=pt.GetY()+r*sin(findTheta(th)*PI/180);
											//pOutputImage[(int)cy][(int)cx]=200;
											if(cx>=0&&cx<Sx&&cy>=0&&cy<Sy)
											{
												Accumulator[(int)cx][(int)cy]++;
												//pOutputImage[(int)cy][(int)cx]+=1;
											}
										}
										//R_Table[index][curCount[index]][0]=Xc-pt.GetX();
										//R_Table[index][curCount[index]][1]=Yc-pt.GetY();
									}
							}//endif
					}
			}//endfor
	}//endif


maxV=0;
ix=0;
iy=0;
for(i=windowL+MASK/2;i<windowR-MASK/2;i++)
{
	for(j=windowU+MASK/2;j<windowD-MASK/2;j++)
	{
			sum=0;
			for(k=-MASK/2;k<=MASK/2;k++)
			{
				for(l=-MASK/2;l<=MASK/2;l++)
				{
					sum+=Accumulator[i+k][j+l];
					//if(Accumulator[i+k][j+l]>2)
					//{
					//i=i;
					//}

				}
			}
		if(maxV<sum)
		{
			if((abs(Xc-i)>6)||(abs(Yc-j)>6)) continue;
			
			maxV=sum;
			ix=i;
			iy=j;
		}
	}
}
if(maxV>((windowR-windowL)+(windowD-windowU))*0.1)
{
mx=(Xc-ix);
my=(Yc-iy);
}
else
{
mx=0;
my=0;
}

(*refEdgeSeg).setMotion(-mx,-my);

//drawrec(pOutputImage,windowL,windowU,windowR,windowD,150);
//drawArrow(pOutputImage,Xc,Yc,-mx,-my,Sx,Sy);

delete [] iniCanEdge;

return 1;
//if((float)maxV/tpoint>=0.3) return 1;
//else return 0;
}