/***********************************************************************
*                   FAU Erlangen WS13/14
*                   Siwir Uebung 4 - Ray Tracing with OpenM
*                   written by Tim Brendel, Dursun Goek
*                   Januar 2014
************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <netpbm/pgm.h>
#include <math.h>
#include <cstdlib>
#include "header.h"

// #define _USE_MATH_DEFINES

int main(int argc, char **argv){
	std::cout<<"hello."<<std::endl;
	
	double P;
	int nRays;
	GRID *grid = new GRID;
	
	if(argc != 5 && argc != 1){
		std::cout<<"usage: ./raytr"<<std::endl;
		std::cout<<"usage: ./raytr P nRays refrIdx.pgm absCoef.pgm"<<std::endl;
		std::cout<<"P: double: radiant power of the light emitted by the light source"<<std::endl;
		std::cout<<"nRays: int: number of used rays"<<std::endl;
		std::cout<<"refrIdx.pgm, absCoef.pgm: refraction index image, absorption input image"<<std::endl;
		exit(1);
	}
	if(argc == 1){
		P = 10.0;
		nRays = 100;
		grid->NX = 60;
		grid->NY = 50;
		grid->hx = 50.0/3.0;		//in m
		grid->hy = 60.0/2.0;
		grid->REFR_INDX = new double [grid->NX*grid->NY];
		grid->ABSO_POWER = new double[grid->NX*grid->NY];
		grid->ABS_COEF = new double[grid->NX*grid->NY];
		for(int i = 0; i < grid->NX * grid->NY; ++i){
			grid->ABSO_POWER[i] = 0.0;
			grid->REFR_INDX[i] = 1;
			grid->ABS_COEF[i] = 0.5;
		}
	}
	else if(argc == 5){
		P = atof(argv[1]);
		nRays = atoi(argv[2]);
		/*read file*/
		FILE *refr,*abso;
		int colsRefr,rowsRefr,colsAbso, rowsAbso, format;
		gray maxval;                                                    // read returns maxval 
		gray *grayrowRefr, *grayrowAbso;
//		int argc = 2;//sizeof(argv);
		pgm_init(&argc,argv);

		refr = pm_openr(argv[3]);               // REFR_INDX
//		printf("%s einlesen\n",argv[3]);

		pgm_readpgminit(refr,&colsRefr,&rowsRefr,&maxval,&format);
//		printf("%d colsRefr \t %d rowsRefr \t %d \n",colsRefr, rowsRefr,format);
		grayrowRefr = pgm_allocrow(colsRefr);

		grid->REFR_INDX = new double [colsRefr*rowsRefr];
		for(int y = 0;y < rowsRefr;++y){
			pgm_readpgmrow(refr,grayrowRefr,colsRefr,maxval,format);
			for(int x=0;x< colsRefr; ++x){
				round((((double)grayrowRefr[x]/(double)255))*4) < 1 ? grayrowRefr[x]=1: grayrowRefr[x]= round((((double)grayrowRefr[x]/(double)255))*4);
//				printf("%d \t",grayrowRefr[x]);
				grid->REFR_INDX[y*colsRefr + x] = grayrowRefr[x];
//				printf("%f", grid->REFR_INDX[y*colsRefr+x]);
			}
		}
		abso = pm_openr(argv[4]);               // ABS_COEF
//		printf("%s einlesen\n",argv[4]);

		pgm_readpgminit(abso,&colsAbso,&rowsAbso,&maxval,&format);
		grid->NY = rowsAbso;
		grid->NX = colsAbso;
		grid->hy = 2.0 / (double) grid->NY;
		grid->hx = 3.0 / (double) grid->NX;
//		printf("%d colsAbso \t %d rowsAbso \t %d \n",colsAbso, rowsAbso,format);
		grayrowAbso = pgm_allocrow(colsAbso);
		grid->ABS_COEF = new double [colsAbso*rowsAbso];
		for(int y = 0;y < rowsRefr;++y){
			pgm_readpgmrow(abso,grayrowAbso,colsAbso,maxval,format);
			for(int x=0;x< colsAbso; ++x){
				round((((double)grayrowAbso[x]/(double)255))*10) < 1 ? grayrowAbso[x]=0: grayrowAbso[x]= round((((double)grayrowAbso[x]/(double)255))*10);
//				printf("%d \t",grayrowAbso[x]);
				grid->ABS_COEF[y*colsAbso + x] = grayrowAbso[x];
			}
		}
		pgm_freerow(grayrowRefr);
		pgm_freerow(grayrowAbso);
		pm_close(refr);
		pm_close(abso);
	}
	std::cout<<"NX:"<<grid->NX<<"   NY:"<<grid->NY<<std::endl;
	std::cout<<"P:"<< P <<"\tnRays:"<< nRays << std::endl;
	std::cout<<"hx: "<<grid->hx<<"    hy: "<<grid->hy<<std::endl;  
	/*struct RAYS allokieren und Leistungs-Werte eintragen*/
	RAYS *rays = new RAYS;
	rays->power = new double[nRays];
	rays->angle = new double[nRays];
	rays->global_x = new double[nRays];
	rays->global_y = new double[nRays];
	rays->local_x = new double[nRays];
	rays->local_y = new double[nRays];
	rays->idx_x = new int[nRays];
	rays->idx_y = new int[nRays];
	rays->CASE = new int[nRays];
