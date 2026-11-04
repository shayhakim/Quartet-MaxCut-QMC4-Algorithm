#include "treeQuartets.h"  

void treeBuildBasedOnQuartets_firstTime_reviseQuartets_isTogetherInQuartet1D(quartet** quartetHead, int idNodeNamesIntersting[], int nNodeNamesIntersting, int adressOfIdInInteresting[], int isTogetherInQuartet1D[])
{

	int i,j,k;
	for (i = 0; i <nNodeNamesIntersting; i++) {
		k = i * nNodeNamesIntersting + i;
		isTogetherInQuartet1D[k] = 0;
		for (j = i+1; j <nNodeNamesIntersting; j++) {
			k = i * nNodeNamesIntersting + j;
			isTogetherInQuartet1D[k] = 0;
			k = j * nNodeNamesIntersting + i;
			isTogetherInQuartet1D[k] = 0;
		};
	};

	int idA, idB, idC, idD, bA, bB, bC, bD, n, nq;
	quartet* q = (*quartetHead);
	quartet* quartetHeadNew = NULL;
	quartet* quartetTailNew = NULL;
	nq=0;

	while (q != NULL) {
		idA = q -> idA;
		idB = q -> idB;
		idC = q -> idC;
		idD = q -> idD;
		bA = (adressOfIdInInteresting[idA] != -1);
		bB = (adressOfIdInInteresting[idB] != -1);
		bC = (adressOfIdInInteresting[idC] != -1);
		bD = (adressOfIdInInteresting[idD] != -1);
		n = bA + bB + bC + bD;
		nq++;
		quartet* q1 = q;
		if (n == 4) {

			i = adressOfIdInInteresting[idA];
			j = adressOfIdInInteresting[idB];
			k = i * nNodeNamesIntersting + j;
			if (i==j) {
				printf("nq=%d\n",nq);
				quartetSigle_PrintToScreen(q,0);
			}
			isTogetherInQuartet1D[k] = 1;
			k = j * nNodeNamesIntersting + i;
			isTogetherInQuartet1D[k] = 1;

			i = adressOfIdInInteresting[idC];
			j = adressOfIdInInteresting[idD];
			k = i * nNodeNamesIntersting + j;
			if (i==j) {
				printf("nq=%d\n",nq);
				quartetSigle_PrintToScreen(q,0);
			}
			isTogetherInQuartet1D[k] = 1;
			k = j * nNodeNamesIntersting + i;
			isTogetherInQuartet1D[k] = 1;			

			if (quartetHeadNew) {
				(quartetTailNew->Next) = q;
				quartetTailNew = q;
			}
			else {
				quartetHeadNew = q;
				quartetTailNew = q;
			};

			q = q -> Next;
			q1 -> Next = NULL;

		} else {
			q = q -> Next;
			free(q1);
		}
	}
	*quartetHead = quartetHeadNew;
};

