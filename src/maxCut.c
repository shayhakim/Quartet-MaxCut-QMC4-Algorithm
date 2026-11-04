#include "maxCut.h"

#define ALPHA_INFINITY 10000000

int* paraSearch(int nNodes, float* goodEdges, float* badEdges, int bPrint)
{

	if (bPrint) printf("\nMaxCut with nNodes=%d.\n", nNodes);
	if (nNodes<3)
	{
		printf("MaxCut: Too small number of nodes for maxCut. nNodes=%d, but should be at least 3.\nExited from maxCut\n\n", nNodes);
		return NULL;
	};
	if (nNodes==3)
	{
		if (bPrint) printf("MaxCut with three nodes: Single node was cutten\nExited from maxCut\n\n");
		return paraSearch3nNodes(goodEdges, badEdges);
	};

	float alpha = 0, alphaLast = 0, alphaMaxPossible, alphaObserved, alphaBest1Or2;
	int nNodesCut;

	int* partBest1Or2 = paraSearchCutExactlyOneOrPair(nNodes, goodEdges, badEdges, &alphaBest1Or2, &nNodesCut, 0);
	if (bPrint) printf("MaxCut: partBest1Or2 was made, alphaBest1Or2=%4.1f, nNodesCut=%d\n",alphaBest1Or2,nNodesCut);

	if (nNodes<6)
	{
		if (nNodesCut==1)
		{
			if (bPrint) printf("MaxCut with %d nodes: Single node was cutten\nExited from maxCut\n\n",nNodesCut);
		}
		else
		{
			if (bPrint) printf("MaxCut with %d nodes: Pair of nodes was cutten\nExited from maxCut\n\n",nNodesCut);
		}
		return partBest1Or2;
	};

	alphaMaxPossible = alphaMaxPossible_get(nNodes, goodEdges);
	if (alphaMaxPossible<=0)
	{
		printf("MaxCut: alphaMaxPossible=%f, but should be >0.\nExited from maxCut\n\n", alphaMaxPossible);
		return NULL;
	};

	float* xEdges =  (float*) calloc (nNodes*nNodes, sizeof(float));

	int* partCurrent = NULL, *partBest = NULL;
	int nNodesInMinOfTwoPartsCurrent = -1;
	int nNodesInMinOfTwoPartsBest = -1;
	int i;
	float scorePart_ForCut;

	{		
		if (alphaBest1Or2<=2)
		{
			alpha = 2;
		} else 
		{
			alpha = alphaBest1Or2;
		};
		if (bPrint) printf("MaxCut: started with alpha = %4.1f\n", alpha);

		compute_xEdges(nNodes, goodEdges, badEdges, xEdges, alpha); 
		if (bPrint) {
			printArray1DFloatToScreen("xEdges", nNodes, xEdges);
		}

		#define N_INITIAL_TRIES 100
		for (i = 0; ((i < N_INITIAL_TRIES) && (partBest == NULL)); i++)
		{
			partCurrent = maxCut_forSingleRandomStart_Run(nNodes, xEdges, &nNodesInMinOfTwoPartsCurrent, bPrint, &scorePart_ForCut);

			if (partCurrent == NULL)
			{

				printf("ERROR: maxCut_forSingleRandomStart -> NULL?! nNodes %d, alpha %4.1f\nExited from maxCut\n\n",nNodes, alpha);
				return NULL;
			}			
			if (bPrint) printf("i=%d, nNodesInMinOfTwoPartsCurrent = %d\n",i,nNodesInMinOfTwoPartsCurrent);

			if ((nNodesInMinOfTwoPartsCurrent < 1) || (scorePart_ForCut<0))
			{
				if (bPrint) printf ("Initial try %d: maxCut_forSingleRandomStart -> Empty cut; nNodesInMinOfTwoPartsCurrent= %d\n", i, nNodesInMinOfTwoPartsCurrent);
				free(partCurrent);
			}
			else 
			{
				partBest = partCurrent;  
				nNodesInMinOfTwoPartsBest = nNodesInMinOfTwoPartsCurrent;
				partCurrent = NULL;
				if (bPrint) printf ("Initial try %d: maxCut_forSingleRandomStart -> Good cut; nNodesInMinOfTwoPartsCurrent= %d\n", i, nNodesInMinOfTwoPartsCurrent);
			};
		};

		if (partBest == NULL)
		{
			if (bPrint) 
			{
				printf("MaxCut: No meaningful partitions was found by maxCut_forSingleRandomStart in initial loop.\n");

				printf("MaxCut: partBest1Or2-based result is used instead of maxCut_forSingleRandomStart\n");
				printf("nNodes = %d, nNodesCut = %d, alpha=%4.1f\n", nNodes, nNodesCut, alphaBest1Or2);
				printf("Exited from maxCut\n\n");
			};

			free(xEdges);
			return partBest1Or2;
		};

		alphaObserved = alphaObserved_get(nNodes, goodEdges, badEdges, partBest);
		free(partBest1Or2);
	};

	if (bPrint) printf("MaxCut: tries to increase alpha. alphaObserved = %f, alphaMaxPossible = %f, nNodes = %d, k = %d\n",alphaObserved,alphaMaxPossible,nNodes,nNodesInMinOfTwoPartsBest); 

	if ((alphaObserved >= alphaMaxPossible) || (alphaObserved == ALPHA_INFINITY))
	{
		if (bPrint) 
		{
			printf("MaxCut: A non empty perfect cut (with alpha = %4.1f >= %4.1f = alphaMaxPossible) was found.\n", alphaObserved, alphaMaxPossible);
			printf("nNodes = %d, nNodesInMinOfTwoPartsBest = %d\n", nNodes, nNodesInMinOfTwoPartsBest);
			printf("Exited from maxCut\n\n");
		};
		free(xEdges);
		return partBest;
	};

	int bAlphaIsOk = 1;
	alphaLast = alphaObserved;
	alpha = alphaLast;
	while (bAlphaIsOk)
	{
		alpha = alpha * 2;

		if (bPrint) printf("MaxCut: UB run with alpha = %4.1f\n", alpha);
		compute_xEdges(nNodes, goodEdges, badEdges, xEdges, alpha);

		partCurrent = maxCut_forSingleRandomStart_Run(nNodes, xEdges, &nNodesInMinOfTwoPartsCurrent, bPrint, &scorePart_ForCut);
		if (partCurrent == NULL)
		{

			printf("ERROR: maxCut_forSingleRandomStart -> NULL?! nNodes %d, alpha %4.1f\nExited from maxCut\n\n",nNodes, alpha);
			return NULL;
		}

		if ((nNodesInMinOfTwoPartsCurrent <= 0) || (scorePart_ForCut<0))
		{
			if ((nNodesInMinOfTwoPartsCurrent == 0) || (scorePart_ForCut<0))
			{
				if (bPrint) printf("Empty cut was found in UB loop\n");
				free(partCurrent);
				partCurrent = NULL;
				bAlphaIsOk = 0;
			}
			else
			{
				printf("MaxCut UB: ERROR: Invalid part returned from maxCutHack: %d\nExited from maxCut\n\n", nNodesInMinOfTwoPartsCurrent);
				return NULL;
			};
		}
		else
		{
			alphaObserved = alphaObserved_get(nNodes, goodEdges, badEdges, partCurrent);
			free(partBest);
			partBest = partCurrent;
			nNodesInMinOfTwoPartsBest = nNodesInMinOfTwoPartsCurrent;
			partCurrent = NULL;
			if ((alphaObserved < alphaMaxPossible) || (alphaObserved < ALPHA_INFINITY))
			{
				if (bPrint) printf ("maxCut UB -> Good cut; k = %d, alpha = %4.1f\n", nNodesInMinOfTwoPartsCurrent, alphaObserved);
				alphaLast = alphaObserved;
				alpha = alphaLast;
			}
			else
			{
				if (bPrint) 
				{
					printf("MaxCut: A non empty perfect cut (with alpha = %4.1f >= %4.1f = alphaMaxPossible) was found.\n", alphaObserved, alphaMaxPossible);
					printf("nNodes = %d, nNodesInMinOfTwoPartsBest = %d\n", nNodes, nNodesInMinOfTwoPartsBest);
					printf("Exited from maxCut\n\n");
				};

				free(xEdges);
				return partBest;				
			};
		};
	};

	if (!partBest)
	{  
		printf("MaxCut UB: ERROR: Must have found some non empty cut\nExited from maxCut\n\n");
		return NULL;
	};
    	if (partCurrent != NULL)
	{
		printf("MaxCut UB: ERROR: partCurrent must be null after initial loop, alpha = %f, alphaLast = %f\n",alpha,alphaLast);
    	};

	float upBound = alpha; 
	float lowBound = alphaLast;
	float deltaMin = 1 / alphaMaxPossible;
	if (deltaMin<0.001) deltaMin = 0.001;
	while ((upBound - lowBound) > deltaMin)
	{
		if (upBound < lowBound) 
		{

		}

		alpha = (upBound + lowBound) / 2;

		if (bPrint) printf("MaxCut BinarySearch: alpha=%f, minAlphaBS=%f, maxAlphaBS=%f, d=maxAlphaBS-minAlphaBS=%f, d0=dStopBS=%f\n", alpha,lowBound,upBound,upBound-lowBound,deltaMin);

		if (bPrint) printf("MaxCut: BinarySearch run with alpha = %f\n", alpha);
		compute_xEdges(nNodes, goodEdges, badEdges, xEdges, alpha);

		partCurrent = maxCut_forSingleRandomStart_Run(nNodes, xEdges, &nNodesInMinOfTwoPartsCurrent, bPrint, &scorePart_ForCut);
		if (partCurrent == NULL)
		{

			printf("ERROR: maxCut_forSingleRandomStart -> NULL?! nNodes %d, alpha %4.1f\nExited from maxCut\n\n",nNodes, alpha);
			return NULL;
		}

		if ((nNodesInMinOfTwoPartsCurrent <= 0) || (scorePart_ForCut<0))
		{
			if ((nNodesInMinOfTwoPartsCurrent == 0) || (scorePart_ForCut<0))
			{
				if (bPrint) printf("MaxCut BinarySearch: Empty cut was found in Binary search loop.\n");
				free(partCurrent);
				partCurrent = NULL;
				upBound = alpha;
			}
			else
			{
				printf("MaxCut BinarySearch: ERROR: Invalid part returned from maxCutHack: %d\nExited from maxCut\n\n", nNodesInMinOfTwoPartsCurrent);
				return NULL;
			};
		}
		else
		{
			alphaObserved = alphaObserved_get(nNodes, goodEdges, badEdges, partCurrent);
			if (bPrint) printf ("maxCut BinarySearch -> Good cut; k = %d, alpha = %f\n", nNodesInMinOfTwoPartsCurrent, alphaObserved);

			free(partBest);
			partBest = partCurrent;
			nNodesInMinOfTwoPartsBest = nNodesInMinOfTwoPartsCurrent;
			partCurrent = NULL;
			alphaLast = alphaObserved;
			lowBound = alphaObserved;
		};
	};

	if (bPrint) 
	{
		printf("MaxCut: A non empty cut with alpha = %4.1f was found.\n", alphaObserved);
		printf("nNodes = %d, nNodesInMinOfTwoPartsBest = %d\n", nNodes, nNodesInMinOfTwoPartsBest);
		printf("Exited from maxCut\n\n");
	};
	free(xEdges); 
	return partBest;				
}

