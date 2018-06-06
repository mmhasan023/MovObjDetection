#include "EdgeClusterList.h"
#include<math.h>
//#include<math.h>


#define MOTION_THRESHOLD 10

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

# define DT 100



void drawrectangle(KSdUint8 **pOutputImage, int minx, int miny, int maxx, int maxy, int color){
int i;

for(i=minx; i<=maxx;i++)
{
pOutputImage[miny][i]=color;
pOutputImage[maxy][i]=color;
}

for(i=miny; i<=maxy;i++)
{
pOutputImage[i][minx]=color;
pOutputImage[i][maxx]=color;
}
}



void showClusterWindow(EdgeClusterType *clusterEdgeList, KSdUint8 **pOutputImage)
{
	int maxx=0, maxy=0, minx=0, miny=0;
	(*clusterEdgeList).getBoundaryParam(&maxx, &maxy, &minx,&miny);
	drawrectangle(pOutputImage,minx,miny,maxx,maxy,255);
}


EdgeClusterList:: EdgeClusterList()
{
	clusterList = NULL ;
	numCluster = 0;
	CID=0;
	clusterList	= new list<EdgeClusterType> ;
}

EdgeClusterList::~EdgeClusterList()
{
	delete clusterList;
}

int EdgeClusterList::getTotalClusterNumber()
{
	return numCluster;
}


void EdgeClusterList::ClusterListClear()
{
	numCluster	= 0;
	CID=0;
	clusterList->clear();
}


void EdgeClusterList::AddNewFullCluster(EdgeClusterType newCluster)
{
	list<EdgeSegmentType>::iterator curSegment;
	int maxx, maxy, minx, miny;
	int cmaxx, cmaxy, cminx, cminy,count;
	float movX, movY,sumX,sumY;

	CID++;
	numCluster++;
	EdgeClusterType *ec=new EdgeClusterType();
	
	curSegment=newCluster.ListBegin();
	(*curSegment).getBoundaryParam(&cmaxx, &cmaxy, &cminx, &cminy);
	
	sumX=sumY=0;
	count=0;
	for(curSegment=newCluster.ListBegin();curSegment!=newCluster.ListEnd();curSegment++)
	{
	(*curSegment).getMotion(&movX,&movY);
	sumX+=movX;
	sumY+=movY;
	ec->AddNewSegment(*curSegment);
	(*curSegment).getBoundaryParam(&maxx, &maxy, &minx, &miny);
	cmaxx=maxx>cmaxx? maxx:cmaxx;
	cmaxy=maxy>cmaxy? maxy:cmaxy;
	cminx=minx<cminx? minx:cminx;
	cminy=miny<cminy? miny:cminy;
	count++;
	}
	ec->setBoundaryParam(maxx, maxy, minx, miny);
	ec->setClusterCenter((maxx+minx)/2, (maxy+miny)/2);
	ec->setClusterIndex(CID);
	ec->setSum((maxx+minx)/2, (maxy+miny)/2);
	ec->setMotion(sumX/count,sumY/count);
	clusterList->push_back((*ec));
	
}

void EdgeClusterList::AddNewFullCluster(EdgeClusterType *newCluster)
{
	list<EdgeSegmentType>::iterator curSegment;
	int maxx, maxy, minx, miny;
	int cmaxx, cmaxy, cminx, cminy,count;
	float movX, movY,sumX,sumY;

	CID++;
	numCluster++;
	EdgeClusterType *ec=new EdgeClusterType();
	
	curSegment=(*newCluster).ListBegin();
	(*curSegment).getBoundaryParam(&cmaxx, &cmaxy, &cminx, &cminy);
	
	sumX=sumY=0;
	count=0;
	for(curSegment=(*newCluster).ListBegin();curSegment!=(*newCluster).ListEnd();curSegment++)
	{
	(*curSegment).getMotion(&movX,&movY);
	sumX+=movX;
	sumY+=movY;
	ec->AddNewSegment(*curSegment);
	(*curSegment).getBoundaryParam(&maxx, &maxy, &minx, &miny);
	cmaxx=maxx>cmaxx? maxx:cmaxx;
	cmaxy=maxy>cmaxy? maxy:cmaxy;
	cminx=minx<cminx? minx:cminx;
	cminy=miny<cminy? miny:cminy;
	count++;
	}
	ec->setBoundaryParam(maxx, maxy, minx, miny);
	ec->setClusterCenter((maxx+minx)/2, (maxy+miny)/2);
	ec->setClusterIndex(CID);
	ec->setSum((maxx+minx)/2, (maxy+miny)/2);
	ec->setMotion(sumX/count,sumY/count);
	clusterList->push_back((*ec));	
}






