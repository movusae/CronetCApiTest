#include "android/log.h"
#include "include/cronet_c.h"
#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_com_complexzeng_cronetcapitest_MainActivity_stringFromJNI(
    JNIEnv *env, jobject /* this */) {
  std::string hello = "Hello from C++";
  Cronet_EnginePtr engine = Cronet_Engine_Create();
  Cronet_EngineParamsPtr params = Cronet_EngineParams_Create();

  if (auto res = Cronet_Engine_StartWithParams(engine, params); res == 0) {
    __android_log_print(ANDROID_LOG_INFO, "cplx",
                        "Cronet engine started successfully");
  } else {
    __android_log_print(ANDROID_LOG_ERROR, "cplx",
                        "Failed to start Cronet engine: %d", res);
  }
  return env->NewStringUTF(hello.c_str());
}
