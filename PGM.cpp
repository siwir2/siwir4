#include<stdio.h>
#include<stdlib.h>
#include<stdlib.h> 
#include<string.h>
#include<netpbm/pgm.h>
#include<math.h>
#include"header.h"

Vektor input_file(char* argv[],int i){
	FILE *input;
	int cols,rows,format;
	gray maxval;							// read returns maxval 
	gray *grayrow;
	int argc = 2;//sizeof(argv)-1;
	pgm_init(&argc,argv);

	input = pm_openr(argv[i]);		// read
	printf("%s einlesen\n",argv[i]);
	
	pgm_readpgminit(input,&cols,&rows,&maxval,&format);
	printf("%d cols \t %d rows \t %d \n",cols, rows,format);
	grayrow = pgm_allocrow(cols);
	
	Vektor inputOne(cols*rows);

	for(int y = 0;y < rows;++y){
		pgm_readpgmrow(input,grayrow,cols,maxval,format);
		for(int x=0;x< cols; ++x){ 
			round((((double)grayrow[x]/(double)255))*4) < 1? grayrow[x]=1: grayrow[x]= round((((double)grayrow[x]/(double)255))*4);
			printf("%d \t",grayrow[x]);
			inputOne.array[y*cols + x] = grayrow[x];
		}
		printf("\n");
	}
	printf("\n\n\n");
	for(int y = 0;y < rows;++y){
		for(int x=0;x< cols; ++x){
			printf("%d \t",inputOne.array[y*cols + x]);	
		}
		printf("\n");
	}
	pgm_freerow(grayrow);
	pm_close(input);

	return inputOne;
}

// 		pgm_writepgmrow(output,grayrow1,cols1,maxval1,0);	// output
void output_file(char** argv,int cols, int rows, gray maxval){	// rows und cols mit uebergeben
	FILE *output;
	gray *outgrayrow;			// alloc row returns type gray
	int argc = sizeof(argv);
	pgm_init(&argc,argv);
	
	output = pm_openw("output.pgm");		// write
	pgm_writepgminit(output,cols,rows,maxval,0);
	outgrayrow = pgm_allocrow(cols);

	pgm_freerow(outgrayrow);	//output
	pm_close(output);	
}



int main (int argc, char * argv[]){

	Vektor one = input_file(argv,1);

	Vektor two = input_file(argv,2);
	return 0;
}

