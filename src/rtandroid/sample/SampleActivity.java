package rtandroid.sample;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import rtandroid.sample.serial.SampleSerialThread;
import rtandroid.thread.ExecutionParameters;
import rtandroid.thread.SchedulingPolicy;

public class SampleActivity extends Activity
{
    private static final String TAG = SampleActivity.class.getSimpleName();

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        try { Log.d(TAG, "RTAndroid: " + rtandroid.BuildInfo.getVersion()); }
        catch (Exception e) { Log.e(TAG, "Failed to get version", e); }
    }

    public void onStartRealtimeThread(View view)
    {
        ExecutionParameters params = new ExecutionParameters();
        params.setSchedulingPolicy(SchedulingPolicy.FIFO);
        params.setSchedulingPriority(60);

        SampleRealtimeThread thread = new SampleRealtimeThread(params);
        thread.start();
    }

    public void onStartSerialThread(View view)
    {
        SampleSerialThread thread = new SampleSerialThread();
        thread.start();
    }
}
