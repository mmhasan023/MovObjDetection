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
//-*- c++ -*-------------------------------------------------------------------
#ifndef _CannyBase_h
#define _CannyBase_h
//
// .NAME CannyBase - a base class for edge detectors
// .LIBRARY Detection
// .HEADER Segmentation package
// .INCLUDE Detection/CannyBase.h
// .FILE CannyBase.h
// .FILE CannyBase.C
//
// .SECTION Description:
//
// Definition of a Base class for doing Canny edge detection. 
// Note that nothing special has been done around the border of the image;
// we have simply ignored a border of size _width all the way round. Perhaps
// this should be changed to provide consistency with the rest of TargetJr.
//
// .SECTION Author:
//       Samer Abdallah - 5/10/95
//       Robotics Research Group, Oxford University
//         CannyBase is built from Canny  which was originally developed by
//         Charlie Rothwell - 25/1/92
//         GE Corporate Research and Development
// .SECTION Modifications :
//       Samer Abdallah (samer@robots.ox.ac.uk) - 18/05/96
//        Cleared some compiler warnings
//
//-----------------------------------------------------------------------------
//#include <cool/decls.h>

/*#undef Image
class Image;
class BufferXY;
class Edge;
class Vertex;
class BufferXY;*/
#include "ListType.h"
#include "KSClass.h" // KSClass들에 대한 헤더

class EdgeListType;
class VertexType;

class CannyBase
{
	// PUBLIC INTERFACE----------------------------------------------------------
    
public:
	
	// Constructors/Initializers/Destructors-------------------------------------
	
	CannyBase(KScScalarImage2dUint8 *sourceImage   , 
		float                  sigma         , 
		float                  low           , 
		float                  high          , 
		bool                   verbose = true);
	~CannyBase();
	
	// Data Access---------------------------------------------------------------
	
	inline int GetStartX() { return _xstart; }
	inline int GetStartY() { return _ystart; }
	inline int GetWidth()  { return _xsize ; }
	inline int GetHeight() { return _ysize ; }
	
	// INTERNALS-----------------------------------------------------------------
	
protected:
    
	float **Make_float_image  (int,int                                       );
	void    initFloatImage    (float**, int, int                             );
	void    Free_float_image  (float**                                       );
	int   **Make_int_image    (int,int                                       );
	void    Free_int_image    (int**                                         );
	float  *Make_float_vector (int                                           );
	void    Set_kernel        (void                                          );
	void    Zero_float_image  (float**                                       );
	void    Zero_int_image    (int**                                         );
	void    Copy_image        (float**,float**                               );
	void    Copy_image        (int  **,int  **                               );	
	void    Copy_image        (int **image1, float **image2)	;
	void    Initial_follow    (int,int,ListType<int>*,ListType<int>*,ListType<float>*    );
	void    Final_follow      (int,int,ListType<int>*,ListType<int>*,ListType<float>*,int);
	void    Follow_junctions  (int,int,ListType<int>*,ListType<int>*                 );
	void    Cluster_centre_of_gravity(ListType<int>&,ListType<int>&,int&,int&        );
	int     Junction_neighbour(int,int                                       );
	//  Vertex *Find_vertex(Vertex&);
	VertexType *Find_vertex(VertexType&);
	
	// Data Members--------------------------------------------------------------
	
protected:
	
	//  BufferXY *_image;    // The original intensity image
	KScScalarImage2dUint8 *_image;
	int _xstart,_ystart; // The origin of the buffer in the image
	int _xsize ,_ysize ;   // The width of the image buffer
	
	KScScalarImage2dUint8 *_dstImage;
	KSdInt32** pDstArray; 
	KSdUint8** pSrcArray;

protected:
	int _dstXsize, _dstYsize;
	
	float **_smooth;     // Smoothed intensity image
	float **_dx    ;     // Derivatives in x, and sub-pixel x coordinates
	float **_dy    ;     // Derivatives in y, and sub-pixel y coordinates
	int      gx, gy;
	float **_grad;       // Gradient image
	
	// Quite a few of the following could be done using hash tables
	float **_thick;      // Gradient image after NMS
	float **_thin ;      // Gradient image after NMS and thinning
	float **_theta;      // Orientation image
	
	
	int   **_junction;   // Binary image true only at junctions ends, and relevant lists
	int   **_jx,**_jy;   // Images of (x,y) coordinates of nearest cluster centre
	int **_dangling;     // Binary image true only at dangling ends, and relevant lists
	
	
	ListType<int> *_xjunc,*_yjunc;
	ListType<int> *m_xjunc,*m_yjunc; //임시
	ListType<EdgeListType*> *_vlist;
	
	float  _gauss_tail;  // The value of the kernel at its tail
	float  _sigma     ;  // Smoothing sigma
	int    _width     ;  // The smoothing kernel width - can change
	int    _w0        ;  // Same as above, but does not change
	int    _k_size    ;  // The kernel is 2*_width+1s
	float *_kernel    ;  // 1-Dimensional convolution kernel of size k_size
	float  _low       ;  // Low threshold for hysteresis
	float  _high      ;  // High threshold for hysteresis
	//float *_low;
	//float *_high;
	
	float  _jval      ;  // A dummy junction intensity step value
	int    _chain_no  ;  // A dummy variable used in following
	bool    verbose   ;
	
};

#endif
// _CannyBase_h
