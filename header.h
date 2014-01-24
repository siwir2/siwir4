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
	double *power;
	double *local_x;
	double *local_y;
	double *angle;
	int *global_x;
	int *global_y;
	int *idx_x;
	int *idx_y;
/*globale position auf die indizes im grid mappen?*/
};

void initialize_lamp(const int nRays, const double P);
bool check_if_in_grid(const int idx);
bool check_ray_for_power(const int idx);
bool check_for_refraction(const int idx, const int CASE);
void refraction_change_alpha(const int idx, const int cell_idx_old, const int cell_idx_new);
void update_power_ray_and_cell(const double laenge, const int cell_idx, const int ray_idx);
int update_position(int cell_idx_old, int CASE);
int update_cell(const int idx, const int CASE);

void input_files(char* argv[]);
void output_file(int cols, int rows);
void output_nomieren();


//kommentar
