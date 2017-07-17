#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <stdio.h>   	// Standard input/output definitions
#include <string.h>  	// String function definitions
#include <unistd.h>  	// UNIX standard function definitions
#include <fcntl.h>   	// File control definitions
#include <errno.h>   	// Error number definitions
#include <termios.h> 	// POSIX terminal control definitions
#include <stdint.h>
#include <stdlib.h>
#include <dirent.h>
#include <math.h>
#include <pthread.h>
#include <wiringPi.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <sys/statvfs.h>
#include <pigpio.h>
#include <inttypes.h>

#include "usb.h"

#define PHOTO 4
#define VIDEO 5

// Handle photo capture interrupt
void photoFunc(void){
	char PhotoCommand[1000];
    sprintf( PhotoCommand, "raspistill -st -w 2592 -h 1944 -t 3000 -ex auto -mm matrix -o /home/pi/pits-camera/camera/photos/%u.jpg", (unsigned)time(NULL));
	system(PhotoCommand);
}
// Handle video capture interrupt
void videoFunc(void){
	char VideoCommand[1000];
    sprintf( VideoCommand, "raspivid -t 180000 -w 1280 -h 720 -fps 60 -o /home/pi/pits-camera/camera/video/%u.h264 & disown", (unsigned)time(NULL));
	system(VideoCommand);
}

int main(void)
{
	// Set up I/O
	if (wiringPiSetup() == -1)
	{
		printf("Cannot initialise WiringPi\n");
		exit (1);
	}

	pthread_t USBThread;

	if (pthread_create(&USBThread, NULL, USBLoop, NULL))
	{
		fprintf(stderr, "Error creating USB thread\n");
		return 1;
	}

    // Code for sending a command to camera.

    pinMode (PHOTO, INPUT);
    pinMode (VIDEO, INPUT);

    // Perform capture of footage on GPIO signals.
    wiringPiISR(PHOTO, INT_EDGE_RISING, &photoFunc);
    wiringPiISR(VIDEO, INT_EDGE_RISING, &videoFunc);

    while (1)
	{
        // Do nothing..
		sleep(5);
	}
}
