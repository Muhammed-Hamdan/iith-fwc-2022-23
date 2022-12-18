#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "coeffs.h"

#define GAUSSIAN_RV_PATH 	"../rvsample/gau.dat"

int  main(void) //main function begins
{
 
//Gaussian random numbers
gaussian(GAUSSIAN_RV_PATH, 1000000);

//Mean and variance of Gaussian 
printf("Gaussian stats:\n");
printf("Mean: %lf\n",mean(GAUSSIAN_RV_PATH));
printf("Variance: %lf\n",variance(GAUSSIAN_RV_PATH));
return 0;
}
