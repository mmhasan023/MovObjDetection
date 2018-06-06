#include "Model.h"
#include<math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TorsoModel:: TorsoModel(int rx, int ry, float s, int th)
{
		scale=s;
		Vsize=scale*60;
		Hsize=scale*60;
		Rx=rx;
		Ry=ry;
		theta=th;
		width=int(34*scale);


		URArmX=Rx+width*1.2;
		URArmY=Ry-width*0.75;

		ULArmX=Rx-width*1.2;
		ULArmY=Ry-width*0.75;
		
		HX=Rx;
		HY=Ry-width*1.5;
		
		CVsize=Vsize;
		CHsize=Vsize;
		CURArmX=URArmX;
		CURArmY=URArmY;
		CULArmX=ULArmX;
		CULArmY=ULArmY;
		CHX=HX;
		CHY=HY;
		CRx=rx;
		CRy=ry;

		Ctheta=theta;

		pl= new PointType[Vsize];
		memset(pl,0,sizeof(PointType)*Vsize);
		for(int i=0;i<Vsize;i++)
		{
			p_temp.SetPoint(Rx-width,Ry+i-Vsize/5);
			pl[i]=p_temp;
		}
		
		VLtorsoEdge=new EdgeSegmentType(Vsize);
		CVLtorsoEdge=new EdgeSegmentType(Vsize);
		VLtorsoEdge->SetPoint(Vsize,pl);
		CVLtorsoEdge->SetPoint(Vsize,pl);


		pr= new PointType[Vsize];
		memset(pr,0,sizeof(PointType)*Vsize);
		for(int i=0;i<Vsize;i++)
		{
			p_temp.SetPoint(Rx+width,Ry+i-Vsize/5);
			pr[i]=p_temp;
		}
		VRtorsoEdge=new EdgeSegmentType(Vsize);
		CVRtorsoEdge=new EdgeSegmentType(Vsize);
		VRtorsoEdge->SetPoint(Vsize,pr);
		CVRtorsoEdge->SetPoint(Vsize,pr);

		pu= new PointType[Hsize];
		memset(pr,0,sizeof(PointType)*Hsize);
		for(int i=0;i<Hsize;i++)
		{
			p_temp.SetPoint(Rx+i-Hsize/2,Ry-Hsize*.75);
			pr[i]=p_temp;
		}
		UtorsoEdge=new EdgeSegmentType(Hsize);
		CUtorsoEdge=new EdgeSegmentType(Hsize);
		UtorsoEdge->SetPoint(Hsize,pr);
		CUtorsoEdge->SetPoint(Hsize,pr);
}

void TorsoModel:: rotateEdge(int rx, int ry, EdgeSegmentType *Cedge, EdgeSegmentType *edge,int th)
{
PointType *tmp_point;
PointType pt;
float rad=3.141592654/180;
float theta=th*rad;

int size=edge->GetSize();
int i,dx,dy;
float x1,y1;
tmp_point=new PointType[size];
memset(pr,0,sizeof(PointType)*size);

for(i=0;i<size;i++)
{
pt=edge->GetPoint(i);
dx=pt.GetX()-Rx;
dy=pt.GetY()-Ry;

x1=dx*cos(theta)-dy*sin(theta);
y1=dx*sin(theta)+dy*cos(theta);


tmp_point[i].SetX(int(x1+rx+0.5));
tmp_point[i].SetY(int(y1+ry+0.5));

}
CRx=rx;
CRy=ry;

Cedge->SetPoint(size,tmp_point);
}

void TorsoModel:: rotatePoint(int rx, int ry, int &Cxs, int &Cys, int xs, int ys, int th)
{

float rad=3.141592654/180;
float theta=th*rad;

int i,dx,dy;
float x1,y1;

dx=xs-Rx;
dy=ys-Ry;

x1=dx*cos(theta)-dy*sin(theta);
y1=dx*sin(theta)+dy*cos(theta);


Cxs=int(x1+rx+0.5);
Cys=int(y1+ry+0.5);

}

void TorsoModel:: rotateTorsoModel(int rx, int ry, int th)
{
rotateEdge(rx,ry,CVLtorsoEdge,VLtorsoEdge,th);
rotateEdge(rx,ry,CVRtorsoEdge,VRtorsoEdge,th);
rotateEdge(rx,ry,CUtorsoEdge,UtorsoEdge,th);
rotatePoint(rx,ry,CURArmX,CURArmY,URArmX,URArmY,th);
rotatePoint(rx,ry,CULArmX,CULArmY,ULArmX,ULArmY,th);
rotatePoint(rx,ry,CHX,CHY,HX,HY,th);
}

