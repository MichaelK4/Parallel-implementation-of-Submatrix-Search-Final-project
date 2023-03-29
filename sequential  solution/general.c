#include <stdio.h>
#include <stdlib.h>
#include "general.h"


void freeAllocations(Picture* pictures, int numPictures, Object* objects, int numObjects)
{
	for (int i = 0; i < numPictures; i++)
	{
		// free picMat for picture i
		free(pictures[i].picMat);
	}
	// free allocated memory for pictures
	free(pictures);
	for (int i = 0; i < numObjects; i++)
	{
		// free objMat for object i
		free(objects[i].objMat);
	}
	// free allocated memory for objects
	free(objects);
}

int writeToOutputFile(const Picture* pictures, int numPictures)
{
	FILE* fp = fopen(OUTPUT_FILE, "w");
	if (!fp)
	{
		printf("Could not open file\n");
		return 0;
	}
	for (int i = 0; i < numPictures; i++)
	{
		Picture pic = pictures[i];
		int picID = pictures[i].picID;
		if (pictures[i].numObj >= NUM_OF_DIFF_OBJ)
		{
			if (pic.foundObj[0] != pic.foundObj[1] && pic.foundObj[1] != pic.foundObj[2])
			{
				fprintf(fp, "Picture %d: found Objects: %d Position(%d,%d); %d Position(%d,%d); %d Position(%d,%d);\n",
					picID, pic.foundObj[0], pic.positions[0][0], pic.positions[0][1],
					pic.foundObj[1], pic.positions[1][0], pic.positions[1][1],
					pic.foundObj[2], pic.positions[2][0], pic.positions[2][1]);
				printf("Saving info for Picture %d to file\n", picID);
				printf("Picture %d: found Objects: %d Position(%d,%d); %d Position(%d,%d); %d Position(%d,%d);\n",
					picID, pictures[i].foundObj[0], pic.positions[0][0], pic.positions[0][1],
					pic.foundObj[1], pic.positions[1][0], pic.positions[1][1],
					pic.foundObj[2], pic.positions[2][0], pic.positions[2][1]);
			}
		}
		if (pic.numObj < NUM_OF_DIFF_OBJ || (pic.foundObj[1] == pic.foundObj[0] || pic.foundObj[1] == pic.foundObj[2] || pic.foundObj[2] == pic.foundObj[0]))
		{
			fprintf(fp, "Picture %d: No three different Objects were found\n", picID);
			printf("Saving info for Picture %d to file\n", picID);
			printf("Picture %d: No three different Objects were found\n", picID);
		}

	}
	fclose(fp);
	return 1;
}





int readObjects(Object* objects, int numObjects, FILE* fp)
{
	printf("---------------------\n");
	printf("Starting to Read Objects -> Number of objects %d:\n", numObjects);
	for (int i = 0; i < numObjects; i++)
	{
		// get object i ID
		if (fscanf(fp, "%d", &objects[i].objID) != 1)
		{
			printf("Failed to read Object ID\n");
			return 0;
		}
		// get object i Dim
		if (fscanf(fp, "%d", &objects[i].objDim) != 1)
		{
			printf("Failed to read Object Dim\n");
			return 0;
		}

		int dim = objects[i].objDim * objects[i].objDim;

		// allocate memory for the mat of object i
		objects[i].objMat = (int*)malloc(dim * sizeof(int));
		if (!objects[i].objMat)
		{
			printf("Failed to allocate memory for object\n");
			return 0;
		}

		// start reading object items
		printf("Reading Object %d\n", objects[i].objID);
		for (int j = 0; j < dim; j++)
		{
			if (fscanf(fp, "%d", &objects[i].objMat[j]) != 1)
			{
				printf("Failed to read Object item at index %d\n", j);
				return 0;
			}
		}
		printf("Success in reading Object %d with Dim %d\n", objects[i].objID, objects[i].objDim);
	}
	return 1;
}

int readPictures(Picture* pictures, int numPictures, FILE* fp)
{
	printf("---------------------\n");
	printf("Starting to Read Pictures -> Number of pictures %d:\n", numPictures);
	for (int i = 0; i < numPictures; i++)
	{
		// get picture i ID
		if (fscanf(fp, "%d", &pictures[i].picID) != 1)
		{
			printf("Failed to read Picture ID\n");
			return 0;
		}
		// get picture i Dim
		if (fscanf(fp, "%d", &pictures[i].picDim) != 1)
		{
			printf("Failed to read Picture Dim\n");
			return 0;
		}

		int dim = pictures[i].picDim * pictures[i].picDim;

		// allocate memory for the mat of picture i
		pictures[i].picMat = (int*)malloc(dim * sizeof(int));
		if (!pictures[i].picMat)
		{
			printf("Failed to allocate memory for picture\n");
			return 0;
		}

		// start reading picture items
		printf("Reading Picture %d\n", pictures[i].picID);
		for (int j = 0; j < dim; j++)
		{
			if (fscanf(fp, "%d", &pictures[i].picMat[j]) != 1)
			{
				printf("Failed to read Picture item at index %d\n", j);
				return 0;
			}
		}
		printf("Success in reading Picture %d with Dim %d\n", pictures[i].picID, pictures[i].picDim);
	}
	return 1;
}


int readInputFile(Picture** pictures, int* numPictures, Object** objects, int* numObjects, float* matching_value)
{
	FILE* fp = fopen(INPUT_FILE, "r");
	if (!fp)
	{
		printf("Could not open the file\n");
		return 0;
	}
	printf("---------------------\n");
	printf("Starting to Read Input File\n");

	//read the matching value first should be 0.100000 ~ 0.1
	if (fscanf(fp, "%f", matching_value) != 1)
	{
		printf("Failed to read matching value\n");
		return 0;
	}
	printf("Matching value: %f\n", *matching_value);

	// read number of pictures should be 5
	if (fscanf(fp, "%d", numPictures) != 1)
	{
		printf("Failed to read number of pictures\n");
		return 0;
	}
	// allocate memory for pictures
	*pictures = (Picture*)malloc(*numPictures * sizeof(Picture));
	if (!*pictures)
	{
		printf("Failed to allocate memory for pictures\n");
		return 0;
	}
	// reading from the file the pictures: we read the ID and the dim of each picture
	if (!readPictures(*pictures, *numPictures, fp))
	{
		printf("Failed reading pictures from file\n");
		free(*pictures);
		fclose(fp);
		return 0;
	}

	//read number of objects should be 8 objects 
	if (fscanf(fp, "%d", numObjects) != 1)
	{
		printf("Failed to read number of objects\n");
		return 0;
	}
	// allocate memory for objects
	*objects = (Object*)malloc(*numObjects * sizeof(Object));
	if (!*objects)
	{
		printf("Failed to allocate memory for objects\n");
		return 0;
	}
	//reading from the file the objects: we read the ID and the dim of each object
	if (!readObjects(*objects, *numObjects, fp))
	{
		printf("Failed to read objects from file\n");
		free(*objects);
		fclose(fp);
		return 0;
	}
	printf("---------------------\n");
	fclose(fp);
	return 1;
}
