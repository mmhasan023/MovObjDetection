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
// Class : Canny
//
// See Canny.h
//
//-----------------------------------------------------------------------------

/*#include <cool/List.h>
#include <cool/ListP.h>

#include <DigitalGeometry/DigitalCurve.h>
#include <Topology/Vertex.h>
#include <Topology/Edge.h>
#include <ImageClasses/RectROI.h>
#include <ImageClasses/Image.h>
#include <Basics/bufferxy.h>*/
#include "Canny.h"
//#include "DigitalCurve.h"


#include <math.h>
#include <stdlib.h>
//#include "EdgeList.h"


const float DUMMYTHETA = 10000.0;

//#define bytePixel(buf,x,y)   (*((byte*)buf->GetElementAddr(x,y)))
//#define shortPixel(buf,x,y)  (*((short*)buf->GetElementAddr(x,y)))
//#define PixelSize(buf)       (buf->GetBitsPixel())

#define bytePixel (_image,x,y)  (*((unsigned char* )_image->GetPixel(x,y)))
#define shortPixel(_image,x,y)  (*((short*)_image->GetPixel(x,y)))
#define PixelSize (_image    )  (          _image->GetBPP  (   ) )



//-----------------------------------------------------------------------------
//
// -- Constructor from a BufferXY.
//
Canny::Canny(KScScalarImage2dUint8  *image , 
			 /*KScScalarImage2dInt32 *dstImg,*/ 
			 float                   sigma , 
			 float                   low   , 
			 float                   high  , 
			 float                   range , 
			 bool                    v     ) : CannyBase(image, sigma, low, high, v)
{
	// Determine the size of the largest convolution kernel
	_range      = (float) 2.0 ;//range;
	_gauss_tail = (float) 0.01;// Canny uses 0.001
	_width      = int(_sigma*sqrt(2*log(1/_gauss_tail))+1); // round up to int
	_w0         = _width;
	_k_size     = 2*_width+ 1;
	_kernel     = Make_float_vector(_k_size);

	// Get the BufferXY so that it goes as close to the edges
	// of the ROI as possible
	_xsize      = image ->GetMainXSize();
	_ysize      = image ->GetMainYSize();
	_xstart     = 0;
	_ystart     = 0;

	//pDstArray   = dstImg->Get2dArray  ();
	//pDstArray   = dstImg->Get2dArray  ();
	pSrcArray   = image ->Get2dArray  ();
	//_image->WriteBytes(image->GetBuffer(), image->GetSize()); //??
	//--------------------------------
	//WriteBtes() --> 영상 복사: 
	//KScScalarImageUint8 *pSrc;
	//버퍼할당
	//KScScalarImageUint8 *pDst;
	//버퍼할당
	//*pDst =  *pSrc; 혹은 pDst->SetFromObject(pSrc);
	//--------------------------------
	//_image = image;
	_image->SetFromObject(image);



	_smooth   = Make_float_image(_xsize,_ysize);
	_dx       = Make_float_image(_xsize,_ysize);
	_dy       = Make_float_image(_xsize,_ysize);
	_grad     = Make_float_image(_xsize,_ysize);

	_thick    = Make_float_image(_xsize,_ysize);
	_thin     = Make_float_image(_xsize,_ysize);

	_theta    = Make_float_image(_xsize,_ysize);

	initFloatImage(_theta, _xsize,_ysize);

	_dangling = Make_int_image(_xsize,_ysize);
	_junction = Make_int_image(_xsize,_ysize);
	_jx       = Make_int_image(_xsize,_ysize);
	_jy       = Make_int_image(_xsize,_ysize);

	_xdang = new ListType<int>;
	_ydang = new ListType<int>;
	_xjunc = new ListType<int>;
	_yjunc = new ListType<int>;

	//_vlist: BaseClass member variable
	_vlist = new ListType<EdgeListType*>;

	_dummy = 1000.0;
	_jval  = 2000.0;

//	SetRefrunce();

	//tmp Buffer
	tmpGdx  = Make_float_image(_xsize,_ysize);
	tmpGdy  = Make_float_image(_xsize,_ysize);
	tmpGrad = Make_float_image(_xsize,_ysize);
	Zero_float_image(tmpGdx);
	Zero_float_image(tmpGdy);
	Zero_float_image(tmpGrad);

}

//-----------------------------------------------------------------------------
//
// -- Destructor.
//
Canny::~Canny()
{
	Free_float_image(_smooth);
	Free_float_image(_dx      );
	Free_float_image(_dy      );
	Free_float_image(_grad    );

	Free_float_image(_thick   );
	Free_float_image(_thin    );
	Free_float_image(_theta   );

	Free_int_image  (_dangling);
	Free_int_image  (_junction);
	Free_int_image  (_jx      );
	Free_int_image  (_jy      );

//	if(m_refImg)Free_int_image  (m_refImg );

	delete _image;

	_vlist->clear();
	delete _vlist;

	delete [] _kernel;
	delete(_xdang);  
	delete(_ydang);
	delete(_xjunc);  
	delete(_yjunc);

	Free_float_image(tmpGdx );
	Free_float_image(tmpGdy );
	Free_float_image(tmpGrad);

}


//-----------------------------------------------------------------------------
//
// -- Method to do Canny on a Canny class that has all the members defined.
// 
//void Canny::Do_canny(CoolListP<Edge*> *edges, int adapt)
void Canny::Do_canny(EdgeListType *edges,int adpt)
{
	InitImages();
	NoiseFiltering();
	ComputeXGrad();
	ComputeYGrad();
	//void getGrad(float** img);함수 사용
	ThinImage();

	EdgeExtraction(edges);

}



//-----------------------------------------------------------------------------
//
// -- Convolves the image with the smoothing kernel.  Protected.
// 
void Canny::Smooth_image()
{

	int x,y,xx,yy,i;
	// Don't care about a border of size _width around the edge
	// of the image. Could do something about is, but I really
	// don't think that it is important. First we smooth in the x 
	// direction and then along y.
	float **tmp = Make_float_image(_xsize,_ysize);
	Zero_float_image(tmp); 
	
// x direction
// 8Bit Image Fixed
//	if( PixelSize(_image) == 8 )

	float temp;
	for( y=0 ; y<_ysize ; y++ )
		for( x=_width ; x<_xsize-_width ; x++ )
			for( i=0,xx=x-_width ; i<_k_size ; i++,xx++ )
				tmp[y][x] += pSrcArray[y][xx]*_kernel[i];

//Commant is 16 Bit Image
//	else if( PixelSize(_image) == 16 )
//		for(y=0;y<_ysize;y++)
//			for(x=_width;x<_xsize-_width;x++)
//				for(i=0,xx=x-_width;i<_k_size;i++,xx++)
//					tmp[y][x] += shortPixel(_image,xx,y)*_kernel[i];
//	else
//		cerr << "Image Depth is more than 16 bits. " << endl;

	// y direction
	for( y=_width ; y<_ysize-_width ; y++ )
		for( x=0 ; x<_xsize ; x++ )
		{
			temp=0;
			for( i=0,yy=y-_width ; i<_k_size ; i++,yy++ )
			{
				temp +=  tmp[yy][x]*_kernel[i] ;
			}
			_smooth[y][x]=temp;
//			pDstArray[y][x]=(temp>255)?255:((temp<0)?0:(unsigned char)temp);
		}

	Free_float_image(tmp);

}



