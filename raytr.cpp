/***********************************************************************
*                   FAU Erlangen WS13/14
*                   Siwir Uebung 4 - Ray Tracing with OpenM
*                   written by Tim Brendel, Dursun Goek
*                   Januar 2014
************************************************************************/

#include <stdlib.h>
#include <iostream>
#include "header.h"

int main(int argc, char **argv){
	std::cout<<"hello."<<std::endl;
	
	double P;
	int nRays;

	if(argc != 5 && argc != 1){
		std::cout<<"usage: ./raytr"<<std::endl;
		std::cout<<"usage: ./raytr P nRays refrIdx.pgm absCoef.pgm"<<std::endl;
		std::cout<<"P: double: radiant power of the light emitted by the light source"<<std::endl;
		std::cout<<"nRays: int: number of used rays"<<std::endl;
		std::cout<<"refrIdx.pgm, absCoef.pgm: refraction index image, absorption input image"<<std::endl;
		exit(1);
	}
	if(argc == 5){
		P = atof(argv[1]);
		nRays = atoi(argv[2]);
		dr_sun_input_func(argv);
	}
	else if(argc == 1){
		P = 10.0;
		nRays = 1000000;
		GRID grid;
		GRID grid;
		grid.NX = 50;
		grid.NY = 60;
		grid.hx = 50.0/3.0;     //in m
		grid.hy = 60.0/2.0;
		grid.ABSO_POWER = new double[my_grid.NX * my_grid.NY];
		grid.REFR_INDX = new double[my_grid.NX * my_grid.NY];
		grid.ABS_COEF = new double[my_grid.NX * my_grid.NY];
	
	}
	std::cout<<"P:"<< P <<"\tnRays:"<< nRays << std::endl;


	initialize_lamp(nRays, P);
	int CASE = LEFT;
	int cell_idx_tmp = 0;

	for(int ray_idx = 0; ray_idx < nRays; ++ray_idx){
		
		if(rays.cell_pos[idx] != -1){

			CASE = update_cell(ray_idx, CASE);

			if(grid.REFR_INDX[cell_idx_old] != grid.REFR_INDX[cell_idx_new]){
				rays.angle[ray_idx] = asin(sin(rays.angle[ray_idx]) * 
											(grid.REFR_INDX[cell_idx_old] 
												/ grid.REFR_INDX[cell_idx_new]));
			}
			update_power_raywise(ray_idx);
			updata_power_cellwise(cell_idx_old);
			cell_idx_new = update_position(cell_idx_old, CASE);
			cell_idx_old = cell_idx_new;
		}
	}
	
	dr_sun_output_func();
	
	exit(0);
}