int* paraSearch3nNodes(float* goodEdges, float* badEdges)
{

	float g[3], b[3], a[3];

	int i, j, k, k1, k2;

	for (i=0; i < 3; i++)
	{
		g[i]=0;
		b[i]=0;
	};
	for (i=0; i < 2; i++)
	{
		for (j=i+1; j < 3; j++)
		{
			k1=i*3+j;
			k2=j*3+i;
			if ((i==0)&&(j==1))
				k=2;
			if ((i==0)&&(j==2))
				k=1;
			if ((i==1)&&(j==2))
				k=0;
			g[k]=g[k]+goodEdges[k1]+goodEdges[k2];
			b[k]=b[k]+badEdges[k1]+badEdges[k2];
		}
	}

	for (i=0; i < 2; i++)
	{
		k1=(i+1) % 3;
		k2=(i+2) % 3;
		a[i] = (b[k1]+b[k2]>0) ? ((g[k1]+g[k2])/(b[k1]+b[k2])) : (ALPHA_INFINITY);

	};

	j = 0;
	for (i=1; i < 3; i++)
	{
		if (a[i]>a[j]) 
			j=i;
	};

	int* part = (int*) calloc (3, sizeof(int));
	for (i=0; i < 3; i++)
		part[i]=0;
	part[j]=1;
	return part;
}

