#include "KSClass.h" 
#include"EdgeSegmentType.h"
#include "EdgeListType.h"
#include"EdgeClusterList.h"
#include<math.h>
#include<stdio.h>
#include"Curvature.h"


//int edgeSegmentMatch(EdgeClusterType *curCluster, EdgeSegmentType edge, EdgeListType *TG, int **pMovDistance, int **pMovLabel, int dx, int dy, float *score, float *movX, float *movY, int *segI);
int edgeSegmentMatch(KSdUint8 ** pOutputImage,EdgeClusterType *curCluster, EdgeSegmentType edge, EdgeListType *TG, int **pMovDistance, int **pMovLabel, int dx, int dy, float *score, float *movX, float *movY, int *segI);

//float AngleBasedMatch(EdgeSegmentType *edge, EdgeSegmentType *canEdge, int *distX, int *distY, float *angle);
float AngleBasedMatch(KSdUint8 ** pOutputImage, EdgeSegmentType *edge, EdgeSegmentType *canEdge, int *distX, int *distY, float *angle);
int edgeMatch(EdgeSegmentType edge, EdgeListType *EdgeList, int **pMovDistance, int **pMovLabel, int dx, int dy, int *segI, int &n);
