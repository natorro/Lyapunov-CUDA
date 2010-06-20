/* Based from lyapunov.c I just want to start writing this stuff 
 * with MPI parallel programming,
 * after a lot of testing, stuff missing (in serial and parallel)
 *
 * Need to check for overflows in map or lyapunov_exp
 * Need to check for -inf going out of lyapunov_exp
 * Rewrite main into smaller functions
 */
   
#include <stdio.h>
#include <mpi.h>

#define MAGIC_VALUE "P3"
#define MAXGREY 255
#define MAXINDEX 64

const double B = 1.95;
const double X0 = 0.0;
const double WIDTH = 1000;
const double HEIGHT = 1000;

double map (double x, double r);
double lyapunov_exp (double x, double y);

int main (int argc, char *argv[]) 
{
  int rank, size;
	
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  

    if (0 == rank) {
    
    printf ("%s \n", MAGIC_VALUE);
    printf ("%f \n", WIDTH);
    printf ("%f \n", HEIGHT);
    printf ("%d \n", MAXGREY);
  }
    
    MPI_Scatter(points, 10, MPI_DOUBLE, &mine, 10, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
  else {
    
  }
  
  MPI_Finalize();
	
  return (0);
}

/*--------------------- FUNCTION DEFINITIONS ----------*/

/* The map function */
double map (double x, double r) 
{
	double y, a;
	a = sin (x + r);
	y = B * a * a;
	return y;
}

/* The Lyapunov exponent function */

double lyapunov_exp (double x, double y) 
{
	static int maxindex = MAXINDEX;
	static int forcing [MAXINDEX];
	char *ff = 0;
	char *ch;
	int bindex = 0;
	double sum;
	double lyap_point, a, b, h;
	int i;
	double xn;
	double lyap;
	double temp, temp2, temp3;
	
	xn = X0;
	lyap = 0;
	
	/* The forcing function */
	
	ff = "aaaaaabbbbbb";
	maxindex = strlen(ff);
	
	ch = ff;
	
	while (bindex < maxindex) {
		if (*ch == 'a')
			forcing[bindex++] = 1;
		else if (*ch == 'b')
			forcing[bindex++] = 0;
		ch++;
	}
	
	/* Let the function to "settle down" */
	
	bindex = 0;
	
       	for (i = 0; i < 600; i++) {
		
		h = (forcing[bindex]) ? x : y;
		xn = map (xn, h);
		
		if (++bindex >= maxindex)
			bindex = 0;
	}
	
	/* Calculate the Lyapunov exponent*/
	
	for (i = 600; i < 10000; i++) {
		
		h = (forcing[bindex]) ? x : y;
		temp = map (xn, h);
		temp2 = temp + h;
		temp3 = fabs (B * sin (2.0 * temp2));
		
		if (temp3 == 0.0) {
			;
		} else {
		
		sum = log (temp3);
		xn = temp; 
		lyap = lyap + sum; 
		
		if (++bindex >= maxindex)
			bindex = 0;
		}
	}
		
	lyap_point = lyap / 9400.0;
	return lyap_point;
}