//-----------------------------------------------------------------------------
//
// -- Convolves with the kernel in the x direction, to compute the local
// derivative in that direction.  Protected.
// 
void Canny::Compute_x_gradient()
{
	int x,y;

	int width  = _xsize;
	int height = _ysize;
	for ( y=_width+1 ; y<height-_width-1 ; y++ )
		for ( x=_width+1 ; x<width-_width-1 ; x++ )
			_dx[y][x] =  _smooth[y + 1][x]  -  _smooth[y - 1][x] ;
}

//-----------------------------------------------------------------------------
//
// -- Convolves the original image with the kernel in the y direction to give
// the local y derivative, at the same time we also compute the absolute
// intensity surface gradient, _grad[][].  Protected.
// 
void Canny::Compute_y_gradient()
{
	int x,y;

	// Again, don't care about a border around the image.
	int width  = _xsize;
	int height = _ysize;
	for (y = _width + 1 ; y < height - _width - 1 ; y++)
		for (x = _width + 1 ; x < width  - _width - 1 ; x++)
		{
			// First compute the directional y derivative
			_dy  [y][x] =  _smooth[y][x + 1]  -  _smooth[y][x - 1] ;

			// Get absolute value of the gradient
			_grad[y][x] = float( sqrt(_dx[y][x]*_dx[y][x] + _dy[y][x]*_dy[y][x]) );
//			pDstArray[y][x] = _grad[y][x];
		}
}



//-----------------------------------------------------------------------------
//
// -- Non-maximally supresses the output image by searching along the edge
// normal and checking that the test edge has a greater that the interpolated
// neighbours in the direction. We have also included sub-pixel interpolation
// of the peak position by parabolic fitting.  Writes edges into the _thick
// image.  Protected.
//
void Canny::Non_maximal_supression()
{
    float *g0,*g1,*g2,*dx,*dy;
    float h1,h2;
    float k = float( 180.0/KS_PI );
    int x,y,orient;
    float theta,grad;
   // float fraction,newx,newy;
	
	for(y=_w0+1;y<_ysize-_w0-1;y++)
    // Add 1 to get rid of border effects
	{
		g0 = _grad[y-1];  
		g1 = _grad[y  ];  
		g2 = _grad[y+1];
		dx = _dx  [y  ];      
		dy = _dy  [y  ];
		
	    for(x=_w0+1;x<_xsize-_w0-1;x++)
		{
			// First check that we have an edge
			if( g1[x] > _low )
			{
				theta = float( k*atan2(dy[x],dx[x]) );
				// Now work out which direction wrt the eight-way
				// neighbours the edge normal points
				if( theta >= 0.0 )
					orient = int(theta/45.0);
				else
					orient = int(theta/45.0+4);
				// if theta == 180.0 we will have orient = 4
				orient = orient%4;
				
				// And now compute the interpolated heights
				switch( orient )
				{
				case 0:
					grad = dy[x]/dx[x];
					h1 = grad*g0[x-1] + (1 - grad)*g0[x];
					h2 = grad*g2[x+1] + (1 - grad)*g2[x];
					break;
					
				case 1:
					grad = dx[x]/dy[x];
					h1 = grad*g0[x-1] + (1 - grad)*g1[x-1];
					h2 = grad*g2[x+1] + (1 - grad)*g1[x+1];
					break;
					
				case 2:
					grad = -dx[x]/dy[x];
					h1 = grad*g2[x-1] + (1 - grad)*g1[x-1];
					h2 = grad*g0[x+1] + (1 - grad)*g1[x+1];
					break;
					
				case 3:
					grad = -dy[x]/dx[x];
					h1 = grad*g2[x-1] + (1 - grad)*g2[x];
					h2 = grad*g0[x+1] + (1 - grad)*g0[x];
					break;
					
				default:
					h1 = h2 = 0.0;  // Dummy values
					cerr << "*** ERROR ON SWITCH IN NMS ***\n";
				}
				
				// If the edge is greater than h1 and h2 we are at a peak,
				// therefore do subpixel interpolation by fitting a parabola
				// along the NMS line and finding its peak
				if( (g1[x]>h1) && (g1[x]>h2) )
				{
					/*
					fraction = float( (h1-h2)/(2.0*(h1-2.0*g1[x]+h2)) );
					switch( orient )
					{
					case 0:
						newy = y + fraction;
						newx = x + dy[x]/dx[x]*fraction;
						break;
						
					case 1:
						newy = y + dx[x]/dy[x]*fraction;
						newx = x + fraction;
						break;
						
					case 2:
						newy = y + dx[x]/dy[x]*fraction;
						newx = x + fraction;
						break;
						
					case 3:
						newx = x - fraction;
						newy = y - dy[y]/dx[y]*fraction;
						break;
						
					default:
						newx = newy = 0.0; // Dummy values
						cerr << "*** ERROR ON SWITCH IN NMS ***\n";
					}*/
					
					// Now store the edge data, re-use _dx[][] and _dy[][]
					// for sub-pixel locations (don't worry about the junk
					// that is already in them).
					// + 0.5 is to account for targetjr display offset
					_thick[y][x] = g1[x]; // Should this be interpolated height -- 
					//Following two lines seem useless
				//	dx[x]        = float( newx + 0.5 );   // = g1[y] + frac*(h2-h1)/4 ?
				//	dy[x]        = float( newy + 0.5 );         
					_theta[y][x] = theta;
//					pDstArray[y][x]=_theta[y][x];
				}
			}
		}
	}
}


//-----------------------------------------------------------------------------
//
// -- Hysteresis follows edgel chains that lie above the _low threshold and
// have at least one edgel above the _high threshold. Once we have followed,
// the good edgelchains are re-writted to the _thin image for further
// processing.  Protected.
//
void Canny::Initial_hysteresis()
{

    ListType<int>         xcoords,ycoords;
    ListType<float>       grad           ;
//    CoolListP<EdgelChain*> edges;
	ListType<EdgelChain*> edges          ;
    int    x,y;
    float *thin,*px,*py,*pg;
	EdgelChain *edgels     ;

//	float kPx[200];
//	for(int i=0; i<200; i++)
//		kPx[i] = 0;

    // Find a point above _high and start to follow it.
    // First time round we are just trying to get rid of the weak dangling chains
    // and so we will record the good edges and then re-insert them in the _thin
    // image and follow a second time.
    edges.clear();
	for(y=_w0;y<_ysize - _w0;y++)
    //for(x=_w0;x<_xsize-_w0;x++)
    {
		thin = _thin[y];
		//for(y=_w0;y<_ysize-_w0;y++)
		for(x=_w0;x<_xsize - _w0;x++)
			if( thin[x]>_high )
			{
				Initial_follow(x,y,&xcoords,&ycoords,&grad);

				// Create an EdgeChain and add to the list
				edgels = new EdgelChain(xcoords.GetLength());
				px = edgels->GetX   ();     
				py = edgels->GetY   ();  
				pg = edgels->GetGrad();

				while( xcoords.GetLength() )
				{
				   *(px++) = (float)xcoords.pop();
				   *(py++) = (float)ycoords.pop();
				   *(pg++) =        grad   .pop();
				}

				edges.push(edgels);
			}    
    }

    // Now re-create the _thin image
    int i;
    Zero_float_image(_thin);
    while(edges.GetLength())
	{
		edgels = edges.pop      ();
		px     = edgels->GetX   ();     
		py     = edgels->GetY   ();  
		pg     = edgels->GetGrad();
		for(i=0;i<edgels->length();i++)
		{
			_thin[int(py[i])][int(px[i])] = pg[i];
		}
		delete(edgels);
	}
}



