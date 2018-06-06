// <begin copyright notice>
// ---------------------------------------------------------------------------
//
//                   Copyright (c) 1997 TargetJr Consortium
//               GE Corporate Research and Development (GE CRD)
//                             1 Research Circle
//                            Niskayuna, NY 12309
//                            All Rights Reserved
//              Reproduction rights limited as described below.
//                               
//      Permission to use, copy, modify, distribute, and sell this software
//      and its documentation for any purpose is hereby granted without fee,
//      provided that (i) the above copyright notice and this permission
//      notice appear in all copies of the software and related documentation,
//      (ii) the name TargetJr Consortium (represented by GE CRD), may not be
//      used in any advertising or publicity relating to the software without
//      the specific, prior written permission of GE CRD, and (iii) any
//      modifications are clearly marked and summarized in a change history
//      log.
//       
//      THE SOFTWARE IS PROVIDED "AS IS" AND WITHOUT WARRANTY OF ANY KIND,
//      EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
//      WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
//      IN NO EVENT SHALL THE TARGETJR CONSORTIUM BE LIABLE FOR ANY SPECIAL,
//      INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND OR ANY
//      DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
//      WHETHER OR NOT ADVISED OF THE POSSIBILITY OF SUCH DAMAGES, OR ON
//      ANY THEORY OF LIABILITY ARISING OUT OF OR IN CONNECTION WITH THE
//      USE OR PERFORMANCE OF THIS SOFTWARE.
//
// ---------------------------------------------------------------------------
// <end copyright notice>
//--------------------------------------------------------------
//
// Class : CannyBase
//
// See CannyBase.h
//
//--------------------------------------------------------------

/*#include <cool/List.h>
#include <Detection/CannyBase.h>
#include <Topology/Vertex.h>
#include <Basics/bufferxy.h>*/
#include "CannyBase.h"

#include <math.h>
#include <stdlib.h>

const float DUMMYTHETA = 10000.0;


//--------------------------------------------------------------
//
// -- Constructor.
//
//CannyBase::CannyBase(KScScalarImage2dUint8 *sourceImage,float sigma, float low, float high, bool v)
CannyBase::CannyBase(KScScalarImage2dUint8 *sourceImage,
					 float                  sigma      , 
					 float                  low        , 
					 float                  high       , 
					 bool                   v          )

{
  verbose = 1   ;//v;
  _sigma  = sigma ;//sigma;
  _low    = low ;//low
  _high   = high;//high

  	// 입력 영상의 크기를 얻음
	int _xsize  = sourceImage->GetMainXSize(              );
	int _ysize  = sourceImage->GetMainYSize(              );
	_image      = new KScScalarImage2dUint8(_xsize, _ysize);
}


//--------------------------------------------------------------
//
// -- Destructor;  this does nothing at all.
//
CannyBase::~CannyBase()
{
}



//-----------------------------------------------------------------------------
//
// -- Returns an m*n array of floats.  Protected.
// 
float **CannyBase::Make_float_image(int dx, int dy)//(dx,dy)
{
	float *array = new float [dx*dy];
	float **data = new float*[dy   ];

	for( register int i=0 ; i<dy ; i++ )
		data[i] = &array[i*dx];

	return data;
}



//------------------------------------------------------------------------
//
// -- Initialise an m*n array of floats with DUMMYTHETA.  Protected.
// 
void CannyBase::initFloatImage(float** theta, int dx, int dy)
{
	for( register int y=0,x ; y<dy ; y++ )
		for( x=0 ; x<dx ; x++ )
			theta[y][x] = DUMMYTHETA;
}



//-----------------------------------------------------------------------------
//
// -- Frees an m*n array of floats.  Protected.
// 
void CannyBase::Free_float_image(float **ptr)
{
	float* array = ptr[0];
	delete [] array;
	delete [] ptr;
}



//-----------------------------------------------------------------------------
//
// -- Returns an m*n array of ints.  Protected.
// 
int **CannyBase::Make_int_image(int dx, int dy)
{
	int *array = new int [dx*dy];
	int **data = new int*[dy   ];
	for( register int i=0 ; i<dy ; i++ )
		data[i] = &array[i*dx];

	return data;
}



//-----------------------------------------------------------------------------
//
// -- Frees an m*n array of ints.  Protected.
// 
void CannyBase::Free_int_image(int **ptr)
{
  int* array = ptr[0];
  delete [] array;
  delete [] ptr;
}



