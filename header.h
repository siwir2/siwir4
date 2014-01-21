/***********************************************************************
*                   FAU Erlangen WS13/14
*                   Siwir Uebung 4 - Ray Tracing with OpenM
*                   written by Tim Brendel, Dursun Goek
*                   Januar 2014
************************************************************************/

#define TOP 0
#define BOTTOM 1
#define CORNER_LEFT_BOTTOM 2
#define LEFT 3
#define CORNER_LEFT_TOP 4

struct GRID{
	double *ABSO_POWER;
	double *REFR_INDX;
	double *ABS_COEF ;
	int NX;
	int NY;
	double hx;
	double hy;
	double OX, OY;
};

struct RAYS{
	double *local_x;
	double *local_y;
	double *angle;
	int *global_x;
	int *global_y;
};

//
extern void initialize_lamp(const int);
extern int update_cell(const int);