void Canny::Final_hysteresis(EdgeListType *edges)
{
	//edges->EdgeListClear();
	//edges->EdgeExtraction(_thin,_junction,_dangling,_low,_w0);
//	FILE *file = fopen("num.txt","w");
//	int             pixelsize=GetMaxNumOfPixel();
//	for (int i=0; i<pixelsize;i++)
//		fprintf(file,"%d       %d    \n",edges->m_Points[i].x,edges->m_Points[i].y);
//	fclose(file);
}

//-----------------------------------------------------------------------------
//
// -- Method to thin the image using the variation of Tsai-Fu thinning used
// by Van-Duc Nguyen in Geo-Calc. This relies on computing the genus of
// an edge location, and removing it if it is not a dangling chain as has
// genus zero.  We also order the edges by strength and try to remove the 
// weaker ones first. This accounts for non-maximal suppression, and does 
// it in a topology preserving way. Note that we are creating a vcl_list 
// with a large number of elements, and then sorting it - 
// this is likely to be quite slow. An alternative implementation would be better.
//

void Canny::Thin_edges()
{
	int x,y,a,b,c,d,e,f,g,h,genus,count;
	float threshold;
    
	// Now do the thinning. Do it twice: the first time to try to remove
	// _dummy edges, and then other edges -- 0.001 turns <= to <
	float increment = (365.0-_low)/3.0;
	for(threshold=_low+increment/2;threshold <=365.0; threshold=threshold + increment)
	{
		count = 1;     // count set to dummy value
		while( count ) // Thin until no Pixels are removed
		{
			count = 0;
			for( x=_w0 ; x<_xsize-_w0 ; x++ )
			{
				for( y=_w0 ; y<_ysize-_w0 ; y++ )
				{
					if( _thin[y][x] >= threshold || _thin[y][x] <= _low )
					//if( _thin[y][x] <= threshold )
						continue;
					
					if( _thin[y-1][x-1] > _low )  a = 1; else a = 0;
					if( _thin[y-1][x  ] > _low )  b = 1; else b = 0;
					if( _thin[y-1][x+1] > _low )  c = 1; else c = 0;
					if( _thin[y  ][x+1] > _low )  d = 1; else d = 0;
					if( _thin[y+1][x+1] > _low )  e = 1; else e = 0;
					if( _thin[y+1][x  ] > _low )  f = 1; else f = 0;
					if( _thin[y+1][x-1] > _low )  g = 1; else g = 0;
					if( _thin[y  ][x-1] > _low )  h = 1; else h = 0;
					
					genus = a+b+c+d+e+f+g+h;
					
					// If the pixel is dangling, record, and go to next loop 
					if( genus == 1 )
						continue;
					
					genus += h*a*b+b*c*d+d*e*f+f*g*h-a*b-b*c-c*d-d*e-e*f-f*g
						- g*h-h*a-h*b-b*d-d*f-f*h-1;
					
					// If the genus is zero delete the edge
					if( genus == 0 )
					{
						count++;
						_thin[y][x] = 0.0;
					}
				}
			}
		}
    }
//	for(y=0;y<_ysize;y++)
//		for(x=0;x<_xsize;x++)
//			pDstArray[y][x]=(unsigned char)_thin[y][x];//,sizeof(unsigned char)*_xsize*_ysize);
}


/*
void Canny::Thin_edges()
{
	int x,y,a,b,c,d,e,f,g,h,genus,count;
	float threshold;
    
	// Now do the thinning. Do it twice: the first time to try to remove
	// _dummy edges, and then other edges -- 0.001 turns <= to <
	float increment = (365.0-_low)/10.0;
	for(threshold=_low+increment/2;threshold <=365.0; threshold=threshold + increment)
    {
		//count = 1;     // count set to dummy value
		//while( count ) // Thin until no Pixels are removed
		//{
		//	count = 0;
			for( x=_w0 ; x<_xsize-_w0 ; x++ )
			{
				for( y=_w0 ; y<_ysize-_w0 ; y++ )
				{
					if( _thin[y][x] >= threshold || _thin[y][x] <= _low )
						continue;
					
					if( _thin[y-1][x-1] > _low )  a = 1; else a = 0;
					if( _thin[y-1][x  ] > _low )  b = 1; else b = 0;
					if( _thin[y-1][x+1] > _low )  c = 1; else c = 0;
					if( _thin[y  ][x+1] > _low )  d = 1; else d = 0;
					if( _thin[y+1][x+1] > _low )  e = 1; else e = 0;
					if( _thin[y+1][x  ] > _low )  f = 1; else f = 0;
					if( _thin[y+1][x-1] > _low )  g = 1; else g = 0;
					if( _thin[y  ][x-1] > _low )  h = 1; else h = 0;
					
					genus = a+b+c+d+e+f+g+h;
					
					// If the pixel is dangling, record, and go to next loop 
					if( genus == 1 )
						continue;
					
					genus += h*a*b+b*c*d+d*e*f+f*g*h-a*b-b*c-c*d-d*e-e*f-f*g
						- g*h-h*a-h*b-b*d-d*f-f*h-1;
					
					// If the genus is zero delete the edge
					if( genus == 0 )
					{
		//				count++;
						_thin[y][x] = 0.0;
					}
				}
			}
	//	}
    }
//	for(y=0;y<_ysize;y++)
//		for(x=0;x<_xsize;x++)
//			pDstArray[y][x]=(unsigned char)_thin[y][x];//,sizeof(unsigned char)*_xsize*_ysize);
}
*/
//-----------------------------------------------------------------------------
//
// -- Searches for single pixel breaks near dangling ends and patches them
// up. This is destructive on the dangling lists.  Protected.
//
/*
void Canny::Jump_single_breaks()
{
	// Take each dangling end in turn and determine whether there is
	// another edgel within the sixteen one-pixel-distant-neighbours.
	
	int x,y,i,j;
	float **t = _thin; 
	// _xdang->ResetCurNode();  _ydang->ResetCurNode();
	
	while(_xdang->GetLength())
    {
		x = _xdang->pop();
		y = _ydang->pop();
		
		// This is messy for efficiency
		if     (_thin[y-1][x-1]) {i=-1;  j=-1;}
		else if(_thin[y-1][x  ]) {i= 0;  j=-1;}
		else if(_thin[y-1][x+1]) {i= 1;  j=-1;}
		else if(_thin[y  ][x+1]) {i= 1;  j= 0;}
		else if(_thin[y+1][x+1]) {i= 1;  j= 1;}
		else if(_thin[y+1][x  ]) {i= 0;  j= 1;}
		else if(_thin[y+1][x-1]) {i=-1;  j= 1;}
		else if(_thin[y  ][x-1]) {i=-1;  j= 0;}
		else { i = j = 0; } // Dummy declaration
		
		// If the entrant edgel is diagonal
		if( i*j != 0 )
		{
			if      ( t[y-2*j][x-2*i]>_low)
			{
				t     [y-j][x-i]=_dummy;
				_thick[y-j][x-i]=_dummy;
			}
			else if(( t[y-2*j][x+i  ]>_low)||(t[y-2*j][x    ]>_low)||(t[y-2*j][x-i  ]>_low))
			{
				t     [y-j][x  ]=_dummy;
				_thick[y-j ][x  ]=_dummy;
			}
			else if(( t[y+j  ][x-2*i]>_low)||(t[y    ][x-2*i]>_low)||(t[y-j  ][x-2*i]>_low))
			{
				t     [y  ][x-i]=_dummy;
				_thick[y  ][x-i]=_dummy;
			}
			else if((!(t[y  ][x+2*i]>_low))&&((t[y-j][x+2*i]>_low)||(t[y-2*j][x+2*i]>_low)))
			{
				t     [y-j][x+i]=_dummy;
				_thick[y-j][x+i]=_dummy;
			}
			else if((!(t[y+2*j][x  ]>_low))&&((t[y+2*j][x-i]>_low)||(t[y+2*j][x-2*i]>_low)))
			{
				t     [y+j][x-i]=_dummy;
				_thick[y+j][x-i]=_dummy;
			}
		}
		
		// For entrant with i=0
		else if( i == 0 )
		{
			if((t[y-2*j][x-1]>_low)||(t[y-2*j][x]>_low)||(t[y-2*j][x+1]>_low))
			{
				t     [y-j][x  ]=_dummy;
				_thick[y-j][x  ]=_dummy;
			}
			else if((t[y-j][x+2]>_low)||(t[y-2*j][x+2]>_low))
			{
				t     [y-j][x+1]=_dummy;
				_thick[y-j][x+1]=_dummy;
			}
			else if((t[y-j][x-2]>_low)||(t[y-2*j][x-2]>_low))
			{
				t     [y-j][x-1]=_dummy;
				_thick[y-j][x-1]=_dummy;
			}
			else if((!(t[y+j][x-2]>_low))&&(t[y  ][x-2]>_low))
			{
				t     [y  ][x-1]=_dummy;
				_thick[y  ][x-1]=_dummy;
			}
			else if((!(t[y+j][x+2]>_low))&&(t[y  ][x+2]>_low))
			{ 
				t     [y  ][x+1]=_dummy;
				_thick[y  ][x+1]=_dummy;
			}
		}
		// or finally for j=0
		
		// *** Bug fix, Samer Abdallah 5/10/95:  next line was
		// else if( i == 0 )
		
		else if( j == 0 )
		{
			if((t[y-1][x-2*i]>_low)||(t[y][x-2*i]>_low)||(t[y][x-2*i]>_low))
			{
				t     [y  ][x-i]=_dummy;
				_thick[y  ][x-i]=_dummy;
			}
			else if((t[y+2][x-i]>_low)||(t[y+2][x-2*i]>_low))
			{
				t     [y+j][x-1]=_dummy;
				_thick[y+j][x-1]=_dummy;
			}
			else if((t[y-2][x-i]>_low)||(t[y-2][x-2*i]>_low))
			{
				t     [y-j][x-1]=_dummy;
				_thick[y-j][x-1]=_dummy;
			}
			else if((!(t[y-2][x+i]>_low))&&(t[y-2][x]>_low))
			{
				t     [y-1][x  ]=_dummy;
				_thick[y-1][x  ]=_dummy;
			}
			else if((!(t[y+2][x+i]>_low))&&(t[y+2][x]>_low))
			{
				t     [y+1][x  ]=_dummy;
				_thick[y+1][x  ]=_dummy;
			}
		}
    }
}

*/

