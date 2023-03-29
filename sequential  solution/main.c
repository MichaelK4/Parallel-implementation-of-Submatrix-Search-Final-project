#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "general.h"


int main()
{
	float matching_value = 0.0;
	int numPictures = 0, numObjects = 0;
	Picture* pictures = NULL;
	Object* objects = NULL;
	clock_t start, end;
	double execTime = 0.0;
	printf("Welcome to the sequential program\n");
	if (!readInputFile(&pictures, &numPictures, &objects, &numObjects, &matching_value))
	{
		printf("Failed to read from Input file\n");
		return 0;
	}

	printf("Starting to search pictures for objects:\n");
	// find the matching for each object in the picture
	start = clock();
	findMatching(pictures, numPictures, objects, numObjects, matching_value);
	end = clock();
	if (!writeToOutputFile(pictures, numPictures))
	{
		printf("Failed to write to output file\n");
		return 0;
	}

	freeAllocations(pictures, numPictures, objects, numObjects);

	execTime = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("---------------------\n");
	printf("Exec time of sequential program: %e or %f sec\n", execTime, execTime);
	printf("---------------------\n");
	return 0;
}