void TorsoModel::getRightHandReferencePoint(int &x, int&y)
{
	x=CURArmX;
	y=CURArmY;
}

void TorsoModel::getLeftHandReferencePoint(int &x, int&y)
{
	x=CULArmX;
	y=CULArmY;
}
void TorsoModel::getHeadReferencePoint(int &x, int&y)
{
	x=CHX;
	y=CHY;
}

TorsoModel::~TorsoModel()
{
	//delete [] p;
	//delete [] p_temp;
	//delete [] VtorsoEdge;

}

void TorsoModel::displayTorsoModel(KSdUint8 ** pOutputImage, int dx,int dy){
int i,j;

for(i=0;i<CVsize;i++)
{
	p_temp=CVLtorsoEdge->GetPoint(i);
	pOutputImage[p_temp.GetY()][p_temp.GetX()]=255;
	p_temp=CVRtorsoEdge->GetPoint(i);
	pOutputImage[p_temp.GetY()][p_temp.GetX()]=255;

}
for(i=0;i<CHsize;i++)
{
	p_temp=CUtorsoEdge->GetPoint(i);
	pOutputImage[p_temp.GetY()][p_temp.GetX()]=255;
}

for(i=-3;i<=3;i++)
{
	for(j=-3;j<=3;j++)
	{
	pOutputImage[CRy+j][CRx+i]=255;
	}
}
for(i=-2;i<=2;i++)
{
	for(j=-2;j<=2;j++)
	{
	pOutputImage[CURArmY+j][CURArmX+i]=255;
	pOutputImage[CULArmY+j][CULArmX+i]=255;
	pOutputImage[CHY+j][CHX+i]=255;
	}
}
}

void TorsoModel::getTorsoEdgeList(EdgeListType* modelEdgeList)
{
	modelEdgeList->EdgeListClear();
	modelEdgeList->AddNewSegment(CVLtorsoEdge);
	modelEdgeList->AddNewSegment(CVRtorsoEdge);
	modelEdgeList->AddNewSegment(CUtorsoEdge);
}




















UpperArmModel:: UpperArmModel(int rx, int ry, float s, int th)
{
		scale=s;
		Vsize=scale*45;
		Rx=rx;
		Ry=ry;
		theta=th;
		width=int(25*scale);

		LArmRX=Rx;
		LArmRY=Ry+Vsize;
				
		CVsize=Vsize;
		CLArmRX=LArmRX;
		CLArmRY=LArmRY;
		Ctheta=theta;
		CRx=rx;
		CRy=ry;

		pl= new PointType[Vsize];
		memset(pl,0,sizeof(PointType)*Vsize);
		for(int i=0;i<Vsize;i++)
		{
			p_temp.SetPoint(Rx-width/2,Ry+i);
			pl[i]=p_temp;
		}
		
		VLtorsoEdge=new EdgeSegmentType(Vsize);
		CVLtorsoEdge=new EdgeSegmentType(Vsize);
		VLtorsoEdge->SetPoint(Vsize,pl);
		CVLtorsoEdge->SetPoint(Vsize,pl);


		pr= new PointType[Vsize];
		memset(pr,0,sizeof(PointType)*Vsize);
		for(int i=0;i<Vsize;i++)
		{
			p_temp.SetPoint(Rx+width/2,Ry+i);
			pr[i]=p_temp;
		}
		VRtorsoEdge=new EdgeSegmentType(Vsize);
		CVRtorsoEdge=new EdgeSegmentType(Vsize);
		VRtorsoEdge->SetPoint(Vsize,pr);
		CVRtorsoEdge->SetPoint(Vsize,pr);

}

void UpperArmModel::rotateEdge(int rx, int ry, EdgeSegmentType *Cedge, EdgeSegmentType *edge,int th)
{
PointType *tmp_point;
PointType pt;
float rad=3.141592654/180;
float theta=th*rad;

int size=edge->GetSize();
int i,dx,dy;
float x1,y1;
tmp_point=new PointType[size];
memset(pr,0,sizeof(PointType)*size);

for(i=0;i<size;i++)
{
pt=edge->GetPoint(i);
dx=pt.GetX()-Rx;
dy=pt.GetY()-Ry;

x1=dx*cos(theta)-dy*sin(theta);
y1=dx*sin(theta)+dy*cos(theta);


tmp_point[i].SetX(int(x1+rx+0.5));
tmp_point[i].SetY(int(y1+ry+0.5));

}
CRx=rx;
CRy=ry;
Cedge->SetPoint(size,tmp_point);
}

