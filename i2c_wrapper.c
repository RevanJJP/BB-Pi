#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

//wPi pin numbers
#define SDA 8
#define SCL 9


int init_i2c() {
    wiringPiSetup();

    //starting i2c by setting high signal on both SDA and SCL
    //default set on output
    pinMode (SDA, OUTPUT);
    digitalWrite(SDA, HIGH);
    usleep(1000);

    pinMode (SCL, OUTPUT);
    digitalWrite(SCL, HIGH);
    usleep(1000);

    return 0;
}

int reset_i2c() {
    if(system("gpio mode 8 alt0")!=0) {
        perror("Couldn't reset SDA pin to ALT0");
        return 1;
    }
    if(system("gpio mode 9 alt0")!=0) {
        perror("Couldn't reset SCL pin to ALT0");
        return 1;
    }
    return 0;
}

void set_scl_high (int frequency) {
    digitalWrite(SCL, HIGH);
}

void set_scl_low (int frequency) {
    digitalWrite(SCL, LOW);
}

int read_scl() { 
    pinMode (SCL, INPUT);
    usleep(100);
    int reading = digitalRead(SDA);
    
    usleep(100);
    pinMode (SCL, OUTPUT);

    return reading;
}

void set_sda_high () {
    digitalWrite(SDA, HIGH);
}

void set_sda_low () {
    digitalWrite(SDA, LOW);
}


void set_input_sda() {
    pinMode (SDA, INPUT);
}

void set_output_sda() {
    pinMode (SDA, OUTPUT);
}

int read_sda() { 
    return digitalRead(SDA);
}
