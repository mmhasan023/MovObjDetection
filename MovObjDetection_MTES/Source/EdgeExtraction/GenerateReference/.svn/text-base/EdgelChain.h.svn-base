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
#ifndef _EdgelChain_h
#define _EdgelChain_h

//-----------------------------------------------------------------------------
//
// .NAME EdgelChain - A class for storing edge detection data
// .LIBRARY DigitalGeometry
// .HEADER SpatialObjects package
// .INCLUDE DigitalGeometry/EdgelChain.h
// .FILE EdgelChain.C
//
// .SECTION Description:
//
// Definition of a class for storing edge data as arrays
// of (x,y,z) location, strength, and orientation in degrees. The
// orientation should be stored as the normal to the edge, and
// points in the direction of increasing intensity. It is likely that
// the orientatation information is not meaningful for 3D data (normally
// we might assume that all the z values are zero). The edgel
// arrays should also be ordered along the edgel chains.
//
// .SECTION Author:
//             Charlie Rothwell - 27/1/94
//             GE Corporate Research and Development
//
// .SECTION Modifications :
//             Peter Vanroose - 18/9/95
//             ESAT/ARROWS - K.U.Leuven
//             Added copy constructor, assignment operator, equality test
//             and concatenation (+) operator.
//
//-----------------------------------------------------------------------------

#include <iostream>
#include <math.h>
using namespace std;

//#include <cool/decls.h>

class EdgelChain
{
  // PUBLIC INTERFACE----------------------------------------------------------

public:

  // Constructors/Initializers/Destructors-------------------------------------
  EdgelChain();
  EdgelChain(int);
  EdgelChain(EdgelChain const& );
  virtual ~EdgelChain();

  // Data Access---------------------------------------------------------------
  inline int length()const  { return _nedgels; }

  //2003/07/06  kellt Ãß°¡
  float GetLength(int n) const;

  inline float *GetX()const      { return _x; }
  inline float *GetY()const      { return _y; }
  inline float *GetZ()const      { return _z; }
  inline float *GetGrad()const   { return _grad; }
  inline float *GetTheta()const  { return _theta; }

  inline float GetX(int i)const      { return _x[i]; }
  inline float GetY(int i)const      { return _y[i]; }
  inline float GetZ(int i)const      { return _z[i]; }
  inline float GetGrad(int i)const   { return _grad[i]; }
  inline float GetTheta(int i)const  { return _theta[i]; }

  float MeanGrad();

  // Data Control--------------------------------------------------------------
  inline void SetX(float x, int i)          { _x[i] = x; }
  inline void SetY(float y, int i)          { _y[i] = y; }
  inline void SetZ(float z, int i)          { _z[i] = z; }
  inline void SetX(float* x) { delete[] _x; _x = x; }
  inline void SetY(float* y) { delete[] _y; _y = y; }
  inline void SetZ(float* z) { delete[] _z; _z = z; }
  inline void SetGrad(float grad, int i)    { _grad[i] = grad; }
  inline void SetTheta(float theta, int i)  { _theta[i] = theta; }
  inline void SetLength(int i) { _nedgels = i; }

  // Utility Methods-----------------------------------------------------------
//  void get_min_max(CoolVector<float> &min, CoolVector<float>& max)const;
//  void Print(ostream &strm=cout);
  void Reverse();
  EdgelChain operator=(const EdgelChain& chn);
  bool operator==(const EdgelChain& chn) const;
  EdgelChain operator+(const EdgelChain& chn) const; // concatenation

  // INTERNALS-----------------------------------------------------------------

  // Data Members--------------------------------------------------------------

protected:
  void init(int n_edgels);
  void clear();
  int _nedgels;                      // Number of edgels in the chain
  float *_x,*_y,*_z,*_grad,*_theta;  // (x,y,z), step intensity and orientation
                                     // arrays
};

#endif
// _EdgelChain_h

