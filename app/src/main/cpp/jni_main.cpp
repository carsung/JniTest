//
// Created by shcha on 2021-01-18.
//

#include "jni_main.h"

#include <android/log.h>

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
//#include <jni.h>
#define LOG_TAG "jni_main.cpp"

static const char *classPathName = "net/abc/sample/myjnitest5/NativeApi";

int thr_id;
int end_flag;
pthread_t p_thread[2];
JavaVM *g_jvmPtr;
jclass jObject;
jmethodID funccb;

void Notify(int n) {
    int value = 0;
    value = n;
    if (!g_jvmPtr) {
        LOGE(LOG_TAG, "error (!g_jvmPtr)");
        return;
    }

    if (!funccb) {
        LOGE(LOG_TAG, "error (!funccb)");
        return;
    }
    JNIEnv *env = NULL;
    g_jvmPtr->AttachCurrentThread(&env, NULL);
    if (env == NULL || jObject == NULL) {
        LOGE(LOG_TAG, "error (env == NULL || jObject == NULL)");
        return;
    }
    env->CallStaticVoidMethod(jObject, funccb, value);
    g_jvmPtr->DetachCurrentThread();
}

void *t_function(void *data) {
    int id;
    int i = 0;
    id = *((int*)data);

    while (end_flag) {
        LOGE(LOG_TAG, "while 1");
        Notify(i);
        LOGE(LOG_TAG, "while 2");
        i++;
        sleep(1);
        LOGE(LOG_TAG, "while 3");
        if (i > 10) end_flag = false;
    }
    LOGE(LOG_TAG, "while 4");
}

jint startThread_Native(JNIEnv *env, jobject thiz) {
    end_flag = 1;
    int b = 2;

    LOGD(LOG_TAG, "Call start thread");

    jclass cls;
    cls = env->FindClass(classPathName);
    if (cls == NULL) {
        LOGE(LOG_TAG, "Can't find the class");
    }
    jObject = (jclass)(env->NewGlobalRef(cls));
    funccb = env->GetStaticMethodID(cls, "callback", "(I)V");

    if (funccb == 0) {
        LOGE(LOG_TAG, "Can't find the function");
        env->DeleteGlobalRef(jObject);
    } else {
        LOGD(LOG_TAG, "Method connect success");
        env->CallStaticVoidMethod(cls, funccb, 10);
    }

    thr_id = pthread_create(&p_thread[1], NULL, t_function, (void *)&b);
    if (thr_id < 0) {
        LOGE(LOG_TAG, "Create thread fail\n");
        return -1;
    }
    return 0;
}

jint endThread_Native(JNIEnv* env, jobject thiz) {
    LOGD(LOG_TAG, "Call end thread");
    end_flag = 0;
    return 0;
}

static jint
add(JNIEnv *env, jobject thiz, jint a, jint b) {
    int result = a + b;
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "add %d + %d = %d", a, b, result);
    return result;
}

_jstring * stringFromJni(JNIEnv *env, jobject thiz)
{
#if defined(__arm__)
#if defined(__ARM_ARCH_7A__)
    #if defined(__ARM_NEON__)
      #if defined(__ARM_PCS_VFP)
        #define ABI "armeabi-v7a/NEON (hard-float)"
      #else
        #define ABI "armeabi-v7a/NEON"
      #endif
    #else
      #if defined(__ARM_PCS_VFP)
        #define ABI "armeabi-v7a (hard-float)"
      #else
        #define ABI "armeabi-v7a"
      #endif
    #endif
  #else
   #define ABI "armeabi"
  #endif
#elif defined(__i386__)
#define ABI "x86"
#elif defined(__x86_64__)
#define ABI "x86_64"
#elif defined(__mips64)  /* mips64el-* toolchain defines __mips__ too */
#define ABI "mips64"
#elif defined(__mips__)
#define ABI "mips"
#elif defined(__aarch64__)
#define ABI "arm64-v8a"
#else
#define ABI "unknown"
#endif

return env->NewStringUTF("Hello from JNI !  Compiled with ABI " ABI ".");
}



static JNINativeMethod methods[] = {
    { "add", "(II)I", (void*)add },
    { "stringFromJni", "()Ljava/lang/String;", (void*)stringFromJni },
    { "startThreadNative", "()I", (void*)startThread_Native },
    { "endThreadNative", "()I", (void*)endThread_Native },
};

/*static int registerNativeMethods(JNIEnv *env, const char* className,
    JNINativeMethod* gMethods, int numMethods) {
    jclass cls;

    cls = env->FindClass(className);
    if (cls == NULL) {
//        LOGE("Native registration unable to find class '%s'", className);
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG,
                "Native registration unable to find class '%s'", className);
        return JNI_FALSE;
    }
    if (env->RegisterNatives(cls, gMethods, numMethods) < 0) {
//        LOGE("RegisterNatives failed for '%s'", className);
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG,
                            "RegisterNatives failed for '%s'", className);
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

static int registerNatives(JNIEnv* env) {
    if (!registerNativeMethods(env, classPathName,
        methods, sizeof(methods) / sizeof(methods[0]))) {
            return JNI_FALSE;
    }
    return JNI_TRUE;
}*/

/*typedef union {
    JNIEnv* env;
    void* venv;
} UnionJNIEnvToVoid;*/

jint JNI_OnLoad(JavaVM *jvm, void *) {
    jint result = -1;
    JNIEnv* env = NULL;
    jclass cls;

    if (jvm->GetEnv((void **) &env, JNI_VERSION_1_6)) {
        LOGE(LOG_TAG,"ERROR: GetEnv failed");
    }

    cls = env->FindClass(classPathName);
    if (cls == NULL) {
        LOGE(LOG_TAG,"ERROR: cls failed");
    }

    if (env->RegisterNatives(cls, methods, sizeof(methods)/sizeof(methods[0])) < 0) {
        LOGE(LOG_TAG,"ERROR: RegisterNatives");
    }
    result = JNI_VERSION_1_6;
    g_jvmPtr = jvm;
    return result;
}

/*jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    UnionJNIEnvToVoid uenv;
    uenv.venv = NULL;
    jint result =  -1;
    JNIEnv* env = NULL;

    LOGD(LOG_TAG, "JNI_OnLoad");
//    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "JNI_OnLoad");

    if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_6) != JNI_OK) {
        LOGE(LOG_TAG,"ERROR: GetEnv failed");
//        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "ERROR: GetEnv failed");
        return JNI_ERR;
    }
    env = uenv.env;

    if (registerNatives(env) != JNI_TRUE) {
        return JNI_ERR;
    }

    result = JNI_VERSION_1_6;
    g_jvmPtr = vm;

    LOGD(LOG_TAG, "result = %d", result);
    return result;
}*/
