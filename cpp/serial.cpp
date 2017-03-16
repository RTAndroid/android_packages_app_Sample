#include <jni.h>
#include <fcntl.h>
#include <stdio.h>
#include <termios.h>

#include <android/log.h>
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "SerialJNI", __VA_ARGS__)

// Save the file descriptor between multiple calls
int serialDescriptor = -1;

extern "C" void Java_rtandroid_sample_serial_SampleSerialDriver_open(JNIEnv* env, jobject /* this */)
{
    if (serialDescriptor > 0) { return; }

    // open in non blocking read/write mode
    serialDescriptor = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (serialDescriptor == -1)
    {
        LOGI("ERR: unable to open UART");
        return;
    }

    struct termios options;
    int config = tcgetattr(serialDescriptor, &options);
    if (config < 0)
    {
        LOGI("ERR: failed to get UART config");
        return;
    }

    options.c_cflag = B115200 | CS8 | CREAD | CLOCAL;
    options.c_iflag = 0;
    options.c_oflag = 0;
    options.c_lflag = 0;
    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 0;

    tcflush(serialDescriptor, TCIFLUSH);
    tcsetattr(serialDescriptor, TCSANOW, &options);
}

extern "C" void write_message(char message[])
{
    if (serialDescriptor < 0) { return; }

    ssize_t count = write(serialDescriptor, message, strlen(message));
    if (count < 0) { LOGI("ERR: failed to send data"); }
}

extern "C" void Java_rtandroid_sample_serial_SampleSerialDriver_logcat(JNIEnv* env, jobject /* this */)
{
    if (serialDescriptor < 0) { return; }

    char cleanup[] = { 12 };
    write_message(cleanup);

    char greeting[] = "Hello, there! I'll print to the logcat everything you type. Please exit with ESC.\n";
    write_message(greeting);

    char rx_buffer[128];
    ssize_t rx_length = 0;
    while (rx_length >= 0)
    {
        rx_length = read(serialDescriptor, &rx_buffer, sizeof(rx_buffer));
        if (rx_length <= 0) { continue; }
        if (rx_buffer[0] == 27) { rx_length = -1; }

        rx_buffer[rx_length] = '\0';
        LOGI("Received %d bytes: %s (%d)", (int) rx_length, rx_buffer, (int) rx_buffer[0]);
    }

    char goodbye[] = "\nGoodbye!";
    write_message(goodbye);
}

extern "C" void Java_rtandroid_sample_serial_SampleSerialDriver_close(JNIEnv* env, jobject /* this */)
{
    if (serialDescriptor < 0) { return; }

    close(serialDescriptor);
    serialDescriptor = -1;
}
