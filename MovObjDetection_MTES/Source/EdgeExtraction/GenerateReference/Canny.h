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
#ifndef _Canny_h
#define _Canny_h
//
//-- kellt: 다음 싸이트 참고 ------------------------------------------------------
//http://foto.hut.fi/research/TargetJr/Manpages/Segmentation/Detection/classes.html
//---------------------------------------------------------------------------------

// Class : Canny
//
// .SECTION Description:
//
// A class for performing traditional Canny edge detection. The basic
// implementatation is as close to that described in Canny's thesis
// as possible, though sub-pixel interpolation of the final edge 
// output has been included, as has edge thinning. Note that there are
// probably differences between this implementation and the documentation
// (that is his Master's thesis).
//
// The improved implementation includes the patching of single pixel
// gaps in the Edgel chains, as well as improved search for edgels near
// to edgelchain terminations (dangling ends). At these places the scale
// of the smoothing convolution kernel is reduced dynamically to remove 
// interference from corners (if they exist) near to the ends. Overall,
// this yeilds a much better understanding of the image topology.
//
// Note that nothing special has been done around the border of the image;
// we have simply ignored a border of size _width all the way round. Perhaps
// this should be changed to provide consistency with the rest of TargetJr.
//
// .NAME Canny - a class for strict Canny edge detection
// .LIBRARY Detection
// .HEADER Segmentation package
// .INCLUDE Detection/Canny.h
// .FILE Canny.h
// .FILE Canny.C
// .EXAMPLE ../Examples/canny.C
//
// .SECTION Author:
//       Charlie Rothwell - 25/1/92
//       GE Corporate Research and Development
//
// .SECTION Modifications :
//       Samer Abdallah - 5/10/95
//       Robotics Research Group, Oxford University
//         Most members and methods placed in new base class CannyBase.
//         Fixed bugs in Final_hysteresis() and Jump_single_pixels().
//
//-----------------------------------------------------------------------------


#include "CannyBase.h"
#include "EdgeListType.h"
#include "PointType.h"
#include "EdgelChain.h"
#include "ListType.h"
//#include "WeightGenType.h"
#include <math.h>


//#include <cool/decls.h>

/*#undef Image
class Image;
class BufferXY;
class Edge;
class Vertex;
class BufferXY;*/

/*//-----------------------------------------------------------------------------------
// Construct a dialog builder to get the Canny parameters
  static float sigma = 1.0;  // Standard deviation of the smoothing kernel
  static float low = 3.0;    // Low hysteresis threshold
  static float high = 12.0;  // High hysteresis threshold
  static float range = 2.0;  // Domain of influence of the smallest smoothing kernel
  static int clr_disp = 1;   // Clear the current segmentation
  static int display = 2;    // Toggle to display edgels/edges/chains/nothing
  static int adapt = 0;      // Use adaptive Canny
  static int dispv = 1;      // Display the vertices
//-----------------------------------------------------------------------------------*/

class Canny : public CannyBase
{
  // PUBLIC INTERFACE----------------------------------------------------------
  
public:
  
	// Constructors/Initializers/Destructors-------------------------------------

	// -- Constructor from an image.
	// ROI
	//Canny(Image*,float sigma, float low, float high, float range, bool verbose = true);

	// -- Constructor from a BufferXY.
	//Canny(KScScalarImage2dUint8*, KScScalarImage2dUint8 *,float sigma, float low, float high, float range, bool verbose = true);
	Canny(KScScalarImage2dUint8* ,float sigma, float low, float high, float range, bool verbose = true);
	~Canny();

	// Utility Methods-----------------------------------------------------------
	// -- Method to do Canny on a Canny class that has all the members defined.
	void Do_canny(EdgeListType *,int);
	
public:
	//Do_Canny()를 세분화한 함수.
	void InitImages();
	void NoiseFiltering();
	void ComputeXGrad();
	void ComputeYGrad();
	//void getGrad(float** img);함수 사용
	void ThinImage();
	void EdgeExtraction(EdgeListType *);
	void EdgeColorShape(EdgeListType *);



public:
	/////////////////////////////////////////////////////////////////////////
	//edge magnitude accumulation 후 thinning 하는 방법응 위한 함수및 변수
	/////////////////////////////////////////////////////////////////////////
	int  m_WeightUpMode;
	//if 1 : edge magnitude image update
	//if 2 : weight image thinning
	//if 3 : edge extraction 
protected:
//	void SetRefrunce();
public:
	void setRef(int** img);
	void setGdx(float** img);
	void setGdy(float** img);
	void setGrad(float** img);

	void getRef(int** img);
	void getGdx(float** img);
	void getGdy(float** img);
	void getGrad(float** img);
public:
	void SetSrcImg(unsigned char **img)
	{
		for(int i=0,j ; i<_ysize ; i++ )
			for( j=0 ; j<_xsize ; j++ )
				pSrcArray[i][j]=img[i][j];
	};

	void SetSrcImg(KScScalarImage2dUint8 *image)
	{
		
		pSrcArray   = image ->Get2dArray  ();
		_image->SetFromObject(image);
	};