void Canny::Jump_single_breaks()
{
	// Take each dangling end in turn and determine whether there is
	// another edgel within the sixteen one-pixel-distant-neighbours.
	
	int x,y;
//	float **t = _thin; 
	// _xdang->ResetCurNode();  _ydang->ResetCurNode();
	
	while(_xdang->GetLength())
    {
		x = _xdang->pop();
		y = _ydang->pop();
		
		// This is messy for efficiency
		if     (_dangling[y-2][x-2]) 
		{
			_thin[y-1][x-1] =_dummy;
			_thick[y-1][x-1] =_dummy;
			_dangling[y-2][x-2] = 0;
			_dangling[y][x] = 0;
		}
		else if     (_dangling[y-2][x-1]) 
		{
			_thin[y-1][x-1] =_dummy;
			_thick[y-1][x-1] =_dummy;
			_dangling[y-2][x-1] = 0;
			_dangling[y][x] = 0;
		}
		else if     (_dangling[y-2][x]) 
		{
			_thin[y-1][x] =_dummy;
			_thick[y-1][x] =_dummy;
			_dangling[y-2][x] = 0;
			_dangling[y][x] = 0;
		}

		else if     (_dangling[y-2][x+1]) 
		{
			_thin[y-1][x+1] =_dummy;
			_thick[y-1][x+1] =_dummy;
			_dangling[y-2][x+1] = 0;
			_dangling[y][x] = 0;
		}
		else if     (_dangling[y-2][x+2]) 
		{
			_thin[y-1][x+1] =_dummy;
			_thick[y-1][x+1] =_dummy;
			_dangling[y-2][x+2] = 0;
			_dangling[y][x] = 0;
		}
		else if     (_dangling[y-1][x-2]) 
		{
			_thin[y-1][x-1] =_dummy;
			_thick[y-1][x-1] =_dummy;
			_dangling[y-1][x-2] = 0;
			_dangling[y][x] = 0;
		}
		else if     (_dangling[y-1][x+2]) 
		{
			_thin[y-1][x+1] =_dummy;
			_thick[y-1][x+1] =_dummy;
			_dangling[y-1][x+2] = 0;
			_dangling[y][x] = 0;
		}
		else if     (_dangling[y][x-2]) 
		{
			_thin[y][x-1] =_dummy;
			_thick[y][x-1] =_dummy;
			_dangling[y][x-2] = 0;
			_dangling[y][x] = 0;
		}
		else if     (_dangling[y][x+2]) 
		{
			_thin[y][x+1] =_dummy;
			_thick[y][x+1] =_dummy;
			_dangling[y][x+2] = 0;
			_dangling[y][x] = 0;
		}
		else if     (_dangling[y+1][x-2]) 
		{
			_thin[y+1][x-1] =_dummy;
			_thick[y+1][x-1] =_dummy;
			_dangling[y+1][x-2] = 0;
			_dangling[y][x] = 0;
		}
		else if     (_dangling[y+1][x+2]) 
		{
			_thin[y+1][x+1] =_dummy;
			_thick[y+1][x+1] =_dummy;
			_dangling[y+1][x+2] = 0;
			_dangling[y][x] = 0;
		}
		else if     (_dangling[y+2][x-2]) 
		{
			_thin[y+1][x-1] =_dummy;
			_thick[y+1][x-1] =_dummy;
			_dangling[y+2][x-2] = 0;
			_dangling[y][x] = 0;
		}
		else if     (_dangling[y+2][x-1]) 
		{
			_thin[y+1][x-1] =_dummy;
			_thick[y+1][x-1] =_dummy;
			_dangling[y+2][x-1] = 0;
			_dangling[y][x] = 0;
		}
		else if     (_dangling[y+2][x]) 
		{
			_thin[y+1][x] =_dummy;
			_thick[y+1][x] =_dummy;
			_dangling[y+2][x] = 0;
			_dangling[y][x] = 0;
		}
		else if     (_dangling[y+2][x+1]) 
		{
			_thin[y+1][x+1] =_dummy;
			_thick[y+1][x+1] =_dummy;
			_dangling[y+2][x+1] = 0;
			_dangling[y][x] = 0;
		}
		else if     (_dangling[y+2][x+2]) 
		{
			_thin[y+1][x+1] =_dummy;
			_thick[y+1][x+1] =_dummy;
			_dangling[y+2][x+2] = 0;
			_dangling[y][x] = 0;
		}
	}
}



