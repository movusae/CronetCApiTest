// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "android/log.h"
#include "cronet.idl_c.extras.h"
#include "include/cronet_c.h"
#include "sample_executor.h"
#include "sample_url_request_callback.h"
#include <iostream>
Cronet_EnginePtr CreateCronetEngine() {
  Cronet_EnginePtr cronet_engine = Cronet_Engine_Create();
  Cronet_EngineParamsPtr engine_params = Cronet_EngineParams_Create();
  Cronet_EngineParams_user_agent_set(engine_params, "CronetSample/1");
  Cronet_EngineParams_enable_quic_set(engine_params, true);
  Cronet_Engine_StartWithParams(cronet_engine, engine_params);
  Cronet_EngineParams_Destroy(engine_params);
  return cronet_engine;
}
void PerformRequest(Cronet_EnginePtr cronet_engine, const std::string &url,
                    Cronet_ExecutorPtr executor) {
  SampleUrlRequestCallback url_request_callback;
  Cronet_UrlRequestPtr request = Cronet_UrlRequest_Create();
  Cronet_UrlRequestParamsPtr request_params = Cronet_UrlRequestParams_Create();
  Cronet_UrlRequestParams_http_method_set(request_params, "GET");
  Cronet_UrlRequest_InitWithParams(
      request, cronet_engine, url.c_str(), request_params,
      url_request_callback.GetUrlRequestCallback(), executor);
  Cronet_UrlRequestParams_Destroy(request_params);
  Cronet_UrlRequest_Start(request);
  url_request_callback.WaitForDone();
  Cronet_UrlRequest_Destroy(request);
  __android_log_print(ANDROID_LOG_INFO, "cplx", "resp len: %zu",
                      url_request_callback.response_as_string().size());
}

// Download a resource from the Internet. Optional argument must specify
// a valid URL.
int test_main() {
  __android_log_print(ANDROID_LOG_INFO, "cplx",
                      "Failed to start Cronet engine: %s",
                      "Hello from Cronet!\n");
  Cronet_EnginePtr cronet_engine = CreateCronetEngine();
  __android_log_print(ANDROID_LOG_INFO, "cplx", "%s %s\n", "Cronet version: ",
                      Cronet_Engine_GetVersionString(cronet_engine));
  std::string url("https://pic-go-1252561521.cos.ap-guangzhou.myqcloud.com/"
                  "01c36a82-8621-477f-a96b-ee7b5a69e644.gif");

  __android_log_print(ANDROID_LOG_INFO, "cplx", "URL: %s\n", url.c_str());
  SampleExecutor executor;
  PerformRequest(cronet_engine, url, executor.GetExecutor());
  Cronet_Engine_Shutdown(cronet_engine);
  Cronet_Engine_Destroy(cronet_engine);
  return 0;
}