  int m_MaxInt;
	void NormalizedImg(float**);

	float** getThin(){return _thin;};
	int** getJunction(){return _junction;};
	int** getDangling(){return _dangling;};

//	int **m_refImg;

	float **tmpGdx;
	float **tmpGdy;
	float **tmpGrad;
//	void   UpdateImage(float** img1, float** img2);

	////////////////////////////////////////////////////////////////////////////

/*
	////////////////////////////////////////////////////////////////////////////
	//Change Detection Method
	////////////////////////////////////////////////////////////////////////////
protected:	
	WeightGenType *m_weightImg;
public:
	void SetWeightGenType(WeightGenType *weight){m_weightImg=weight};
	
	int  m_WeightUpMode;
	//if 1 : edge magnitude image update
	//if 2 : weight image thinning
	//if 3 : edge extraction 
	////////////////////////////////////////////////////////////////////////////
*/


	// INTERNALS-----------------------------------------------------------------
  
	//ListType<EdgeListType*>* GetEdgeList();
protected:
    
	void Smooth_image();// -- Convolves the image with the smoothing kernel.  Protected.
	
	// -- Convolves with the kernel in the x direction, to compute the local derivative in that direction.  Protected.
	void Compute_x_gradient();
	
	// -- Convolves the original image with the kernel in the y direction to give
	// the local y derivative, at the same time we also compute the absolute
	// intensity surface gradient, _grad[][].  Protected.
	void Compute_y_gradient();

	// -- Non-maximally supresses the output image by searching along the edge
	// normal and checking that the test edge has a greater that the interpolated
	// neighbours in the direction. We have also included sub-pixel interpolation
	// of the peak position by parabolic fitting.  Writes edges into the _thick
	// image.  Protected.
	void Non_maximal_supression();

	// -- Hysteresis follows edgel chains that lie above the _low threshold and
	// have at least one edgel above the _high threshold. Once we have followed,
	// the good edgelchains are re-writted to the _thin image for further
	// processing.  Protected.
	void Initial_hysteresis();

	// -- Hysteresis follows edgel chains that lie above the _low threshold and
	// have at least one edgel above the _high threshold. Due to the Initial_hysteresis
	// phase, all edges greater than _low will be by default good and so have a member
	// greater than _high.  Protected.
	void Final_hysteresis(EdgeListType*);


	void Jump_gap(int,int,int,int,int*,int*);

	// -- Method to thin the image using the variation of Tsai-Fu thinning used
	// by Van-Duc Nguyen in Geo-Calc. This relies on computing the genus of
	// an edge location, and removing it if it is not a dangling chain as has
	// genus zero.  Protected.
	void Thin_edges();

	// -- Searches for single pixel breaks near dangling ends and patches them
	// up. This is destructive on the dangling lists.  Protected.
	void Jump_single_breaks();
	
	// -- Searches for high contrast changes in the vicinity of dangling ends. This
	// is done by halving the previously used Canny sigma to reduce the effects of
	// smoothing near corners. Ultimately the kenel size will be scaled down until
	// its radius of influence is only two pixels; at that stage pixel-jumping should
	// fix any problems.  Protected.
	void Adaptive_Canny();

	// -- Computes the gradient images with the origin at (x0,y0) and of square
	// size image_size.  Protected.
	void Compute_adaptive_images(int,int,int,float**,float**,float**);

	// -- Wherever the _thick image has an edge marked set the new gradient value
	// to zero.  Protected.
	void Subtract_thick(int,int,int,float**);

	// -- Returns the eight-way neighbour with the strongest contrast change.
	void Best_eight_way(int,int,float**,int*,int*);

	//  -- Searches for the junctions in the image.  Protected.
	void Find_dangling_ends();

	//  -- Tests whether a points is a dangling end.  Protected.
	int Dangling_end(int,int);

	//  -- Searches for the junctions in the image.  Protected.
	void Find_junctions();

	// -- Locate junction clusters using the following method of hysteresis.
	void Find_junction_clusters();

	int  GetMaxNumOfPixel(void);

	//입력되는 비율에 따라 Threshold값을 구한다.
	//_low,_high를 초기에는 비율로 입력받은후 이함수를 통해 실제 threshold값으로 
	//계산한다.
	void ComputeThreshold();

	void Final_follow(  float**          thin   ,
								  int**          junction,
								  int**            dangling,
								  PointType        p     , 
								  list<PointType> *points,
								  int              low,
								  int              label,
								  int              start );
 
	// Data Members--------------------------------------------------------------

protected:
    
//	int **_dangling;     // Binary image true only at dangling ends, and relevant lists
	ListType<int> *_xdang,*_ydang;
	float _range;       // The maximal region of effect of the smallest smoothing kernel

	// Parameters for the adaptive smoothing
	float _old_sigma;   // Smoothing sigma
	int _old_width;     // The smoothing kernel width
	int _old_k_size;    // The kernel is 2*_width+1s

	float _dummy;       // A dummy intensity step value 
    
//	EdgeListType edge;
	ListType<EdgeListType*> *m_edges;
	int _count;
};

#endif





