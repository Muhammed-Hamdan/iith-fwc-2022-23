#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "coeffs.h"

#define UNIFORM_RV_PATH 	"../rvsample/uni.dat"

int  main(void) //main function begins
{
 
//Uniform random numbers
uniform(UNIFORM_RV_PATH, 1000000);

//Mean and variance of Uniform 
printf("Uniform stats:\n");
printf("Mean: %lf\n",mean(UNIFORM_RV_PATH));
printf("Variance: %lf\n",variance(UNIFORM_RV_PATH));
return 0;
}


