#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>
#include "general.h"
#include "MPI_Functions.h"

int main(int argc, char** argv)
{
	int rank, numProcs;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
	
	if(numProcs != 2)
	{
		printf("The program should run with two processes only\n");
		MPI_Abort(MPI_COMM_WORLD, 1);
		return 0;

	}
	
	if(rank == MASTER)
	{
		float matching_value = 0.0;
		Picture* pictures = NULL;
		Object* objects = NULL;
		int numPictures = 0, numObjects = 0;
		double start = 0.0, end = 0.0;
		double execTime = 0.0;
		
		//int tag = 0;
		
		int numPicturesForWorker = 0, numPicturesForMaster = 0;
		Picture* picturesForWorker = NULL;
		Picture* picturesForMaster = NULL;
		
		
		
		printf("Welcome to the parallel program\n");
		if (!readInputFile(&pictures, &numPictures, &objects, &numObjects, &matching_value)) // reading input file
		{
			printf("Failed to read from Input file\n");
			return 0;
		}
		

		int numPicturesPerRank = numPictures / numProcs; // 2
		int remainingPicture = numPictures % numProcs; // 1

		numPicturesForMaster = numPicturesPerRank * (numProcs - 1) + remainingPicture;; // 3
		numPicturesForWorker = numPictures - numPicturesForMaster; // 2
		


		// allocate some pictures for master and some for worker to send to worker, the heavy pictures will remain in rank 0
		picturesForWorker = (Picture*)malloc(numPicturesForWorker * sizeof(Picture));
		picturesForMaster = (Picture*)malloc(numPicturesForMaster * sizeof(Picture));


		int j = 0;
		int k = 0;
		for(int i = 0; i < numPictures; i++)
		{
			
			for (int j = 0; j < NUM_OF_DIFF_OBJ; j++) // init -1 in all foundObj and positions
			{
				pictures[i].foundObj[j] = -1;
				for (int k = 0; k < 2; k++)
				{
					pictures[i].positions[j][k] = -1;
				}
			}	
			
			if (j < numPicturesForMaster)
			{
				picturesForMaster[j] = pictures[i];
				j++;
			}
			else
			{
				picturesForWorker[k] = pictures[i];
				k++;
			}
			
		}
		
		
		
		printf("Starting to search pictures for objects:\n");
		// find the matching for each object in the picture
		start = MPI_Wtime();
		printf("---------------------\n");
		printf("Starting to run on search in rank 0\n");
		#pragma omp parallel for num_threads(2)
		for(int i = 0; i < numPicturesForMaster; i++)
		{			
			findMatching(&picturesForMaster[i], objects, numObjects, matching_value, MASTER);
		}
			
		
		
		printf("---------------------\n");
		printf("Sending info from rank 0  to rank 1:\n");	
		
		MPI_Send(&numPicturesForWorker, 1, MPI_INT, WORKER, 0, MPI_COMM_WORLD);
		MPI_Send(&numObjects, 1, MPI_INT, WORKER, 0, MPI_COMM_WORLD);
		MPI_Send(&matching_value, 1, MPI_FLOAT, WORKER, 0, MPI_COMM_WORLD);
		
		
		printf("---------------------\n");
		printf("Sending %d pictures and %d objects from rank 0  to rank 1:\n", numPicturesForWorker, numObjects);
		
		MPI_SendObjectsToWorker(objects, numObjects);
		MPI_SendPicturesToWorker(picturesForWorker, numPicturesForWorker);
		
		printf("Finished sending info from rank 0  to rank 1:\n");
		printf("---------------------\n");

		
		MPI_RecvResultsFromWorker(picturesForWorker, numPicturesForWorker);
		end = MPI_Wtime();
		printf("Received Results from rank 1 to rank 0 for %d number of pictures\n", numPicturesForWorker);
		
		printf("---------------------\n");
		printf("Getting pictures to one struct\n");
		combinePictureStruct(pictures, picturesForMaster, picturesForWorker, numPictures, numPicturesForWorker, numPicturesForMaster);
		printf("Finished getiing to struct\n");
		printf("---------------------\n");
		
		
		if (!writeToOutputFile(pictures, numPictures))
		{
			printf("Failed to write to output file\n");
			return 0;
		}
		execTime = end - start;
		printf("---------------------\n");
		printf("Exec time of parallel program: %e or %f sec\n", execTime, execTime);
		printf("---------------------\n");
			
			
		printf("free allocation for rank 0\n");
			
		freeAllocations(pictures, numPictures, objects, numObjects);
			
		printf("finished free allocation rank 0\n");
		
		printf("---------------------\n");

		printf("last free allocation for rank 0\n");
		free(picturesForWorker);
		free(picturesForMaster);
		printf("finished last free allocation rank 0\n");	
		printf("---------------------\n");
		
	}
	else if(rank == WORKER)
	{
		float* matching_value = (float*)malloc(sizeof(float));
		int* numPicturesForWorker = (int*)malloc(sizeof(int));; 
		int* numObjects = (int*)malloc(sizeof(int));;
		
		
		MPI_Recv(numPicturesForWorker, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // 3
		MPI_Recv(numObjects, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // 8
		MPI_Recv(matching_value, 1, MPI_FLOAT, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // 0.10000
		
		printf("---------------------\n");
		
		Object* objects = MPI_RecvObjectsFromMaster(*numObjects);
		Picture* picturesForWorker = MPI_RecvPicturesFromMaster(*numPicturesForWorker); 
		
		printf("Received %d objects and %d pictures and also matching_value %f that rank 1 will use form rank 0\n", *numObjects, *numPicturesForWorker, *matching_value);
		
		printf("---------------------\n");
		
		
		printf("---------------------\n");
		printf("Starting to run on search in rank 1\n");
		#pragma omp parallel for num_threads(2)
		for(int i = 0; i < *numPicturesForWorker; i++)
		{			
			findMatching(&picturesForWorker[i], objects, *numObjects, *matching_value, WORKER);
		}
		printf("---------------------\n");
		
		printf("Sending results from rank 1 to rank 0\n");
		
		MPI_SendResultsToMaster(picturesForWorker, *numPicturesForWorker);
		
		printf("Finished Sending results from rank 1 to rank 0\n");
		printf("---------------------\n");
		
		
		printf("free allocation for rank 1\n");
		freeAllocations(picturesForWorker, *numPicturesForWorker, objects, *numObjects);
		free(numObjects);
		free(numPicturesForWorker);
		free(matching_value);
		printf("finished free allocation rank 1\n");
		printf("---------------------\n");
	}	
	MPI_Finalize();
	return 0;
}
