#include "SceTypes.hpp"
#include "prx/libc/include/general/VabiMacros.hpp"
#include <cstdint>
#include <cstdlib>
#include <cstring>

extern "C" {
int32_t APS5_VABI sceSystemGestureOpen(int32_t input_type, const void* param);
int APS5_VABI sceSystemGestureClose(int32_t gesture_handle);
int APS5_VABI sceSystemGestureCreateTouchRecognizer(int32_t gesture_handle, SystemGestureTouchRecognizer* recognizer, int32_t type, const SystemGestureRectangle* rectangle, const void* param);
int APS5_VABI sceSystemGestureAppendTouchRecognizer(int32_t gesture_handle, SystemGestureTouchRecognizer* recognizer);
int APS5_VABI sceSystemGestureGetPrimitiveTouchEventsCount(int32_t gesture_handle);
int APS5_VABI sceSystemGestureGetPrimitiveTouchEvents(int32_t gesture_handle, SystemGesturePrimitiveTouchEvent* event_buffer, uint32_t capacity_of_buffer, uint32_t* number_of_event);
int APS5_VABI sceSystemGestureGetTouchEventsCount(int32_t gesture_handle, const SystemGestureTouchRecognizer* recognizer);
int APS5_VABI sceSystemGestureGetTouchEvents(int32_t gesture_handle, const SystemGestureTouchRecognizer* recognizer, SystemGestureTouchEvent* event_buffer, uint32_t capacity_of_buffer, uint32_t* number_of_event);
int APS5_VABI sceSystemGestureUpdateAllTouchRecognizer(int32_t gesture_handle);
int APS5_VABI sceSystemGestureResetPrimitiveTouchRecognizer(int32_t gesture_handle);
}

namespace {

void Require(bool value) { if (!value) std::abort(); }

constexpr int kInvalidHandle = static_cast<int>(0x80D10003);

}

int main() {
    const int32_t handle = sceSystemGestureOpen(0, nullptr);
    Require(handle == 1);
    SystemGestureTouchRecognizer recognizer{};
    SystemGestureRectangle rectangle{};
    Require(sceSystemGestureCreateTouchRecognizer(handle, &recognizer, 0, &rectangle, nullptr) == 0);
    Require(sceSystemGestureAppendTouchRecognizer(handle, &recognizer) == 0);
    Require(sceSystemGestureGetPrimitiveTouchEventsCount(handle) == 0);
    uint32_t count = 9;
    Require(sceSystemGestureGetPrimitiveTouchEvents(handle, nullptr, 0, &count) == 0);
    Require(count == 0);
    Require(sceSystemGestureGetTouchEventsCount(handle, &recognizer) == 0);
    count = 9;
    Require(sceSystemGestureGetTouchEvents(handle, &recognizer, nullptr, 0, &count) == 0);
    Require(count == 0);
    Require(sceSystemGestureUpdateAllTouchRecognizer(handle) == 0);
    Require(sceSystemGestureResetPrimitiveTouchRecognizer(handle) == 0);
    Require(sceSystemGestureClose(handle) == 0);
    Require(sceSystemGestureClose(0) == kInvalidHandle);
}
