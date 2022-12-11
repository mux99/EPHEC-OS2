void wipe_data(float* data, int len_cars)
{
    int i;
    int len_data = (len_cars + 1) * 14;
	for (i = 0; i < len_data; i++)
	{
		if (((i + 1) % (len_cars + 1) == 0) && (i > 0))
		{
			data[i] = end;
		}
		else
		{
			data[i] = 0;
		}
	}
}

void wipe_data_segment(float* data)
{
    int i;
    for (i = 0; data[i] != end; i++)
    {
        data[i] = 0;
    }
}

////SIMULATION/////////////////////////////////////////////////////////////////////////

void practice(int gp, int len_cars)
{
	int i; int status;
	pid_t pid_childs[len_cars];
	for (i = 0; i < len_cars; i++)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			car_sim_practice(i, 3600, gp);
			exit(0);
		}
		else if (pid > 0) pid_childs[i] = pid;
	}
	for (i = 0; i < len_cars; i++) waitpid(pid_childs[i], &status, 0);
}

void qualifications(int gp, int len_cars, int step, int lenght)
{
	int i; int status;
	pid_t pid_childs[len_cars];
	for (i = 0; i < len_cars; i++)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			car_sim_qualifs(i, gp, lenght, step);
			exit(0);
		}
		else if (pid > 0) pid_childs[i] = pid;
	}
	
	for (i = 0; i < len_cars; i++) waitpid(pid_childs[i], &status, 0);
}

void sprint(int gp, int len_cars, int lenght)
{
	int i; int status;
	pid_t pid_childs[len_cars];
	for (i = 0; i < len_cars; i++)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			car_sim_sprint(i, gp, lenght);
			exit(0);
		}
		else if (pid > 0) pid_childs[i] = pid;
	}
	for (i = 0; i < len_cars; i++) waitpid(pid_childs[i], &status, 0);
}

void race(int gp, int len_cars, int lenght)
{
	int i; int status;
	pid_t pid_childs[len_cars];
	for (i = 0; i < len_cars; i++)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			car_sim_race(i, gp, lenght);
			exit(0);
		}
		else if (pid > 0) pid_childs[i] = pid;
	}
	for (i = 0; i < len_cars; i++) waitpid(pid_childs[i], &status, 0);
}

////ORGANISATION/////////////////////////////////////////////////////////////////////////////////////////////////////////

void qualify(float* data, struct Car *cars, int len_cars, int step)
{
	int i;
	switch(step)
	{
		case 1:
			int* order_q1 = sort(data+(q1*(len_cars+1)));
			for (i=len_cars-1;i>=0;i--)
			{
				if (i >= 15) cars[order_q1[i]].is_out_q2 = true;
				data[(srt*(len_cars+1))+i] = order_q1[i];
			}
			free(order_q1);
			break;
		
		case 2:
			int* order_q2 = sort(data+(q2*(len_cars+1)));
			for (i=14;i>=10;i--)
			{
				cars[order_q2[i]].is_out_q3 = true;
				data[(srt*(len_cars+1))+i] = order_q2[i];
			}
			free(order_q2);
			break;

		case 3:
			int* order_q3 = sort(data+(q3*(len_cars+1)));
			for (i=9;i>=0;i--)
			{
				data[(srt*(len_cars+1))+i] = order_q3[i];
			}
			free(order_q3);
			break;
		
		case 4:
			int* order_sti = sort(data+(sti*(len_cars+1)));
			for (i=len_cars-1;i>=0;i--)
			{
				data[(srt*(len_cars+1))+i] = order_sti[i];
			}
			free(order_sti);
			break;
	}
}

void ask_save(struct GrandPrix* gps, struct Car* cars, float* data, int gp, int step, int len_cars)
{
	char input; int c;
	scanf("%c",&input);
	if (input == 'y'){
		save_data(gps,cars,data,gp,step,len_cars);
	}
	while ((c = getchar()) != '\n' && c != EOF);
}

