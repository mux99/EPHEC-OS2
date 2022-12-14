#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>
#include <semaphore.h>

// Options
char cars_file[50] = {0};
char gps_file[50] = {0};
float speed; // 0 <= value < 45

int pit_min; // default 16
int pit_max; // default 70
int pit_time; // default 25

int practice_lenght; //in seconds
int qualif_1_lenght; //in seconds
int qualif_2_lenght; //in seconds
int qualif_3_lenght; //in seconds
int sprint_lenght; //in meters
int race_lenght; //in meters

// Global variables (DO NOT CHANGE)
const int end = -10;
sem_t sem_data;
sem_t sem_cars;
sem_t sem_gps;
key_t shm_key; // will also use shm_key+1 and +2

//MEMO on int *data:
//
int p1 = 0;//-0- practice best lap
int q1 = 1;//-1- qualifications round 1 best lap
int q2 = 2;//-2- qualifications round 2 best lap
int q3 = 3;//-3- qualifications round 3 best lap
int srt = 4;//-4- race start positions (! not position by car but car by position)
int sti = 5;//-5- sprint time
int slp = 6;//-6- sprint best lap
int rti = 7;//-7- race time
int rlp = 8;//-8- race best lap
int pts = 9;//-9- points earned
int lpc = 10;//-10- lap count (for all)
int s1 = 11;//-11- S1 best (re-used for each step)
int s2 = 12;//-12- S2 best (re-used for each step)
int s3 = 13;//-13- S3 best (re-used for each step)


#include "split.h"
#include "struct_CAR.h"
#include "struct_GP.h"
#include "display.h"
#include "car_sim.h"
#include "file_interactions.h"
#include "simulation_logic.h"
#include "option_parser.h"

//
//	EPHEC TI-02 OS
// project:
//	simulate a F1 racing championchip
// author:
//	Maxime Dourov (mux99)
//

// ALL TIMES IN s (float)
// ALL DISTENCES IN m (int)

/* ----MAIN----*/
int main(int argc, char const *argv[])
{
	parse_options("options.txt");
	
	int len_cars = countlines(cars_file);
	int len_gps = countlines(gps_file);
	int len_data = (len_cars + 1) * 14;

	shm_key = ftok(".",getpid());

	// init cars shared memory
	int shmid_cars = shmget(shm_key, len_cars * sizeof(struct Car), IPC_CREAT | IPC_EXCL | 0666);
	if (shmid_cars < 0) {
		printf("error getting shared memory segment: cars\n");
		exit(-1);
	}
	struct Car *cars = shmat(shmid_cars, NULL, 0);
	
	int shmid_gps = shmget(shm_key + 1, len_gps * sizeof(struct GrandPrix), IPC_CREAT | IPC_EXCL | 0666);
	if (shmid_gps < 0) {
		printf("error getting shared memory segment: gps\n");
		exit(-1);
	}
	struct GrandPrix *gps = shmat(shmid_gps, NULL, 0);

	int shmid_data = shmget(shm_key + 2, len_data * sizeof(float), IPC_CREAT | IPC_EXCL | 0666);
	if (shmid_data < 0) {
		printf("error getting shared memory segment: data\n");
		exit(-1);
	}
	float *data = shmat(shmid_data, NULL, 0);

	sem_init(&sem_cars, 0, 1);
	sem_init(&sem_data, 0, 1);
	sem_init(&sem_gps, 0, 1);

	// init structs lists
	init_CARs(cars,read_file(cars_file));
	init_GPs(gps,read_file(gps_file), len_cars);

	//run_gp(0, len_cars, gps, data, cars);
	int i;
	for (i=0; i < len_gps; i++){
		run_gp(i,len_cars,gps,data,cars);
	}

	shmdt(gps);
	shmctl(shmid_gps,IPC_RMID,0);
	shmdt(cars);
	shmctl(shmid_cars,IPC_RMID,0);
	shmdt(data);
	shmctl(shmid_data,IPC_RMID,0);

	sem_destroy(&sem_cars);
	sem_destroy(&sem_data);
	sem_destroy(&sem_gps);
	return 0;
}