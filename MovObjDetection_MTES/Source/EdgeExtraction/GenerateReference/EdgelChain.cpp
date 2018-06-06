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
//-----------------------------------------------------------------------------
//
// Class : EdgelChain
//
// Modifications : see EdgeChain.h
//
//-----------------------------------------------------------------------------

//#include <DigitalGeometry/EdgelChain.h>
#include "EdgelChain.h"
//#include <cool/Vector.h>
#include <memory.h>     // for memset()

//--------------------------------------------------------------
//
// Constructor and Destructor Functions
//

// -- Default constructor
EdgelChain::EdgelChain()
{
  this->init(1);//just not to cause trouble
}
// -- Constructor for n length chain.

	EdgelChain::EdgelChain(int n)
	{
	  this->init(n);
	}

// -- Copy constructor

EdgelChain::EdgelChain(EdgelChain const& chn)
{
    int n = chn._nedgels; int i;
    _nedgels = n;
    _x = new float[n]; for (i=0;i<n;i++) _x[i]=chn._x[i];
    _y = new float[n]; for (i=0;i<n;i++) _y[i]=chn._y[i];
    _z = new float[n]; for (i=0;i<n;i++) _z[i]=chn._z[i];
    _grad=new float[n]; for (i=0;i<n;i++) _grad[i]=chn._grad[i];
    _theta=new float[n];for (i=0;i<n;i++)_theta[i]=chn._theta[i];

}


//This is a kludge to permit redefining the size of the edgel chain 
//since some digital curves may not know how many edgels are really needed at
//construction time.
void EdgelChain::clear()
{
  delete []_x;
  delete []_y;
  delete []_z;
  delete []_grad;
  delete []_theta;
}
// -- Destructor deletes all arrays associated with this chain
EdgelChain::~EdgelChain()
{
  this->clear();
}

void EdgelChain::init(int n_edgels)
{
    _nedgels = n_edgels;
    _x = new float [n_edgels];
    _y = new float [n_edgels];
    _z = new float [n_edgels];
    _grad =new float [n_edgels];
    _theta = new float [n_edgels];
    
    // initialize values to 0.0
    int size = n_edgels * sizeof(float);
    memset(_x, 0, size);
    memset(_y, 0, size);
    memset(_z, 0, size);
    memset(_grad, 0, size);
    memset(_theta, 0, size);
}  
//--------------------------------------------------------------
//
// -- Return the mean edgel strength.
//
float EdgelChain::MeanGrad()
{
    float sum=0.0;
    for(int i=0;i<_nedgels;i++) sum += _grad[i];
    return(sum/_nedgels);
}

/*void EdgelChain::get_min_max(CoolVector<float>& min, CoolVector<float>& max)const
{
    float *endx=_x+_nedgels;
    float *x=_x,*y=_y,*z=_z;
    float minx=*x, maxx=*x++, miny=*y, maxy=*y++, minz=*z, maxz=*z++;

    while (x < endx)
    {
        if (*x < minx) minx = *x;
        else if (*x > maxx) maxx = *x;
        if (*y < miny) miny = *y;
        else if (*y > maxy) maxy = *y;
        if (*z < minz) minz = *z;
        else if (*z > maxz) maxz = *z;
        x++; y++; z++;
    }
    min[0] = minx; min[1] = miny; min[2] = minz;
    max[0] = maxx; max[1] = maxy; max[2] = maxz;
}*/

//--------------------------------------------------------------
//
// -- Prints an EdgelChain on the stdout
//
/*
void EdgelChain::Print(ostream& strm)
{
    strm << "EdgelChain:\n";
    for(int i=0;i<_nedgels;i++)
        strm << _x[i] << " " << _y[i] << " " << _z[i] << endl;
    strm << endl;
}
*/
//--------------------------------------------------------------
//
// -- Reverses the order of the data in the EdgelChain.
//
void EdgelChain::Reverse()
{
    int down,up;
    float tmp;

    for(down=0,up=_nedgels-1;down<up;down++,up--)
    {
        tmp = _x[down];      _x[down] = _x[up];         _x[up] = tmp;
        tmp = _y[down];      _y[down] = _y[up];         _y[up] = tmp;
        tmp = _z[down];      _z[down] = _z[up];         _z[up] = tmp;
        tmp = _grad[down];   _grad[down] = _grad[up];   _grad[up] = tmp;
        tmp = _theta[down];  _theta[down] = _theta[up]; _theta[up] = tmp;
    }
}

