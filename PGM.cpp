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
	
	Grid grid;
	grid.REFR_INDX= refr_indx;			// dem struct zuweisen
	grid.ABS_COEF = abs_coef;
	
	pgm_freerow(grayrowRefr);
	pgm_freerow(grayrowAbso);
	pm_close(refr);
	pm_close(abso);
}

// 		pgm_writepgmrow(output,grayrow1,colsRefr1,maxval1,0);	// output
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


double update_power(int power_in, double zelle,int xin, int xout, int yin, int yout){		// zelle = mygrid.ABS_COEF[i]
	double l = sqrt((xin-xout)*(xin-xout)+ (yin-yout)*(yin-yout));
	return  power_in * exp(-zelle * l);// power_out =
}



int main (int argc, char * argv[]){

// 	input_files(argv);
	
	return 0;
}

