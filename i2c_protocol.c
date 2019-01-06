#include "i2c_wrapper.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>


//gcc -lm -lwiringPi i2c_wrapper.c i2c_protocol.c -o demo
#define WAIT_TIME 100
#define SIGNAL_TIME 1000

void error() {
    reset_i2c();
    exit(EXIT_FAILURE);
}

void START(){
    init_i2c();
    usleep(WAIT_TIME);
    set_sda_low();
    usleep(WAIT_TIME);
    set_scl_low();
}

void STOP(){
    set_scl_low();
    usleep(WAIT_TIME);
    set_sda_low();
    usleep(WAIT_TIME);
    set_scl_high();
    usleep(WAIT_TIME);
    set_sda_high();
    usleep(WAIT_TIME);
    reset_i2c();
}

void SEND_1BIT()  {
    set_sda_high();
    usleep(WAIT_TIME);
    set_scl_high();
    usleep(SIGNAL_TIME);
    set_scl_low();
    usleep(WAIT_TIME);
}

void SEND_0BIT()  {
    set_sda_low();
    usleep(WAIT_TIME);
    set_scl_high();
    usleep(SIGNAL_TIME);
    set_scl_low();
    usleep(WAIT_TIME);
}

int READ_BIT() {
    set_sda_high();
    usleep(WAIT_TIME);
    set_input_sda();
    usleep(WAIT_TIME);
    set_scl_high();
    usleep(SIGNAL_TIME);
    int bit = read_sda();
    set_scl_low();
    usleep(WAIT_TIME);
    set_output_sda();

    return bit;
}

int write_byte(int byte) {
    int buf[8];
    for(int i=0; i<8; i++) {
        buf[7-i] = byte%2;
        byte /= 2;
    }
    
    for(int i=0; i<8; i++) {
        if(buf[i] == 0) SEND_1BIT();
        else SEND_0BIT();
    }
    
    return 0;
}

int write_i2c(int slave, int reg, int data) {
    
    //start
    START();
   
    //writing adress
    write_byte(2*slave);

    if(READ_BIT()!=0) {
        printf("Wrong acknoladge bit A1");
        error();
    }
    usleep(WAIT_TIME);

    write_byte(reg);

    if(READ_BIT()!=0) {
        printf("Wrong acknoladge bit A2");
        error();
    }
    usleep(WAIT_TIME);

    write_byte(data);

    if(READ_BIT()!=0) {
        printf("Wrong acknoladge bit A3");
        error();
    }
   
    STOP();

    return 0;
}

int main()
{
    write_i2c(0x40, 0x00, 0x01);

    return 0;
}