int* paraSearchCutExactlyOne(int nNodes, float* goodEdges, float* badEdges, float* alphaBest)
{

	int i,j,k,k1=-1,m;
	float alpha,b,g;
	for (i=0; i < nNodes; i++)
	{
		g=0;
		b=0;
		for (m=0; m < nNodes; m++)
		{
			g=g+goodEdges[i*nNodes + m];
			b=b+badEdges[i*nNodes + m];
		}
		alpha = (b>0) ? (g/b) : (ALPHA_INFINITY);

		k=0;
		if (k1<0)
		{
			k=1;
		} else if (*alphaBest<alpha)
			k=1; 
		if (k)
		{
			k1=i;
			*alphaBest=alpha;
		}
	}

	int* part = (int*) calloc (nNodes, sizeof(int));
	for (i=0; i < nNodes; i++)
		part[i]=0;
	part[k1]=1;
	return part;
}

int* paraSearchCutExactlyPair(int nNodes, float* goodEdges, float* badEdges, float* alphaBest)
{

	int i,j,k,k1=-1,k2,m;
	float alpha,b,g;
	for (i=0; i < nNodes-1; i++)
	{
		for (j=i+1; j < nNodes; j++)
		{
			g=0;
			b=0;
			for (m=0; m < nNodes; m++)
			{
				if ((m != i)&&(m != j))
				{
					g = g + goodEdges[i*nNodes + m] + goodEdges[j*nNodes + m];
					b = b + badEdges[i*nNodes + m] + badEdges[j*nNodes + m];
				};
			};
			alpha = (b>0) ? (g/b) : (ALPHA_INFINITY);

			k=0;
			if (k1<0)
			{
				k=1;
			} else if (*alphaBest<alpha)
				k=1; 
			if (k)
			{
				k1=i;
				k2=j;
				*alphaBest=alpha;
			}
		}
	}

	int* part = (int*) calloc (nNodes, sizeof(int));
	for (i=0; i < nNodes; i++)
		part[i]=0;
	part[k1]=1;
	part[k2]=1;
	return part;
}