void UpperArmModel:: rotatePoint(int rx, int ry, int &Cxs, int &Cys, int xs, int ys, int th)
{

float rad=3.141592654/180;
float theta=th*rad;

int i,dx,dy;
float x1,y1;

dx=xs-Rx;
dy=ys-Ry;

x1=dx*cos(theta)-dy*sin(theta);
y1=dx*sin(theta)+dy*cos(theta);


Cxs=int(x1+rx+0.5);
Cys=int(y1+ry+0.5);

}

void UpperArmModel:: rotateUpperArmModel(int rx, int ry, int th)
{
rotateEdge(rx,ry,CVLtorsoEdge,VLtorsoEdge,th);
rotateEdge(rx,ry,CVRtorsoEdge,VRtorsoEdge,th);
rotatePoint(rx,ry,CLArmRX,CLArmRY,LArmRX,LArmRY,th);
}

void UpperArmModel::getLowerArmReferencePoint(int &x, int&y)
{
	x=CLArmRX;
	y=CLArmRY;
}

UpperArmModel::~UpperArmModel()
{
	//delete [] p;
	//delete [] p_temp;
	//delete [] VtorsoEdge;

}

void UpperArmModel::displayUpperArmModel(KSdUint8 ** pOutputImage, int dx,int dy){
int i,j;

for(i=0;i<CVsize;i++)
{
	p_temp=CVLtorsoEdge->GetPoint(i);
	pOutputImage[p_temp.GetY()][p_temp.GetX()]=255;
	p_temp=CVRtorsoEdge->GetPoint(i);
	pOutputImage[p_temp.GetY()][p_temp.GetX()]=255;

}

for(i=-3;i<=3;i++)
{
	for(j=-3;j<=3;j++)
	{
	pOutputImage[CRy+j][CRx+i]=255;
	}
}
for(i=-2;i<=2;i++)
{
	for(j=-2;j<=2;j++)
	{
	pOutputImage[CLArmRY+j][CLArmRX+i]=255;
	}
}
}

void UpperArmModel::getUpperArmEdgeList(EdgeListType* modelEdgeList)
{
	modelEdgeList->EdgeListClear();
	modelEdgeList->AddNewSegment(CVLtorsoEdge);
	modelEdgeList->AddNewSegment(CVRtorsoEdge);
}












LowerArmModel::LowerArmModel(int rx, int ry, float s, int th)
{
		scale=s;
		Vsize=scale*70;
		Rx=rx;
		Ry=ry;
		theta=th;
		width=int(20*scale);

		LArmRX=Rx;
		LArmRY=Ry+Vsize;
				
		CVsize=Vsize;
		CLArmRX=LArmRX;
		CLArmRY=LArmRY;
		Ctheta=theta;
		CRx=rx;
		CRy=ry;

		pl= new PointType[Vsize];
		memset(pl,0,sizeof(PointType)*Vsize);
		for(int i=0;i<Vsize;i++)
		{
			p_temp.SetPoint(Rx-width/2,Ry+i);
			pl[i]=p_temp;
		}
		
		VLtorsoEdge=new EdgeSegmentType(Vsize);
		CVLtorsoEdge=new EdgeSegmentType(Vsize);
		VLtorsoEdge->SetPoint(Vsize,pl);
		CVLtorsoEdge->SetPoint(Vsize,pl);


		pr= new PointType[Vsize];
		memset(pr,0,sizeof(PointType)*Vsize);
		for(int i=0;i<Vsize;i++)
		{
			p_temp.SetPoint(Rx+width/2,Ry+i);
			pr[i]=p_temp;
		}
		VRtorsoEdge=new EdgeSegmentType(Vsize);
		CVRtorsoEdge=new EdgeSegmentType(Vsize);
		VRtorsoEdge->SetPoint(Vsize,pr);
		CVRtorsoEdge->SetPoint(Vsize,pr);

		pu= new PointType[width];
		memset(pu,0,sizeof(PointType)*width);
		for(int i=0;i<width;i++)
		{
			p_temp.SetPoint(Rx+i-width/2,Ry+Vsize);
			pu[i]=p_temp;
		}
		HtorsoEdge=new EdgeSegmentType(width);
		CHtorsoEdge=new EdgeSegmentType(width);
		HtorsoEdge->SetPoint(width,pu);
		CHtorsoEdge->SetPoint(width,pu);

}

