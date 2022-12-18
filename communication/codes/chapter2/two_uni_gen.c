#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "coeffs.h"

#define UNI1_PATH	"../rvsample/uni1.dat"
#define UNI2_PATH	"../rvsample/uni2.dat"

int  main(void) //main function begins
{
 
//Uniform random numbers
uniform(UNI1_PATH, 1000000);
uniform(UNI2_PATH, 1000000);

return 0;
}