//initialize lamp
	srand48 (157);
	int n_r = 1;
	int NA = 0.5;       //numerische apertur
	double alpha = M_PI/6;
	/*Intervall runden fuer 0<NY<100*/
	double itv = 0.1 * (double) grid->NY;
	itv += 0.5;
	itv = (int) itv;
	int st_idx = grid->NY/2 -1;
	for(int i = 0; i< nRays; ++i){
		rays->power[i] = P/nRays;
		rays->angle[i] = -alpha+2*alpha*drand48();               //in [-pi/6 , pi/6] oder [-30,30]
		rays->global_x[i] = 0.0;
		double d;
		d = drand48();
		rays->global_y[i] = -0.1+0.2*d;                // in [-0.1,0.1]
		rays->idx_y[i] = d*itv + st_idx;
		rays->local_x[i] = 0.0;
		rays->CASE[i] = LEFT;
	}
	
	for(int ray_idx = 0; ray_idx < nRays; ++ray_idx){
		for(int i = 0; i<10; ++i){
// 		if(check_if_in_grid(cell_idx_old) && check_ray_for_power(P/nRays, ray_idx)){
			update_cell(ray_idx, grid, rays);
			std::cout<<rays->CASE[ray_idx]<<"  ";
			update_position(rays->CASE[ray_idx], rays);
			std::cout<<rays->idx_x<<"  "<<rays->idx_y<<std::endl;
// 			if(check_for_refraction(ray_idx, cell_idx_old, cell_idx_new, CASE)){
// 				/*benutzen hier cell_idx_new und cell_idx_old:
// 				d.h es muss schon update_position gemacht worden sein*/
// 				refraction_change_alpha(ray_idx, cell_idx_old, cell_idx_new);
// 			}
// //TODO: laenge berechnen
// 		//	update_power_ray_and_cell(laenge, cell_idx_old, ray_idx);
// 			cell_idx_old = cell_idx_new;
// 		}
		}
	}

std::cout<<"4\n";
// 	// output_nomieren(){
// 	double maximum = grid->ABSO_POWER[0];
// 	for(int y = 0; y<grid->NY; ++y){
// 		for(int x = 0;x<grid->NX; ++x){
// 			if (grid->ABSO_POWER[y*grid->NX+x] > maximum){
// 				maximum = grid->ABSO_POWER[y*grid->NX+x];
// 			}
// 		}
// 	}
// 	for(int y = 0; y<grid->NY; ++y){
// 		for(int x = 0;x<grid->NX; ++x){
// 			grid->ABSO_POWER[y*grid->NX+x] = round((((double)grid->ABSO_POWER[y*grid->NX+x]/(double)maximum))*255);
// 		}
// 	}
// 
// //	output file
// 	FILE *output;
// 	gray *outgrayrow;                       // alloc row returns type gray
// 	gray maxval;
// 	int cols = grid->NX;
// 	int rows = grid->NY;
// //	int argc = sizeof(argv);
// //	pgm_init(&argc,argv);
// 
// 	output = pm_openw("output.pgm");                // write
// 	pgm_writepgminit(output,cols,rows,maxval,0);
// 	outgrayrow = pgm_allocrow(cols);
//       
// 	for(int y = 0; y< rows; ++y){
// 	pgm_writepgmrow(output,outgrayrow,cols,maxval,0);
// 		for(int x = 0; x< cols; ++x){
// 			outgrayrow[x]= grid->ABSO_POWER[y*cols+x];               // das ergebnis in die output hinzufuegen
// 		}
// 	}
//       
// 	pgm_freerow(outgrayrow);        //output
// 	pm_close(output);       


	exit(0);
}


