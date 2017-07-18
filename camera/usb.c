#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <wiringSerial.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <inttypes.h>

#define READY 22 // set hi to allow flight computer to know if capsule is ready for release.
#define RELEASE 23 // in case of emergency release.
#define DEAD 21 // If NOGO response is received.

#define HANDLE serialOpen("/dev/ttyAMA0", 38400)

void releasefunc(void) {
    // Other flight computer has initiated a release. A release will only be initiated if the capsule has not been declared DEAD.
    char *eject = "EJECT";
    char ejectResponse[10];
    serialFlush(HANDLE);
    serialPuts (HANDLE, eject);
    sleep(2);
    int responseLen =  serialDataAvail(HANDLE);
    int i;
    for(i = 0; i<responseLen; i++) {
        ejectResponse[i] = serialGetchar(HANDLE);
    }
    if(strcmp(ejectResponse,"READY") == 0) {
        // Capsule is ready for release.
        digitalWrite (READY, 1);
    }
    else{
        // response is garbled.
        digitalWrite (READY, 0);
        releasefunc();
    }
}

void *USBLoop(void* notused)
{
	pinMode (READY, OUTPUT);
	pinMode (DEAD, OUTPUT);
	pinMode (RELEASE, INPUT);

    wiringPiISR(RELEASE, INT_EDGE_RISING, &releasefunc);
    // TODO Implement functionality if capsule is critical.
	while (1)
	{
        serialFlush(HANDLE);
        char *health = "HEALTH?";
        char response[10];
        serialPuts (HANDLE, health);
        sleep(5);
        int responseLen =  serialDataAvail(HANDLE);
        int i;
        for(i = 0; i<responseLen; i++) {
            response[i] = serialGetchar(HANDLE);
        }
        if(strcmp(response,"OKAY") == 0) {
            // Capsule is okay.
            digitalWrite (DEAD, 0);
        }
        else if(strcmp(response,"NOGO") == 0) {
            // Capsule is dead.
            digitalWrite (DEAD, 1);
        }
        else{
            // response is garbled, do nothing.
        }
        sleep(1);
	}
    serialClose(HANDLE);
	return 0;
}