/*void  EdgeClusterList::AddNewCluster(EdgeClusterType *newCluster)
{
	if(clusterList==NULL){ 
		clusterList = new list<EdgeClusterType>;
		numCluster	= 0;
	}
	clusterList->push_back(*newCluster);
	numCluster++;
}*/
/*
int EdgeClusterList::DeleteCluster(int index)
{ 
	EdgeClusterType *tmpCluster;
	tmpCluster = new EdgeClusterType;
	list<EdgeClusterType>::iterator curCluster;
	for(curCluster=clusterList->begin();curCluster!=clusterList->end();curCluster++)
	{
		*tmpCluster = (*curCluster);
		int curIndex = (*tmpCluster).getClusterIndex();
		if(curIndex == index){
			clusterList->erase(curCluster);
			numCluster--;
			return 1;
		}
	}
	return 0;
}*/

int Dist(int x1, int y1, int x2, int y2){
return ((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}
void EdgeClusterList::addNewCluster(EdgeSegmentType edge)
{
		int maxx, maxy, minx, miny;
		float movX, movY;

		CID++;
		numCluster++;
		EdgeClusterType *ec=new EdgeClusterType();
		ec->AddNewSegment(edge);
		edge.getMotion(&movX, &movY);
		edge.getBoundaryParam(&maxx, &maxy, &minx, &miny);
		ec->setBoundaryParam(maxx, maxy, minx, miny);
		ec->setClusterCenter((maxx+minx)/2, (maxy+miny)/2);
		ec->setClusterIndex(CID);
		ec->setSum((maxx+minx)/2, (maxy+miny)/2);
		ec->setMotion(movX,movY);
		clusterList->push_back((*ec));

}


void EdgeClusterList::addNewCluster(EdgeSegmentType *edge)
{
		int maxx, maxy, minx, miny;
		CID++;
		numCluster++;
		EdgeClusterType *ec=new EdgeClusterType();
		ec->AddNewSegment(*edge);
		edge->getBoundaryParam(&maxx, &maxy, &minx, &miny);
		ec->setBoundaryParam(maxx, maxy, minx, miny);
		ec->setClusterCenter((maxx+minx)/2, (maxy+miny)/2);
		ec->setClusterIndex(CID);
		ec->setSum((maxx+minx)/2, (maxy+miny)/2);
		clusterList->push_back((*ec));

}

void EdgeClusterList::AddNewSegment(EdgeSegmentType *edge)
{
	int maxx, maxy, minx, miny,flag=1,clusterdel=1;
	int cmaxx, cmaxy, cminx, cminy;
	float sumX,sumY,movX,movY;
	int ID,count;
	float cx,cy,x,y,d, mind;
	list<EdgeClusterType>::iterator curCluster;
	list<EdgeClusterType>::iterator tmpCluster;
	list<EdgeClusterType>::iterator allClusterCenter;
	list<EdgeSegmentType>::iterator curSegment;
	list<EdgeSegmentType>::iterator tmpSegment;
	
	if(clusterList==NULL){ 
		clusterList = new list<EdgeClusterType> ;
		numCluster	= 0;
		CID=0;
		addNewCluster(edge);
	}
	else if(numCluster==0)
	{
		addNewCluster(edge);
	}
	else
	{
		edge->getBoundaryParam(&maxx, &maxy, &minx, &miny);
		x=(maxx+minx)/2;
		y=(maxy+miny)/2;
		mind=32767;
	
			for(curCluster=clusterList->begin();curCluster!=clusterList->end();curCluster++)
			{
				(*curCluster).getClusterCenter(&cx,&cy);
				d=Dist(x,y,cx,cy);
					if(mind>d)
					{ 
						mind=d;
						ID=(*curCluster).getClusterIndex();
					}
			}

			if(mind>DT)
			{
				addNewCluster(edge);
			}
			else
			{
			
			for(curCluster=clusterList->begin();curCluster!=clusterList->end();curCluster++)
			if(ID==(*curCluster).getClusterIndex()) break;
			if(ID==(*curCluster).getClusterIndex())
			{
				(*curCluster).AddNewSegment(*edge);
				edge->getBoundaryParam(&maxx, &maxy, &minx, &miny);
				(*curCluster).setSum((maxx+minx)/2, (maxy+miny)/2);
				(*curCluster).getSum(&x, &y);
				(*curCluster).setClusterCenter(x/(*curCluster).getTotalEdge(), y/(*curCluster).getTotalEdge());

				(*curCluster).getBoundaryParam(&cmaxx, &cmaxy, &cminx, &cminy);
				maxx=maxx>cmaxx? maxx:cmaxx;
				maxy=maxy>cmaxy? maxy:cmaxy;
				minx=minx<cminx? minx:cminx;
				miny=miny<cminy? miny:cminy;
				(*curCluster).setBoundaryParam(maxx, maxy, minx, miny);

						
				flag=1;
			
				while(flag)
				{
					flag=0;

					for(curCluster=clusterList->begin();curCluster!=clusterList->end();)
					{ 		
						clusterdel=1;
				
						for(curSegment=(*curCluster).ListBegin();curSegment!=(*curCluster).ListEnd();)
						{
							(*curSegment).getBoundaryParam(&maxx, &maxy, &minx, &miny);
							x=(maxx+minx)/2;
							y=(maxy+miny)/2;
							mind=32767;

							for(allClusterCenter=clusterList->begin();allClusterCenter!=clusterList->end();allClusterCenter++)
							{
								(*allClusterCenter).getClusterCenter(&cx,&cy);
								d=Dist(x,y,cx,cy);
								if(mind>d)
								{ 
									mind=d;
									ID=(*curCluster).getClusterIndex();
								}
							}
							
							if(ID!=(*curCluster).getClusterIndex()) 
							{
								(*curSegment).getBoundaryParam(&maxx, &maxy, &minx, &miny);
								(*curCluster).setSum(-(maxx+minx)/2, -(maxy+miny)/2);
								(*curCluster).getSum(&x, &y);
								(*curCluster).setClusterCenter(x/(*curCluster).getTotalEdge(), y/(*curCluster).getTotalEdge());
							
					
					

								for(tmpCluster=clusterList->begin();tmpCluster!=clusterList->end();tmpCluster++)
								if(ID==(*tmpCluster).getClusterIndex()) break;
								(*tmpCluster).AddNewSegment(*curSegment);
								(*tmpCluster).setSum((maxx+minx)/2, (maxy+miny)/2);
								(*tmpCluster).getSum(&x, &y);
								(*tmpCluster).setClusterCenter(x/(*tmpCluster).getTotalEdge(), y/(*tmpCluster).getTotalEdge());
									
									(*tmpCluster).getBoundaryParam(&cmaxx, &cmaxy, &cminx, &cminy);
									maxx=maxx>cmaxx? maxx:cmaxx;
									maxy=maxy>cmaxy? maxy:cmaxy;
									minx=minx<cminx? minx:cminx;
									miny=miny<cminy? miny:cminy;
									(*tmpCluster).setBoundaryParam(maxx, maxy, minx, miny);

							flag=1;	
							
								curSegment=(*curCluster).DeleteSegment(curSegment);
								if((*curCluster).getTotalEdge()>0)
								{
									tmpSegment=(*curCluster).ListBegin();
									(*tmpSegment).getBoundaryParam(&cmaxx, &cmaxy, &cminx, &cminy);
									for(;tmpSegment!=(*curCluster).ListEnd();tmpSegment++)
										{
											(*tmpSegment).getBoundaryParam(&maxx, &maxy, &minx, &cminy);
											cmaxx=maxx>cmaxx? maxx:cmaxx;
											cmaxy=maxy>cmaxy? maxy:cmaxy;
											cminx=minx<cminx? minx:cminx;
											cminy=miny<cminy? miny:cminy;
										}
									(*curCluster).setBoundaryParam(cmaxx, cmaxy, cminx, cminy);
								}
								else
								{	
									curCluster=DeleteCluster(curCluster);
									clusterdel=0;
								}

							continue;
							}
							curSegment++;	
						}//inner end for
					if(clusterdel) curCluster++;
					}//end outer for
				}//end while
			}//end if CID=
		}//end else addNewCluster
	}//end else

	//merge intersecting blocks

	for(curCluster=clusterList->begin();curCluster!=clusterList->end();curCluster++)
	{
		(*curCluster).getBoundaryParam(&maxx, &maxy, &minx, &miny);
		//tmpCluster=curCluster;
		//tmpCluster++;
		tmpCluster=clusterList->begin();
		for(;tmpCluster!=clusterList->end();)
		{
			if(tmpCluster==curCluster) {
				tmpCluster++;
				continue;
			}

		clusterdel=1;
			(*tmpCluster).getBoundaryParam(&cmaxx, &cmaxy, &cminx, &cminy);
			if(minx>=cminx&&minx<=cmaxx&&miny>=cminy&&miny<=cmaxy||maxx>=cminx&&maxx<=cmaxx&&miny>=cminy&&miny<=cmaxy||maxx>=cminx&&maxx<=cmaxx&&maxy>=cminy&&maxy<=cmaxy||minx>=cminx&&minx<=cmaxx&&maxy>=cminy&&maxy<=cmaxy||cminx>=minx&&cminx<=maxx&&cminy>=miny&&cminy<=maxy||cmaxx>=minx&&cmaxx<=maxx&&cminy>=miny&&cminy<=maxy||cmaxx>=minx&&cmaxx<=maxx&&cmaxy>=miny&&cmaxy<=maxy||cminx>=minx&&cminx<=maxx&&cmaxy>=miny&&cmaxy<=maxy) //intersecting block
		//	if((abs(minx-cminx)+abs(maxx-cmaxx)+abs(miny-cminy)+abs(maxy-cmaxy))<(abs(maxx-minx)+abs(maxy-miny)+abs(cmaxx-cminx)+abs(cmaxy-cminy))) //intersecting block
			{
				for(curSegment=(*tmpCluster).ListBegin();curSegment!=(*tmpCluster).ListEnd();curSegment++)
				{
					(*curSegment).getBoundaryParam(&maxx, &maxy, &minx, &miny);
					(*curCluster).AddNewSegment(*curSegment);
					(*curCluster).setSum((maxx+minx)/2, (maxy+miny)/2);
					(*curCluster).getSum(&x, &y);
					(*curCluster).setClusterCenter(x/(*curCluster).getTotalEdge(), y/(*curCluster).getTotalEdge());
									
					(*curCluster).getBoundaryParam(&cmaxx, &cmaxy, &cminx, &cminy);
					maxx=maxx>cmaxx? maxx:cmaxx;
					maxy=maxy>cmaxy? maxy:cmaxy;
					minx=minx<cminx? minx:cminx;
					miny=miny<cminy? miny:cminy;
					(*curCluster).setBoundaryParam(maxx, maxy, minx, miny);
				}
			
			tmpCluster=DeleteCluster(tmpCluster);
			clusterdel=0;
			(*curCluster).getBoundaryParam(&maxx, &maxy, &minx, &miny);
			}
		if(clusterdel) tmpCluster++;
		}
	}

	
	for(curCluster=clusterList->begin();curCluster!=clusterList->end();curCluster++)
	{
		sumX=0;
		sumY=0;
		count=0;
		for(curSegment=(*curCluster).ListBegin();curSegment!=(*curCluster).ListEnd();curSegment++)
		{
			(*curSegment).getMotion(&movX, &movY);
			sumX=sumX+movX;
			sumY=sumY+movY;
			count++;
		}
		(*curCluster).setMotion(sumX/count,sumY/count);		
	}

}

void EdgeClusterList::AddNewSegment(EdgeSegmentType edge)
{

	int maxx, maxy, minx, miny,flag=1,clusterdel=1;
	int cmaxx, cmaxy, cminx, cminy;
	float movX, movY;
	float x,y;
	list<EdgeClusterType>::iterator curCluster;
	list<EdgeClusterType>::iterator tmpCluster;
	list<EdgeClusterType>::iterator allClusterCenter;
	list<EdgeSegmentType>::iterator curSegment;
	list<EdgeSegmentType>::iterator tmpSegment;


	edge.getMotion(&movX,&movY);
	
	if(clusterList==NULL){ 
		clusterList = new list<EdgeClusterType> ;
		numCluster	= 0;
		CID=0;
		addNewCluster(edge);
	}
	else /* if(numCluster==0)*/
	{
		addNewCluster(edge);
	}
/*	else
	{
		edge.getBoundaryParam(&maxx, &maxy, &minx, &miny);
		x=(maxx+minx)/2;
		y=(maxy+miny)/2;
		mind=32767;
	
			for(curCluster=clusterList->begin();curCluster!=clusterList->end();curCluster++)
			{
				(*curCluster).getClusterCenter(&cx,&cy);
				d=Dist(x,y,cx,cy);
					if(mind>d)
					{ 
						mind=d;
						ID=(*curCluster).getClusterIndex();
						(*curCluster).getMotion(&movCX,&movCY);
						(*curCluster).getVariance(&varX,&varY);
					}
			}

			if(mind>DT)
			{
				addNewCluster(edge);
			}
			else if((fabs(movX+movCX)<MOTION_THRESHOLD)
								&&
								(fabs(movY+movCY)<MOTION_THRESHOLD)
								&&
								(fabs(movX)<fabs(2*varX)+1)
								&&
								(fabs(movY)<fabs(2*varY)+1)
								)

			{
			
			for(curCluster=clusterList->begin();curCluster!=clusterList->end();curCluster++)
			if(ID==(*curCluster).getClusterIndex()) break;
			if(ID==(*curCluster).getClusterIndex())
			{
				(*curCluster).AddNewSegment(edge);
				edge.getBoundaryParam(&maxx, &maxy, &minx, &miny);
				(*curCluster).setSum((maxx+minx)/2, (maxy+miny)/2);
				(*curCluster).getSum(&x, &y);
				(*curCluster).setClusterCenter(x/(*curCluster).getTotalEdge(), y/(*curCluster).getTotalEdge());

				(*curCluster).getBoundaryParam(&cmaxx, &cmaxy, &cminx, &cminy);
				maxx=maxx>cmaxx? maxx:cmaxx;
				maxy=maxy>cmaxy? maxy:cmaxy;
				minx=minx<cminx? minx:cminx;
				miny=miny<cminy? miny:cminy;
				(*curCluster).setBoundaryParam(maxx, maxy, minx, miny);

						
				flag=1;
			
				while(flag)
				{
					flag=0;

					for(curCluster=clusterList->begin();curCluster!=clusterList->end();)
					{ 		
						clusterdel=1;
				
						for(curSegment=(*curCluster).ListBegin();curSegment!=(*curCluster).ListEnd();)
						{
							(*curSegment).getBoundaryParam(&maxx, &maxy, &minx, &miny);
							x=(maxx+minx)/2;
							y=(maxy+miny)/2;
							mind=32767;

							for(allClusterCenter=clusterList->begin();allClusterCenter!=clusterList->end();allClusterCenter++)
							{
								(*allClusterCenter).getClusterCenter(&cx,&cy);
								d=Dist(x,y,cx,cy);
								if(mind>d)
								{ 
									mind=d;
									ID=(*curCluster).getClusterIndex();
								}
							}
							
							if(ID!=(*curCluster).getClusterIndex()) 
							{
								(*curSegment).getBoundaryParam(&maxx, &maxy, &minx, &miny);
								(*curCluster).setSum(-(maxx+minx)/2, -(maxy+miny)/2);
								(*curCluster).getSum(&x, &y);
								(*curCluster).setClusterCenter(x/(*curCluster).getTotalEdge(), y/(*curCluster).getTotalEdge());
							
					
					

								for(tmpCluster=clusterList->begin();tmpCluster!=clusterList->end();tmpCluster++)
								if(ID==(*tmpCluster).getClusterIndex()) break;
								(*tmpCluster).AddNewSegment(*curSegment);
								(*tmpCluster).setSum((maxx+minx)/2, (maxy+miny)/2);
								(*tmpCluster).getSum(&x, &y);
								(*tmpCluster).setClusterCenter(x/(*tmpCluster).getTotalEdge(), y/(*tmpCluster).getTotalEdge());
									
									(*tmpCluster).getBoundaryParam(&cmaxx, &cmaxy, &cminx, &cminy);
									maxx=maxx>cmaxx? maxx:cmaxx;
									maxy=maxy>cmaxy? maxy:cmaxy;
									minx=minx<cminx? minx:cminx;
									miny=miny<cminy? miny:cminy;
									(*tmpCluster).setBoundaryParam(maxx, maxy, minx, miny);

							flag=1;	
							
								curSegment=(*curCluster).DeleteSegment(curSegment);
								if((*curCluster).getTotalEdge()>0)
								{
									tmpSegment=(*curCluster).ListBegin();
									(*tmpSegment).getBoundaryParam(&cmaxx, &cmaxy, &cminx, &cminy);
									for(;tmpSegment!=(*curCluster).ListEnd();tmpSegment++)
										{
											(*tmpSegment).getBoundaryParam(&maxx, &maxy, &minx, &cminy);
											cmaxx=maxx>cmaxx? maxx:cmaxx;
											cmaxy=maxy>cmaxy? maxy:cmaxy;
											cminx=minx<cminx? minx:cminx;
											cminy=miny<cminy? miny:cminy;
										}
									(*curCluster).setBoundaryParam(cmaxx, cmaxy, cminx, cminy);
								}
								else
								{	
									curCluster=DeleteCluster(curCluster);
									clusterdel=0;
								}

							continue;
							}
							curSegment++;	
						}//inner end for
					if(clusterdel) curCluster++;
					}//end outer for
				}//end while
			}//end if CID=
		}//end else ifaddNewCluster
		else
		{
          addNewCluster(edge);
		}
	}//end else
*/
	//merge intersecting blocks
/*
	for(curCluster=clusterList->begin();curCluster!=clusterList->end();curCluster++)
	{
		(*curCluster).getBoundaryParam(&maxx, &maxy, &minx, &miny);
		//tmpCluster=curCluster;
		//tmpCluster++;
		tmpCluster=clusterList->begin();
		for(;tmpCluster!=clusterList->end();)
		{
			if(tmpCluster==curCluster) {
				tmpCluster++;
				continue;
			}

		clusterdel=1;
			(*tmpCluster).getBoundaryParam(&cmaxx, &cmaxy, &cminx, &cminy);
			if(minx>=cminx&&minx<=cmaxx&&miny>=cminy&&miny<=cmaxy||maxx>=cminx&&maxx<=cmaxx&&miny>=cminy&&miny<=cmaxy||maxx>=cminx&&maxx<=cmaxx&&maxy>=cminy&&maxy<=cmaxy||minx>=cminx&&minx<=cmaxx&&maxy>=cminy&&maxy<=cmaxy||cminx>=minx&&cminx<=maxx&&cminy>=miny&&cminy<=maxy||cmaxx>=minx&&cmaxx<=maxx&&cminy>=miny&&cminy<=maxy||cmaxx>=minx&&cmaxx<=maxx&&cmaxy>=miny&&cmaxy<=maxy||cminx>=minx&&cminx<=maxx&&cmaxy>=miny&&cmaxy<=maxy) //intersecting block
		//	if((abs(minx-cminx)+abs(maxx-cmaxx)+abs(miny-cminy)+abs(maxy-cmaxy))<(abs(maxx-minx)+abs(maxy-miny)+abs(cmaxx-cminx)+abs(cmaxy-cminy))) //intersecting block
			{
				for(curSegment=(*tmpCluster).ListBegin();curSegment!=(*tmpCluster).ListEnd();curSegment++)
				{
					(*curSegment).getBoundaryParam(&maxx, &maxy, &minx, &miny);
					(*curCluster).AddNewSegment(*curSegment);
					(*curCluster).setSum((maxx+minx)/2, (maxy+miny)/2);
					(*curCluster).getSum(&x, &y);
					(*curCluster).setClusterCenter(x/(*curCluster).getTotalEdge(), y/(*curCluster).getTotalEdge());
									
					(*curCluster).getBoundaryParam(&cmaxx, &cmaxy, &cminx, &cminy);
					maxx=maxx>cmaxx? maxx:cmaxx;
					maxy=maxy>cmaxy? maxy:cmaxy;
					minx=minx<cminx? minx:cminx;
					miny=miny<cminy? miny:cminy;
					(*curCluster).setBoundaryParam(maxx, maxy, minx, miny);
				}
			
			tmpCluster=DeleteCluster(tmpCluster);
			clusterdel=0;
			(*curCluster).getBoundaryParam(&maxx, &maxy, &minx, &miny);
			}
		if(clusterdel) tmpCluster++;
		}
	}*/
}

list<EdgeClusterType>::iterator EdgeClusterList::DeleteCluster(list<EdgeClusterType>::iterator segment)
{
	numCluster--;
	return clusterList->erase(segment);
}


list<EdgeClusterType>::iterator EdgeClusterList::ListBegin()
{
	return clusterList->begin();
}

list<EdgeClusterType>::iterator EdgeClusterList::ListEnd()
{
	return clusterList->end();
}


void EdgeClusterList::AddSegmentToCluster(EdgeSegmentType edge, EdgeClusterType *ec)
{
	int maxx, maxy, minx, miny;
	int cmaxx, cmaxy, cminx, cminy;
	list<EdgeClusterType>::iterator curCluster;
	float x,y;

	if(clusterList==NULL){ 
		clusterList = new list<EdgeClusterType> ;
		numCluster	= 0;
		CID=0;
		addNewCluster(edge);
	}
	else if(numCluster==0)
	{
		addNewCluster(edge);
	}
	else
	{
		//edge.getBoundaryParam(&maxx, &maxy, &minx, &miny);
		//x=(maxx+minx)/2;
		//y=(maxy+miny)/2;
		//mind=32767;
		//ec.getBoundaryParam(&maxxc, &maxyc, &minxc, &minyc);
		//maxxc=maxx>maxxc?maxx:maxxc;
		//maxyc=maxy>maxyc?maxy:maxyc;
		//minxc=minx<minxc?minx:minxc;
		//minyc=miny<minyc?miny:minyc;
		//ec.setBoundaryParam(maxxc, maxyc, minxc, minyc);
		for(curCluster=clusterList->begin();curCluster!=clusterList->end();curCluster++)
		if(ec->getClusterIndex()==(*curCluster).getClusterIndex()) break;
		if(ec->getClusterIndex()==(*curCluster).getClusterIndex()){
		(*curCluster).AddNewSegment(edge);
		edge.getBoundaryParam(&maxx, &maxy, &minx, &miny);
		(*curCluster).setSum((maxx+minx)/2, (maxy+miny)/2);
		(*curCluster).getSum(&x, &y);
		(*curCluster).setClusterCenter(x/(*curCluster).getTotalEdge(), y/(*curCluster).getTotalEdge());
	    (*curCluster).getBoundaryParam(&cmaxx, &cmaxy, &cminx, &cminy);
		maxx=maxx>cmaxx? maxx:cmaxx;
		maxy=maxy>cmaxy? maxy:cmaxy;
		minx=minx<cminx? minx:cminx;
		miny=miny<cminy? miny:cminy;
	//	if(edge.getWeight()>=95) 
	//	{
		(*curCluster).setBoundaryParam(maxx, maxy, minx, miny);
	//	}
		}
	}
}
void EdgeClusterList::deleteSegmentFromCluster(list<EdgeSegmentType>::iterator *edge, EdgeClusterType *ec){
	int maxx, maxy, minx, miny;
	int cmaxx, cmaxy, cminx, cminy, segIndex;
	list<EdgeClusterType>::iterator curCluster;
	list<EdgeSegmentType>::iterator curSegment;
	float x,y;
		
	for(curCluster=clusterList->begin();curCluster!=clusterList->end();curCluster++)
	if(ec->getClusterIndex()==(*curCluster).getClusterIndex()) break;
	
	if(ec->getClusterIndex()==(*curCluster).getClusterIndex())
	{
			(*edge)->getBoundaryParam(&maxx, &maxy, &minx, &miny);
			(*curCluster).setSum(-(maxx+minx)/2, -(maxy+miny)/2);
			(*curCluster).getSum(&x, &y);
			(*curCluster).setClusterCenter(x/(*curCluster).getTotalEdge(), y/(*curCluster).getTotalEdge());
			segIndex=(*edge)->GetSegIndex();
			//ec->getBoundaryParam(&maxx,&maxy,&minx,&miny);
			//maxx=0;minx=;maxy=0;miny=0; 
			for(curSegment=(*curCluster).ListBegin();curSegment!=(*curCluster).ListEnd();curSegment++)
			{
				if(segIndex==(*curSegment).GetSegIndex())
				{
					*edge=(*curCluster).DeleteSegment(*edge);
					break;
				}
			}
		/*	for(curSegment=(*curCluster).ListBegin();curSegment!=(*curCluster).ListEnd();curSegment++)
			{
				if((*curCluster).getTotalEdge()==0) {
					curCluster=this->DeleteCluster(curCluster);
					break;
				}*/
			if((*curCluster).getTotalEdge()>0)
			{
			curSegment=(*curCluster).ListBegin();
			(*curSegment).getBoundaryParam(&maxx, &maxy, &minx, &miny);
			for(curSegment=(*curCluster).ListBegin();curSegment!=(*curCluster).ListEnd();curSegment++)
				{
				(*curSegment).getBoundaryParam(&cmaxx, &cmaxy, &cminx, &cminy);
				maxx=maxx>cmaxx? maxx:cmaxx;
				maxy=maxy>cmaxy? maxy:cmaxy;
				minx=minx<cminx? minx:cminx;
				miny=miny<cminy? miny:cminy;
				}
				(*curCluster).setBoundaryParam(maxx, maxy, minx, miny);
			}
		
	}
}