void treeBuildBasedOnQuartets_clustering(quartet* quartetHead, int idNodeNamesIntersting[], int nNodeNamesIntersting, int adressOfIdInInteresting[], int* isOk, int isTogetherInQuartet1D[], FILE** fileLog, int isPrint, int isPrintDetails, int *nClusters, int* idClusterForIntersting) {
	*nClusters=0;

	clusteringBasedOnIsEdje1D(isTogetherInQuartet1D, nNodeNamesIntersting, nClusters, idClusterForIntersting); 

	if (isPrint) {
		printf("Aho with %d, iMin=%d, nClusters=%d\n",nNodeNamesIntersting,idNodeNamesIntersting[0],*nClusters);
		fprintf(*fileLog,"Aho with %d, iMin=%d, nClusters=%d\n",nNodeNamesIntersting,idNodeNamesIntersting[0],*nClusters);

		printArray1DIntToScreen("isTogetherInQuartet1D", nNodeNamesIntersting, isTogetherInQuartet1D); 

		if (isPrintDetails) {
			printArrayIntToFile(fileLog, "idClusterForIntersting", nNodeNamesIntersting, idClusterForIntersting);
		};

	};

	if (*nClusters<2) {
		int bClusteringHalf=0;
		int bClusteringMaxCut=1;

		if (bClusteringHalf) {

			clusteringHalf(nNodeNamesIntersting, nClusters, idClusterForIntersting);	
			if (isPrint) {
				printf("clusteringHalf (should to be maxCut) with %d nodes: Finished\n",nNodeNamesIntersting);
				fprintf(*fileLog,"clusteringHalf (should to be maxCut) with %d nodes: Finished\n",nNodeNamesIntersting);
			}
		};

		if (bClusteringMaxCut) {
			if (isPrint) {
				printf("Clustering by maxCut: goods and bads...\n");
				printf("Clustering by maxCut: nNodeNamesIntersting=%d\n",nNodeNamesIntersting);
			}

			float* goodEdges = malloc((nNodeNamesIntersting*nNodeNamesIntersting+1)*sizeof(float));
			float* badEdges = malloc((nNodeNamesIntersting*nNodeNamesIntersting+1)*sizeof(float));
			ArrayFloatAllZero(goodEdges, nNodeNamesIntersting*nNodeNamesIntersting); 
			ArrayFloatAllZero(badEdges, nNodeNamesIntersting*nNodeNamesIntersting); 

			int idA,idB,idC,idD,iA,iB,iC,iD,k1,k2;
			float w;
			int nQuartets = 0;

			quartet* q = quartetHead;
			while (q !=NULL) {
				idA = q -> idA;
				idB = q -> idB;
				idC = q -> idC;
				idD = q -> idD;
				w = q -> weight;

				iA = adressOfIdInInteresting[idA];
				iB = adressOfIdInInteresting[idB];
				iC = adressOfIdInInteresting[idC];
				iD = adressOfIdInInteresting[idD];

				k1 = iA * nNodeNamesIntersting + iC;
				k2 = iC * nNodeNamesIntersting + iA;
				goodEdges[k1] = goodEdges[k1] + w;
				goodEdges[k2] = goodEdges[k2] + w;

				k1 = iA * nNodeNamesIntersting + iD;
				k2 = iD * nNodeNamesIntersting + iA;
				goodEdges[k1] = goodEdges[k1] + w;
				goodEdges[k2] = goodEdges[k2] + w;

				k1 = iB * nNodeNamesIntersting + iC;
				k2 = iC * nNodeNamesIntersting + iB;
				goodEdges[k1] = goodEdges[k1] + w;
				goodEdges[k2] = goodEdges[k2] + w;

				k1 = iB * nNodeNamesIntersting + iD;
				k2 = iD * nNodeNamesIntersting + iB;
				goodEdges[k1] = goodEdges[k1] + w;
				goodEdges[k2] = goodEdges[k2] + w;

				k1 = iA * nNodeNamesIntersting + iB;
				k2 = iB * nNodeNamesIntersting + iA;
				badEdges[k1] = badEdges[k1] + w;
				badEdges[k2] = badEdges[k2] + w;

				k1 = iC * nNodeNamesIntersting + iD;
				k2 = iD * nNodeNamesIntersting + iC;
				badEdges[k1] = badEdges[k1] + w;
				badEdges[k2] = badEdges[k2] + w;

				nQuartets++;

				q = q -> Next;
			};
			if (isPrint) {
				printArray1DFloatToScreen("goodEdges", nNodeNamesIntersting, goodEdges);
				printArray1DFloatToScreen("badEdges", nNodeNamesIntersting, badEdges);
			}

			int *idClusterForInterstingServ;
			idClusterForInterstingServ = paraSearch(nNodeNamesIntersting, goodEdges, badEdges, isPrint);
			*nClusters=2;

			int i;
			if (nNodeNamesIntersting==4) {
				int n1 = 0;
				for (i = 0; i <nNodeNamesIntersting; i++) {
					n1 = n1 + idClusterForInterstingServ[i];
				}
				if ((n1==1) || (n1==3)) {

					idClusterForInterstingServ[0] = 0;
					idClusterForInterstingServ[1] = 1;
					idClusterForInterstingServ[2] = 2;
					idClusterForInterstingServ[3] = 3;
					*nClusters=4;
				}
			}
			for (i = 0; i <nNodeNamesIntersting; i++) {
				idClusterForIntersting[i] = idClusterForInterstingServ[i];
			}

			free(goodEdges);
			free(badEdges);
			if (isPrint) {
				printf("maxCut with %d nodes: Finished\n",nNodeNamesIntersting);
				fprintf(*fileLog,"maxCut with %d nodes: Finished\n",nNodeNamesIntersting);
			}
		};
		if (isPrint) {

			printArrayIntToScreen("idClusterForIntersting", nNodeNamesIntersting, idClusterForIntersting);
			printArrayIntToFile(fileLog, "idClusterForIntersting", nNodeNamesIntersting, idClusterForIntersting); 
			PressAnyKey();
		}
	};
}

