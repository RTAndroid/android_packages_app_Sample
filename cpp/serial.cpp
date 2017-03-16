#include <jni.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <pthread.h>
#include <sched.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <sys/select.h>

#include <android/log.h>
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "BenchmarkJNI", __VA_ARGS__)

void start_serial(void)
{
    printf("# serial test\n");
    LOGE("Started serial stuff");

    //----- SETUP USART 0 -----

    //At bootup, pins 8 and 10 are already set to UART0_TXD, UART0_RXD (ie the alt0 function) respectively
    int uart0_filestream = -1;

    //OPEN THE UART
    //The flags (defined in fcntl.h):
    //	Access modes (use 1 of these):
    //		O_RDONLY - Open for reading only.
    //		O_RDWR - Open for reading and writing.
    //		O_WRONLY - Open for writing only.
    //
    //	O_NDELAY / O_NONBLOCK (same function) - Enables nonblocking mode. When set read requests on the file can return immediately with a failure status
	//											if there is no input immediately available (instead of blocking). Likewise, write requests can also return
	//											immediately with a failure status if the output can't be written immediately.
	//
	//	O_NOCTTY - When set and path identifies a terminal device, open() shall not cause the terminal device to become the controlling terminal for the process.
    printf("# opening the file\n");
//	uart0_filestream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
    uart0_filestream = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
	if (uart0_filestream == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT
		printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
        return;
	}
	
	//CONFIGURE THE UART
	//The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
	//	Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
	//	CSIZE:- CS5, CS6, CS7, CS8
	//	CLOCAL - Ignore modem status lines
	//	CREAD - Enable receiver
	//	IGNPAR = Ignore characters with parity errors
	//	ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
	//	PARENB - Parity enable
	//	PARODD - Odd parity (else even)
	struct termios options;
	int config = tcgetattr(uart0_filestream, &options);
    if (config < 0)
    {
        printf(" * failed to get config\n");
        return;
    }

	options.c_cflag = B115200 | CS8 | CREAD | CLOCAL;
	options.c_iflag = 0;//IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 0;

    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);
    
    //----- TX BYTES -----

    char str[] = "Hello, there!";
    printf(" * sending data\n");

    // filestream, bytes to write, number of bytes to write
    int count = write(uart0_filestream, str, strlen(str));
    if (count < 0)
    {
        printf("UART TX error\n");
        return;
    }
    
    printf(" * receiving data\n");
    
    // Read up to 255 characters from the port if they are there
    char rx_buffer[128];
    int i, rx_length;
    
    //Filestream, buffer to store in, number of bytes to read (max)
    for (i = 0; i < 5; i++)
    {
        printf(" new iteration\n");
        fflush(stdout);

        rx_length = read(uart0_filestream, &rx_buffer, sizeof(rx_buffer));
        if (rx_length == 0)
        {
            printf("  - no data found\n");
        }
        else
        {
            rx_buffer[rx_length] = '\0';
            printf("  - %i bytes read : %s\n", rx_length, rx_buffer);
        }

        sleep(1);
    }

    printf("Closing the stream\n");
    close(uart0_filestream);
}

extern "C" void Java_rtandroid_sample_SampleSerialThread_perform(JNIEnv* env, jobject /* this */)
{
    start_serial();
}
