#ifndef GENERAL_H
#define GENERAL_H
#include "helper.h"
#define INPUT_FILE "input.txt"
#define OUTPUT_FILE "output.txt"


int readInputFile(Picture** pictures, int* numPictures, Object** objects, int* numObjects, float* matching_value);

int readPictures(Picture* pictures, int numPictures, FILE* fp);
int readObjects(Object* objects, int numObjects, FILE* fp);

int writeToOutputFile(const Picture* pictures, int numOfPictures);

void freeAllocations(Picture* pictures, int numPictures, Object* objects, int numObjects);

void combinePictureStruct(Picture* pictures, Picture* picturesForMaster, Picture* picturesForWorker, int numPictures, int numPicturesForWorker, int numPicturesForMaster);

#endif
