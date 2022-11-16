#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
//#include <sys/types.h>


#include "struct_CAR.h"
#include "struct_GP.h"
#include "file_interactions.h"
#include "simulation_logic.h"

//ALL TIMES IN ms
//ALL DISTENCES IN m

/*
int maxArray(int* array){
    int max = 0;
    for(int i = 0; array[i] != NULL; i++){
        if(array[i] > max){
            max = array[i];
        }
    }
    return max;
}
int get_time() {
	return rand() % ( + 1 - 25000) + 25000;
};*/

/* ----MAIN----*/
int main(int argc, char const *argv[])
{
	//1
	printf("%s\n", "1");
	char*** data = read_CSV("data/cars.csv"); //retourne une matrice de tring malloc
	struct Car* cars = init_CARs(data); //retourne une liste de struct
	free(data);

	//2
	printf("%s\n", "2");
	char*** data2 = read_CSV("data/grand_prix.csv"); //retourne une matrice de tring malloc
	struct GrandPrix* gps = init_GPs(data2); //retourne une liste de struct
	free(data2);

	printf("%s\n", "3");

	return 0;
}