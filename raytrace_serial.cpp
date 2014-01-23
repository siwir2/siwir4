/***********************************************************************
*                   FAU Erlangen WS13/14
*                   Siwir Uebung 4 - Ray Tracing with OpenM
*                   written by Tim Brendel, Dursun Goek
*                   Januar 2014
************************************************************************/

#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <header.h>


//TODO: Leistung auf strahlen aufteilen
void initialize_lamp(const int nRays, const double P){

	srand48 (157);
	int n_r = 1;
	int NA = 0.5;       //numerische apertur
	double alpha = M.PI/6;
	int intervall = 0.1 * grid.NY;
	int start_idx = grid.NY/2 - intervall/2;
//	int end_idx = grid.NY/2 + intervall -intervall/2
//	if(end_idx - start_idx != intervall){
//		cout<<"error: mapping auf lokale koordinaten"<<endl;
//		exit(1);
//	}
	for(int i = 0; i< nRays; ++i){
		rays.angles[i] = -alpha+2*alpha*drand48();		//in [-pi/6 , pi/6] oder [-30,30]
		//rays.global_y[i] = -0.1+0.2*drand48();		// innerhalb [-0.1,0.1]
//brauche globale koodinaten noch?  - nur ein aufruf von drand()!!!!

		/*umrechnen auf lokale koordinaten*/
		//rays.idx_y[i] = grid.NY/2 * rays.global_y[i] + grid.NY/2;
		rays.idx_y[i] = start_idx + intervall * drand48();
	}
}


bool check_if_in_grid(const int idx){
	
	if(rays.global_x[idx] > 0 && rays.global_x[idx] < grid.NX){
		if(rays.global_y[idx] > 0 && rays.global_y[idx] < grid.NY){
			return true;
		}
	}
	else{
		return false;
	}
}


bool check_ray_for_power(const int idx){

	if(rays.power[idx] < (P/nRays - rays.power[idx]*0.999)){
		return false;
	}
	else{
		return true;
	}
}


