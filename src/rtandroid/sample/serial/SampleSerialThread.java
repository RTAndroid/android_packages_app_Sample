package rtandroid.sample.serial;

import android.util.Log;

import java.io.File;

public class SampleSerialThread extends Thread
{
    private static final String TAG = SampleSerialThread.class.getSimpleName();
    static{ System.loadLibrary("serial"); }

    @Override
    public void run()
    {
        int tid = android.os.Process.myTid();
        Log.i(TAG, "Started a new serial thread with tid=" + tid);

        /*
         * We need access to the serial port in /dev/ttyS0
         */

        try
        {
            rtandroid.root.PrivilegeElevator.enableRoot();
            File tty = new File("/dev/ttyS0");
            tty.setReadable(true, false);
            tty.setWritable(true, false);
            rtandroid.root.PrivilegeElevator.disableRoot();
        }
        catch (Exception e) { Log.i(TAG, "Exception: " + e.getMessage()); }

        /*
         * At bootup, pins 8 and 10 are already set to UART0_TXD, UART0_RXD (i.e. the alt0 function) respectively
         */

        SampleSerialDriver.open();
        SampleSerialDriver.logcat();
        SampleSerialDriver.close();

        Log.i(TAG, "Terminating serial thread with tid=" + tid);
    }
}