int* paraSearchCutExactlyOneOrPair(int nNodes, float* goodEdges, float* badEdges, float *alphaBest, int *nNodesCut, int bPrefere1)
{
	float alphaBest1, alphaBest2;
	int* part1=paraSearchCutExactlyOne(nNodes, goodEdges, badEdges, &alphaBest1);
	int* part2=paraSearchCutExactlyPair(nNodes, goodEdges, badEdges, &alphaBest2);
	int* partOk;
	int* partNoNeed;
	if ((alphaBest1>alphaBest2) || (bPrefere1 && (alphaBest2==alphaBest1))) {
		partOk=part1;
		partNoNeed=part2;
		*nNodesCut=1;
		*alphaBest=alphaBest1;
	} else {
		partOk=part2;
		partNoNeed=part1;
		*nNodesCut=2;
		*alphaBest=alphaBest2;
	}
	free (partNoNeed); 
	partNoNeed = NULL;
	return partOk;
}

void compute_xEdges(int nNodes, float* goodEdges, float* badEdges, float* xEdges, float alpha)
{
	int i,j;

	for (i=0; i < nNodes; i++)
	{
		for (j=0; j < nNodes; j++)
		{
			if ((goodEdges[i*nNodes + j]) || (goodEdges[j*nNodes + i]) || (badEdges[i*nNodes + j]) || (badEdges[j*nNodes + i]))
			{

				if ((badEdges[i*nNodes + j]) !=  (badEdges[j*nNodes + i]))
				{
					printf ("ERROR: No symmetry in badEdges: [%d, %d] = %g =/= %g = [%d, %d]\n\n", i,j, badEdges[i*nNodes + j], badEdges[j*nNodes + i], j, i);
					exit(7);
				};

				if ((i==j) && (badEdges[i*nNodes + j])) 
				{
					printf ("ERROR: badEdge from i to i: i=%d, badEdges[i*nNodes + j]=%f\n", i,badEdges[i*nNodes + j]);
					exit(7);
				};

				if ((goodEdges[i*nNodes + j]) !=  (goodEdges[j*nNodes + i]))
				{
					printf ("ERROR: No symmetry in goodEdges: [%d, %d] = %g =/= %g = [%d, %d]\n\n", i,j, goodEdges[i*nNodes + j], goodEdges[j*nNodes + i], j, i);
					exit(7);
				}

				if ((i==j) && (goodEdges[i*nNodes + j])) 
				{
					printf ("ERROR: goodEdge from i to i: i=%d\n", i);
					exit(7);
				};

				xEdges[i*nNodes + j] = goodEdges[i*nNodes + j] - alpha * badEdges[i*nNodes + j];
			}
			else 
			{
				xEdges[i*nNodes + j] = 0;
			}
		}
	}

	for (i=0; i < nNodes; i++)
	{
		for (j=0; j < nNodes; j++)
		{

			if (xEdges[i * nNodes +j] != xEdges[j * nNodes +i])
			{
				printf ("ERROR ERROR: xEdges is not symmetric at %d and %d\n\n",i,j);
				exit(7);
			}

			if (xEdges[i * nNodes +j] != 0)
			{

			}
		}
	}
}