//-----------------------------------------------------------------------------
//
// -- Searches for high contrast changes in the vicinity(주변) of dangling ends. This
// is done by halving(이등분하다) the previously used Canny sigma to reduce the effects of
// smoothing near corners. Ultimately the kenel size will be scaled down until
// its radius of influence is only two pixels; at that stage pixel-jumping should
// fix any problems.  Protected.
//
void Canny::Adaptive_Canny()
{
	// Reset the smoothing kernel parameters by
	// halfing the size of the smoothing sigma
	_old_sigma  = _sigma;  
	_sigma     /=  2.0  ;  
	_old_width  = _width;
	_width      = int(_sigma*sqrt(2*log(1/_gauss_tail))+1);
	_old_k_size = _k_size;  
	_k_size     = 2*_width+ 1;
	delete(_kernel);        
	_kernel     = Make_float_vector(_k_size);
	Set_kernel();
	
	// Define the new ROI to account for the domain of the old smoothing
	// kernel and for that of the new one
	int image_size = _old_k_size + _k_size - 1;
	int  half_size = (image_size - 1)/2;
	
	if(verbose) cerr << "new image region " 
		<< image_size << " by " << image_size << endl
		<< "Sigma           = " << _sigma << endl
		<< "Kernel size     = " << _k_size << endl;
	
	// Set up the new images
	float **  dx = Make_float_image(image_size,image_size);
	float **  dy = Make_float_image(image_size,image_size);
	float **grad = Make_float_image(image_size,image_size);
	
	// For each dangling-end (X,Y), search for more edges at the reduced scale
	int i,X,Y,x0,y0,count=0;
	if(verbose) cerr << "percentage of endings examined =   0";
	for(_xdang->ResetCurNode(),_ydang->ResetCurNode();_xdang->next(),_ydang->next();)
    {
		X  = _xdang->value();  
		Y  = _ydang->value();
		x0 =  X - half_size ;     
		y0 =  Y - half_size ;  // Region origin in image coords
		
		// Make sure that the region around the end is within the
		// bounds of the original image, and that the dangling end
		// is not a neighbour to a junction (if it is we recover the
		// topology more robustly by growing something else towards
		// it, rather than the other way round)
		if( !Junction_neighbour(X,Y) && (x0>=0) && (x0+image_size<=_xsize) &&
			(y0>=0) && (y0+image_size<=_ysize) )
		{
			// Compute the new image intensity gradients
			Compute_adaptive_images(x0,y0,image_size,dx,dy,grad);
			
			// Delete the effects of the _thick image - we don't want to
			// locate edges in the same places as before
			Subtract_thick(x0,y0,image_size,grad);
			
			// Now, if possible, grow the edgelchain out from (X,Y) in a
			// one dimensional direction up to the boundary of the old kernel
			int newx,newy;
			X -= x0;  
			Y -= y0;   // Do a coordinate shift; centre in local coords
			for(i=1;(i<=_old_k_size) && Dangling_end(X+x0,Y+y0);i++)
			{
				// Find the eight-way neighbour with the strongest edge
				newx = X;  
				newy = Y;
				Best_eight_way(X,Y,grad,&newx,&newy);
				// If no new edge has been found we should break
				if ((newx==X) && (newy==Y) )
					break;
				// Else record a dummy edgel
				else 
				{
					_thin [newy+y0][newx+x0] = _dummy;  
					_thick[newy+y0][newx+x0] = _dummy;
					X = newx;  
					Y = newy;
				}
			}
		}
		if(verbose) fprintf(stderr,"\b\b\b%3d", 10*((++count)*10/_xdang->GetLength()));
    }
	if(verbose)   cerr << endl;
	
	// Remove the image arrays
	Free_float_image(dx  );
	Free_float_image(dy  );
	Free_float_image(grad);
}



//-----------------------------------------------------------------------------
//
// -- Computes the gradient images with the origin at (x0,y0) and of square
// size image_size.  Protected.
//
void Canny::Compute_adaptive_images(int x0, int y0, int image_size,
									float **dx, float **dy, float **grad)
{
	// Zero the derivative images
	int i,j;
	for(j=0;j<image_size;j++)
		for(i=0;i<image_size;i++)
		{
			dx  [i][j] = 0.0; 
			dy  [i][j] = 0.0;  
			grad[i][j] = 0.0;
		}
		
		// Do the different convolutions - we checked that we would not
		// go beyond the image bounds before calling Compute_adaptive_images
		int x,y;
//		if( PixelSize(_image) == 8 )
//		{
			// Compute the image gradients in the x direction

		//8bit Image fixed
		for(y=_width;y<image_size-_width;y++)
			for(x=_width;x<image_size-_width;x++)
				for(i=0,j=x-_width;i<_k_size;i++,j++)
////					dx[y][x] += bytePixel(_image,j+x0,y+y0)*_kernel[i];
					
					// and in the y direction with grad
		for(x=_width;x<image_size-_width;x++)
			for(y=_width;y<image_size-_width;y++)
				for(i=0,j=y-_width;i<_k_size;i++,j++)
				{
////					dx  [y][x] += bytePixel(_image,x+x0,j+y0)*_kernel[i];
					grad[y][x] = float( sqrt(dx[y][x]*dx[y][x] + dy[y][x]*dy[y][x]) );
				}
//		}
/*		else if( PixelSize(_image) == 16 )
		{
			// Compute the image gradients in the x direction
			for(y=_width;y<image_size-_width;y++)
				for(x=_width;x<image_size-_width;x++)
					for(i=0,j=x-_width;i<_k_size;i++,j++)
						dx[x][y] += shortPixel(_image,j+x0,y+y0)*_kernel[i];
					
			// and in the y direction with grad
			for(x=_width;x<image_size-_width;x++)
				for(y=_width;y<image_size-_width;y++)
					for(i=0,j=y-_width;i<_k_size;i++,j++)
					{
						dx[x][y] += shortPixel(_image,x+x0,j+y0)*_kernel[i];
						grad[x][y] = float(sqrt(dx[x][y]*dx[x][y] + dy[x][y]*dy[x][y]));
					}
		}

		else
			cerr << "Image Depth is more than 16 bits. " << endl;
*/
}



//-----------------------------------------------------------------------------
//
// -- Wherever the _thick image has an edge marked set the new gradient value
// to zero.  Protected.
//
void Canny::Subtract_thick(int x0, int y0, int image_size, float **grad)
{
	int x,y;
	for(y=_width;y<image_size-_width;y++)
		for(x=_width;x<image_size-_width;x++)
			if( _thick[y+y0][x+x0] > _low )
				grad[y][x] = 0.0;
}



//-----------------------------------------------------------------------------
//
// -- Returns the eight-way neighbour with the strongest contrast change.
// Protected.
//
void Canny::Best_eight_way(int x, int y, float **grad, int *xnew, int *ynew)
{
	float max = _low;
	
	if( grad[y-1][x-1] > max ) {*xnew = x-1;  *ynew = y-1;}
	if( grad[y-1][x  ] > max ) {*xnew = x  ;  *ynew = y-1;}
	if( grad[y-1][x+1] > max ) {*xnew = x+1;  *ynew = y-1;}
	if( grad[y  ][x+1] > max ) {*xnew = x+1;  *ynew = y  ;}
	if( grad[y+1][x+1] > max ) {*xnew = x+1;  *ynew = y+1;}
	if( grad[y+1][x  ] > max ) {*xnew = x  ;  *ynew = y+1;}
	if( grad[y+1][x-1] > max ) {*xnew = x-1;  *ynew = y+1;}
	if( grad[y  ][x-1] > max ) {*xnew = x-1;  *ynew = y  ;}
	
	// Zero all of those places tested
	int i,j;
	for(j=y-1;j<=y+1;j++)
		for(i=x-1;i<=x+1;i++)
			grad[j][i] = 0.0;
}



