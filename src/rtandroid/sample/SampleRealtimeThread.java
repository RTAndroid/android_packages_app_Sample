package rtandroid.sample;

import android.util.Log;

import rtandroid.thread.ExecutionParameters;
import rtandroid.thread.RealtimeThread;
import rtandroid.thread.SchedulingPolicy;

public class SampleRealtimeThread extends RealtimeThread
{
    private static final String TAG = SampleRealtimeThread.class.getSimpleName();

    public SampleRealtimeThread(ExecutionParameters executionParameters)
    {
        super(executionParameters);
    }

    @Override
    public void run()
    {
        int tid = android.os.Process.myTid();
        Log.i(TAG, "Started a new real-time thread with tid=" + tid);

        try
        {
            SchedulingPolicy policy = getSchedulingPolicy();
            Log.v(TAG, "current policy = " + policy.name());

            int priority = getSchedulingPriority();
            Log.v(TAG, "current priority = " + priority);

                /*
                 * Insert more logic
                 */

            Thread.sleep(3000);
        }
        catch (Exception e) { Log.i(TAG, "Exception occurred: " + e.getMessage()); }

        Log.i(TAG, "Terminating real-time thread with tid=" + tid);
    }
}