void treeBuildBasedOnQuartets_ArraysForClusters_partI(int nClusters, int idClusterForIntersting[], int nNodeNamesIntersting, int idNodeNamesIntersting[], int idMax, FILE** fileLog, int isPrint, int nNodeNamesInterstingCluster[], int* idNodeNamesInterstingCluster[], int* adressOfIdInInterestingCluster[], int idNodeNameShouldBeReplacedByRootCluster[]) {

	int idCluster;

	int i,n;

	for (idCluster = 0; idCluster <nClusters; idCluster++) {
		nNodeNamesInterstingCluster[idCluster] = 0;
		idNodeNamesInterstingCluster[idCluster] = (int *)malloc((nNodeNamesIntersting+2)*sizeof(int));
		adressOfIdInInterestingCluster[idCluster] = (int *)malloc((idMax+2)*sizeof(int));
		idNodeNameShouldBeReplacedByRootCluster[idCluster] = -1;
	};

	for (i = 0; i < nNodeNamesIntersting; i++) {

		idCluster = idClusterForIntersting[i];
		n = nNodeNamesInterstingCluster[idCluster] + 1;
		nNodeNamesInterstingCluster[idCluster] = n;
		idNodeNamesInterstingCluster[idCluster][n-1] = idNodeNamesIntersting[i];
	};

	if (nClusters>1) {
		if ((nNodeNamesInterstingCluster[0]>0)&&(nNodeNamesInterstingCluster[1]>0)) {
			int idNodeNamesIntersting;

			idNodeNamesIntersting = idNodeNamesInterstingCluster[1][0]; 

			idCluster = 0;
			idNodeNameShouldBeReplacedByRootCluster[idCluster] = idNodeNamesIntersting;
			n = nNodeNamesInterstingCluster[idCluster] + 1;
			nNodeNamesInterstingCluster[idCluster] = n;
			idNodeNamesInterstingCluster[idCluster][n-1] = idNodeNamesIntersting;

			idNodeNamesIntersting = idNodeNamesInterstingCluster[0][0]; 
			for (idCluster = 1; idCluster < nClusters; idCluster++) {
				idNodeNameShouldBeReplacedByRootCluster[idCluster] = idNodeNamesIntersting;
				n = nNodeNamesInterstingCluster[idCluster] + 1;
				nNodeNamesInterstingCluster[idCluster] = n;
				idNodeNamesInterstingCluster[idCluster][n-1] = idNodeNamesIntersting;
			}
		}
	}

	if (isPrint) {
		fprintf(*fileLog,"treeBuildBasedOnQuartets_ArraysForClusters_partI: 1.\n");
		for (idCluster = 0; idCluster < nClusters; idCluster++) {
			n = nNodeNamesInterstingCluster[idCluster];

			char str1[40];
			sprintf(str1, "idCluster=%d, Interesting:", idCluster);
			printArrayIntToFile(fileLog, str1, n, idNodeNamesInterstingCluster[idCluster]);
		};
	};

	for (idCluster = 0; idCluster <nClusters; idCluster++) {
		make_adressInListOfId(idMax, idNodeNamesInterstingCluster[idCluster], nNodeNamesInterstingCluster[idCluster], adressOfIdInInterestingCluster[idCluster]);
	};	
};

