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

void *USBLoop(void* notused)
{
    // Open serial channel.
    int handle = serialOpen ("/dev/ttyAMA0", 19200);

	// We have 2 LED outputs
	pinMode (READY, OUTPUT);
	pinMode (DEAD, OUTPUT);
	pinMode (RELEASE, INPUT);

	while (1)
	{
        char *health = "HEALTH?";
        char response[10];
        serialPuts (handle, health);
        sleep(2);
        int responseLen =  serialDataAvail(handle);
        for(int i = 0; i<responseLen; i++) {
            response[i] = serialGetchar(handle);
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
            // response is garbled.
        }
        if(digitalRead(RELEASE)) {
            // Other flight computer has initiated a release.
            char *eject = "EJECT";
            char ejectResponse[10];
            serialPuts (handle, eject);
            sleep(2);
            int responseLen =  serialDataAvail(handle);
            for(int i = 0; i<responseLen; i++) {
                ejectResponse[i] = serialGetchar(handle);
            }
            if(strcmp(ejectResponse,"READY") == 0) {
                // Capsule is okay.
                digitalWrite (READY, 1);
            }
            else{
                // response is garbled.
            }
        }

		//
		// digitalWrite (Config.LED_OK, (GPS->Satellites >= 4) && (GPS->Altitude < 2000) && (Flash ^= 1));
		sleep(1);
	}
    serialClose(handle);
	return 0;
}
