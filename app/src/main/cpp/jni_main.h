//
// Created by shcha on 2021-01-18.
//

#ifndef MYJNITEST5_JNI_MAIN_H
#define MYJNITEST5_JNI_MAIN_H

#include <jni.h>
#include <stdio.h>

#define LOGD(tag, fmt, ...) __android_log_print(\
                                ANDROID_LOG_DEBUG, tag, fmt \
                                "\n--> %s\n----> %s:%d", \
                                ##__VA_ARGS__, __FILE__, __FUNCTION__, __LINE__)
#define LOGE(tag, fmt, ...) __android_log_print(\
                                ANDROID_LOG_ERROR, tag, fmt \
                                "\n--> %s\n----> %s:%d", \
                                ##__VA_ARGS__, __FILE__, __FUNCTION__, __LINE__)



class jni_main {

};



#endif //MYJNITEST5_JNI_MAIN_H
