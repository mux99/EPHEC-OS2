void parse_options(char* path) {
    char** options = split(read_file(path),"\n");
    char** tmp;
    tmp = split(options[0],"=");
    memcpy(cars_file,tmp[1],50);
    free(tmp);
    tmp = split(options[1],"=");
    memcpy(gps_file,tmp[1],50);
    free(tmp);
    tmp = split(options[2],"=");
    speed = atoi(tmp[1]);
    free(tmp);
    tmp = split(options[3],"=");
    pit_min = atoi(tmp[1]);
    free(tmp);
    tmp = split(options[4],"=");
    pit_max = atoi(tmp[1]);
    free(tmp);
    tmp = split(options[5],"=");
    pit_time = atoi(tmp[1]);
    free(tmp);
    tmp = split(options[6],"=");
    practice_lenght = atoi(tmp[1]);
    free(tmp);
    tmp = split(options[7],"=");
    qualif_1_lenght = atoi(tmp[1]);
    free(tmp);
    tmp = split(options[8],"=");
    qualif_2_lenght = atoi(tmp[1]);
    free(tmp);
    tmp = split(options[9],"=");
    qualif_3_lenght = atoi(tmp[1]);
    free(tmp);
    tmp = split(options[10],"=");
    sprint_lenght = atoi(tmp[1]);
    free(tmp);
    tmp = split(options[11],"=");
    race_lenght = atoi(tmp[1]);
    free(tmp);
    free(options);
}