package rtandroid.sample;

import android.provider.Settings;
import android.util.Log;

import rtandroid.thread.ExecutionParameters;
import rtandroid.thread.RealtimeThread;
import rtandroid.thread.SchedulingPolicy;

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
         * Connect your input to pins 8 (TX) and 10 (RX)
         */

        try
        {
            perform();
            Thread.sleep(3000);
        }
        catch (Exception e) { Log.i(TAG, "Exception occurred: " + e.getMessage()); }

        Log.i(TAG, "Terminating serial thread with tid=" + tid);
    }

    private native void perform();
}
