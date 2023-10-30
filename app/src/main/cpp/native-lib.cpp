#include "main.h"
#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_com_complexzeng_cronetcapitest_MainActivity_stringFromJNI(
    JNIEnv *env, jobject /* this */) {
  std::string hello = "Hello from C++";
  test_main();
  return env->NewStringUTF(hello.c_str());
}