//-----------------------------------------------------------------------------
//
// -- Returns a vector of floats GetLength m.  Protected.
// 
float *CannyBase::Make_float_vector(int m)
{
    float *tmp;
    tmp = new float[m];

    if(tmp == NULL)
	{
		cerr << "Can't allocate a " << m << " vector\n";
		exit(1);
	}
	
    return tmp;
}



//-----------------------------------------------------------------------------
//
// -- Sets up the convolution kernel based on the first derivative
// of a Gaussian.  Protected.
// 
void CannyBase::Set_kernel()
{
    register int i,x;
    float s2  = float(2.0*_sigma*_sigma);
    float det = float(_sigma*sqrt(2.0*KS_PI));

    for( i=0,x=-_width ;i<_k_size ; i++,x++ )
		_kernel[i] = float(exp(-x*x/s2)/det); 
}



//-----------------------------------------------------------------------------
//
// -- Zeros a floating point image.  Protected.
// 
void CannyBase::Zero_float_image(float **image)
{
	register int x,y;

	for( y=0 ; y<_image->GetYSize() ; y++ )
		for( x=0 ; x<_image->GetXSize() ;x++ )
			image[y][x]=0.0;
}



//-----------------------------------------------------------------------------
//
// -- Zeros an int image.  Protected.
// 
void CannyBase::Zero_int_image(int **image)
{
    register int x,y;
	
    for( y=0 ; y<_image->GetYSize() ; y++ )
	    for( x=0 ; x<_image->GetXSize() ; x++ )
            image[y][x] = 0;
}



//-----------------------------------------------------------------------------
// 
// -- Following routine looking for connectiveness of edgel chains
// and accounts for single pixel gaps in the chains.  Protected.
//
void CannyBase::Initial_follow(int x, int y,
			   ListType<int>   *xc  ,
			   ListType<int>   *yc  ,
			   ListType<float> *grad)
{
	if (_thin[y][x] == 0)
		return;


	// Make sure that we are not likely to overun the border of the image
	if( (x<=0) || (x>=_image->GetXSize()-1) || (y<=0) || (y>=_image->GetYSize()-1) )
		return;
	
	// Add the current point to the coordinate lists, and delete from
	// the edge image
	xc  ->push(x          );  
	yc  ->push(y          );  
	grad->push(_thin[y][x]);
	_thin[y][x] = 0.0;
	
	
	// Now recursively look for connected eight-neighbours
	if( _thin[y-1][x  ] > _low ) Initial_follow(x  ,y-1,xc,yc,grad);
	if( _thin[y  ][x-1] > _low ) Initial_follow(x-1,y  ,xc,yc,grad);
	if( _thin[y+1][x  ] > _low ) Initial_follow(x  ,y+1,xc,yc,grad);
	if( _thin[y  ][x+1] > _low ) Initial_follow(x+1,y  ,xc,yc,grad);
	if( _thin[y-1][x+1] > _low ) Initial_follow(x+1,y-1,xc,yc,grad);
	if( _thin[y-1][x-1] > _low ) Initial_follow(x-1,y-1,xc,yc,grad);
	if( _thin[y+1][x-1] > _low ) Initial_follow(x-1,y+1,xc,yc,grad);
	if( _thin[y+1][x+1] > _low ) Initial_follow(x+1,y+1,xc,yc,grad);

}



//-----------------------------------------------------------------------------
// 
// -- Following routine looking for connectiveness of edgel chains
// and accounts for single pixel gaps in the chains.  Protected.
//
/*
void CannyBase::Final_follow(int x,   int y       , 
							 ListType<int  > *xc  , 
							 ListType<int  > *yc  ,
							 ListType<float> *grad, 
							 int reverse          )
{
	
	ListType<PointType> Q;
	Q.clear();
	PointType tmp;

	int connect=0;


	tmp.x=x;tmp.y=y;
	Q.push(tmp);
	
	while (Q.GetLength())
	{
		tmp=Q.pop();
		connect=0;
		x = tmp.x; y = tmp.y;
		xc  ->push(x);
		yc  ->push(y);
		grad->push(_thin[y][x]);
		_thin[y][x]=0.0;
		if (_thin[y-1][x-1]!= 0 ) 
		{ 
			tmp.y = y-1; tmp.x = x-1;
			Q.push(tmp);
			connect++  ;
		}
		if (_thin[y-1][x  ]!= 0 )  
		{   
			tmp.y = y-1; tmp.x = x  ;
			Q.push(tmp);	
			connect++  ;
		}
		if (_thin[y-1][x+1]!= 0 )  
		{   
			tmp.y = y-1; tmp.x = x+1;
			Q.push(tmp);	
			connect++  ;
		}
		
		if (_thin[y][x-1]!= 0 )  
		{   
			tmp.y = y  ; tmp.x = x-1;
			Q.push(tmp);	
			connect++  ;
		}
		if (_thin[y][x+1]!= 0 )  
		{   
			tmp.y = y  ; tmp.x = x+1;
			Q.push(tmp);	
			connect++  ;
		}
		
		if (_thin[y+1][x-1]!= 0 )  
		{   
			tmp.y = y+1; tmp.x = x-1;
			Q.push(tmp);	
			connect++  ;
		}
		if (_thin[y+1][x  ]!= 0 )  
		{   
			tmp.y = y+1; tmp.x = x  ;
			Q.push(tmp);	
			connect++  ;
		}
		if (_thin[y+1][x+1]!= 0 )  
		{   
			tmp.y = y+1; tmp.x = x+1;
			Q.push(tmp);	
			connect++  ;
		}
		
	}
	
}
*/


