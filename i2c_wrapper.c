#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Your libraries
#include <wiringPi.h>
//

//Your SDA and SCL definitions
#define SDA 8
#define SCL 9
//

#define DATA 'D'
#define CLOCK 'C'

#define SDA_PATH 
#define SCL_PATH 

FILE* debug_sda;
FILE* debug_scl;

uint64_t TIME_START = 0;
uint64_t current_time() {
    //https://stackoverflow.com/questions/10192903/time-in-milliseconds
    struct timespec my_time;
    clock_gettime(CLOCK_MONOTONIC_RAW, &my_time);

    uint64_t time_data = my_time.tv_sec*1000000 + my_time.tv_nsec/1000;
    
    return time_data;
}

void debug(char line, char* val) {
    uint64_t time=current_time()-TIME_START;
    if(val[0] != '0' && val[0] != '1') {
        if(line==DATA)   {
           fprintf(debug_sda, "%s\n", val);
        }
        if(line==CLOCK)   {
            fprintf(debug_scl, "%s\n", val);
        }
    }
    else {
        if(line==DATA)   {
            fprintf(debug_sda, "%s %lld\n", val, time);
        }
        if(line==CLOCK)   {
            fprintf(debug_scl, "%s %lld\n", val, time);
        }
    }
}

int init_i2c() {
    debug_sda=fopen("debug_sda.txt", "w");
    debug_scl=fopen("debug_scl.txt", "w");

    if(debug_sda < 0) {
        perror("File SDA didn't open");
        exit(1);
    }
    if(debug_scl < 0) {
        perror("File SCL didn't open");
        exit(1);
    }
    
    TIME_START=current_time();

    //Your init|setup function
    wiringPiSetup();

    //starting i2c by setting high signal on both SDA and SCL
    //default set on output
    debug(DATA, "\nSTART\n");
    debug(CLOCK, "\nSTART\n");

    //Set SDA and SCL on Output
    pinMode (SDA, OUTPUT);
    pinMode (SCL, OUTPUT);
    //


    digitalWrite(SDA, HIGH);
    SIGNAL_WAIT;

    digitalWrite(SCL, HIGH);
    SIGNAL_WAIT;

    debug(DATA, "\nEND START\n");
    debug(CLOCK, "\nEND START\n");
    return 0;
}

int reset_i2c() {
    debug(DATA, "\nC\n");
    debug(CLOCK, "\nC\n");

    if (fclose(debug_scl) < 0) {
        printf("Descriptor: %d\n",(int) debug_scl);
        perror("File SCL didn't close");
        exit(1);
    }
    if (fclose(debug_sda) < 0) {
        printf("Descriptor: %d\n",(int) debug_sda);
        perror("File SDA didn't close");
        exit(1);
    }

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
    debug(CLOCK, "1");
}

void set_scl_low (int frequency) {
    digitalWrite(SCL, LOW);
    debug(CLOCK, "0");
}

void set_sda_high () {
    //Set SDA on HIGH signal
    digitalWrite(SDA, HIGH);
    //
    debug(DATA, "1");
}

void set_sda_low () {
    //Set SDA on LOW signal
    digitalWrite(SDA, LOW);
    //

    debug(DATA, "0");
}


void set_input_sda() {
    //Set SDA intput on your device
    pinMode (SDA, INPUT);
    //

    debug(DATA, "\nI\n");
}

void set_output_sda() {
    //Set SDA output on your device
    pinMode (SDA, OUTPUT);
    //

    debug(DATA, "\nO\n");
}

int read_sda() { 
    int val=0;

    //Receive data from SDA to 'val'
    val=digitalRead(SDA);
    //

    if(val == 0) {
        debug(DATA, "0");
        return 0;
    }
    else {
        debug(DATA, "1");
        return 1;
    }
}

// int read_scl() { 
//     pinMode (SCL, INPUT);
//     usleep(100);
//     int reading=0;

//     reading = digitalRead(SDA);
    
//     usleep(100);
//     pinMode (SCL, OUTPUT);

//     //do wywalenia
//     debug(CLOCK, "0");
    
//     if(reading == 0){
//         debug(CLOCK, "0");
//         return 0;
//     } else {
//         debug(CLOCK, "1");
//         return 1;
//     }
// }