float alphaMaxPossible_get(int nNodes, float* goodEdges)
{
	int i,j;
	float sum = 0;
	for (i=0; i < nNodes; i++)
	{
		for (j=0; j < nNodes; j++)
		{
			sum = sum + goodEdges[i*nNodes + j];
		};
	};
	return sum;
}

float alphaObserved_get(int nNodes, float* goodEdges, float* badEdges, int* part)
{
	float b = scorePart(nNodes, badEdges, part);
	float g = scorePart(nNodes, goodEdges, part);
	if (b==0)
	{
		return ALPHA_INFINITY;
	}
	else
	{
		return g/b;
	};
}

int* maxCut_forSingleRandomStart_Run(int nNodes, float* xEdges, int* nNodesInMinOfTwoParts, int bPrint, float *scorePart_ForCut)
{
   int *partitionBest = NULL;

   partitionBest = maxCut_forSingleRandomStart(nNodes, xEdges, nNodesInMinOfTwoParts, bPrint, scorePart_ForCut);
   return partitionBest;
}

int* maxCut_forSingleRandomStart(int nNodes, float* xEdges, int *nNodesInMinOfTwoParts, int bPrint, float *scorePart_ForCut)
{
	float** pointPositionOnSphere = (float**) calloc (nNodes, sizeof(float*));	

	#define DIM 3	

	if (bPrint) printf("\nmaxCut_forSingleRandomStart: Started.\n");

	{
		int i;
		for (i=0 ; i<nNodes ; i++)
		{
			int dim;
			pointPositionOnSphere[i] = (float*) calloc (DIM, sizeof(float));
			for (dim=0 ; dim<DIM ; dim++)
			{
				pointPositionOnSphere[i][dim]= randfloat()-0.5;
			}
			normalize(pointPositionOnSphere[i],DIM);
		};
	}

	{
		#define EPSILON .01
		#define NSTEPS 280

		float** pointPositionOnSphereOld = (float**) calloc (nNodes, sizeof(float*));	
		int i, j, k, dim, step;
		for (i=0 ; i<nNodes ; i++)
		{
			pointPositionOnSphereOld[i] = (float*) calloc (DIM, sizeof(float));
		}

		for (step=0 ; step < NSTEPS ; step++)
		{

			for (j = 0 ; j < nNodes ; j++)
			{
				for (dim=0; dim < DIM; dim++)
				{
					pointPositionOnSphereOld[j][dim]=pointPositionOnSphere[j][dim];
				}
			}

			for (j = 0 ; j < nNodes ; j++)
			{

				for (i = 0; i < nNodes; i++)
				{
					k=j * nNodes + i;
					if (xEdges[k])
					{
						for (dim=0; dim < DIM; dim++)
						{
							pointPositionOnSphere[j][dim] += -EPSILON * pointPositionOnSphereOld[i][dim] * xEdges[k];
						}
					}
				}
				normalize(pointPositionOnSphere[j], DIM);
			}
		}

		for ( i=0 ; i < nNodes ; i++)
		{
			free (pointPositionOnSphereOld[i]); 
			pointPositionOnSphereOld[i] = NULL;
		}
		free (pointPositionOnSphereOld); 
		pointPositionOnSphereOld = NULL;
	}

	int nNodesInPart[2] = {nNodes,0};	
	int* partitionBest = (int*) calloc (nNodes, sizeof(int));	
	float scoreBest = 0; 
	int nNodesInMinPart=0;

	{
		#define N_ATTEMPTS 50

		int iAttempt;
		int* partitionTemp = (int*) calloc (nNodes, sizeof(int));	

		int nNodesInPartTemp[2] = {0,0};			
		int nNodesInMinPartTemp;
		for (iAttempt = 0; iAttempt < N_ATTEMPTS ; iAttempt++)
		{

			float randVec[DIM];
			int dim;
			for (dim=0; dim < DIM; dim++)
			{
				randVec[dim]= randfloat()-0.5;
			}

			nNodesInPartTemp[0] = nNodesInPartTemp[1] = 0;				
			int i;
			for (i=0; i< nNodes; i++)
			{

				if (dotProduct(randVec, pointPositionOnSphere[i], DIM) <=0.0)
				{
					partitionTemp[i] = 0;
					nNodesInPartTemp[0]++;
				}
				else
				{
					partitionTemp[i] = 1;
					nNodesInPartTemp[1]++;
				}
			};

			nNodesInMinPartTemp=(nNodesInPartTemp[0] <= nNodesInPartTemp[1]) ? (nNodesInPartTemp[0]) : (nNodesInPartTemp[1]);

			if (nNodesInMinPartTemp > 0)
			{
				float score;
				score = scorePart(nNodes, xEdges, partitionTemp);

				if (((scoreBest == score) && (nNodesInMinPart<nNodesInMinPartTemp)) || (scoreBest < score))
				{

					if (scoreBest == score)
					{

					};

					for (i=0; i< nNodes; i++)
					{
						partitionBest[i] = partitionTemp[i];
					}
					nNodesInPart[0] = nNodesInPartTemp[0];
					nNodesInPart[1] = nNodesInPartTemp[1];
					nNodesInMinPart = nNodesInMinPartTemp;
					scoreBest = score;
				};
			};
		};

		if (nNodesInMinPart == 1)
		{

		};
		if (scoreBest < 0)
		{
			printf("maxCut_forSingleRandomStart: Error: negative scoreBest = %f \n", scoreBest);
		};

		free(partitionTemp); 
	};

	{
		int i;
		for ( i=0 ; i < nNodes ; i++)
		{
			free (pointPositionOnSphere[i]); 
			pointPositionOnSphere[i] = NULL;
		}
	}
	free (pointPositionOnSphere); 
	pointPositionOnSphere = NULL;

	{

		int i;
		for ( i = 0 ; i < nNodes; i++) 
		{

			if (partitionBest[i]> 1 || partitionBest[i] < 0)
			{

			};
		};
	};

	FiducciaMattheyses(partitionBest, xEdges, nNodes, bPrint);

	nNodesInPart[0] = nNodesInPart[1] = 0;
	int i;
	for ( i = 0 ; i < nNodes; i++) 
	{

		if (partitionBest[i]> 1 || partitionBest[i] < 0)
		{

		}
		nNodesInPart[partitionBest[i]]++;
	};
	if (nNodesInPart[0] + nNodesInPart[1] != nNodes)
	{

	};

	scoreBest = scorePart(nNodes, xEdges, partitionBest);
	nNodesInMinPart = (nNodesInPart[0] <= nNodesInPart[1]) ? (nNodesInPart[0]) : (nNodesInPart[1]);

	if (nNodesInMinPart==0)
	{

	}
	else if (nNodesInMinPart == 1)
	{

	};

	*nNodesInMinOfTwoParts = nNodesInMinPart;
	*scorePart_ForCut = scoreBest;

	if (bPrint) printf("maxCut_forSingleRandomStart: Finished. nNodesInMinPart = %d, scoreBest = %f\n\n", nNodesInMinPart, scoreBest);
	return partitionBest;
};

