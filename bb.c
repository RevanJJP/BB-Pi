#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
//#include <linux/i2c-dev.h>
#include <math.h>

#define CONTROL_SIZE 1

enum CONTROLS {
	FORWARD ='w',
	BACKWARD = 'x',
	LEFT = 'a',
	RIGHT = 'd',
	FORWARD_LEFT = 'q',
	FORWARD_RIGHT = 'e',
	BACKWARD_LEFT = 'z',
	BACKWARD_RIGHT = 'c',
	STOP = 's',
	PAUSE = 'p'
};

void error(char* message) {
		perror("Error while forking");
		exit(EXIT_FAILURE);
}

FILE* get_controler();
void control(FILE* input);
void servo_control(char command);

int main(int argc, char const *argv[])
{

	//get descriptor for input
	FILE* input_descriptor=get_controler();
	if((int)input_descriptor==-1) error("Couldn't get input descriptor");

	//int id=fork();
	//printf("ID:%i", id);
	printf("DZIALAM:\n");
	//if(id==-1) error("Error while forking");

	//if(id==0) {
		control(input_descriptor);
		//exit(EXIT_SUCCESS);	
	//}

	//wait(&id);
	return 0;
}

void control(FILE* input)	{
	char buffer[CONTROL_SIZE];
	while (1) {
		fread(buffer, sizeof(char), CONTROL_SIZE, input);
		if(buffer[0]!=PAUSE) servo_control(buffer[0]);
		else return;
	}

}

FILE* get_controler(){
	FILE* input = fopen("/Users/revan/Library/Mobile Documents/com~apple~CloudDocs/Informatyka/Semestr 5/ZSC/ServoPi/bb8", "rb");
	return input;
}

void servo_control(char command) {
	
	switch(command) {
			case FORWARD:
			 printf("\t↑\n");
			 break;
			case BACKWARD:
			 printf("\t↓\n");
			 break;
			case LEFT:
			 printf("\t←\n");
			 break;
			case RIGHT:
			 printf("\t→\n");
			 break;
			case FORWARD_LEFT:
			 printf("\t↖\n");
			 break;
			case FORWARD_RIGHT:
			 printf("\t↗\n");
			 break;
			case BACKWARD_LEFT:
			 printf("\t↙\n");
			 break;
			case BACKWARD_RIGHT:
			 printf("\t↘\n");
			 break;
			case STOP:
			 printf("\to\n");
			 break;
		}
}