bool check_for_refraction(const int idx, const int CASE){

	if(grid.REFR_INDX[cell_idx_old] != grid.REFR_INDX[cell_idx_new]){
		return true;
	}
	else{
		return false;
}


void refraction_change_alpha(const int idx, const int cell_idx_old, const int cell_idx_new){

	rays.angle[idx] = asin(sin(rays.angle[ray_idx]) * 
										(grid.REFR_INDX[cell_idx_old] 
										/ grid.REFR_INDX[cell_idx_new]));
}


/*berechnet den Leistungsverlust des Strahls bzw. den Leistungszuwachs der Zelle*/
void update_power_ray_and_cell(const double laenge, const int cell_idx, const int ray_idx){
	double tmp = rays.power[ray_idx];
	rays.power[ray_idx] = rays.power[ray_idx] * exp(-grid.ABS_COEF[cell_idx] * l);	//zelle = grid.ABS_COEF[cell_idx]
	double delta_power = rays.power[ray_idx] - tmp;
	grid.ABSO_POWER[cell_idx] += delta_power;
}


/*gibt den nächsten index aus*/
int update_position(int cell_idx_old, int CASE){
	if(CASE == LEFT){
		//eins nach rechts
		return cell_idx_new = cell_idx_old + 1;
	}
	else if(CASE == TOP){
		//eins nach unten
		return cell_idx_new =  cell_idx_old + grid.NX;
	}
	else if (CASE == BOTTOM){
		//eins nach oben
		return cell_idx_new = cell_idx_old - grid.NX;
	}
	
	else if(CASE == CORNER_LEFT_BOTTOM){
		//eins nach oben eins nach rechts
		return cell_idx_new = cell_idx_old - grid.NX + 1;
	}
	else if(VASE == CORNER_LEFT_TOP){
		//eins nach unten eins nach rechts
		return cell_idx_new = cell_idx_old + grid.NX + 1;
	}
}


int update_cell(const int idx, const int CASE){

	double y_out, x_out, m;
	double y_in = rays.pos_y[idx];
	double x_in = rays.pos_x[idx];
	double alpha = rays.angle[idx];
	//Geradengleichung m*x + y_in
	m = tan(alpha);
	if(   (alpha < 0 && m >= 0)   ||   (alpha > 0 && m <= 0)   ){
		printf("tangens function\n");
		exit(1);
	}
	if(alpha == 0){
		printf("alpha == 0\n");
		explode();
	}

	if(CASE == TOP){
		m = -tan( M.PI/2 - alpha);
		if(alpha > 0 && alpha < M.PI/2){
			x_out = -y_in/m;
			rays.pos_x[idx] = x_out;
			if(h_x < x_out){
				//case LEFT
				y_out = m * h_x + y_in;
				rays.pos_y[idx] = y_out;
				return LEFT;
			}
			else if(h_x > x_out){
				//case TOP
				y_out = +(h_y + y_in);
				rays.pos_y[idx] = y_out;
				return TOP;
			}
			else if(h_x == x_out){
				//case CORNER_LEFT_TOP
				x_out = 0;
				rays.pos_x[idx] = x_out;
				y_out = h_y;
				rays.pos_y[idx] = y_out;
				return CORNER_LEFT_TOP;
			}
		}
		else if(alpha = 0){
			//case TOP
			x_out = x_in;
			y_out = h_y;
			rays.pos_y[idx] = y_out;
			rays.pos_x[idx] = x_out;
			return TOP;
		}
		else if(alpha == M.PI/2){
			//case LEFT
			x_out = 0;
			y_out = h_y;
			rays.pos_y[idx] = y_out;
			rays.pos_x[idx] = x_out;
			return LEFT;
		}
		else{
			printf("error: TOP: alpha\n");
			exit(1);
		}
	}
	else if(CASE == BOTTOM){
		m = tan(abs(alpha));
		if(alpha < 0 && alpha > -M.PI/2){
			x_out = (h_y - y_in) / m;
			rays.pos_x[idx] = x_out;
			if(h_x < x_out){
				//case LEFT
				y_out = m * h_x + y_in;
				rays.pos_y[idx] = y_out;
				return LEFT;
			}
			else if(h_x > x_out){
				//case BOTTOM
				y_out = -(h_y - y_in);
				rays.pos_y[idx] = y_out;
				return BOTTOM;
			}
			else if(h_x == x_out){
				//case CORNER_LEFT_BOTTOM
				x_out = 0;
				y_out = 0;
				rays.pos_y[idx] = y_out;
				rays.pos_x[idx] = x_out;
				return CORNER_LEFT_BOTTOM;
			}
		}
		else if(alpha == 0){
			//case BOTTOM
			x_out = x_in;
			y_out = 0;
			rays.pos_y[idx] = y_out;
			rays.pos_x[idx] = x_out;
			return BOTTOM;
		}
		else if(alpha == -M.PI/2){
			//case LEFT
			x_out = 0;
			y_out = 0;
			rays.pos_y[idx] = y_out;
			rays.pos_x[idx] = x_out;
			return LEFT;
		}
		else{
			printf("error: BOTTOM: alpha\n");
			exit(1);
		}
	}
	else if(CASE == CORNER_LEFT_BOTTOM){
//TODO: alpha von boden weg messen
		if(alpha > 0 && alpha < M.PI/2){
			m = tan(alpha);
			x_out = h_y / m;
			rays.pos_x[idx] = x_out;
			if(h_x < x_out){
				//case LEFT
				y_out = m * h_x;
				rays.pos_y[idx] = y_out;
				return LEFT;
			}
			else if(h_x > x_out){
				//case BOTTOM
				y_out = -h_y;
				rays.pos_y[idx] = y_out;
				return BOTTOM;
			}else if(h_x == x_out){
				//case CORNER_LEFT_BOTTOM
				x_out = 0;
				y_out = 0;
				rays.pos_y[idx] = y_out;
				rays.pos_x[idx] = x_out;
				return CORNER_LEFT_BOTTOM;
			}
		}
		else if(alpha == 0){
			//case LEFT
			x_out = 0;
			y_out = 0;
			rays.pos_y[idx] = y_out;
			rays.pos_x[idx] = x_out;
			return LEFT;
		}
		else if(alpha == M.PI/2){
			//case BOTTOM
			x_out = 0;
			y_out = 0;
			rays.pos_y[idx] = y_out;
			rays.pos_x[idx] = x_out;
			return BOTTOM;
		}
		else{
			printf("error: CORNER_LEFT_BOTTOM: alpha\n");
		}
	}
	else if(CASE == LEFT){
		m = tan(alpha);
		if(alpha > 0 && alpha < M.PI/2){
			//mx+y=hy
			//Berechung des Schnittpunkts der Geraden mit y=hy
			x_out = (h_y - y_in) / m;
			rays.pos_x[idx] = x_out;
			if(x_out < 0){
				printf("assert error: wrong x_out case LEFT\n");
				exit(1);
			}
			if(h_x < x_out){
				//case BOTTOM
				y_out = -(h_y - y_in);
				rays.pos_y[idx] = y_out;
				return BOTTOM;
			}
			else if(h_x > x_out){
				//case LEFT
				y_out = m * h_x + y_in;
				rays.pos_y[idx] = y_out;
				return LEFT;
			}
			else if(h_x = x_out){
				//case CORNER_LEFT_BOTTOM
				x_out = 0;
				y_out = 0;
				rays.pos_y[idx] = y_out;
				rays.pos_x[idx] = x_out;
				return CORNER_LEFT_BOTTOM;
			}
		}
		else if(alpha < 0 && alpha > -M.PI/2){
			x_out = -y_in / m;
			if(x_out < 0){
				printf("assert error: wrong x_out case LEFT\n");
				exit(1);
			}
			if(h_x < x_out){
				//case TOP
				y_out = +(h_y + y_in);
				rays.pos_y[idx] = y_out;
				return TOP;
			}
			else if(h_x > x_out){
				//case LEFT
				y_out = m * h_x + y_in;
				rays.pos_y[idx] = y_out;
				return LEFT;
			}
			else if(h_x == x_out){
				//case CORNER_LEFT_TOP
				x_out = 0;
				y_out = h_y;
				rays.pos_y[idx] = y_out;
				rays.pos_x[idx] = x_out;
				return CORNER_LEFT_TOP;
			}
		}
		else if(alpha == 0){
			//case LEFT
			x_out = 0;
			y_out = y_in;
			rays.pos_y[idx] = y_out;
			rays.pos_x[idx] = x_out;
			return LEFT;
		}
		else if(alpha == M.PI/2){
			//case BOTTOM
			y_out = 0;
			x_out = 0;
			rays.pos_y[idx] = y_out;
			rays.pos_x[idx] = x_out;
			return BOTTOM;
		}
		else if(alpha == -M.PI/2){
			//case TOP
			y_out = h_y;
			x_out = 0;
			rays.pos_y[idx] = y_out;
			rays.pos_x[idx] = x_out;
			return TOP;
		}
		else{
			printf("error: LEFT: alpha\n");
			exit(1);
		}
	}
	else if(CASE == CORNER_LEFT_TOP){
//TODO: alpha von der decke weg messen
		m = tan(alpha);
		if(alpha < 0 && alpha > -M.PI/2){
			x_out = -y_in / m;
			rays.pos_x[idx] = x_out;
			if(h_x < x_out){
				//case LEFT
				y_out = m * h_x + y_in;
				rays.pos_y[idx] = y_out;
				return LEFT;
			}
			else if(h_x > x_out){
				//case TOP
				y_out = y_in + h_y; 
				rays.pos_y[idx] = y_out;
				//muss identisch sein mit 2*h_y
				return TOP;
			}
			else if(h_x == x_out){
				//case CORNER_LEFT_TOP
				x_out = 0;
				y_out = h_y;
				rays.pos_y[idx] = y_out;
				rays.pos_x[idx] = x_out;
				return CORNER_LEFT_TOP;
			}
		}
		else if(alpha == 0){
			//case LEFT
			x_out = 0;
			y_out = h_y;
			rays.pos_y[idx] = y_out;
			rays.pos_x[idx] = x_out;
			return LEFT;
		}
		else if(alpha == -M.PI/2){
			//case TOP
			x_out = 0;
			y_out = h_y;
			rays.pos_y[idx] = y_out;
			rays.pos_x[idx] = x_out;
			return TOP;
		}
		else{
			printf("error: CORNER_LEFT_TOP: alpha\n");
			exit(1);
		}
	}
	else{
		printf("error: CASE: not defined\n")
		exit(1);
	}
}


