#include "struct.h"

void ppmMaker(Img *image, FILE* fput, int num) {
	
	if (num == 6)
	{
		fprintf(fput, "P%d\n%d %d\n%d\n", num, image->width, image->height, 255);

		if (fwrite(image->color, 1, (size_t)image->width * image->height * 3, fput) != image->width * image->height *3)
		{
			fprintf(stderr, "Error: Couldn't properly write the PPM file\n");
			exit(1);
		}
	}
}