//-----------------------------------------------------------------------------
//
//  -- Searches for the junctions in the image.  Protected.
//
void Canny::Find_dangling_ends()
{
	int x,y,a,b,c,d,e,f,g,h;
	
	// Reset the dangling ends 
	_xdang->clear();  
	_ydang->clear();  
	Zero_int_image(_dangling);
	
	for(y=_w0;y<_ysize-_w0;y++)
		for(x=_w0;x<_xsize-_w0;x++)
		{
			if( _thin[y][x] <= _low )
				continue;
			
			if( _thin[y-1][x-1] > _low )  a = 1; else a = 0;
			if( _thin[y-1][x  ] > _low )  b = 1; else b = 0;
			if( _thin[y-1][x+1] > _low )  c = 1; else c = 0;
			if( _thin[y  ][x+1] > _low )  d = 1; else d = 0;
			if( _thin[y+1][x+1] > _low )  e = 1; else e = 0;
			if( _thin[y+1][x  ] > _low )  f = 1; else f = 0;
			if( _thin[y+1][x-1] > _low )  g = 1; else g = 0;
			if( _thin[y  ][x-1] > _low )  h = 1; else h = 0;
			
			if( (a+b+c+d+e+f+g+h) == 1 )
			{
				_xdang->push(x);  
				_ydang->push(y);
				_dangling[y][x] = 1;
//				pDstArray[y][x]=1;
			}
		}
}



//-----------------------------------------------------------------------------
//
//  -- Tests whether a points is a dangling end.  Protected.
//
int Canny::Dangling_end(int x, int y)
{
	int a,b,c,d,e,f,g,h;
	
	if( _thin[y][x] <= _low )
		return(0);
	
	if( _thin[y-1][x-1] > _low )  a = 1; else a = 0;
	if( _thin[y-1][x  ] > _low )  b = 1; else b = 0;
	if( _thin[y-1][x+1] > _low )  c = 1; else c = 0;
	if( _thin[y  ][x+1] > _low )  d = 1; else d = 0;
	if( _thin[y+1][x+1] > _low )  e = 1; else e = 0;
	if( _thin[y+1][x  ] > _low )  f = 1; else f = 0;
	if( _thin[y+1][x-1] > _low )  g = 1; else g = 0;
	if( _thin[y  ][x-1] > _low )  h = 1; else h = 0;
	
	if( (a+b+c+d+e+f+g+h) == 1 )
		return(1);
	else
		return(0);
}


//-----------------------------------------------------------------------------
//
//  -- Searches for the junctions in the image.  Protected.
//
void Canny::Find_junctions()
{
	int x,y,a,b,c,d,e,f,g,h;
	
	// Reset the junction variables
	_xjunc->clear();     
	_yjunc->clear();
	Zero_int_image(_junction);
	
	register int i = _xsize;
	register int j = _ysize;
	for( y=_w0 ; y<j-_w0 ; y++ )
		for( x=_w0 ; x< i -_w0 ; x++ )
		{
			if( _thin[y][x] <= _low )
				continue;
			
			if( _thin[y-1][x-1] > _low ) 
				a = 1; 
			else a = 0;
			
			if( _thin[y-1][x  ] > _low )  
				b = 1; 
			else b = 0;
			
			if( _thin[y-1][x+1] > _low )  
				c = 1; 
			else c = 0;
			
			if( _thin[y  ][x+1] > _low )  
				d = 1; 
			else d = 0;
			
			if( _thin[y+1][x+1] > _low )  
				e = 1; 
			else e = 0;
			
			if( _thin[y+1][x  ] > _low )  
				f = 1; 
			else f = 0;
			
			if( _thin[y+1][x-1] > _low )  
				g = 1; 
			else g = 0;
			
			if( _thin[y  ][x-1] > _low )  
				h = 1; 
			else h = 0;
			
			if( (a+b+c+d+e+f+g+h) > 2)
			{
				_xjunc->push(x);  
				_yjunc->push(y);
				_junction[y][x] = 1;
//				pDstArray[y][x]=1;
			}
		}

/*		FILE *junkFile;
		junkFile = fopen("c:\\junkFile.txt", "w");
		int **  temp = Make_int_image(i,j);
		Zero_int_image(temp);
		float dist;
		float max;
		int k, flag;
		int pos[2000][2];
		int index;
		float xDif, yDif;
		for( y=_w0 ; y<j-_w0 ; y++ )
			for( x=_w0 ; x< i -_w0 ; x++ )
			{	
				index =0;
				flag = 1;
				while(( flag)	){
					pos[index][0] = x;
					pos[index][1] = y;
					temp[y][x] = 1;
					flag = 0;
					max = _low;
					if( _thin[y-1][x-1] > max && _junction[y-1][x-1] ==0 && temp[y-1][x-1]==0)
						{x = x-1;  y = y-1; flag = 1;}
					if( _thin[y-1][x  ] > max && _junction[y-1][x  ] ==0 && temp[y-1][x]==0)
						{x = x  ;  y = y-1; flag = 1;}
					if( _thin[y-1][x+1] > max && _junction[y-1][x+1] ==0 && temp[y-1][x+1]==0)
						{x = x+1;  y = y-1; flag = 1;}
					if( _thin[y  ][x+1] > max && _junction[y  ][x+1] ==0 && temp[y  ][x+1]==0)
						{x = x+1;  y = y; flag = 1;}
					if( _thin[y+1][x+1] > max && _junction[y+1][x+1] ==0 && temp[y+1][x+1]==0)
						{x = x+1;  y = y+1; flag = 1;}
					if( _thin[y+1][x  ] > max && _junction[y+1][x  ] ==0 && temp[y+1][x  ]==0)
						{x = x  ;  y = y+1; flag = 1;}
					if( _thin[y+1][x-1] > max && _junction[y+1][x-1] ==0 && temp[y+1][x-1]==0)
						{x = x-1;  y = y+1; flag = 1;}
					if( _thin[y  ][x-1] > max && _junction[y  ][x-1] ==0 && temp[y  ][x-1]==0)
						{x = x-1;  y = y; flag = 1;}
														
					index++;
				}
			//	if(index>20)
			//		fprintf(junkFile,"index: %d \n",index);
				k = 0;
				while ( k+30< index){
					xDif = pos[k][0] - pos[k+30][0];
					yDif = pos[k][1] - pos[k+30][1];
					dist = sqrt(xDif*xDif + yDif*yDif);
					fprintf(junkFile,"index: %d, distance: %f  %d, %d,   %d, %d\n",index, dist, pos[k][0], pos[k][1], pos[k+30][0], pos[k+30][1]);
					if(dist< 29.5){
						_xjunc->push(pos[k+15][0]);  
						_yjunc->push(pos[k+15][1]);
						_junction[pos[k+15][1]][pos[k+15][0]] = 1;
						fprintf(junkFile,"\nx: %d y: %d\n\n",pos[k+15][0], pos[k+15][1]);
						k = k +15;
					}
					else 
						k = k + 5;
					
				}
			}
		fclose(junkFile);
		Free_int_image(temp);
*/
}