void FiducciaMattheyses(int* part, float* xEdges, int nNodes, int bPrint)
{
	if (nNodes<=1) return;

	int* partition = (int*) calloc (nNodes, sizeof(int));
	int isPossibleImprove = 1;
	int* frozen = (int*) calloc (nNodes, sizeof(int));
	float* deltaScore = (float*) calloc (nNodes, sizeof(float));
	float score;
	float scoreBest;
	int i,j,k,iDefrosting=-1;
	if (!partition || !part)
	{

	}

	{
		scoreBest = scorePart(nNodes, xEdges, part);
		for (i = 0 ; i < nNodes ; i++)
		{
			partition[i] = part[i];
		}
		score = scoreBest;

		for (j = 0 ; j < nNodes ; j++)
		{
			deltaScore[j] = FiducciaMattheyses_NodeDelta_get(nNodes, xEdges, partition, j);

		};
	};

	while(isPossibleImprove)
	{

		iDefrosting = iDefrosting + 1;
		for (i = 0 ; i < nNodes ; i++)
		{
			frozen[i] = 0;
		}
		if (bPrint) printf("FM: iDefrosting = %d\n",iDefrosting);

		int idSwap = 0;
		for (idSwap = 0 ; idSwap < nNodes ; idSwap++)
		{

			int deltaScoreArgMax = -1;
			float deltaScoreMax = 0;
			for (j = 0; j < nNodes ; j++)
			{
				if (!frozen[j])	
				{
					if ((deltaScoreArgMax < 0) || (deltaScoreMax < deltaScore[j]))
					{
						deltaScoreArgMax = j;
						deltaScoreMax = deltaScore[j];

					}
				}
			};

			float dd = scoreBest - (score + deltaScoreMax);	
			if (bPrint) printf("FM: idSwap = %d, ArgMax = %d, d=%4.1f, D=%4.1f\n",idSwap,deltaScoreArgMax,deltaScoreMax,dd);

			if (deltaScoreArgMax < 0 )	 
			{

				printf("FiducciaMattheyses: Error: deltaScoreArgMax < 0");
				return;
			};

			if (score + deltaScoreMax <= scoreBest) {

				idSwap = nNodes + 1;
			}
			else {

				partition[deltaScoreArgMax] = (partition[deltaScoreArgMax] ? 0 : 1);
				score = score + deltaScore[deltaScoreArgMax];

				for (j = 0 ; j < nNodes ; j++) 
				{
					k=deltaScoreArgMax * nNodes + j;
					if (xEdges[k])
					{

						if (partition[j] == partition[deltaScoreArgMax])
						{
							deltaScore[j] -= 2 * xEdges[k];
						}
						else
						{
							deltaScore[j] += 2 * xEdges[k];
						};

					}
				}
				deltaScore[deltaScoreArgMax] = - deltaScore[deltaScoreArgMax];	

				frozen[deltaScoreArgMax] = 1;	

			};	
		};	

		if (score > scoreBest)
		{
			scoreBest = score;

			for (i = 0 ; i < nNodes ; i++)
			{
				part[i] = partition[i];
			};	    
			isPossibleImprove = 1;	
		}
		else {
			isPossibleImprove = 0;
		};

	};	

	free(frozen); 
	frozen = NULL;
	free(deltaScore); 
	deltaScore = NULL;
	free(partition); 
	partition  = NULL;

	if (bPrint) printf("FM: Finished!\n");
};

float FiducciaMattheyses_NodeDelta_get(int nNodes, float* xEdges, int* part, int idNode)
{
	float value=0;
	value -= scorePart_Node(nNodes, xEdges, part, idNode);
	part[idNode] = (part[idNode] ? 0 : 1);

	value += scorePart_Node(nNodes, xEdges, part, idNode);
	part[idNode] = (part[idNode] ? 0 : 1);

	;

	return 2*value;	
};

float scorePart(int nNodes, float* xEdges, int* part)	
{
	float score=0;
	int j;
	for (j = 0; j < nNodes; j++)
	{
		score += scorePart_Node(nNodes, xEdges, part, j);
	};

	return score;
};

float scorePart_Node(int nNodes, float* xEdges, int* part, int idNode)
{
	float score=0;
	int i=0,k;
	for (i = 0; i < nNodes; i++)
	{
		if (part[i] != part[idNode])
		{
			k= idNode * nNodes + i;
			score += xEdges[k];
		}
	}

	return score;
};
