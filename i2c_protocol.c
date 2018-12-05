#include "i2c_wrapper.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>


//gcc -lm -lwiringPi i2c_wrapper.c i2c_protocol.c -o demo
int WAIT_TIME = 100;

void error() {
    reset_i2c();
    exit(EXIT_FAILURE);
}

int write_byte(int byte) {
    int buf[8];
    for(int i=0; i<8; i++) {
        buf[7-i] = byte%2;
        byte /= 2;
    }
    
    for(int i=0; i<8; i++) {
        if(buf[i] == 0) set_sda_high();
        else set_sda_low();
        usleep(WAIT_TIME);
    }
    
    return 0;
}

int write_i2c(int slave, int reg, int data) {
    
    //start
    set_sda_low();
    usleep(WAIT_TIME);
   
    //writing adress
    write_byte(2*slave);

    if(read_sda!=0) {
        printf("Wrong acknoladge bit A1");
        error();
    }
    usleep(WAIT_TIME);

    write_byte(reg);

    if(read_sda!=0) {
        printf("Wrong acknoladge bit A2");
        error();
    }
    usleep(WAIT_TIME);

    write_byte(data);

    if(read_sda!=0) {
        printf("Wrong acknoladge bit A3");
        error();
    }
    usleep(WAIT_TIME);

    set_sda_high();
    usleep(WAIT_TIME);
}

int main()
{
    init_i2c();
    write_i2c(0x40, 0x00, 0x01);
    reset_i2c();

    return 0;
}