void treeBuildBasedOnQuartets_ArraysForClusters_partII(quartet** quartetHead, int nClusters, int idClusterForIntersting[], int adressOfIdInInteresting[], int idNodeNameShouldBeReplacedByRootCluster[], int nNodeNamesInterstingCluster[], int* adressOfIdInInterestingCluster[], int* idNodeNamesInterstingCluster[], int idMax, FILE** fileLog, int isPrint, int* isTogetherInQuartet1DCluster[], quartet** quartetHeadCluster[]) {

	int idCluster;

	quartet** quartetTailCluster = (quartet **)malloc((nClusters+1)*sizeof(quartet *));
	int* nQuartets3Cluster = (int*)malloc((nClusters+1)*sizeof(int *));
	for (idCluster = 0; idCluster <nClusters; idCluster++) {
		(*quartetHeadCluster)[idCluster] = NULL;
		quartetTailCluster[idCluster] = NULL;
		nQuartets3Cluster[idCluster]=0;
	}

	for (idCluster = 0; idCluster <nClusters; idCluster++) {
		long nn = nNodeNamesInterstingCluster[idCluster] + 1;
		nn = nn * nn + 1;
		isTogetherInQuartet1DCluster[idCluster] = (int *)malloc(nn*sizeof(int));
		ArrayIntAllZero(isTogetherInQuartet1DCluster[idCluster], nn); 
		if (isPrint) printf("treeBuildBasedOnQuartets_ArraysForClusters_partII: idNodeNameShouldBeReplacedByRootCluster[%d]=%d\n", idCluster, idNodeNameShouldBeReplacedByRootCluster[idCluster]);
	};

	quartet* q = *quartetHead;
	while (q != NULL) {
		int idA, idB, idC, idD;
		idA = q -> idA;
		idB = q -> idB;
		idC = q -> idC;
		idD = q -> idD;

		int iA, iB, iC, iD;
		iA = adressOfIdInInteresting[idA];
		iB = adressOfIdInInteresting[idB];
		iC = adressOfIdInInteresting[idC];
		iD = adressOfIdInInteresting[idD];

		int idClusterA, idClusterB, idClusterC, idClusterD;
		idClusterA = idClusterForIntersting[iA];
		idClusterB = idClusterForIntersting[iB];
		idClusterC = idClusterForIntersting[iC];
		idClusterD = idClusterForIntersting[iD];

		int n=0;
		int ii=0;
		if (idClusterA == idClusterB) {
			if ((idClusterB == idClusterC) && (idClusterC == idClusterD)) {
				n = 4;
				idCluster = idClusterA;
			}
			else {
				if (idClusterC == idClusterA) {
					n = 3;
					ii = 4;
					idCluster = idClusterA;
					idD = idNodeNameShouldBeReplacedByRootCluster[idCluster];
					q -> idD = idD;
				}
				if (idClusterD == idClusterA) {
					n = 3;
					ii = 3;
					idCluster = idClusterA;
					idC = idNodeNameShouldBeReplacedByRootCluster[idCluster];
					q -> idC = idC;
				}
			}
		}
		else {
			if ((idClusterC == idClusterA) && (idClusterD == idClusterA)) {
				n = 3;
				ii = 2;
				idCluster = idClusterA;
				idB = idNodeNameShouldBeReplacedByRootCluster[idCluster];
				q -> idB = idB;
			}
			else {
				if ((idClusterC == idClusterB) && (idClusterD == idClusterB)) {
					n = 3;
					ii = 1;
					idCluster = idClusterB;
					idA = idNodeNameShouldBeReplacedByRootCluster[idCluster];
					q -> idA = idA;
				}
			}
		}
		if (n==3) {
			nQuartets3Cluster[idCluster]++;
		}

		quartet* q1 = q;
		if ((n == 4)|| (n == 3)) {
			int i, j, k;
			int nn = nNodeNamesInterstingCluster[idCluster];

			i = adressOfIdInInterestingCluster[idCluster][idA];
			j = adressOfIdInInterestingCluster[idCluster][idB];

			k = i * nn + j;
			isTogetherInQuartet1DCluster[idCluster][k] = 1;
			k = j * nn + i;
			isTogetherInQuartet1DCluster[idCluster][k] = 1;

			i = adressOfIdInInterestingCluster[idCluster][idC];
			j = adressOfIdInInterestingCluster[idCluster][idD];

			k = i * nn + j;
			isTogetherInQuartet1DCluster[idCluster][k] = 1;
			k = j * nn + i;
			isTogetherInQuartet1DCluster[idCluster][k] = 1;

			if ((*quartetHeadCluster)[idCluster]) {

				(quartetTailCluster[idCluster]->Next) = q;
				quartetTailCluster[idCluster] = q;

			}
			else {

				(*quartetHeadCluster)[idCluster] = q;

				quartetTailCluster[idCluster] = q;

			};
			q = q -> Next;
			q1 -> Next = NULL;

		} else {
			q = q -> Next;
			free(q1);
		}

	};

	for (idCluster = 0; idCluster <nClusters; idCluster++) {
		if (isPrint) printArray1DIntToScreen("isTogetherInQuartet1D_a", nNodeNamesInterstingCluster[idCluster], isTogetherInQuartet1DCluster[idCluster]); 
		if (!(nQuartets3Cluster[idCluster])) {
			int iShouldBeReplacedByRootCluster = idNodeNameShouldBeReplacedByRootCluster[idCluster];
			if (iShouldBeReplacedByRootCluster >=0) {
				idNodeNameShouldBeReplacedByRootCluster[idCluster]=-1;
				int n = nNodeNamesInterstingCluster[idCluster];
				idNodeNamesInterstingCluster[idCluster][n-1] = -1;
				nNodeNamesInterstingCluster[idCluster]--;
				int nNew = nNodeNamesInterstingCluster[idCluster];
				int i, j, k, v;
				for (i = 0; i < n; i++) {
					for (j = 0; j < n; j++) {
						k = i * n + j;
						v = isTogetherInQuartet1DCluster[idCluster][k];
						k = i * nNew + j;
						isTogetherInQuartet1DCluster[idCluster][k]=v;
					}
				}
				make_adressInListOfId(idMax, idNodeNamesInterstingCluster[idCluster], nNodeNamesInterstingCluster[idCluster], adressOfIdInInterestingCluster[idCluster]);
			}
		};
		if (isPrint) printArray1DIntToScreen("isTogetherInQuartet1D_b", nNodeNamesInterstingCluster[idCluster], isTogetherInQuartet1DCluster[idCluster]); 
	}

	for (idCluster = 0; idCluster <nClusters; idCluster++) {
		quartetTailCluster[idCluster] = NULL;
	}

	free(quartetTailCluster);

	free(nQuartets3Cluster);
}