void LowerArmModel::rotateEdge(int rx, int ry, EdgeSegmentType *Cedge, EdgeSegmentType *edge,int th)
{
PointType *tmp_point;
PointType pt;
float rad=3.141592654/180;
float theta=th*rad;

int size=edge->GetSize();
int i,dx,dy;
float x1,y1;
tmp_point=new PointType[size];
memset(pr,0,sizeof(PointType)*size);

for(i=0;i<size;i++)
{
pt=edge->GetPoint(i);
dx=pt.GetX()-Rx;
dy=pt.GetY()-Ry;

x1=dx*cos(theta)-dy*sin(theta);
y1=dx*sin(theta)+dy*cos(theta);


tmp_point[i].SetX(int(x1+rx+0.5));
tmp_point[i].SetY(int(y1+ry+0.5));

}
CRx=rx;
CRy=ry;
Cedge->SetPoint(size,tmp_point);
}

void LowerArmModel:: rotatePoint(int rx, int ry, int &Cxs, int &Cys, int xs, int ys, int th)
{

float rad=3.141592654/180;
float theta=th*rad;

int i,dx,dy;
float x1,y1;

dx=xs-Rx;
dy=ys-Ry;

x1=dx*cos(theta)-dy*sin(theta);
y1=dx*sin(theta)+dy*cos(theta);


Cxs=int(x1+rx+0.5);
Cys=int(y1+ry+0.5);

}

void LowerArmModel:: rotateLowerArmModel(int rx, int ry, int th)
{
rotateEdge(rx,ry,CVLtorsoEdge,VLtorsoEdge,th);
rotateEdge(rx,ry,CVRtorsoEdge,VRtorsoEdge,th);
rotateEdge(rx,ry,CHtorsoEdge,HtorsoEdge,th);
rotatePoint(rx,ry,CLArmRX,CLArmRY,LArmRX,LArmRY,th);
}

void LowerArmModel::getLowerArmEndPoint(int &x, int&y)
{
	x=CLArmRX;
	y=CLArmRY;
}

LowerArmModel::~LowerArmModel()
{
	//delete [] p;
	//delete [] p_temp;
	//delete [] VtorsoEdge;

}

void LowerArmModel::displayLowerArmModel(KSdUint8 ** pOutputImage, int dx,int dy){
int i,j;

for(i=0;i<CVsize;i++)
{
	p_temp=CVLtorsoEdge->GetPoint(i);
	pOutputImage[p_temp.GetY()][p_temp.GetX()]=255;
	p_temp=CVRtorsoEdge->GetPoint(i);
	pOutputImage[p_temp.GetY()][p_temp.GetX()]=255;
}

for(i=0;i<width;i++)
{
	p_temp=CHtorsoEdge->GetPoint(i);
	pOutputImage[p_temp.GetY()][p_temp.GetX()]=255;
}

for(i=-3;i<=3;i++)
{
	for(j=-3;j<=3;j++)
	{
	pOutputImage[CRy+j][CRx+i]=255;
	}
}
for(i=-2;i<=2;i++)
{
	for(j=-2;j<=2;j++)
	{
	pOutputImage[CLArmRY+j][CLArmRX+i]=255;
	}
}
}

void LowerArmModel::getLowerArmEdgeList(EdgeListType* modelEdgeList)
{
	modelEdgeList->EdgeListClear();
	modelEdgeList->AddNewSegment(CVLtorsoEdge);
	modelEdgeList->AddNewSegment(CVRtorsoEdge);
	modelEdgeList->AddNewSegment(CHtorsoEdge);
}







HeadModel::HeadModel(int rx, int ry, float s, int th)
{
		scale=s;
		Vsize=scale*45;
		Hsize=scale*38;
		Rx=rx;
		Ry=ry;
		theta=th;

		CVsize=Vsize;
		CHsize=Hsize;
		Ctheta=theta;
		CRx=rx;
		CRy=ry;

		pl= new PointType[Vsize];
		memset(pl,0,sizeof(PointType)*Vsize);
		for(int i=0;i<Vsize;i++)
		{
			p_temp.SetPoint(Rx-Hsize/2,Ry-i);
			pl[i]=p_temp;
		}
		
		VLtorsoEdge=new EdgeSegmentType(Vsize);
		CVLtorsoEdge=new EdgeSegmentType(Vsize);
		VLtorsoEdge->SetPoint(Vsize,pl);
		CVLtorsoEdge->SetPoint(Vsize,pl);


		pr= new PointType[Vsize];
		memset(pr,0,sizeof(PointType)*Vsize);
		for(int i=0;i<Vsize;i++)
		{
			p_temp.SetPoint(Rx+Hsize/2,Ry-i);
			pr[i]=p_temp;
		}
		VRtorsoEdge=new EdgeSegmentType(Vsize);
		CVRtorsoEdge=new EdgeSegmentType(Vsize);
		VRtorsoEdge->SetPoint(Vsize,pr);
		CVRtorsoEdge->SetPoint(Vsize,pr);




		pu= new PointType[Hsize];
		memset(pu,0,sizeof(PointType)*Hsize);
		for(int i=0;i<Hsize;i++)
		{
			p_temp.SetPoint(Rx+i-Hsize/2,Ry-Vsize);
			pu[i]=p_temp;
		}
		
		HLtorsoEdge=new EdgeSegmentType(Hsize);
		CHLtorsoEdge=new EdgeSegmentType(Hsize);
		HLtorsoEdge->SetPoint(Hsize,pu);
		CHLtorsoEdge->SetPoint(Hsize,pu);


		pd= new PointType[Hsize];
		memset(pd,0,sizeof(PointType)*Hsize);
		for(int i=0;i<Hsize;i++)
		{
			p_temp.SetPoint(Rx+i-Hsize/2,Ry);
			pd[i]=p_temp;
		}
		HRtorsoEdge=new EdgeSegmentType(Hsize);
		CHRtorsoEdge=new EdgeSegmentType(Hsize);
		HRtorsoEdge->SetPoint(Hsize,pd);
		CHRtorsoEdge->SetPoint(Hsize,pd);
}