//bool check_if_in_grid(const int idx){
//	
//	if(rays->global_x[idx] > 0 && rays->global_x[idx] < grid->NX){
//		if(rays->global_y[idx] > 0 && rays->global_y[idx] < grid->NY){
//			return true;
//		}
//	}
//	else{
//		return false;
//	}
//}
//

//bool check_ray_for_power(const double pw_div_nRys, const int idx){
//
//	if(rays->power[idx] < (pw_div_nRys - rays->power[idx]*0.999)){
//		return false;
//	}
//	else{
//		return true;
//	}
//}
//
//
//bool check_for_refraction(const int idx, const int cell_idx_old, const int cell_idx_new, const int CASE){
//
//	if(grid->REFR_INDX[cell_idx_old] != grid->REFR_INDX[cell_idx_new]){
//		return true;
////	}
////	else{
//		return false;
//	}
//}


//void refraction_change_alpha(const int ray_idx, const int cell_idx_old, const int cell_idx_new){
//
//	rays->angle[ray_idx] = asin(sin(rays->angle[ray_idx]) * 
//										(grid->REFR_INDX[cell_idx_old] 
//										/ grid->REFR_INDX[cell_idx_new]));
//}
//
//
/*berechnet den Leistungsverlust des Strahls bzw. den Leistungszuwachs der Zelle*/
//void update_power_ray_and_cell(const double laenge, const int cell_idx, const int ray_idx){
//	double tmp = rays->power[ray_idx];
//	rays->power[ray_idx] = rays->power[ray_idx] * exp(-grid->ABS_COEF[cell_idx] * laenge);	//zelle = grid->ABS_COEF[cell_idx]
//	double delta_power = rays->power[ray_idx] - tmp;
//	grid->ABSO_POWER[cell_idx] += delta_power;
//}


/*gibt den nächsten index aus*/
void update_position(const int idx, RAYS *rays){
	int cell_idx_new;
	if(rays->CASE[idx] == LEFT){
		//eins nach rechts
		rays->idx_x[idx]++;
// 		return cell_idx_new = cell_idx_old + 1;
	}
	else if(rays->CASE[idx] == TOP){
		//eins nach unten
		rays->idx_y[idx]++;
// 		return cell_idx_new =  cell_idx_old + grid->NX;
	}
	else if(rays->CASE[idx] == BOTTOM){
		//eins nach oben
		rays->idx_y[idx]--;
// 		return cell_idx_new = cell_idx_old - grid->NX;
	}
	
	else if(rays->CASE[idx] == CORNER_LEFT_BOTTOM){
		//eins nach oben eins nach rechts
		rays->idx_y[idx]--;
		rays->idx_x[idx]++;
// 		return cell_idx_new = cell_idx_old - grid->NX + 1;
	}
	else if(rays->CASE[idx] == CORNER_LEFT_TOP){
		//eins nach unten eins nach rechts
		rays->idx_y[idx]++;
		rays->idx_x[idx]++;
		//return cell_idx_new = cell_idx_old + grid->NX + 1;
	}
}

