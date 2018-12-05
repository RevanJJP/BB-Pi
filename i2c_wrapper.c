#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

//wPi pin numbers
#define SDA 8
#define SCL 9


int init_i2c() {
    wiringPiSetup();
    pinMode (SDA, OUTPUT);

    return 0;
}

int reset_i2c() {
    pinMode(SDA, 4);

    return 0;
}

void set_scl (int frequency) {

}

int get_scl_freq(){
    return 0;
}

void set_sda_high () {
    digitalWrite(SDA, HIGH);
}

void set_sda_low () {
    digitalWrite(SDA, LOW);
}

int read_sda() { 
    pinMode (SDA, INPUT);
    return digitalRead(SDA);
}
