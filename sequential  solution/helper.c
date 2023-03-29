#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "helper.h"



float matching(int* picMat, int* objMat, int picDim, int objDim, int p, int o)
{
	float matching_val = 0.0;
	int objSize = objDim * objDim;
	float diff = 0.0;
	for (int i = 0; i < objDim; i++)
	{
		for (int j = 0; j < objDim; j++)
		{
			int picIndex = (p + i) * picDim + (o + j);
			int objIndex = i * objDim + j;
			int picPix = picMat[picIndex];
			int objPix = objMat[objIndex];
			diff = (float)abs((picPix - objPix) / picPix);
			matching_val += diff;
		}
	}
	matching_val /= objSize;
	return matching_val;
}



void findMatching(Picture* pictures, int numPictures, Object* objects, int numObjects, float matching_value)
{
	for (int i = 0; i < numPictures; i++)
	{
		Picture* pic = &pictures[i];
		int picDim = pictures[i].picDim;
		int* picMat = pictures[i].picMat;
		//int picID = pictures[i].picID;
		pic->numObj = 0;
		for (int j = 0; j < numObjects; j++)
		{
			int objDim = objects[j].objDim;
			int* objMat = objects[j].objMat;
			int objID = objects[j].objID;
			int found = 0;
			int maxI, maxJ;
			maxI = maxJ = picDim - objDim;


			for (int p = 0; p <= maxI && pic->numObj < NUM_OF_DIFF_OBJ; p++)
			{
				for (int o = 0; o <= maxJ && pic->numObj < NUM_OF_DIFF_OBJ; o++)
				{
					float tempMatching = matching(picMat, objMat, picDim, objDim, p, o);

					if (tempMatching <= matching_value) //found
					{

						//printf("Object %d: found in picture %d at position (%d,%d)\n",objID, picID, p, o);
						if (pic->numObj == 0 || pic->foundObj[pic->numObj - 1] != objID)
						{
							pic->foundObj[pic->numObj] = objID;
							pic->positions[pic->numObj][0] = p;
							pic->positions[pic->numObj][1] = o;
							pic->numObj++;
						}
						if (pic->numObj == NUM_OF_DIFF_OBJ)
						{
							found = 1;
							break;
						}
					}
				}
				if (found)
					break;
			}
			if (numObjects - j - 1 < NUM_OF_DIFF_OBJ - pic->numObj)
				break;
			if (found)
				break;
		}

	}
}