//--------------------------------------------------------------
//
// -- EdgelChain assignment, with appropriate allocation
//
EdgelChain EdgelChain::operator=(const EdgelChain& chn)
{
    int n = chn._nedgels; int i;
    _nedgels = n;
    delete[]_x; _x = new float[n]; for (i=0;i<n;i++) _x[i]=chn._x[i];
    delete[]_y; _y = new float[n]; for (i=0;i<n;i++) _y[i]=chn._y[i];
    delete[]_z; _z = new float[n]; for (i=0;i<n;i++) _z[i]=chn._z[i];
    delete[]_grad; _grad=new float[n]; for (i=0;i<n;i++) _grad[i]=chn._grad[i];
    delete[]_theta;_theta=new float[n];for (i=0;i<n;i++)_theta[i]=chn._theta[i];
    return *this;
}

//--------------------------------------------------------------
//
// -- Test if two EdgelChains are completely identical
//
bool EdgelChain::operator==(const EdgelChain& chn) const
{
    if (this == &chn) return true; // identical memory position
    int n = _nedgels; int i;
    if (chn._nedgels != n) return false;
    for (i=0;i<n;i++) if (_x[i] != chn._x[i]) return false;
    for (i=0;i<n;i++) if (_y[i] != chn._y[i]) return false;
    for (i=0;i<n;i++) if (_grad[i] != chn._grad[i]) return false;
    for (i=0;i<n;i++) if (_theta[i] != chn._theta[i]) return false;
    for (i=0;i<n;i++) if (_z[i] != chn._z[i]) return false; // most likely equal
    return true;
}

//--------------------------------------------------------------
//
// -- Concatenate two EdgelChains
//
EdgelChain EdgelChain::operator+(const EdgelChain& chn) const
{
    int i, n=_nedgels, m=chn._nedgels;
    EdgelChain c(n+m);
    for (i=0;i<n;i++) c._x[i] = _x[i];
    for (i=0;i<n;i++) c._y[i] = _y[i];
    for (i=0;i<n;i++) c._z[i] = _z[i];
    for (i=0;i<n;i++) c._grad[i] = _grad[i];
    for (i=0;i<n;i++) c._theta[i] = _theta[i];
    for (i=0;i<m;i++) c._x[n+i] = chn._x[i];
    for (i=0;i<m;i++) c._y[n+i] = chn._y[i];
    for (i=0;i<m;i++) c._z[n+i] = chn._z[i];
    for (i=0;i<m;i++) c._grad[n+i] = chn._grad[i];
    for (i=0;i<m;i++) c._theta[n+i] = chn._theta[i];
    return c;
}


// -- Length of the curve
// 
float EdgelChain::GetLength(int count) const
{
	EdgelChain *ec;
	ec = new EdgelChain(count);
	int n = ec->length(); // number of pixels in the chain
//	float len = LengthOfInterval(0, n-1);
	int start_index = 0;
	int end_index = n-1;
	float prev_x = ec->GetX(start_index);
	float prev_y = ec->GetY(start_index);
	float prev_z = ec->GetZ(start_index);
	float len = 0.0;
	for(int i = start_index +1 ; i<=end_index; i++)
	{
	  float curr_x = ec->GetX(i);
	  float curr_y = ec->GetY(i);
	  float curr_z = ec->GetZ(i);
	  float delx = curr_x - prev_x;
	  float dely = curr_y - prev_y;
	  float delz = curr_z - prev_z;
	  len += float(sqrt(delx*delx + dely*dely + delz*delz));
	  prev_x = curr_x;
	  prev_y = curr_y;
	  prev_z = curr_z;
	}

	return len;
}
