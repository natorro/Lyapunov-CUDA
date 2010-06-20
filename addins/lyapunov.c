/* Based from lyapunov.c (really rewritten.c) 
 * after a lot of testing, stuff missing:
 *
 * Need to check for overflows in map or lyapunov_exp
 * Need to check for -inf going out of lyapunov_exp
 * Rewrite main into smaller functions
 */
   
#include <stdio.h>
#include <math.h>

#define MAGIC_VALUE "P3"
#define MAXGREY 255
#define MAXINDEX 64

const double B = 1.95;
const double X0 = 0.0;
const double WIDTH = 100;
const double HEIGHT = 100;

double map (double x, double r);
double lyapunov_exp (double x, double y);

int main () 
{
	
	double x_init, y_init, x_final, y_final;
	double delta_x, delta_y;
	double a, b, point;
	int j, k;
	double points [(int)HEIGHT] [(int)WIDTH];
	double max_value, min_value, max_value2, min_value2, abs_min_value, interval_size;
	int indexed;
	
	
	x_init = 0.5;
	y_init = 0.5;
	x_final = 1.0;
	y_final = 0.0;
	max_value = 0.0;
	min_value = 0.0;
	
	delta_x = fabs (x_init - x_final) / WIDTH;
	delta_y = fabs (y_init - y_final) / HEIGHT;
	
	a = x_init;
	b = y_init;
	
	printf ("%s \n", MAGIC_VALUE);
	printf ("%f \n", WIDTH);
	printf ("%f \n", HEIGHT);
	printf ("%d \n", MAXGREY);
	

	/* FIXME 
	   I need to change that -118.46 value because it doesn't need
	   to be checked if everything goes fine */

	/* Let's put everything in an array instead of printing it to
	   the standard output, find the maximum and minimal value of 
	   the array and put it in max_value */
	
	for (k = 0; k < HEIGHT ; k++) {
		for (j = 0; j < WIDTH; j++) {
			point = lyapunov_exp (a, b);
			points [k] [j] = point;
			if (max_value < points [k] [j]){
				max_value2 = max_value;
				max_value = points [k] [j];
			}
			if (points [k] [j] < min_value) {
				min_value2 = min_value;
				min_value = points [k] [j];
			}
			if (points [k] [j] < -118.462937)
				printf ("#--- VALOR CULERO (%f, %f) ----\n", a, b);
			a = a + delta_x;
		}
		a = x_init;
		b = b - delta_y;
	}
	
	
	/* Check the max and min values */
	
	printf ("#------ MAX_VALUE %f----------\n", max_value);
	printf ("#------ second MAX_VALUE %f----------\n", max_value2);
	printf ("#------ MIN_VALUE %f----------\n", min_value);
	printf ("#------ second MIN_VALUE %f----------\n", min_value2);
	
	
	/* index the results */
	/* Print the results and in color */
	
	abs_min_value = fabs (min_value2);
	interval_size = max_value - min_value2;
	
	for (k = 0; k < HEIGHT; k++) {
		for (j = 0; j < WIDTH; j++) {			
			indexed = 0;
			
			if ( points [k] [j] < 0.0 ) {
				indexed = ((-points [k] [j] * MAXGREY) / max_value);
				indexed = indexed % MAXGREY;
				printf ("%i 0 0  ", indexed);
			} else if (points [k] [j] > 0.0) {
				indexed = ((points [k] [j] * MAXGREY) / max_value);
				indexed = indexed % MAXGREY;
				printf ("0 0 %i ", indexed);
			}
		}
	}
}


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

/* FIXME
/* we need to prevent overflow and underflow 
if ((prod > 1.0e12) || (prod < 1.0e-12)) {
	*/
/*FIXME

Need to use the 255 module to get a good image

*/
