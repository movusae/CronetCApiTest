// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "sample_url_request_callback.h"
#include "android/log.h"
#include <iostream>
#include <sstream>
SampleUrlRequestCallback::SampleUrlRequestCallback()
    : callback_(Cronet_UrlRequestCallback_CreateWith(
          SampleUrlRequestCallback::OnRedirectReceived,
          SampleUrlRequestCallback::OnResponseStarted,
          SampleUrlRequestCallback::OnReadCompleted,
          SampleUrlRequestCallback::OnSucceeded,
          SampleUrlRequestCallback::OnFailed,
          SampleUrlRequestCallback::OnCanceled)) {
  Cronet_UrlRequestCallback_SetClientContext(callback_, this);
}
SampleUrlRequestCallback::~SampleUrlRequestCallback() {
  Cronet_UrlRequestCallback_Destroy(callback_);
}
Cronet_UrlRequestCallbackPtr SampleUrlRequestCallback::GetUrlRequestCallback() {
  return callback_;
}
void SampleUrlRequestCallback::OnRedirectReceived(
    Cronet_UrlRequestPtr request, Cronet_UrlResponseInfoPtr info,
    Cronet_String newLocationUrl) {
  std::stringstream ss;
  ss << "OnRedirectReceived called: " << newLocationUrl << std::endl;
  __android_log_print(ANDROID_LOG_INFO, "cplx", "%s", ss.str().c_str());
  Cronet_UrlRequest_FollowRedirect(request);
}
void SampleUrlRequestCallback::OnResponseStarted(
    Cronet_UrlRequestPtr request, Cronet_UrlResponseInfoPtr info) {

  std::stringstream ss;
  ss << "OnResponseStarted called." << std::endl;
  __android_log_print(ANDROID_LOG_INFO, "cplx", "%s", ss.str().c_str());

  std::stringstream sss;
  sss << "HTTP Status: " << Cronet_UrlResponseInfo_http_status_code_get(info)
      << " " << Cronet_UrlResponseInfo_http_status_text_get(info) << std::endl;
  __android_log_print(ANDROID_LOG_INFO, "cplx", "%s", sss.str().c_str());
  // Create and allocate 32kb buffer.
  Cronet_BufferPtr buffer = Cronet_Buffer_Create();
  Cronet_Buffer_InitWithAlloc(buffer, 32 * 1024);
  // Started reading the response.
  Cronet_UrlRequest_Read(request, buffer);
}
void SampleUrlRequestCallback::OnReadCompleted(Cronet_UrlRequestPtr request,
                                               Cronet_UrlResponseInfoPtr info,
                                               Cronet_BufferPtr buffer,
                                               uint64_t bytes_read) {
  std::stringstream ss;
  ss << "OnReadCompleted called: " << bytes_read << " bytes read." << std::endl;

  __android_log_print(ANDROID_LOG_INFO, "cplx", "%s", ss.str().c_str());
  std::string last_read_data(
      reinterpret_cast<char *>(Cronet_Buffer_GetData(buffer)), bytes_read);
  response_as_string_ += last_read_data;
  // Continue reading the response.
  Cronet_UrlRequest_Read(request, buffer);
}
void SampleUrlRequestCallback::OnSucceeded(Cronet_UrlRequestPtr request,
                                           Cronet_UrlResponseInfoPtr info) {
  std::stringstream ss;
  ss << "OnSucceeded called." << std::endl;
  __android_log_print(ANDROID_LOG_INFO, "cplx", "%s", ss.str().c_str());
  SignalDone(true);
}
void SampleUrlRequestCallback::OnFailed(Cronet_UrlRequestPtr request,
                                        Cronet_UrlResponseInfoPtr info,
                                        Cronet_ErrorPtr error) {
  std::stringstream ss;
  ss << "OnFailed called: " << Cronet_Error_message_get(error) << std::endl;
  last_error_message_ = Cronet_Error_message_get(error);
  __android_log_print(ANDROID_LOG_INFO, "cplx", "%s", ss.str().c_str());
  SignalDone(false);
}
void SampleUrlRequestCallback::OnCanceled(Cronet_UrlRequestPtr request,
                                          Cronet_UrlResponseInfoPtr info) {
  std::stringstream ss;
  ss << "OnCanceled called." << std::endl;
  __android_log_print(ANDROID_LOG_INFO, "cplx", "%s", ss.str().c_str());
  SignalDone(false);
}
/* static */
SampleUrlRequestCallback *
SampleUrlRequestCallback::GetThis(Cronet_UrlRequestCallbackPtr self) {
  return static_cast<SampleUrlRequestCallback *>(
      Cronet_UrlRequestCallback_GetClientContext(self));
}
/* static */
void SampleUrlRequestCallback::OnRedirectReceived(
    Cronet_UrlRequestCallbackPtr self, Cronet_UrlRequestPtr request,
    Cronet_UrlResponseInfoPtr info, Cronet_String newLocationUrl) {
  GetThis(self)->OnRedirectReceived(request, info, newLocationUrl);
}
/* static */
void SampleUrlRequestCallback::OnResponseStarted(
    Cronet_UrlRequestCallbackPtr self, Cronet_UrlRequestPtr request,
    Cronet_UrlResponseInfoPtr info) {
  GetThis(self)->OnResponseStarted(request, info);
}
/* static */
void SampleUrlRequestCallback::OnReadCompleted(
    Cronet_UrlRequestCallbackPtr self, Cronet_UrlRequestPtr request,
    Cronet_UrlResponseInfoPtr info, Cronet_BufferPtr buffer,
    uint64_t bytesRead) {
  GetThis(self)->OnReadCompleted(request, info, buffer, bytesRead);
}
/* static */
void SampleUrlRequestCallback::OnSucceeded(Cronet_UrlRequestCallbackPtr self,
                                           Cronet_UrlRequestPtr request,
                                           Cronet_UrlResponseInfoPtr info) {
  GetThis(self)->OnSucceeded(request, info);
}
/* static */
void SampleUrlRequestCallback::OnFailed(Cronet_UrlRequestCallbackPtr self,
                                        Cronet_UrlRequestPtr request,
                                        Cronet_UrlResponseInfoPtr info,
                                        Cronet_ErrorPtr error) {
  GetThis(self)->OnFailed(request, info, error);
}
/* static */
void SampleUrlRequestCallback::OnCanceled(Cronet_UrlRequestCallbackPtr self,
                                          Cronet_UrlRequestPtr request,
                                          Cronet_UrlResponseInfoPtr info) {
  GetThis(self)->OnCanceled(request, info);
}
