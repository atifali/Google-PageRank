#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "engine.h"
#include "PageRank.h"
     
int main (void) {

	/* Variables for the web file reading and parsing */
	int i; // iterators for the file parsing
	int j;
	int error = 0;
	FILE * web_file = NULL; //a pointer to the file that is to be parsed
	int dimension = 0; //varibale for the dimension of the input web matrix
	double ** connectivityMat = NULL; // the 2D matrix for the results of the file reading
	/* Variables for the MATLAB related PageRank procedure */
    Engine *ep = NULL; // A pointer to a MATLAB engine object
	mxArray *connectivity = NULL; // mxArray is the fundamental type underlying MATLAB data 
	mxArray *result = NULL; // the result of the PageRank procedure 

	/* Procedure for reading the web file */
	// open the file for the parsing
	error = fopen_s( &web_file, WEB, "r" );
	// check for possibe errors while opening
	if (error) {
		fprintf(stderr, "\nERROR: Unable to open file: %s\n", WEB);
		system("pause");
		return 1;
	}
	// get the dimension of the web matrix and copy the matrix into the memory
	if (web_file) {
		dimension = get_web_dimension(web_file);
		printf("The dimension of the web matrix is: %d x %d\n", dimension, dimension);
		connectivityMat = parse_web(web_file, dimension);
		printf("\nThe web matrix is as follows:\n");
		for (i = 0; i < dimension; i++) {
			for (j = 0; j < dimension; j++) {
				printf("%d ", (int)connectivityMat[i][j]);
			}
			printf("\n");
		}
	}
	else {
		fprintf(stderr, "\nERROR: Unable to parse web file: %s\n", WEB);
		system("pause");
		return 1;
	}

	/* Starts a MATLAB process to open the engine */
	printf("\nStarting the MATLAB Engine...\n");
	if (!(ep = engOpen(NULL))) {
		fprintf(stderr, "\nERROR: Can't start MATLAB engine.\n");
		system("pause");
		return 1;
	}
	printf("\nMATLAB Engine Started!\n");

	// Allocate memory for the PageRank Process and copy the appropriate values for the matrices
	printf("\nAllocating memory for the MATLAB input matrix...\n");
	connectivity = mxCreateDoubleMatrix(dimension, dimension, mxREAL);
	for (j = 0; j < dimension; ++j) {
		for (i = 0; i < dimension; ++i) {
			memcpy((void *)(mxGetPr(connectivity)+i+(dimension*j)), (void *)(&connectivityMat[i][j]), sizeof(double));
		}
	}
	printf("\nDone allocating the input values at the appropriate locations!\n");

	// place web connectivity array into MATLAB and diagnose for any errors
	printf("\nPutting the input variable into MATLAB workspace...\n");
	if (engPutVariable(ep, "ConnectivityMatrix", connectivity)) {
		fprintf(stderr, "\nERROR: Cannot write connectivity array to MATLAB \n");
		system("pause");
		exit(1); // Same as return 1;
	}
	printf("\nDone writing input matrix into MATLAB workspace!\n");

	printf("\nStarting the MATLAB PageRank procedure...\n");
	// Carry out the MATLAB Commands for the PageRank Procedure and diagnose for any errors
	if (engEvalString(ep,	"[rows, columns] = size(ConnectivityMatrix);"
							"dimension = size(ConnectivityMatrix, 1);"
							"columnsums = sum(ConnectivityMatrix, 1);"
							"p = 0.85;"
							"zerocolumns = find(columnsums~= 0);"
							"D = sparse(zerocolumns, zerocolumns, 1./ columnsums(zerocolumns), dimension, dimension);"
							"StochasticMatrix = ConnectivityMatrix * D;"
							"[row, column] = find(columnsums == 0);"
							"StochasticMatrix(:, column) = 1. / dimension;"
							"Q = ones(dimension, dimension);"
							"TransitionMatrix = p * StochasticMatrix + (1 - p) * (Q / dimension);"
							"pageRank = ones(dimension, 1);"
							"for i=1:100;pageRank = TransitionMatrix * pageRank;end;"
							"pageRank = pageRank / sum(pageRank)")) {
		fprintf(stderr, "\nError calculating the page rank during the MATLAB procedure.\n");
		system("pause");
		exit(1);
	}
	printf("\nDone performing the MATLAB PageRank procedure!\n");

	//result of the PageRank Algorithm
	printf("\nRetrieving the PageRank Result...\n");
	if ((result = engGetVariable(ep, "pageRank")) == NULL) {
		fprintf(stderr, "\nFailed to retrieve the PageRank vector\n");
		system("pause");
		exit(1);
	}
	else {
		size_t sizeOfResult = mxGetNumberOfElements(result);
		int i = 0;
		printf("The result of PageRank is as follows:\n\n");
		printf("NODE	RANK\n");
		printf("---	----\n");
		for (i = 0; i < sizeOfResult; ++i) {
			printf("%d	%f\n", i+1, *(mxGetPr(result) + i));
		}
		printf("\n");
	}

	mxDestroyArray(connectivity);
	mxDestroyArray(result);
	connectivity = NULL;
	result = NULL;
	if (engClose(ep)) {
		fprintf(stderr, "\nERROR: Failed to close MATLAB engine\n");
	}

	system("pause"); // So the terminal window remains open long enough for you to read it
	return 0; // Because main returns 0 for successful completion
}

int get_web_dimension(FILE* web_file) {

	int  dimension = 0;
	char line_buffer[BUFFER];

	dimension = strlen(fgets(line_buffer, BUFFER, web_file));

	/* It 'resets' the file's internal pointer to the
	beginning of the file. */
	fseek(web_file, 0, SEEK_SET);

	/* Checks if text file was created in Windows and contains '\r'
	IF TRUE reduce strlen by 2 in order to omit '\r' and '\n' from each line
	ELSE    reduce strlen by 1 in order to omit '\n' from each line */
	if (strchr(line_buffer, '\r') != NULL) {
		return dimension - 2;
	}
	else {
		return dimension - 1;
	}
}

double ** parse_web (FILE * web_file, int dimension)
{
	/* Variables */
	char line_buffer[BUFFER];
	int row = 0;
	int column = 0;
	double ** connectivityMat = NULL;

	/* Allocates memory for correctly-sized web matrix */
	connectivityMat = (double *)calloc(dimension, sizeof(double));

	for (row = 0; row < dimension; ++row) {
		connectivityMat[row] = (double *)calloc(dimension, sizeof(double));
	}

	/* Copies web file to memory */
	row = 0;
	while (fgets(line_buffer, BUFFER, web_file)) {
		for (column = 0; column < dimension; ++column) {
			//note the type conversion from char to int below
			connectivityMat[row][column] = line_buffer[column] - '0';
		}
		row++;
	}
	return connectivityMat;
}