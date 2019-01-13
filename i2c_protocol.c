#include "i2c_wrapper.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

//gcc -lm -lwiringPi i2c_wrapper.c i2c_protocol.c -o demo

void error() {
    reset_i2c();
    exit(EXIT_FAILURE);
}

void START(){
    init_i2c();
    OUTPUT_WAIT;
    set_scl_low();
    OUTPUT_WAIT;
    set_sda_low();
}

void STOP(){
    set_scl_low();
    OUTPUT_WAIT;
    set_sda_low();
    OUTPUT_WAIT;
    set_scl_high();
    OUTPUT_WAIT;
    set_sda_high();
    OUTPUT_WAIT;
    reset_i2c();
}

void SEND_1BIT()  {
    set_sda_high();
    OUTPUT_WAIT;
    set_scl_high();
    SIGNAL_WAIT;
    set_scl_low();
    OUTPUT_WAIT;
}

void SEND_0BIT()  {
    set_sda_low();
    OUTPUT_WAIT;
    set_scl_high();
    SIGNAL_WAIT;
    set_scl_low();
    OUTPUT_WAIT;
}

int READ_BIT() {
    set_sda_high();
    OUTPUT_WAIT;
    set_input_sda();
    OUTPUT_WAIT;
    set_scl_high();
    SIGNAL_WAIT;
    int bit = read_sda();
    set_scl_low();
    OUTPUT_WAIT;
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
    OUTPUT_WAIT;

    write_byte(reg);

    if(READ_BIT()!=0) {
        printf("Wrong acknoladge bit A2");
        error();
    }
    OUTPUT_WAIT;

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