void CannyBase::Final_follow(int x,   int y       , 
							 ListType<int  > *xc  , 
							 ListType<int  > *yc  ,
							 ListType<float> *grad,
							 int              first)
//							 int reverse          )
//							 EdgeListType    *edges,
//
{


	// Make sure that we are not likely to overun the border of the image
	if( (x<=0) || (x>=_image->GetXSize()-1) || (y<=0) || (y>=_image->GetYSize()-1) )
		return;
	
	// Add the current point to the coordinate lists, and delete from
	// the edge image
	xc  ->push(x          );  
	yc  ->push(y          );  
	grad->push(_thin[y][x]);
	_thin[y][x] = 0.0;
	
	if (first&&(_junction[y][x]||_dangling[y][x]))
		return;
	
	// Now recursively look for connected eight-neighbours
	if( _thin[y-1][x  ] > _low ) Final_follow(x  ,y-1,xc,yc,grad,1);
	if( _thin[y  ][x-1] > _low ) Final_follow(x-1,y  ,xc,yc,grad,1);
	if( _thin[y+1][x  ] > _low ) Final_follow(x  ,y+1,xc,yc,grad,1);
	if( _thin[y  ][x+1] > _low ) Final_follow(x+1,y  ,xc,yc,grad,1);
	if( _thin[y-1][x+1] > _low ) Final_follow(x+1,y-1,xc,yc,grad,1);
	if( _thin[y-1][x-1] > _low ) Final_follow(x-1,y-1,xc,yc,grad,1);
	if( _thin[y+1][x-1] > _low ) Final_follow(x-1,y+1,xc,yc,grad,1);
	if( _thin[y+1][x+1] > _low ) Final_follow(x+1,y+1,xc,yc,grad,1);


	

}
/*							 int              edgIndex)
{
	if (_thin[y][x] == 0)
		return;

	
	// Make sure that we are not likely to overun the border of the image
	if( (x<=0) || (x>=_image->GetXSize()-1) || (y<=0) || (y>=_image->GetYSize()-1) )
		return;
	
	// Add the current point to the coordinate lists, and delete from
	// the edge image 
	if( !reverse )
    {
		xc  ->push(x          );  
		yc  ->push(y          );  
		grad->push(_thin[x][y]);
	}
	_thin[y][x] = 0.0;
//	edges->m_Points[edgIndex].x=x;
//	edges->m_Points[edgIndex].y=y;
	
	// Now recursively look for connected eight-neighbours
	if     ( (_thin[y-1][x  ]>_low) && (_junction[y-1][x  ]==0) ) Final_follow(x  ,y-1,xc,yc,grad,0);
	else if( (_thin[y  ][x-1]>_low) && (_junction[y  ][x-1]==0) ) Final_follow(x-1,y  ,xc,yc,grad,0);
	else if( (_thin[y+1][x  ]>_low) && (_junction[y+1][x  ]==0) ) Final_follow(x  ,y+1,xc,yc,grad,0);
	else if( (_thin[y  ][x+1]>_low) && (_junction[y  ][x+1]==0) ) Final_follow(x+1,y  ,xc,yc,grad,0);
	else if( (_thin[y-1][x+1]>_low) && (_junction[y-1][x+1]==0) ) Final_follow(x+1,y-1,xc,yc,grad,0);
	else if( (_thin[y-1][x-1]>_low) && (_junction[y-1][x-1]==0) ) Final_follow(x-1,y-1,xc,yc,grad,0);
	else if( (_thin[y+1][x-1]>_low) && (_junction[y+1][x-1]==0) ) Final_follow(x-1,y+1,xc,yc,grad,0);
	else if( (_thin[y+1][x+1]>_low) && (_junction[y+1][x+1]==0) ) Final_follow(x+1,y+1,xc,yc,grad,0);
	
	// Else see if there is a junction nearby, and record it. The _chain_no
	// variable is used to prevent the same junction being inserted at both
	// ends of the EdgelChains when reversel occurs next to the junction
	// (in that case there will only be two stored points: the edge and the junction)
	else if( _junction[y-1][x  ] && ((xc->GetLength()>2)||(_junction[y-1][x  ]!=_chain_no)) )
    {
		xc  ->push(_jx[y-1][x  ]);  
		yc  ->push(_jy[y-1][x  ]);  
		grad->push(_jval        );
		_junction[y-1][x  ] = _chain_no;
	}
	else if( _junction[y  ][x-1] && ((xc->GetLength()>2)||(_junction[y  ][x-1]!=_chain_no)) )
    {
		xc  ->push(_jx[y  ][x-1]);  
		yc  ->push(_jy[y  ][x-1]);  
		grad->push(_jval        );
		_junction[y  ][x-1] = _chain_no;
	}
	else if( _junction[y+1][x  ] && ((xc->GetLength()>2)||(_junction[y+1][x  ]!=_chain_no)) )
    {
		xc  ->push(_jx[y+1][x  ]);  
		yc  ->push(_jy[y+1][x  ]);  
		grad->push(_jval        );
		_junction[y+1][x  ] = _chain_no;
	}
	else if( _junction[y  ][x+1] && ((xc->GetLength()>2)||(_junction[y  ][x+1]!=_chain_no)) )
    {
		xc  ->push(_jx[y  ][x+1]);  
		yc  ->push(_jy[y  ][x+1]);  
		grad->push(_jval        );
		_junction[y  ][x+1] = _chain_no;
	}
	else if( _junction[y-1][x+1] && ((xc->GetLength()>2)||(_junction[y-1][x+1]!=_chain_no)) )
    {
		xc  ->push(_jx[y-1][x+1]);  
		yc  ->push(_jy[y-1][x+1]);  
		grad->push(_jval        );
		_junction[y-1][x+1] = _chain_no;
	}
	else if( _junction[y-1][x-1] && ((xc->GetLength()>2)||(_junction[y-1][x-1]!=_chain_no)) )
    {
		xc  ->push(_jx[y-1][x-1]);  
		yc  ->push(_jy[y-1][x-1]);  
		grad->push(_jval        );
		_junction[y-1][x-1] = _chain_no;
	}
	else if( _junction[y+1][x-1] && ((xc->GetLength()>2)||(_junction[y+1][x-1]!=_chain_no)) )
    {
		xc  ->push(_jx[y+1][x-1]);  
		yc  ->push(_jy[y+1][x-1]);  
		grad->push(_jval        );
		_junction[y+1][x-1] = _chain_no;
	}
	else if( _junction[y+1][x+1] && ((xc->GetLength()>2)||(_junction[y+1][x+1]!=_chain_no)) )
    {
		xc  ->push(_jx[y+1][x+1]);  
		yc  ->push(_jy[y+1][x+1]);  
		grad->push(_jval        );
		_junction[y+1][x+1] = _chain_no;
	}
}

*/

