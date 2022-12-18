#include<stdio.h>
#include<time.h>
#include<stdlib.h>

#define NUM_SAMPLES	1000000
#define BUFF_SIZE	1000

void generate_uniform_sample(float *buff, float min, float max, int size){
	int i;
	for(i = 0; i<size; i++){
		buff[i] = min + (rand()*(max-min))/RAND_MAX;
	}
}

int main(){
	int i;
	float sample_buff[1000];
	const char file_name[]="uni.dat";
	FILE *sample_file = NULL;

	srand(time(NULL));
	sample_file = fopen(file_name, "w");
	for(i=0; i < NUM_SAMPLES; i+=BUFF_SIZE){
		generate_uniform_sample(sample_buff, 0.0, 1.0, BUFF_SIZE);
		fwrite(sample_buff, sizeof(float), BUFF_SIZE, sample_file);
	}
	fclose(sample_file);
	return 0;
}
