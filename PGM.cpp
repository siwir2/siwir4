#include<stdio.h>
#include<stdlib.h>
#include<stdlib.h> 
#include<string.h>
#include<netpbm/pgm.h>
#include<math.h>
#include"header.h"

void input_files(char* argv[]){
	FILE *refr,*abso;
	int colsRefr,rowsRefr,colsAbso, rowsAbso, format;
	gray maxval;							// read returns maxval 
	gray *grayrowRefr, *grayrowAbso;
	int argc = 2;//sizeof(argv);
	pgm_init(&argc,argv);

	refr = pm_openr(argv[3]);		// REFR_INDX
	printf("%s einlesen\n",argv[3]);
	
	pgm_readpgminit(refr,&colsRefr,&rowsRefr,&maxval,&format);
	printf("%d colsRefr \t %d rowsRefr \t %d \n",colsRefr, rowsRefr,format);
	grayrowRefr = pgm_allocrow(colsRefr);

	double *refr_indx= new double [colsRefr*rowsRefr];
	for(int y = 0;y < rowsRefr;++y){
		pgm_readpgmrow(refr,grayrowRefr,colsRefr,maxval,format);
		for(int x=0;x< colsRefr; ++x){ 
			round((((double)grayrowRefr[x]/(double)255))*4) < 1 ? grayrowRefr[x]=1: grayrowRefr[x]= round((((double)grayrowRefr[x]/(double)255))*4);
			printf("%d \t",grayrowRefr[x]);
			refr_indx[y*colsRefr + x] = grayrowRefr[x];
		}
	}
	
	
	abso = pm_openr(argv[4]);		// ABS_COEF
	printf("%s einlesen\n",argv[4]);
	
	pgm_readpgminit(abso,&colsAbso,&rowsAbso,&maxval,&format);
	printf("%d colsAbso \t %d rowsAbso \t %d \n",colsAbso, rowsAbso,format);
	grayrowAbso = pgm_allocrow(colsAbso);
	
	double *abs_coef= new double [colsAbso*rowsAbso];
	for(int y = 0;y < rowsRefr;++y){
		pgm_readpgmrow(abso,grayrowAbso,colsAbso,maxval,format);
		for(int x=0;x< colsAbso; ++x){ 
			round((((double)grayrowAbso[x]/(double)255))*10) < 1 ? grayrowAbso[x]=0: grayrowAbso[x]= round((((double)grayrowAbso[x]/(double)255))*10);
			printf("%d \t",grayrowAbso[x]);
			abs_coef[y*colsAbso + x] = grayrowAbso[x];
		}
	}
	
	grid.REFR_INDX= refr_indx;			// dem struct zuweisen
	grid.ABS_COEF = abs_coef;
	
	pgm_freerow(grayrowRefr);
	pgm_freerow(grayrowAbso);
	pm_close(refr);
	pm_close(abso);
}


void output_file(int cols, int rows){	// rows und cols mit uebergeben
	FILE *output;
	gray *outgrayrow;			// alloc row returns type gray
	gray maxval;
// 	int argc = sizeof(argv);
// 	pgm_init(&argc,argv);
	
	output = pm_openw("output.pgm");		// write
	pgm_writepgminit(output,cols,rows,maxval,0);
	outgrayrow = pgm_allocrow(cols);
	
	for(int y = 0; y< rows; ++y){
		pgm_writepgmrow(output,outgrayrow,cols,maxval,0);
		for(int x = 0; x< cols; ++x){
			outgrayrow[x]= grid.ABSO_POWER[y*cols+x];		// das ergebnis in die output hinzufuegen
		}
	}
	
	pgm_freerow(outgrayrow);	//output
	pm_close(output);	
}



//MAIN:
void output_nomieren(){
	double maximum = grid.ABSO_POWER[0];
	for(int y = 0; y<grid.NY; ++y){
		for(int x = 0;x<grid.NX; ++x){
			if (grid.ABSO_POWER[y*grid.NX+x] > maximum){
				maximum = grid.ABSO_POWER[y*grid.NX+x];
			}
		}
	}
	for(int y = 0; y<grid.NY; ++y){
		for(int x = 0;x<grid.NX; ++x){
			grid.ABSO_POWER[y*grid.NX+x] = round((((double)grid.ABSO_POWER[y*grid.NX+x]/(double)maximum))*255);
		}
	}
}
