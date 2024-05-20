#include <jni.h>

#include "api/transport/field_trial_based_config.h"
#include "sdk/android/src/jni/jni_helpers.h"
#include "sdk/android/src/jni/video_encoder_factory_wrapper.h"
#include "sdk/android/src/jni/video_codec_info.h"
#include "sdk/android/native_api/codecs/wrapper.h"
#include "media/engine/simulcast_encoder_adapter.h"
#include "rtc_base/logging.h"

using namespace webrtc;
using namespace webrtc::jni;

#ifdef __cplusplus
extern "C" {
#endif

// (VideoEncoderFactory primary, VideoEncoderFactory fallback, VideoCodecInfo info)
JNIEXPORT jlong JNICALL Java_org_webrtc_SimulcastVideoEncoder_nativeCreateEncoder(JNIEnv *env, jclass klass, jlong webrtcEnvRef, jobject primary, jobject fallback, jobject info) {
    RTC_LOG(LS_INFO) << "Create simulcast video encoder";
    JavaParamRef<jobject> info_ref(info);
    SdpVideoFormat format = VideoCodecInfoToSdpVideoFormat(env, info_ref);

    // TODO: 影響は軽微だが、リークする可能性があるので将来的に修正したい
    // https://github.com/shiguredo-webrtc-build/webrtc-build/pull/16#pullrequestreview-600675795
    return NativeToJavaPointer(std::make_unique<SimulcastEncoderAdapter>(
                            *reinterpret_cast<const webrtc::Environment*>(webrtcEnvRef),
                            JavaToNativeVideoEncoderFactory(env, primary).release(),
                            fallback != nullptr ? JavaToNativeVideoEncoderFactory(env, fallback).release() : nullptr,
                            format).release());
}

#ifdef __cplusplus
}
#endif