void run_gp(int gp, int len_cars, struct GrandPrix *gps, float *data, struct Car *cars)
{
	int i;
	pid_t pid = fork();
	if (pid == 0)
	{
		display(gp);
		exit(0);
	}

	int type = gps[gp].weekend_type;

	// clear shared memory
	wipe_data(data,len_cars);

	// P1
	gps[gp].GP_state = 1;
	practice(gp, len_cars);
	gps[gp].GP_state = -1;
	ask_save(gps,cars,data,gp,1,len_cars);

	// if (type == 1) {
	// 	// P2
	// 	wipe_data_segment(data+s1*(len_cars+1));
	// 	wipe_data_segment(data+s2*(len_cars+1));
	// 	wipe_data_segment(data+s3*(len_cars+1));
	// 	wipe_data_segment(data+lpc*(len_cars+1));
	// 	gps[gp].GP_state = 1;
	// 	practice(gp, len_cars);
	// 	gps[gp].GP_state = -1;
	// 	ask_save(gps,cars,data,gp,1,len_cars);

	// 	// P3
	// 	wipe_data_segment(data+s1*(len_cars+1));
	// 	wipe_data_segment(data+s2*(len_cars+1));
	// 	wipe_data_segment(data+s3*(len_cars+1));
	// 	wipe_data_segment(data+lpc*(len_cars+1));
	// 	gps[gp].GP_state = 1;
	// 	practice(gp, len_cars);
	// 	gps[gp].GP_state = -1;
	// 	ask_save(gps,cars,data,gp,1,len_cars);
	// }

	// // Q1
	// wipe_data_segment(data+s1*(len_cars+1));
	// wipe_data_segment(data+s2*(len_cars+1));
	// wipe_data_segment(data+s3*(len_cars+1));
	// wipe_data_segment(data+lpc*(len_cars+1));
	// gps[gp].GP_state = 2;
	// qualifications(gp, len_cars, 0, 1080);
	// gps[gp].GP_state = -2;
	// qualify(data,cars,len_cars,1);
	// ask_save(gps,cars,data,gp,2,len_cars);

	// // Q2
	// wipe_data_segment(data+s1*(len_cars+1));
	// wipe_data_segment(data+s2*(len_cars+1));
	// wipe_data_segment(data+s3*(len_cars+1));
	// wipe_data_segment(data+lpc*(len_cars+1));
	// gps[gp].GP_state = 3;
	// qualifications(gp, len_cars, 1, 900);
	// gps[gp].GP_state = -3;
	// qualify(data,cars,len_cars,2);
	// ask_save(gps,cars,data,gp,3,len_cars);

	// // Q3
	// wipe_data_segment(data+s1*(len_cars+1));
	// wipe_data_segment(data+s2*(len_cars+1));
	// wipe_data_segment(data+s3*(len_cars+1));
	// wipe_data_segment(data+lpc*(len_cars+1));
	// gps[gp].GP_state = 4;
	// qualifications(gp, len_cars, 2, 720);
	// gps[gp].GP_state = -4;
	// qualify(data,cars,len_cars,3);
	// ask_save(gps,cars,data,gp,4,len_cars);

	// if (type == 2)
	// {
	// 	// P2
	// 	wipe_data_segment(data+s1*(len_cars+1));
	// 	wipe_data_segment(data+s2*(len_cars+1));
	// 	wipe_data_segment(data+s3*(len_cars+1));
	// 	wipe_data_segment(data+lpc*(len_cars+1));
	// 	gps[gp].GP_state = 1;
	// 	practice(gp, len_cars);
	// 	gps[gp].GP_state = -1;
	// 	ask_save(gps,cars,data,gp,1,len_cars);

	// 	// SPRINT
	// 	wipe_data_segment(data+s1*(len_cars+1));
	// 	wipe_data_segment(data+s2*(len_cars+1));
	// 	wipe_data_segment(data+s3*(len_cars+1));
	// 	wipe_data_segment(data+lpc*(len_cars+1));
	// 	gps[gp].GP_state = 5;
	// 	sprint(gp, len_cars, gps[gp].sprint_laps_number);
	// 	gps[gp].GP_state = -5;
	// 	qualify(data,cars,len_cars,4);
	// 	ask_save(gps,cars,data,gp,5,len_cars);
	// }

	// // RACE
	// wipe_data_segment(data+s1*(len_cars+1));
	// wipe_data_segment(data+s2*(len_cars+1));
	// wipe_data_segment(data+s3*(len_cars+1));
	// wipe_data_segment(data+lpc*(len_cars+1));
	// gps[gp].GP_state = 6;
	// race(gp, len_cars, gps[gp].race_laps_number);
	// gps[gp].GP_state = -6;
	// ask_save(gps,cars,data,gp,6,len_cars);

	// //recap
	// gps[gp].GP_state = 7;
	// ask_save(gps,cars,data,gp,7,len_cars);

	// end gp
	gps[gp].GP_state = 100;
	shmdt(gps);
	int status;
	waitpid(pid,&status,0);
}