//-----------------------------------------------------------------------------
//
//  -- Copies image1 to image2.  Protected.
//                                  thick     thin
void CannyBase::Copy_image(float **image1, float **image2)
{
	int x,y;
	for( y=0 ; y<_image->GetYSize() ; y++ )
		for( x=0 ; x<_image->GetXSize() ;x++ )
			image2[y][x] = image1[y][x];
}

void CannyBase::Copy_image(int **image1, int **image2)
{
	int x,y;
	for( y=0 ; y<_image->GetYSize() ; y++ )
		for( x=0 ; x<_image->GetXSize() ;x++ )
			image2[y][x] = image1[y][x];
}

void CannyBase::Copy_image(int **image1, float **image2)
{
	int x,y;
	for( y=0 ; y<_image->GetYSize() ; y++ )
		for( x=0 ; x<_image->GetXSize() ;x++ )
			image2[y][x] = (float)image1[y][x];
}



//-----------------------------------------------------------------------------
// 
// -- Following routine looking for searching out junction clusters.  Protected.
//
void CannyBase::Follow_junctions(int x,   int y   , 
								 ListType<int> *xc, 
								 ListType<int> *yc)
{
	
	// Add the current junction to the coordinate lists, and delete from
	// the junction image
	xc->push(x);  
	yc->push(y);  
	_junction[y][x] = 0;
	
	// Now recursively look for connected eight-neighbours
	if( _junction[y-1][x  ] ) 
		Follow_junctions(x  ,y-1,xc,yc);
	if( _junction[y  ][x-1] ) 
		Follow_junctions(x-1,y  ,xc,yc);
	if( _junction[y+1][x  ] ) 
		Follow_junctions(x  ,y+1,xc,yc);
	if( _junction[y  ][x+1] ) 
		Follow_junctions(x+1,y  ,xc,yc);
	if( _junction[y-1][x+1] ) 
		Follow_junctions(x+1,y-1,xc,yc);
	if( _junction[y-1][x-1] ) 
		Follow_junctions(x-1,y-1,xc,yc);
	if( _junction[y+1][x-1] ) 
		Follow_junctions(x-1,y+1,xc,yc);
	if( _junction[y+1][x+1] ) 
		Follow_junctions(x+1,y+1,xc,yc);
}



