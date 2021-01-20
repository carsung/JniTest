package net.abc.sample.myjnitest5;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    NativeApi nativeApi;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView tv = findViewById(R.id.text_result);
        nativeApi = new NativeApi();
//        tv.setText("result = " + na.jniAdd(9, 10));
        tv.setText(nativeApi.getAbi());
        int ret = nativeApi.startThread();
        Log.i("jni_test", "startThread: " + ret);
    }

    @Override
    protected void onStop() {
        super.onStop();
//        int ret = nativeApi.endThread();
//        Log.i("jni_test", "endThread: " + ret);
    }
}