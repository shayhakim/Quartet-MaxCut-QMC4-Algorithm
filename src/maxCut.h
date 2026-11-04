#include <stdlib.h> 
#include <stdio.h> 
#include <stdint.h>
#include <time.h> 
#include <math.h> 

#include "servMath.h"

int* paraSearch(int nNodes, float* goodEdges, float* badEdges, int bPrint);

int* paraSearch3nNodes(float* goodEdges, float* badEdges);

int* paraSearchCutExactlyOne(int nNodes, float* goodEdges, float* badEdges, float* alphaBest);

int* paraSearchCutExactlyPair(int nNodes, float* goodEdges, float* badEdges, float* alphaBest);

int* paraSearchCutExactlyOneOrPair(int nNodes, float* goodEdges, float* badEdges, float *alphaBest, int *nNodesCut, int bPrefere1);

void compute_xEdges(int nNodes, float* goodEdges, float* badEdges, float* xEdges, float alpha);

float alphaMaxPossible_get(int nNodes, float* goodEdges);
float alphaObserved_get(int nNodes, float* goodEdges, float* badEdges, int* part);

int* maxCut_forSingleRandomStart_Run(int nNodes, float* xEdges, int* nNodesInMinOfTwoParts, int bPrint, float *scorePart_ForCut);

int* maxCut_forSingleRandomStart(int nNodes, float* xEdges, int *nNodesInMinOfTwoParts, int bPrint, float *scorePart_ForCut);

float scorePart(int nNodes, float* xEdges, int* part);	
float scorePart_Node(int nNodes, float* xEdges, int* part, int idNode);

void FiducciaMattheyses(int* part, float* xEdges, int nNodes, int bPrint);
float FiducciaMattheyses_NodeDelta_get(int nNodes, float* xEdges, int* part, int idNode);
