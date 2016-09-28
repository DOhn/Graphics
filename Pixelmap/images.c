#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sameType(FILE *fh, FILE *out, char* get) {
	// Iterates line by line through the file and rewrites it into the new file.
	while(fgets(get, 100, fh) != NULL) {
		fputs(get, out);
	}
}

void convertToP3(FILE *fh, FILE *out, char* get, int size) {
	// Use new buffer to get unsigned char instead of char
	unsigned char* buff = (unsigned char*)get;
	unsigned char num;
	int i;

	// Iterates through the read file by each character
	while(fread(buff, size, 1, fh)) {
		for (i=0; i<size; i++) {
			num = buff[i];
			// Rewrites the data as ASCII
			fprintf(out, "%u \n", num);
		}
	}
}

void convertToP6(FILE *fh, FILE *out, char* get, int size) {
	unsigned char num;
	char* tok;
	
	// Iterates through the read file to the end of the file
	while(fread(get, size, 1, fh)) {
		// strtok is used to get rid of whitespace and \n's within the file
		tok = strtok(get, " \n");
		while(tok) {
			num = (unsigned char)atoi(tok);
			// Rewrites data as binary
			fputc(num, out);
			tok = strtok(NULL, " \n");
		}
	}
}

int main(int argc, char *argv[]) {
	// Checks if correct amount of arugments are inserted.
	if (argc != 4) {
		printf("Error: Incorrect amount of arguments.  Three is needed.\n");
		return(1);
	}

	// Checks if first argument is valid.
	if (strncmp(argv[1], "6", 1) == 1 || strncmp(argv[1], "3", 1) == 1) {
		printf("Error: First Argument must be in P3 or P6.\n");
		return(1);
	}

	// Opens the read file and checks if it's valid
	FILE *fh;
	fh = fopen(argv[2], "r");

	if(fh == NULL) {
		perror("Error: Couldn't open file.");
		return(-1);
	}

	// Declares values used within the program
	int i, color;
	int op_type;
	char type[1];

	// Opens the write file and creates it if it doesn't exist
	FILE *out;
	out = fopen(argv[3], "w+");

	// get is used to iterate through the read file
	char* get = malloc(500); 
	int size = 500;

	// While loop that obtains the header information
	while(1) {	
		get = fgets(get, 100, fh);	
		
		// Checks if it starts with magic number and what the file is
		if (strcmp(get, "3\n") == 0 || strcmp(get, "6\n") == 0) {
			type[0] = get[0];
			if (strncmp(type, argv[1], 1) == 0) {
				fputs(get, out);
				get = fgets(get, 100, fh);
				op_type = 0;
			} 
			else {
				fputs(argv[1], out);
				fputs("\n", out);
				get = fgets(get, 100, fh);
				op_type = 1;
			}
		}

		// Doesn't keep comments
		if (get[0] == '#') {
			continue;
		}
		else {
			fputs(get, out);
		}

		// Gets the color and breaks out of the loop
		if (strcmp(get, "255\n") == 0) {
			color = atoi(get);
			break;
		}
	}

	// Determines if it's making the file to the same type
	if (op_type == 0) {
		sameType(fh, out, get);
	} 
	else {
		// Checks what the user wants to convert the .ppm file too
		if (strncmp("6", argv[1], 1) == 0) {
			convertToP6(fh, out, get, size);
		} 
		else {
			convertToP3(fh, out, get, size);
		}
	}

	// Closes the read and write file, then exits the program
	fclose(out);
	fclose(fh);
	return(0);
}