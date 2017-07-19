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

#define READY 22 // set HIGH to put capsule in flight mode.
#define RELEASE 23
#define DEAD 21 // set HIGH if capsule healthy.
#define CAPSULE 25

#define HANDLE serialOpen("/dev/ttyACM0", 38400)

void releasefunc(void) {
    // Other flight computer has initiated a release. A release will only be initiated if the capsule has not been declared DEAD.
    digitalWrite (READY, 1);
}

void *USBLoop(void* notused)
{
	pinMode (READY, OUTPUT); // To capsule.
	pinMode (DEAD, OUTPUT); // To flight computer.
	pinMode (RELEASE, INPUT); // To flight computer.
    pinmode (CAPSULE, INPUT); // To capsule.

    digitalWrite (DEAD, 0);
    digitalWrite (READY, 0);
    wiringPiISR(RELEASE, INT_EDGE_RISING, &releasefunc);

	while (1) {
        if(digitalRead(CAPSULE) == 1) {
            // Capsule is okay.
            digitalWrite (DEAD, 0);
        }
        else if(digitalRead(CAPSULE) == 0) {
            // Capsule is dead.
            digitalWrite (DEAD, 1);
        }
        sleep(5);
	}
	return 0;
}
