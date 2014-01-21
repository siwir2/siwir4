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

int update_position(cell_idx_old, CASE){
	grid.NX
	grid.NY
	
}


void initialize_lamp(const int nRays){
	srand48 (157);
	int n_r = 1;
	int NA = 0.5;       //numerische apertur
	double alpha = M.PI/6;
	for(int i = 0; i< 2*nRays; ++i){
		rays.angles[i] = -alpha+2*alpha*drand48();         //in [-pi/6 , pi/6] oder [-30,30]
		rays.pos_y[i] = -0.1+0.2*drand48();    // innerhalb [-0.1,0.1]
	}
}

bool check_for_position(const int idx){
	if(rays.global_x[idx] > 0 && rays.global_x[idx] < grid.NX){
		if(rays.global_y[idx] > 0 && rays.global_y[idx] < grid.NY){
			return true;
		}
	}
	else{
		return false;
	}
}

bool check_for_refraction(const int idx, const int CASE){

	if(grid.REFR_INDX[cell_idx_old] != grid.REFR_INDX[cell_idx_new]){
		return false;
	}
	else{
		return true;
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