//-----------------------------------------------------------------------------
// 
// -- Finds which member of the lists lies closest to the centre
// of gravity of the lists.  Protected.
//
void CannyBase::Cluster_centre_of_gravity(ListType<int> &xc, 
										  ListType<int> &yc,
										  int           &x0, 
										  int           &y0)
{
	if( !xc.GetLength() )
		return;
	
	// First find the CofG
	double x=0.0,y=0.0;
	int xdata, ydata;
	/*for(xc.ResetCurNode(),yc.ResetCurNode();xc.next(),yc.next();)
    {
		x += xc.value();  
		y += yc.value();
    }*/
	xc.ResetCurNode();  yc.ResetCurNode();
	while(xc.GetNextItem(xdata) && yc.GetNextItem(ydata))
	{
		x += xdata;
		y += ydata;
	}
	x /= xc.GetLength();  y /= yc.GetLength();
	
	// Now find the point closest to the CofG
	float dist,newdist;
	dist = float(_image->GetXSize()*_image->GetYSize()); // A number larger than the image size

	//for(xc.ResetCurNode(),yc.ResetCurNode();xc.next(),yc.next();)
	xc.ResetCurNode();  yc.ResetCurNode();
	while(xc.GetNextItem(xdata) && yc.GetNextItem(ydata))
	{
		//if( (newdist=float(hypot(x-xc.value(),y-yc.value()))) < dist ) //hypot 빗변계산
		if( (newdist=float(hypot(x-xdata, y-ydata))) < dist )
		{
			//x0   = xc.value();  
			//y0   = yc.value();
			x0 = xdata;
			y0 = ydata;
			dist = newdist   ;
		}
	}	
	// Set up the (_jx,_jy) arrays to point to the cluster centre
	//for(xc.ResetCurNode(),yc.ResetCurNode();xc.next(),yc.next();)
	xc.ResetCurNode();  yc.ResetCurNode();
	while(xc.GetNextItem(xdata) && yc.GetNextItem(ydata))
	{
		//_jx[yc.value()][xc.value()] = x0;
		//_jy[yc.value()][xc.value()] = y0;
		_jx[ydata][xdata] = x0;
		_jy[ydata][xdata] = y0;
	}
}



//-----------------------------------------------------------------------------
//
// -- Detemines whether the point (x,y) is a neighbour to a
// junction.  Protected.
//
int CannyBase::Junction_neighbour(int x, int y)
{

	// Find the neighbour of (x][y) in the image
	if( _junction[y-1][x-1] || _junction[y-1][x  ] || _junction[y-1][x+1] ||
		_junction[y  ][x+1] || _junction[y+1][x+1] || _junction[y+1][x  ] ||
		_junction[y+1][x-1] || _junction[y  ][x-1]                         )
		return(1);
	else
		return(0);
}



//-----------------------------------------------------------------------------
//
// -- Finds the address of a Vertex that matches vertex in _vlist.
// Returns NULL if one cannot be found.  Protected.
//
//다시 구현
VertexType *CannyBase::Find_vertex(VertexType &vertex)
{
/*	for(_vlist->ResetCurNode();_vlist->next();)
	{
		EdgeListType a = *(_vlist->value());
		//if( vertex == *(_vlist->value()) )
		//	return(_vlist->value());
	}*/
	// If we get this far there is no match
	return(NULL);

}
