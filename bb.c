#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <math.h>
#include <ncurses.h>
#include <signal.h>
#include "ABE_ServoPi.h"

#define CONTROL_SIZE 1

#define servoMin 0// Minimum pulse length out of 4096
#define servoMed 400  // Medium pulse length out of 4096
#define servoMax 500  // Maximum pulse length out of 4096

#define CLOCKWISE 200
#define COUNTER_CLOCKWISE 500

#define LEFT_ENGINE 4
#define RIGHT_ENGINE 3
#define HEAD_ENGINE 2

#define DEVICE 0x40
//gcc -lm -lncurses ABE_ServoPi.c bb.c -o bb

enum CONTROLS {
	FORWARD ='w',
	BACKWARD = 'x',
	LEFT = 'a',
	RIGHT = 'd',
	FORWARD_LEFT = 'q',
	FORWARD_RIGHT = 'e',
	BACKWARD_LEFT = 'z',
	BACKWARD_RIGHT = 'c',
	STOP = 'p',
	PAUSE = 's'
};

void sighandler(int signum) {
	set_pwm(HEAD_ENGINE, 0, 0, DEVICE);
	set_pwm(RIGHT_ENGINE, 0, 0, DEVICE);
	set_pwm(LEFT_ENGINE, 0, 0, DEVICE);
	printf("Dostalem sigterma!");
	exit(1);
}


void error(char* message) {
		perror("Error while forking");
		exit(EXIT_FAILURE);
}

void servo_control(char command);


int kbhit() {
    int ch = getch();

    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else
        return 0;
}

void servo_control(char command) {
	usleep(10000);
	switch(command) {
			case FORWARD:
			 	printf("↑\r\n");
				set_pwm(RIGHT_ENGINE, 0, CLOCKWISE, DEVICE);
				usleep(10000);
				set_pwm(LEFT_ENGINE, 0, COUNTER_CLOCKWISE, DEVICE);
			 	break;
			case BACKWARD:
				printf("↓\r\n");
				set_pwm(RIGHT_ENGINE, 0, COUNTER_CLOCKWISE, DEVICE);
				usleep(10000);
				set_pwm(LEFT_ENGINE, 0, CLOCKWISE, DEVICE);
			 	break;
			case LEFT:
			 	printf("←\r\n");
				set_pwm(RIGHT_ENGINE, 0, CLOCKWISE, DEVICE);
				usleep(10000);
				set_pwm(LEFT_ENGINE, 0, CLOCKWISE, DEVICE);

			 	break;
			case RIGHT:
			 	printf("→\r\n");
				set_pwm(RIGHT_ENGINE, 0, COUNTER_CLOCKWISE, DEVICE);
				usleep(10000);
				set_pwm(LEFT_ENGINE, 0, COUNTER_CLOCKWISE, DEVICE);

			 	break;
			case FORWARD_LEFT:
			 	printf("↖\r\n");
				set_pwm(RIGHT_ENGINE, 0, CLOCKWISE, DEVICE);
				usleep(10000);
				set_pwm(LEFT_ENGINE, 0, CLOCKWISE, DEVICE);

			 	break;
			case FORWARD_RIGHT:
				set_pwm(RIGHT_ENGINE, 0, COUNTER_CLOCKWISE, DEVICE);
				usleep(10000);
				set_pwm(LEFT_ENGINE, 0, COUNTER_CLOCKWISE, DEVICE);

			 	printf("↗\r\n");
			 	break;
			case BACKWARD_LEFT:
				//set_pwm(RIGHT_ENGINE, 0, FPULSE, DEVICE);
				//set_pwm(LEFT_ENGINE, 0, FPULSE, DEVICE);

			 	printf("↙\r\n");
			 	break;
			case BACKWARD_RIGHT:
				//set_pwm(RIGHT_ENGINE, 0, FPULSE, DEVICE);
				//set_pwm(LEFT_ENGINE, 0, FPULSE, DEVICE);

			 	printf("↘\r\n");
			 	break;
			case PAUSE:
			 	set_pwm(HEAD_ENGINE, 0, 0, DEVICE);
			 	usleep(10000);
				set_pwm(RIGHT_ENGINE, 0, 0, DEVICE);
				usleep(10000);
				set_pwm(LEFT_ENGINE, 0, 0, DEVICE);
				usleep(10000);
			 	printf("o\r\n");
			 	break;
        default:
            break;
		}
		usleep(1000);
}


int init_control() {
	setvbuf(stdout, NULL, _IONBF, 0); // needed to print to the command line

	int x;
	int a;

	a = servopi_init(0x40, 1);
	if (a != 0) {
		if (a == 1) {
			printf("Error enabling GPIO Pin");
		}
		if (a == 2) {
			printf("Error setting GPIO Pin direction");
		}
		return (1);
	}
	set_pwm_freq(50, DEVICE);
	output_enable();

	return 0;
}

int main(void)
{
	signal(SIGTERM, sighandler);
	
	
	//setvbuf(stdout, NULL, _IONBF, 0); // needed to print to the command line

	int x;
	int a;

	a = servopi_init(0x40, 1);
	if (a != 0) {
		if (a == 1) {
			printf("Error enabling GPIO Pin");
		}
		if (a == 2) {
			printf("Error setting GPIO Pin direction");
		}
		return (1);
	}
	set_pwm_freq(50, DEVICE);
	output_enable();

    initscr();

    cbreak();
    noecho();
    nodelay(stdscr, TRUE);

    scrollok(stdscr, TRUE);
    while (1) {
        if (kbhit()) {
            char key_pressed = (char) getch();
            printw("Key pressed! It was: %c\n", key_pressed);
            if(key_pressed == STOP) {
                printw("Finishing.\r\n");
                set_pwm(HEAD_ENGINE, 0, 0, DEVICE);
				set_pwm(RIGHT_ENGINE, 0, 0, DEVICE);
				set_pwm(LEFT_ENGINE, 0, 0, DEVICE);
				endwin();
                exit(0);
            }
            servo_control(key_pressed);
            refresh();
        } else {
            refresh();
            sleep(1);
        }
    }
}
