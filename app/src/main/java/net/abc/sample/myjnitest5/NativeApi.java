package net.abc.sample.myjnitest5;

import android.util.Log;

public class NativeApi {
    static {
        System.loadLibrary("csh_test-lib");
    }

    private native int add (int a, int b);
    private native String stringFromJni();
    private native int startThreadNative();
    private native int endThreadNative();

    public int jniAdd(int a, int b) {
        return add(4, 7);
    }
    public String getAbi() {
        return stringFromJni();
    }
    public int startThread() {
        return startThreadNative();
    }
    public int endThread() {
        return endThreadNative();
    }

    public static void callback(int a) {
        Log.i("jni_test", "callback: " + a);
    }
}