void treeBuildBasedOnQuartets(quartet* quartetHead, int idNodeNamesIntersting[], int nNodeNamesIntersting, int adressOfIdInInteresting[], int isTogetherInQuartet1D[], int idNodeNameShouldBeReplacedByRoot, int idMax, int* isOk, FILE** fileLog, int isPrint, int isFirstTime, int indexOfRecursion, treeNode** fatherOfSubtree, int *nNodesInSubTree) {

	int isPrintDetails=0;

	indexOfRecursion++;

	if (isPrint) {
		fprintf(*fileLog,"treeBuildBasedOnQuartets started: nNodeNamesIntersting=%d, indexOfRecursion=%d\n", nNodeNamesIntersting, indexOfRecursion);
		printf("treeBuildBasedOnQuartets started: nNodeNamesIntersting=%d, indexOfRecursion=%d\n", nNodeNamesIntersting, indexOfRecursion);

		int i,j,k;
		printArrayIntToScreen("idNodeNamesIntersting", nNodeNamesIntersting, idNodeNamesIntersting);
		printArrayIntToFile(fileLog, "idNodeNamesIntersting", nNodeNamesIntersting, idNodeNamesIntersting); 

		printf("4\n");
		quartet_PrintToScreen(quartetHead, 0);

		if (!isFirstTime) {
			printArrayIntToScreen("adressOfIdInInteresting", idMax, adressOfIdInInteresting);
			printArrayIntToFile(fileLog, "adressOfIdInInteresting", idMax, adressOfIdInInteresting); 

			printArray1DIntToScreen("isTogetherInQuartet1D", nNodeNamesIntersting, isTogetherInQuartet1D); 
			printArray1DIntToFile(fileLog, "isTogetherInQuartet1D", nNodeNamesIntersting, isTogetherInQuartet1D);
		};
		PressAnyKey();
	};

	*isOk=1;

	if (nNodeNamesIntersting < 1) {
		printf("NB! nNodeNamesIntersting=%d<1\n",nNodeNamesIntersting);
		fprintf(*fileLog,"NB! nNodeNamesIntersting=%d<1\n",nNodeNamesIntersting);
		(*isOk)=0;
		return;
	};

	if (isFirstTime) {

		make_adressInListOfId(idMax, idNodeNamesIntersting, nNodeNamesIntersting, adressOfIdInInteresting);	

		treeBuildBasedOnQuartets_firstTime_reviseQuartets_isTogetherInQuartet1D(&quartetHead, idNodeNamesIntersting, nNodeNamesIntersting, adressOfIdInInteresting, isTogetherInQuartet1D);

		if (isPrint) {
			printArrayIntToScreen("adressOfIdInInteresting->Set", idMax, adressOfIdInInteresting);
			printArrayIntToFile(fileLog, "adressOfIdInInteresting->Set", idMax, adressOfIdInInteresting);
			printArray1DIntToScreen("isTogetherInQuartet1D->Set", nNodeNamesIntersting, isTogetherInQuartet1D);
			printArray1DIntToFile(fileLog, "isTogetherInQuartet1D->Set", nNodeNamesIntersting, isTogetherInQuartet1D);

			printf("Making of adressOfIdInInteresting and isTogetherInQuartet1D: Finished.\n");

			PressAnyKey();
		};		
	}

	if ((nNodeNamesIntersting < 4) || (!quartetHead)) {

		(*fatherOfSubtree) = starTree(idNodeNamesIntersting, nNodeNamesIntersting, nNodesInSubTree);

		if (isPrintDetails) {

			printf("treeBuildBasedOnQuartets: STAR-tree caused by simple case:\n");
			tree_PrintToScreen_IdInsteadName_NoLengths(*fatherOfSubtree); 
			printf("treeBuildBasedOnQuartets: simple case...Finished, indexOfRecursion=%d\n", indexOfRecursion);
		}
		return;
	};

	int nClusters;
	int* idClusterForIntersting = (int*) calloc (nNodeNamesIntersting+1, sizeof(int));

	treeBuildBasedOnQuartets_clustering(quartetHead, idNodeNamesIntersting, nNodeNamesIntersting, adressOfIdInInteresting, isOk, isTogetherInQuartet1D, fileLog, isPrint, isPrintDetails, &nClusters, idClusterForIntersting);

	int *nNodeNamesInterstingCluster = (int *)malloc((nClusters+1)*sizeof(int));
	int **idNodeNamesInterstingCluster= (int **)malloc((nClusters+1)*sizeof(int *));
	int **adressOfIdInInterestingCluster= (int **)malloc((nClusters+1)*sizeof(int *));
	int *idNodeNameShouldBeReplacedByRootCluster = (int *)malloc((nClusters+1)*sizeof(int));

	int **isTogetherInQuartet1DCluster = (int **)malloc((nClusters+1)*sizeof(int *));
	quartet** quartetHeadCluster = (quartet **)malloc((nClusters+1)*sizeof(quartet *));

	treeBuildBasedOnQuartets_ArraysForClusters_partI(nClusters, idClusterForIntersting, nNodeNamesIntersting, idNodeNamesIntersting, idMax, fileLog, isPrint, nNodeNamesInterstingCluster,  idNodeNamesInterstingCluster, adressOfIdInInterestingCluster, idNodeNameShouldBeReplacedByRootCluster); 

	treeBuildBasedOnQuartets_ArraysForClusters_partII(&quartetHead, nClusters, idClusterForIntersting, adressOfIdInInteresting, idNodeNameShouldBeReplacedByRootCluster,  nNodeNamesInterstingCluster, adressOfIdInInterestingCluster, idNodeNamesInterstingCluster, idMax, fileLog, isPrint, isTogetherInQuartet1DCluster, &quartetHeadCluster);

	if (isPrintDetails) {
		fprintf(*fileLog,"treeBuildBasedOnQuartets with %d, iMin=%d  --> p1\n",nNodeNamesIntersting,idNodeNamesIntersting[0]);
	};

	treeNode* t = NULL;
	treeNode* tt  = newNode(NULL, 0);
	*fatherOfSubtree = tt;
	(*fatherOfSubtree) -> idNodeName = -1;		
	(*fatherOfSubtree) -> isLeaf = 0;
	*nNodesInSubTree = 1;
	if (isPrint) printf("treeBuildBasedOnQuartets: n(indexOfRecursion,idCluster)=n(%d,Root)=1\n",indexOfRecursion);

	int idCluster;
	for (idCluster = 0; idCluster < nClusters; idCluster++) 
	{

		if (isPrintDetails) 
		{
			fprintf(*fileLog,"treeBuildBasedOnQuartets with %d, iMin=%d  --> idCluster=%d, nClusters=%d\n",nNodeNamesIntersting,idNodeNamesIntersting[0],idCluster,nClusters);
		}

		if (nNodeNamesInterstingCluster[idCluster]==1) 
		{
			(*nNodesInSubTree)++;
			if (isPrint) printf("treeBuildBasedOnQuartets: n(indexOfRecursion,idCluster)=n(%d,%d)=1\n",indexOfRecursion,idCluster);
			t = newNode(*fatherOfSubtree, *nNodesInSubTree);
			t -> idNodeName = idNodeNamesInterstingCluster[idCluster][0];
			t -> isLeaf = 1;
			if (isPrint) {
				printf("treeBuildBasedOnQuartets: tree for cluster = single node with (t -> idNodeName)=%d\n",t -> idNodeName);
			}
		}
		else 
		{
			int n;
			treeBuildBasedOnQuartets(quartetHeadCluster[idCluster], idNodeNamesInterstingCluster[idCluster], nNodeNamesInterstingCluster[idCluster], adressOfIdInInterestingCluster[idCluster], isTogetherInQuartet1DCluster[idCluster], idNodeNameShouldBeReplacedByRootCluster[idCluster], idMax, isOk, fileLog, isPrint, 0, indexOfRecursion, &t, &n);

			*nNodesInSubTree = (*nNodesInSubTree) + n;
			if (isPrint) printf("treeBuildBasedOnQuartets: n(indexOfRecursion,idCluster)=n(%d,%d)=%d\n",indexOfRecursion,idCluster,n);
			if (*isOk==0) 
			{
				fprintf(*fileLog,"NB! Continue to exit because of error\n");
				return;
			};

		};
		if (isPrint) {
			printf("treeBuildBasedOnQuartets: tree for cluster...\n");
			tree_PrintToScreen_IdInsteadName_NoLengths(t); 

		}

		makeNodeBeOlderstSon(*fatherOfSubtree, t);

		free(idNodeNamesInterstingCluster[idCluster]);
		idNodeNamesInterstingCluster[idCluster]=NULL;

			free(adressOfIdInInterestingCluster[idCluster]);
			adressOfIdInInterestingCluster[idCluster]=NULL;

			free(idNodeNamesInterstingCluster[idCluster]);
			idNodeNamesInterstingCluster[idCluster]=NULL;
			free(isTogetherInQuartet1DCluster[idCluster]);
			isTogetherInQuartet1DCluster[idCluster]=NULL;
	};

	free(nNodeNamesInterstingCluster);

	nNodeNamesInterstingCluster=NULL;

	free(idNodeNamesInterstingCluster);

	idNodeNamesInterstingCluster=NULL;

	free(adressOfIdInInterestingCluster);

	adressOfIdInInterestingCluster=NULL;

	free(isTogetherInQuartet1DCluster);

	isTogetherInQuartet1DCluster=NULL;

	free(idClusterForIntersting);
	idClusterForIntersting=NULL;

	free(idNodeNameShouldBeReplacedByRootCluster);
	idNodeNameShouldBeReplacedByRootCluster=NULL;

	treeNode** nodeInternalFirstSuchThatNotRootAndHaveNoBrothers;
	int nNodesInSubTreePP;

	int bOk = testSubTreeForFatherSonRelationship(*fatherOfSubtree, NULL, nodeInternalFirstSuchThatNotRootAndHaveNoBrothers, 1, &nNodesInSubTreePP, 1);  
	if (!bOk) {
		printf("treeBuildBasedOnQuartets ERROR: bOk = testSubTreeForFatherSonRelationship()=False\n");
		exit(4);
	}

	if (isPrint) {
		if (nNodesInSubTreePP != *nNodesInSubTree) {
			tree_PrintToScreen_IdInsteadName_NoLengths(*fatherOfSubtree); 
			printf("treeBuildBasedOnQuartets: nNodesInSubTreePP = %d != %d = *nNodesInSubTree\n", nNodesInSubTreePP, *nNodesInSubTree);
			PressAnyKey();

		}
	}

	reRootTree_run(idNodeNameShouldBeReplacedByRoot, isPrint, fatherOfSubtree, "treeBuildBasedOnQuartets", idMax, nNodesInSubTree);  

	if (isPrint) {
		printf("treeBuildBasedOnQuartets: Finished, indexOfRecursion=%d\n", indexOfRecursion);
	}
	return;
}