void update_cell(const int idx, GRID *grid, RAYS *rays){

	double y_out, x_out, m;
	double alpha = rays->angle[idx];
	//Geradengleichung m*x + rays->local_y[idx]
	m = tan(rays->angle[idx]);
	if(   (rays->angle[idx] < 0 && m >= 0)   ||   (rays->angle[idx] > 0 && m <= 0)   ){
		printf("tangens function\n");
		exit(1);
	}
	if(rays->angle[idx] == 0){
		printf("rays->angle[idx] == 0\n");
		exit(1);
	}

	if(rays->CASE[idx] == TOP){
		m = -tan( M_PI/2 - rays->angle[idx]);
		if(rays->angle[idx] > 0 && rays->angle[idx] < M_PI/2){
			x_out = -rays->local_y[idx]/m;
			rays->local_x[idx] = x_out;
			if(grid->hx < x_out){
				//case LEFT
				y_out = m * grid->hx + rays->local_y[idx];
				rays->local_y[idx] = y_out;
				rays->CASE[idx] = LEFT;
			}
			else if(grid->hx > x_out){
				//case TOP
				y_out = +(grid->hy + rays->local_y[idx]);
				rays->local_y[idx] = y_out;
				rays->CASE[idx] = TOP;
			}
			else if(grid->hx == x_out){
				//case CORNER_LEFT_TOP
				x_out = 0;
				rays->local_x[idx] = x_out;
				y_out = grid->hy;
				rays->local_y[idx] = y_out;
				rays->CASE[idx] = CORNER_LEFT_TOP;
			}
		}
		else if(rays->angle[idx] = 0){
			//case TOP
			x_out = rays->local_x[idx];
			y_out = grid->hy;
			rays->local_y[idx] = y_out;
			rays->local_x[idx] = x_out;
			rays->CASE[idx] = TOP;
		}
else if(rays->angle[idx] == M_PI/2){
			//case LEFT
			x_out = 0;
			y_out = grid->hy;
			rays->local_y[idx] = y_out;
			rays->local_x[idx] = x_out;
			rays->CASE[idx] = LEFT;
		}
		else{
			printf("error: TOP: rays->angle[idx]\n");
			exit(1);
		}
	}
	else if(rays->CASE[idx] == BOTTOM){
		m = tan(abs(rays->angle[idx]));
		if(rays->angle[idx] < 0 && rays->angle[idx] > -M_PI/2){
			x_out = (grid->hy - rays->local_y[idx]) / m;
			rays->local_x[idx] = x_out;
			if(grid->hx < x_out){
				//case LEFT
				y_out = m * grid->hx + rays->local_y[idx];
				rays->local_y[idx] = y_out;
				rays->CASE[idx] = LEFT;
			}
			else if(grid->hx > x_out){
				//case BOTTOM
				y_out = -(grid->hy - rays->local_y[idx]);
				rays->local_y[idx] = y_out;
				rays->CASE[idx] = BOTTOM;
			}
			else if(grid->hx == x_out){
				//case CORNER_LEFT_BOTTOM
				x_out = 0;
				y_out = 0;
				rays->local_y[idx] = y_out;
				rays->local_x[idx] = x_out;
				rays->CASE[idx] = CORNER_LEFT_BOTTOM;
			}
		}
		else if(rays->angle[idx] == 0){
			//case BOTTOM
			x_out = rays->local_x[idx];
			y_out = 0;
			rays->local_y[idx] = y_out;
			rays->local_x[idx] = x_out;
			rays->CASE[idx] = BOTTOM;
		}
		else if(rays->angle[idx] == -M_PI/2){
			//case LEFT
			x_out = 0;
			y_out = 0;
			rays->local_y[idx] = y_out;
			rays->local_x[idx] = x_out;
			rays->CASE[idx] = LEFT;
		}
		else{
			printf("error: BOTTOM: alpha\n");
			exit(1);
		}
	}
	else if(rays->CASE[idx] == CORNER_LEFT_BOTTOM){
//TODO: alpha von boden weg messen
		if(rays->angle[idx] > 0 && rays->angle[idx] < M_PI/2){
			m = tan(rays->angle[idx]);
			x_out = grid->hy / m;
			rays->local_x[idx] = x_out;
			if(grid->hx < x_out){
				//case LEFT
				y_out = m * grid->hx;
				rays->local_y[idx] = y_out;
				rays->CASE[idx] = LEFT;
			}
			else if(grid->hx > x_out){
				//case BOTTOM
				y_out = -grid->hy;
				rays->local_y[idx] = y_out;
				rays->CASE[idx] = BOTTOM;
			}else if(grid->hx == x_out){
				//case CORNER_LEFT_BOTTOM
				x_out = 0;
				y_out = 0;
				rays->local_y[idx] = y_out;
				rays->local_x[idx] = x_out;
				rays->CASE[idx] = CORNER_LEFT_BOTTOM;
			}
		}
		else if(rays->angle[idx] == 0){
			//case LEFT
			x_out = 0;
			y_out = 0;
			rays->local_y[idx] = y_out;
			rays->local_x[idx] = x_out;
			rays->CASE[idx] = LEFT;
		}
		else if(rays->angle[idx] == M_PI/2){
			//case BOTTOM
			x_out = 0;
			y_out = 0;
			rays->local_y[idx] = y_out;
			rays->local_x[idx] = x_out;
			rays->CASE[idx] = BOTTOM;
		}
		else{
			printf("error: CORNER_LEFT_BOTTOM: alpha\n");
		}
	}
	else if(rays->CASE[idx] == LEFT){
		m = tan(rays->angle[idx]);
		if(rays->angle[idx] > 0 && rays->angle[idx] < M_PI/2){
			//mx+y=hy
			//Berechung des Schnittpunkts der Geraden mit y=hy
			x_out = (grid->hy - rays->local_y[idx]) / m;
			rays->local_x[idx] = x_out;
			if(x_out < 0){
				printf("assert error: wrong x_out case LEFT\n");
				exit(1);
			}
			if(grid->hx < x_out){
				//case BOTTOM
				y_out = -(grid->hy - rays->local_y[idx]);
				rays->local_y[idx] = y_out;
				rays->CASE[idx] = BOTTOM;
			}
			else if(grid->hx > x_out){
				//case LEFT
				y_out = m * grid->hx + rays->local_y[idx];
				rays->local_y[idx] = y_out;
				rays->CASE[idx] = LEFT;
			}
			else if(grid->hx == x_out){
				//case CORNER_LEFT_BOTTOM
				x_out = 0;
				y_out = 0;
				rays->local_y[idx] = y_out;
				rays->local_x[idx] = x_out;
				rays->CASE[idx] = CORNER_LEFT_BOTTOM;
			}
		}
		else if(rays->angle[idx] < 0 && rays->angle[idx] > -M_PI/2){
			x_out = -rays->local_y[idx] / m;
			if(x_out < 0){
				printf("assert error: wrong x_out case LEFT\n");
				exit(1);
			}
			if(grid->hx < x_out){
				//case TOP
				y_out = +(grid->hy + rays->local_y[idx]);
				rays->local_y[idx] = y_out;
				rays->CASE[idx] = TOP;
			}
			else if(grid->hx > x_out){
				//case LEFT
				y_out = m * grid->hx + rays->local_y[idx];
				rays->local_y[idx] = y_out;
				rays->CASE[idx] = LEFT;
			}
			else if(grid->hx == x_out){
				//case CORNER_LEFT_TOP
				x_out = 0;
				y_out = grid->hy;
				rays->local_y[idx] = y_out;
				rays->local_x[idx] = x_out;
				rays->CASE[idx] = CORNER_LEFT_TOP;
			}
		}
		else if(rays->angle[idx] == 0){
			//case LEFT
			x_out = 0;
			y_out = rays->local_y[idx];
			rays->local_y[idx] = y_out;
			rays->local_x[idx] = x_out;
			rays->CASE[idx] = LEFT;
		}
		else if(rays->angle[idx] == M_PI/2){
			//case BOTTOM
			y_out = 0;
			x_out = 0;
			rays->local_y[idx] = y_out;
			rays->local_x[idx] = x_out;
			rays->CASE[idx] = BOTTOM;
		}
		else if(rays->angle[idx] == -M_PI/2){
			//case TOP
			y_out = grid->hy;
			x_out = 0;
			rays->local_y[idx] = y_out;
			rays->local_x[idx] = x_out;
			rays->CASE[idx] = TOP;
		}
		else{
			printf("error: LEFT: alpha\n");
			exit(1);
		}
	}
	else if(rays->CASE[idx] == CORNER_LEFT_TOP){
//TODO: alpha von der decke weg messen
		m = tan(rays->angle[idx]);
		if(rays->angle[idx] < 0 && rays->angle[idx] > -M_PI/2){
			x_out = -rays->local_y[idx] / m;
			rays->local_x[idx] = x_out;
			if(grid->hx < x_out){
				//case LEFT
				y_out = m * grid->hx + rays->local_y[idx];
				rays->local_y[idx] = y_out;
				rays->CASE[idx] = LEFT;
			}
			else if(grid->hx > x_out){
				//case TOP
				y_out = rays->local_y[idx] + grid->hy; 
				rays->local_y[idx] = y_out;
				//muss identisch sein mit 2*grid->hy
				rays->CASE[idx] = TOP;
			}
			else if(grid->hx == x_out){
				//case CORNER_LEFT_TOP
				x_out = 0;
				y_out = grid->hy;
				rays->local_y[idx] = y_out;
				rays->local_x[idx] = x_out;
				rays->CASE[idx] = CORNER_LEFT_TOP;
			}
		}
		else if(rays->angle[idx] == 0){
			//case LEFT
			x_out = 0;
			y_out = grid->hy;
			rays->local_y[idx] = y_out;
			rays->local_x[idx] = x_out;
			rays->CASE[idx] = LEFT;
		}
		else if(rays->angle[idx] == -M_PI/2){
			//case TOP
			x_out = 0;
			y_out = grid->hy;
			rays->local_y[idx] = y_out;
			rays->local_x[idx] = x_out;
			rays->CASE[idx] = TOP;
		}
		else{
			printf("error: CORNER_LEFT_TOP: alpha\n");
			exit(1);
		}
	}
	else{
		printf("error: CASE: not defined\n");
		exit(1);
	}
	rays->local_y[idx] = y_out;
	rays->local_x[idx] = x_out;
}
