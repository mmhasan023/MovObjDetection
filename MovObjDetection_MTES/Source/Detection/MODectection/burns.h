
#if !defined(BURNS_LINE_DECTACTION_INCLUDED_)
#define BURNS_LINE_DECTACTION_INCLUDED_


#include <stdio.h>
#include <malloc.h>
#include <math.h>


///////////////////////////////////////////////////////////////////////////////
//
// Define Structure And Constant
//

#define PI360     6.283185
#define PI180     3.141592
#define PI90      1.570796
#define PI45      0.785398
#define PI135     2.356194
#define MAG_BCKGRND    255
#define PHASE_BCKGRND  361
#define SPHASE_BCKGRND 255
#define COLOR_BCKGRND  0
#define v_srch_d(t1, t2)  for(k = 2, x = j + 2 * t1, y = i + 2 * t2; hashtable[pPhase[y][x]]==lbl \
			    && pMag[y][x] == base; k++,x += (t1), y += (t2) ); \
			     if(pMag[y][x] > base) for(x -= (t1), y -= (t2); k > 0; k--, \
			     pPhase[y][x] = 361, x -= (t1), y -= (t2))
#define WINDOWSIZE  1 // number of lines not processed for using two dimension edge operator
#define actan2(ax, ay, t) (((t=atan2(ay, ax)) >= 0.0) ? t : (PI360 + t))


///////////////////////////////////////////////////////////////////////////////
//
// Type Definition
//

typedef struct _mmstr_type {
    double m00, m10, m01, m11, m20, m02;
} mmstr_type;

typedef struct _mstr_type {
    int length;
    int xstrt, ystrt, xend;
    int grdntang;
} mstr_type;

typedef struct _vmstr_type {
    int length;
    unsigned int area, votes;
    int nwcolor;
    int grdntang;
} vmstr_type;

typedef struct _master_type { // length,xs,ys,xe,ye,grad
	int    tag; //?
	int    length;
    int    xstart, xend, ystart, yend;
    int    grdntang;   // 8 direction grad angle. + for y=ax+b
    double ornt;       // angle in radient 
} master_type;

typedef struct _line_list_type {
    int numoflines;
    master_type *master_line;
//    char description[40];
} line_list_type;

#endif