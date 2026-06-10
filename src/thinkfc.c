/*
 * thinkfc - ThinkPad Fan Controller
 * Copyright (C) 2026 chrv80
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

FILE *temp_fd;
FILE *fan_ctrl_fd;
int temp_scale[8];

void fan_ctrl_error(){
    perror("No access to fan control file");
    exit(-2);
}

void defaut_temp_scale_set(){
        temp_scale[0] = 35;
        temp_scale[1] = 41;
        temp_scale[3] = 47;
        temp_scale[4] = 51;
        temp_scale[5] = 54;
        temp_scale[6] = 58;
        temp_scale[7] = 62;
}

void init(){
    FILE *conf_fd = fopen("/etc/thinkfc.conf", "r");

    if (conf_fd == NULL){
        printf("No config file found, loading default settings!\n");
        defaut_temp_scale_set();
        return;
    }

    for (int i = 0; i < 8; i++) {
        fscanf(conf_fd, "%d", &temp_scale[i]);
    }

    fclose(conf_fd);
}

int get_temp(){ //gets the max temp from the system
    char buffer[100];
    int max = -128; //-128 is missing sensor
    int temp;

    temp_fd = fopen("/proc/acpi/ibm/thermal","r");

    if (temp_fd == NULL){
        perror("No access to temperature file");
        exit(-1);
    }

    while (fscanf(temp_fd, "%s", buffer) > 0){
        temp = atoi(buffer);
        if (temp > max) {
            max = temp;
        }
    }
    fclose(temp_fd);
    return max;

}

void control_fan(int temp){
    fan_ctrl_fd = fopen("/proc/acpi/ibm/fan", "w");

    if (fan_ctrl_fd == NULL){
        fan_ctrl_error();
    }

    if (temp > temp_scale[7]){
        if(fprintf(fan_ctrl_fd,"level full-speed\n") <= 0){
            fan_ctrl_error();
        }
    } else if (temp > temp_scale[6]) {
        if(fprintf(fan_ctrl_fd,"level 7\n") <= 0){
            fan_ctrl_error();        
        }
    } else if (temp > temp_scale[5]) {
        if(fprintf(fan_ctrl_fd,"level 6\n") <= 0){
            fan_ctrl_error();        
        }
    } else if (temp > temp_scale[4]) {
        if(fprintf(fan_ctrl_fd,"level 5\n") <= 0){
            fan_ctrl_error();        
        }
    } else if (temp > temp_scale[3]) {
        if(fprintf(fan_ctrl_fd,"level 4\n") <= 0){
            fan_ctrl_error();        
        }
    } else if (temp > temp_scale[2]) {
        if(fprintf(fan_ctrl_fd,"level 3\n") <= 0){
            fan_ctrl_error();        
        }
    } else if (temp > temp_scale[1]) {
        if(fprintf(fan_ctrl_fd,"level 2\n") <= 0){
            fan_ctrl_error();        
        }
    } else if (temp > temp_scale[0]) {
        if(fprintf(fan_ctrl_fd,"level 1\n") <= 0){
            fan_ctrl_error();        
        }
    } else {
        if(fprintf(fan_ctrl_fd,"level auto\n") <= 0){
            fan_ctrl_error();        
        }
    } 


    fclose(fan_ctrl_fd);
}

int main(int argc, char *argv[]) {
    int temp = 100;
    init();
    while(1){
        temp = get_temp();
        control_fan(temp);
        sleep(2);
    }

    return 0;
}
