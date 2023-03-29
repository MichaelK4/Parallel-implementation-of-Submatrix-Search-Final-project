#ifndef HELPER_H
#define HELPER_H


#define NUM_OF_DIFF_OBJ 3
#define NUM_THREADS 4

typedef struct Picture
{
	int picID;
	int picDim;
	int* picMat;
	int foundObj[NUM_OF_DIFF_OBJ];
	int numObj;
	int positions[NUM_OF_DIFF_OBJ][2];
}Picture;

typedef struct Object
{
	int objID;
	int objDim;
	int* objMat;
}Object;



void findMatching(Picture* pictures, Object* objects, int numObjects, float matching_value, int rank);
float matching(int* picMat, int* objMat, int picDim, int objDim, int p, int o);


#endif
