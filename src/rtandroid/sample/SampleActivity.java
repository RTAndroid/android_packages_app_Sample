package rtandroid.sample;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import rtandroid.cpu.CpuPackage;
import rtandroid.thread.ExecutionParameters;
import rtandroid.thread.RealtimeThread;
import rtandroid.thread.SchedulingPolicy;

public class SampleActivity extends Activity
{
    private static final String TAG = SampleActivity.class.getSimpleName();

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView tvInfo = (TextView) findViewById(R.id.tv_info);
        tvInfo.setText(R.string.app_desc);

        ExecutionParameters params = new ExecutionParameters();
        params.setSchedulingPolicy(SchedulingPolicy.FIFO);
        params.setSchedulingPriority(60);

        SampleThread thread = new SampleThread(params);
        thread.start();
    }

    private static class SampleThread extends RealtimeThread
    {
        SampleThread(ExecutionParameters executionParameters)
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
}
