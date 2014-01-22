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
		grid.ABSO_POWER = new double[grid.NX * grid.NY];
		grid.REFR_INDX = new double[grid.NX * grid.NY];
		grid.ABS_COEF = new double[my_grid.NX * my_grid.NY];
		for(int i = 0; i < grid.NX * grid.NY; ++i){
			grid.ABSO_POWER = 0.0;
			grid.REFR_INDX = 1;
			grid.ABS_COEF = 0.5;
		}
		
	}
	std::cout<<"P:"<< P <<"\tnRays:"<< nRays << std::endl;
	
	/*struct RAYS allokieren und Leistungs-Werte eintragen*/
	Rays rays;
	rays.power[] = new double[nRays];
	rays.global_x[] = new double[nRays];
	rays.global_y[] = new double[nRays];
	rays.local_x[] = new double[nRays];
	rays.local_x[] = new double[nRays];
	
	for(int i = 0; i< nRays; ++i){
		rays.power[i] = P/nRays;
		rays.global_x = 0.0;
	}
	initialize_lamp(nRays, P);
	
	int CASE = LEFT;
	int cell_idx_old;
	int cell_idx_new;

	for(int ray_idx = 0; ray_idx < nRays; ++ray_idx){
		if(check_if_in_grid(cell_idx_old) && check_ray_for_power(ray_idx){

			CASE = update_cell(ray_idx, CASE);
			cell_idx_new = update_position(cell_idx_old, CASE);
			if(check_for_refraction(ray_idx){
				/*benutzen hier cell_idx_new und cell_idx_old:
				d.h es muss schon update_position gemacht worden sein*/
				refraction_change_alpha(ray_idx, cell_idx_old, cell_idx_new);
			}

			update_power_of_ray_and_cell(ray_idx, cell_idx_old, power_in, laenge);
			update_power_of_cell(cell_idx_old, delta_power);
			cell_idx_old = cell_idx_new;
		}
	}
	
	dr_sun_output_func();
	
	exit(0);
}