void HeadModel::rotateEdge(int rx, int ry, EdgeSegmentType *Cedge, EdgeSegmentType *edge,int th)
{
PointType *tmp_point;
PointType pt;
float rad=3.141592654/180;
float theta=th*rad;

int size=edge->GetSize();
int i,dx,dy;
float x1,y1;
tmp_point=new PointType[size];
memset(pr,0,sizeof(PointType)*size);

for(i=0;i<size;i++)
{
pt=edge->GetPoint(i);
dx=pt.GetX()-Rx;
dy=pt.GetY()-Ry;

x1=dx*cos(theta)-dy*sin(theta);
y1=dx*sin(theta)+dy*cos(theta);


tmp_point[i].SetX(int(x1+rx+0.5));
tmp_point[i].SetY(int(y1+ry+0.5));

}
CRx=rx;
CRy=ry;
Cedge->SetPoint(size,tmp_point);
}

void HeadModel:: rotatePoint(int rx, int ry, int &Cxs, int &Cys, int xs, int ys, int th)
{

float rad=3.141592654/180;
float theta=th*rad;

int i,dx,dy;
float x1,y1;

dx=xs-Rx;
dy=ys-Ry;

x1=dx*cos(theta)-dy*sin(theta);
y1=dx*sin(theta)+dy*cos(theta);


Cxs=int(x1+rx+0.5);
Cys=int(y1+ry+0.5);

}

void HeadModel:: rotateHeadModel(int rx, int ry, int th)
{
rotateEdge(rx,ry,CVLtorsoEdge,VLtorsoEdge,th);
rotateEdge(rx,ry,CVRtorsoEdge,VRtorsoEdge,th);
rotateEdge(rx,ry,CHLtorsoEdge,HLtorsoEdge,th);
rotateEdge(rx,ry,CHRtorsoEdge,HRtorsoEdge,th);
}

HeadModel::~HeadModel()
{
	//delete [] p;
	//delete [] p_temp;
	//delete [] VtorsoEdge;

}

void HeadModel::displayHeadModel(KSdUint8 ** pOutputImage, int dx,int dy){
int i,j;

for(i=0;i<CVsize;i++)
{
	p_temp=CVLtorsoEdge->GetPoint(i);
	pOutputImage[p_temp.GetY()][p_temp.GetX()]=255;
	p_temp=CVRtorsoEdge->GetPoint(i);
	pOutputImage[p_temp.GetY()][p_temp.GetX()]=255;
}

for(i=0;i<CHsize;i++)
{
	p_temp=CHLtorsoEdge->GetPoint(i);
	pOutputImage[p_temp.GetY()][p_temp.GetX()]=255;
	p_temp=CHRtorsoEdge->GetPoint(i);
	pOutputImage[p_temp.GetY()][p_temp.GetX()]=255;
}

for(i=-3;i<=3;i++)
{
	for(j=-3;j<=3;j++)
	{
	pOutputImage[CRy+j][CRx+i]=255;
	}
}
}
void HeadModel::getHeadEdgeList(EdgeListType* modelEdgeList)
{
	modelEdgeList->EdgeListClear();
	modelEdgeList->AddNewSegment(CVLtorsoEdge);
	modelEdgeList->AddNewSegment(CVRtorsoEdge);
	modelEdgeList->AddNewSegment(CHLtorsoEdge);
	modelEdgeList->AddNewSegment(CHRtorsoEdge);
}

