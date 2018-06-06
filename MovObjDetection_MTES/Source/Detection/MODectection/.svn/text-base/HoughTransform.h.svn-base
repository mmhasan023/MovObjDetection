//#include "EdgeClusterList.h"
//#include "EdgeListType.h"
#include"EdgeSegmentMatch.h"
#include"Kalman.h"

#include <math.h>

int findHoughClusterMatch(KSdUint8 ** pOutputImage, EdgeClusterType *curCluster, EdgeListType *EdgeList, int **pMovDistance, int **pMovLabel, int Sx, int Sy, int *posX, int *posY);
//void updateClusterMemberSegments(KSdUint8 ** pOutputImage, EdgeClusterType *curCluster, EdgeListType *EdgeList, int **pMovDistance, int **pMovLabel, int Sx, int Sy, int *posX, int *posY);
//void findHoughClusterMatch(EdgeClusterType *curCluster, EdgeListType *EdgeList, int **pMovDistance, int **pMovLabel, int Sx, int Sy, int *posX, int *posY);
void updateClusterMemberSegments(KSdUint8 ** pOutputImage, EdgeClusterList *clusterList, EdgeClusterType *curCluster, EdgeListType *EdgeList, int **pMovDistance, int **pMovLabel, int Sx, int Sy, int posX, int posY, int flag);
void assignClusterIDtoMovingSegments(KSdUint8 ** pOutputImage,EdgeListType * movEdgeList, EdgeListType * MatchedEdgeList, int **pMovDistance, int **pMovLabel, int dx, int dy);
int findSelectedHoughClusterMatch(KSdUint8 ** pOutputImage, EdgeClusterType *curCluster, EdgeListType *EdgeList, int **pMovDistance, int **pMovLabel, int Sx, int Sy, int *posX, int *posY);
void updateSelectedClusterMemberSegments(KSdUint8 ** pOutputImage, EdgeClusterList *clusterList, EdgeClusterType *curCluster, EdgeListType *EdgeList, int **pMovDistance, int **pMovLabel, int Sx, int Sy, int posX, int posY, int found);
int findHoughEdgeMotion(KSdUint8 ** pOutputImage, EdgeSegmentType *refEdgeSeg, EdgeListType *curEdgeList, KScScalarImage2dInt32 *refDistImage, KScScalarImage2dInt32 *refLabelImage, int &movX, int &movY, int Sx, int Sy);
void drawArrow(KSdUint8 **pOutputImage,int Xc,int Yc,int mx,int my,int dx,int dy);
void drawrec(KSdUint8 **pOutputImage, int minx, int miny, int maxx, int maxy, int color);