//-----------------------------------------------------------------------------
//
// -- Locate junction clusters using the following method of hysteresis.
// Protected.
//
void Canny::Find_junction_clusters()
{
	ListType<int> xcoords,ycoords,xvertices,yvertices,xjunc,yjunc;
	int x,y,x0,y0;
	
	// Find a junction and follow
	xvertices.clear();  
	yvertices.clear();
	xjunc    .clear();      
	yjunc    .clear();
	for( y=_w0 ; y<_ysize-_w0 ; y++ )
	{
		for( x=_w0 ; x<_xsize-_w0 ; x++ )
			if( _junction[y][x] )
			{
				// Each cluster is written to (xcoords,ycooords)
				xcoords.clear();  
				ycoords.clear();
				Follow_junctions(x,y,&xcoords,&ycoords);
				
				// Find the `centre' of the cluster. This is defined as the
				// junction closest to the centre of gravity of the cluser
				Cluster_centre_of_gravity(xcoords,ycoords,x0,y0);
				
				// Add both the junctions and the new cluster centre to
				// the main lists            
				xvertices.push(x0);       
				yvertices.push(y0);
				xjunc.prepend(xcoords);   
				yjunc.prepend(ycoords);
			}
	}
	

	// Reset the junction image - this is order dependent because
	// the cluster centres appear in both lists
	// xjunc.ResetCurNode();  yjunc.ResetCurNode();
	while( xjunc.GetLength() )
	{
		_junction[yjunc.pop()][xjunc.pop()] = 1;
	}
	
	// Construct the list of junction cluster centres
	_vlist->clear();
	int xdata, ydata;
	//for(xvertices.ResetCurNode(),yvertices.ResetCurNode();xvertices.next(),yvertices.next();)
	
	xvertices.ResetCurNode(); 
	yvertices.ResetCurNode();

	//수정시작 2003 07 16 1044 안기옥
	//무한 루프에 빠지는 문제 수정
	while(xvertices.GetLength())
	{
		xdata = xvertices.pop();
		ydata = yvertices.pop();
		int i=0;
		//                           x                             y                  z	
		//Vertex *v = new Vertex(xvertices.value()+_xstart,yvertices.value()+_ystart,0.0);
//		EdgeListType *eV = new EdgeListType();
//		eV->verArr[i  ].pt.x = xdata;
//		eV->verArr[i++].pt.y = ydata;
//		_vlist->push(eV);
		_junction[ydata][xdata] = 2;
//		pDstArray[ydata][xdata]=2;
	}
	//수정 끝 2003 07 16 1044 안기옥
	xvertices.clear();  
	yvertices.clear();
}

/*
int  Canny::GetMaxNumOfPixel(void)
{
	int pixel=0,junction=0;
	for(register int y=_w0,x ; y<_ysize-_w0 ; y++ )
		for( x=_w0 ; x<_xsize-_w0 ; x++ )
		{
			if( _thin[y][x]!=0 )
				pixel++;
			if( _junction[y][x]!=0 )
				junction++;
		}
	return pixel+junction*4;//중복 가능한 최대 정션까지 예측
}

*/

void Canny::setGdx(float** img)
{
	register int i,j;
	for( i=0 ; i<_ysize ; i++ )
		for( j=0 ; j<_xsize ; j++ )
		{
			tmpGdx[i][j]=img[i][j];
		}
}

void Canny::setGdy(float** img)
{
	register int i,j;
	for( i=0 ; i<_ysize ; i++ )
		for( j=0 ; j<_xsize ; j++ )
		{
			tmpGdy[i][j]=img[i][j];
		}
}

void Canny::setGrad(float** img)
{
	register int i,j;
	for( i=0 ; i<_ysize ; i++ )
		for( j=0 ; j<_xsize ; j++ )
		{
			_grad[i][j]=img[i][j];
		}
}

void Canny::getGdx(float** img)
{
	register int i,j;
	for( i=0 ; i<_ysize ; i++ )
		for( j=0 ; j<_xsize ; j++ )
		{
			img[i][j]=+tmpGdx[i][j];
		}
}

void Canny::getGdy(float** img)
{
	register int i,j;
	for( i=0 ; i<_ysize ; i++ )
		for( j=0 ; j<_xsize ; j++ )
		{
			img[i][j]=+tmpGdy[i][j];
		}
}

/*
void Canny::getGrad(float** img)
{
	register int i,j;
	for( i=0 ; i<_ysize ; i++ )
		for( j=0 ; j<_xsize ; j++ )
		{
			img[i][j]=+tmpGrad[i][j];
		}
}
*/
void Canny::getGrad(float** img)
{
	register int i,j;
	for( i=0 ; i<_ysize ; i++ )
		for( j=0 ; j<_xsize ; j++ )
		{
			img[i][j]=_grad[i][j];
		}
}
/*Blocked by Julius
void   Canny::UpdateImage(float** img1, float** img2)
{
	register int i,j;
	for( i=0 ; i<_ysize ; i++ )
		for( j=0 ; j<_xsize ; j++ )
		{
			img1[i][j] = (img2[i][j]+img1[i][j])/2.0;
		}
}
*/
//Do_Canny()를 세분화한 함수.
//시작
void Canny::InitImages()
{
	// Do the traditional Canny parts
	Set_kernel();		
	//Sets up the convolution kernel based on the first derivative of a Gaussian.  Protected.
	
	Zero_float_image(_smooth);
	Zero_float_image(_dx    );    
	Zero_float_image(_dy    ); 
	Zero_float_image(_grad  );  
	Zero_float_image(_thick );
	Zero_float_image(_thin  );  
}

void Canny::NoiseFiltering()
{
	Smooth_image();
}

void Canny::ComputeXGrad()
{
	Compute_x_gradient();
}

void Canny::ComputeYGrad()
{
	Compute_y_gradient();
}

void Canny::ThinImage()
{
	ComputeThreshold();
	Non_maximal_supression();

	Copy_image(_thick,_thin);
	Thin_edges();//_thin Buffer 사용
	Initial_hysteresis();		

	Find_dangling_ends();
	Jump_single_breaks();
	Thin_edges();
//	Find_dangling_ends();
	Find_junctions();
	Find_junction_clusters();
}

/* Original Order
	ComputeThreshold();
	Non_maximal_supression();
	Copy_image(_thick,_thin);
	Thin_edges();//_thin Buffer 사용
	Initial_hysteresis();		
	Jump_single_breaks();
	Thin_edges();
	Find_dangling_ends();
	Find_junctions();
	Find_junction_clusters();
*/
void Canny::EdgeColorShape(EdgeListType *edges)
{



//i will complete it later Murshed


//	EdgeSegmentType *tmpEdge;
//	tmpEdge = new EdgeSegmentType;
//	PointType p;
//	list<EdgeSegmentType>::iterator curEdge;
//	for(curEdge=edges->ListBegin();curEdge!=edges->ListEnd();curEdge++)
//	{
//		*tmpEdge = (*curEdge);
//		int size = (*curEdge).GetSize();
//		for(int i=0 ; i<size ; i++)
//		{
//			p=(*curEdge).GetPoint(i);
//			pOutImage[p.GetY()][p.GetX()] = KSdUint8((*curEdge).GetGrad(i));
			//pOutImage[p.GetY()][p.GetX()] = 255;

//		}
//	}
	/////////////////////*/
	
}



