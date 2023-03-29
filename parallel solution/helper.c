#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "helper.h"



float matching(int* picMat, int* objMat, int picDim, int objDim, int p, int o) // running on all the obj to find the avg matching value
{
	float matching_val = 0.0;
	int objSize = objDim * objDim; // the hole size of the obj
	float diff = 0.0;
	for (int i = 0; i < objDim; i++)
	{
		for (int j = 0; j < objDim; j++)
		{
			int picIndex = (p + i) * picDim + (o + j); // getting the index of the picture
			int objIndex = i * objDim + j; //geting the index of the object
			int picPix = picMat[picIndex]; // the pixel of the picture
			int objPix = objMat[objIndex]; // the pixel of the object
			diff = (float)abs((picPix - objPix) / picPix); // diff = abs((p - o) / p)
			matching_val += diff; // diff for each pixel += the matching value for the object
		}
	}
	matching_val /= objSize; // matching value being div by the obj size as of the avg matching val per one pixel
	return matching_val;
}



void findMatching(Picture* pictures, Object* objects, int numObjects, float matching_value, int rank)
{
		Picture* pic = pictures;
		int picDim = pictures->picDim;
		int* picMat = pictures->picMat;
		int picID = pictures->picID;
		pic->numObj = 0;
		printf("Rank %d searching picture %d\n", rank, picID);	
		for (int j = 0; j < numObjects; j++)
		{
			int objDim = objects[j].objDim;
			int* objMat = objects[j].objMat;
			int objID = objects[j].objID;
			int found = 0; // found is playing a boolean role in this function
			int maxI, maxJ;
			maxI = maxJ = picDim - objDim; // the max pixel we can run for the picture
			// for example if the pic is with dim 100 and the obj is with dim 10 the the maxI and maxJ will be 90


			for (int p = 0; p <= maxI && pic->numObj < NUM_OF_DIFF_OBJ; p++) // running up to maxI that there can be and if number of unique obj is 3 finish
			{
				for (int o = 0; o <= maxJ && pic->numObj < NUM_OF_DIFF_OBJ; o++) // same us above just for maxJ
				{
					float tempMatching = matching(picMat, objMat, picDim, objDim, p, o);

					if (tempMatching <= matching_value) // found obj
					{

						printf("\tObject %d: found in picture %d at position (%d,%d)\n",objID, picID, p, o);
						if (pic->numObj == 0 || pic->foundObj[pic->numObj - 1] != objID)
						{
							pic->foundObj[pic->numObj] = objID; // saving the obj id in the pic struct in the array of foundObj 
							pic->positions[pic->numObj][0] = p; // saving cord x
							pic->positions[pic->numObj][1] = o; // saving cord y
							pic->numObj++;
						}
						if (pic->numObj == NUM_OF_DIFF_OBJ) // found 3 unique obj in pic
						{
							found = 1;
							break; // breaking the third for loop
						}
						
					}
				}
				if (found) // breaking the seconf for loop
					break;
			}
			if (numObjects - j - 1 < NUM_OF_DIFF_OBJ - pic->numObj) // if there are not left too many obj to search break e.g if we found 1 obj and only one left to find just break
				break;
			if (found) // breaking the first for loop no need to search for more obj
				break;
		}
}
