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

void *USBLoop(void* notused)
{
    // Open serial channel.
    int handle = serialOpen ("/dev/ttyAMA0", 19200);

	// We have 2 LED outputs
	// pinMode (Config.LED_Warn, OUTPUT);
	// pinMode (Config.LED_OK, OUTPUT);

	while (1)
	{
        // serialPuts (handle,char *s) ;
		// digitalWrite (Config.LED_Warn, (GPS->Satellites < 4) && (GPS->Altitude < 2000) && (GPS->MessageCount & 1));
		// digitalWrite (Config.LED_OK, (GPS->Satellites >= 4) && (GPS->Altitude < 2000) && (Flash ^= 1));

		sleep(1);
	}
    serialClose(handle);
	return 0;
}