void Canny::EdgeExtraction(EdgeListType *edges)
{
	edges   ->EdgeListClear();
//	edges   ->EdgeExtraction(_thin,_junction,_dangling,_low,_w0);
	
	list<PointType> *points;
	points = new list<PointType>;
	Two_Stack<PointType> *s;
	s = new Two_Stack<PointType>;
	s->clear();
	int tmpAry[8],size;
	memset(tmpAry,-1,sizeof(int)*8);

//	int pointSize=GetMaxNumOfPixel(_thin,_junction,_w0);
//	CreatePoints(pointSize);
	
	int pixelsize =0;
	register int y,x;

	//Vertex 정보 저장
	//수정필요..
	//-Label을 조사하여 SetPoints()함수 과정에 넣는다..
	for(y=_w0,x ; y<_ysize-_w0 ; y++)
	{
		for(x=_w0 ; x<_xsize-_w0 ; x++)
		{
			if(_junction[y][x]==2)
			{
				tmpAry[0]=_thin[y  ][x+1];// 0: EE : [y  ][x+1]
				tmpAry[1]=_thin[y-1][x+1];// 1: NE : [y-1][x+1]
				tmpAry[2]=_thin[y-1][x  ];// 2: NN : [y-1][x  ]
				tmpAry[3]=_thin[y-1][x-1];// 3: NW : [y-1][x-1]
				tmpAry[4]=_thin[y  ][x-1];// 4: WW : [y  ][x-1]
				tmpAry[5]=_thin[y+1][x-1];// 5: SW : [y+1][x-1]
				tmpAry[6]=_thin[y+1][x  ];// 6: SS : [y+1][x  ]
				tmpAry[7]=_thin[y+1][x+1];// 7: SE : [y+1][x+1]
				//AddNewVertex(x,y,tmpAry);
			}
		}
	}

	//Edge Segment 저장
	//Ceeatepoint
	PointType p;
	//-label 설정필요.
	int label=-1;
	for(y=_w0 ; y<_ysize-_w0 ; y++) 
		for(x=_w0 ; x<_xsize-_w0; x++)
		{
			if(_thin[y][x]>0)
			{
				p.SetX(x);p.SetY(y);
				points->push_back(p);
				//thin[y][x]=0.0;
				_thin[y][x]=label;
				s->clear();
				//int x = p.GetX();
				//int y = p.GetY();
				if(_thin[y  ][x+1]){p.SetX(x+1);p.SetY(y)  ;s->push(p);_thin[p.GetY()][p.GetX()]=label;}// 0: EE : [y  ][x+1]
				if(_thin[y-1][x+1]){p.SetX(x+1);p.SetY(y-1);s->push(p);_thin[p.GetY()][p.GetX()]=label;}// 1: NE : [y-1][x+1]
				if(_thin[y-1][x  ]){p.SetX(x)  ;p.SetY(y-1);s->push(p);_thin[p.GetY()][p.GetX()]=label;}// 2: NN : [y-1][x  ]
				if(_thin[y-1][x-1]){p.SetX(x-1);p.SetY(y-1);s->push(p);_thin[p.GetY()][p.GetX()]=label;}// 3: NW : [y-1][x-1]
				if(_thin[y  ][x-1]){p.SetX(x-1);p.SetY(y)  ;s->push(p);_thin[p.GetY()][p.GetX()]=label;}// 4: WW : [y  ][x-1]
				if(_thin[y+1][x-1]){p.SetX(x-1);p.SetY(y+1);s->push(p);_thin[p.GetY()][p.GetX()]=label;}// 5: SW : [y+1][x-1]
				if(_thin[y+1][x  ]){p.SetX(x)  ;p.SetY(y+1);s->push(p);_thin[p.GetY()][p.GetX()]=label;}// 6: SS : [y+1][x  ]
				if(_thin[y+1][x+1]){p.SetX(x+1);p.SetY(y+1);s->push(p);_thin[p.GetY()][p.GetX()]=label;}// 7: SE : [y+1][x+1]
				//if(s->GetCur()==0)
				//	continue;
				while(s->GetCur())
				{
					p=*(s->pop());
					if     (s->GetCur()==1)
						Final_follow(_thin,_junction,_dangling,p,points,_low,label,1);
					else if(s->GetCur()==0)
						Final_follow(_thin,_junction,_dangling,p,points,_low,label,0);
				}
			}
			size=points->size();
			if(size>8)
			{
				edges->AddNewSegment(size,points, -(label), _thick, _theta);
				label--;
			}
			points->clear();
		}
	
	delete s;
	delete points;
}

void Canny::Final_follow(  float**          thin   ,
								  int**          junction,
								  int**            dangling,
								  PointType        p     , 
								  list<PointType> *points,
								  int              low,
								  int              label,
								  int              start )
{

	int x=p.GetX(),y=p.GetY();
	if(thin[y][x]>0) p.SetValue(thin[y][x]);
	else             p.SetValue(-thin[y][x]);

	// Make sure that we are not likely to overun the border of the image
	if( (p.GetX()<=1) || (p.GetX()>=_xsize-1) || (p.GetY()<=1) || (p.GetY()>=_ysize-1) )
		return;
	
	// Add the current point to the coordinate lists, and delete from
	// the edge image
	if      (start==1)
		points->push_back(p);
	else if (start==0)
		points->push_front(p);
	
	//thin[y][x] = 0.0;
	thin[y][x] = label;
	
	if ((junction[y][x]||dangling[y][x]))
		return;
	
	// Now recursively look for connected eight-neighbours
	if( thin[y  ][x+1] > low ){
		p.SetX(x+1);p.SetY(y)  ;
		Final_follow(thin,junction,dangling,p,points,low,label,start);
	}
	if( thin[y-1][x+1] > low ){
		p.SetX(x+1);p.SetY(y-1);
		Final_follow(thin,junction,dangling,p,points,low,label,start);
	}
	if( thin[y-1][x  ] > low ){
		p.SetX(x)  ;p.SetY(y-1);
		Final_follow(thin,junction,dangling,p,points,low,label,start);
	}
	if( thin[y-1][x-1] > low ){
		p.SetX(x-1);p.SetY(y-1);
		Final_follow(thin,junction,dangling,p,points,low,label,start);
	}
	if( thin[y  ][x-1] > low ){
		p.SetX(x-1);p.SetY(y) ;
		Final_follow(thin,junction,dangling,p,points,low,label,start);
	}
	if( thin[y+1][x-1] > low ){
		p.SetX(x-1);p.SetY(y+1);
		Final_follow(thin,junction,dangling,p,points,low,label,start);
	}
	if( thin[y+1][x  ] > low ){
		p.SetX(x)  ;p.SetY(y+1);
		Final_follow(thin,junction,dangling,p,points,low,label,start);
	}
	if( thin[y+1][x+1] > low ){
		p.SetX(x+1);p.SetY(y+1);
		Final_follow(thin,junction,dangling,p,points,low,label,start);
	}

}

void Canny::ComputeThreshold()
{
	int histo[256];
	int sum  [256]; 
	register int i,j;
	float low_scale  = _ysize*_xsize*(_low );
	float high_scale = _ysize*_xsize*(_high);

	for( i=0 ; i<256 ; i++ ) histo[i]=sum[i]=0;
	for( i=0 ; i<_ysize ; i++ )
		for( j=0 ; j<_xsize ; j++ )
			histo[unsigned char(_grad[i][j])]++;
	sum[0]=histo[0];
	for( i=1 ; i<256 ; i++ ) 
	{
		sum[i]=histo[i]+sum[i-1];
		if(sum[i]<= low_scale) _low =i;
		if(sum[i]>=high_scale) {
			_high=i;
			break;
		}
	}
	_dummy = _low + 0.001;
}


