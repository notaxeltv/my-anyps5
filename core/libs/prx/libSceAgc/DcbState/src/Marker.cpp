#include "prx/libSceAgc/DcbState/include/Marker.hpp"

#include "prx/libSceAgc/Command/include/Packet.hpp"
#include <cstdint>
#include <cstddef>
#include <cstring>
#include "SceTypes.hpp"
#include "prx/libc/include/General.hpp"

// Debug markers are custom NOP packets: PUSH carries the NUL-terminated text, POP has no payload.
// Marker colors are only meaningful to capture tools and are not encoded.
namespace {

constexpr std::uint32_t OpcodeNop = 0x10;
constexpr std::uint32_t CustomPushMarker = 0x0b;
constexpr std::uint32_t CustomPopMarker = 0x0c;

}

namespace Agc::Marker {

std::uint32_t* Push(CommandBuffer* buf, const char* str, const char* function) {
    const char* text = str ? str : "";
    return PushSpan(buf, text, static_cast<std::uint32_t>(std::strlen(text)), function);
}

std::uint32_t* PushSpan(CommandBuffer* buf, const void* text, std::uint32_t byte_count, const char* function) {
    Agc::Command::Require(buf != nullptr, function, "null command buffer");
    Agc::Command::Require(byte_count == 0 || text != nullptr, function, "null marker span");
    const std::size_t bytes = static_cast<std::size_t>(byte_count) + 1;
    const auto payload = static_cast<std::uint32_t>((bytes + 3) / 4);
    Agc::Command::Require(payload <= 0x4000u, function, "marker text too long");
    auto* packet = Agc::Command::Allocate(buf, payload + 1, function);
    packet[0] = Agc::Command::Header(OpcodeNop, payload + 1, CustomPushMarker << 2);
    std::memset(packet + 1, 0, payload * sizeof(std::uint32_t));
    if (byte_count != 0) {
        std::memcpy(packet + 1, text, byte_count);
    }
    return packet;
}

std::uint32_t* Pop(CommandBuffer* buf, const char* function) {
    Agc::Command::Require(buf != nullptr, function, "null command buffer");
    auto* packet = Agc::Command::Allocate(buf, 2, function);
    packet[0] = Agc::Command::Header(OpcodeNop, 2, CustomPopMarker << 2);
    packet[1] = 0;
    return packet;
}

}

extern "C" {

uint32_t* APS5_VABI sceAgcDcbSetMarker(CommandBuffer* buf, const char* str, uint32_t color) {
    (void)color;
    auto* packet = Agc::Marker::Push(buf, str, __func__);
    Agc::Marker::Pop(buf, __func__);
    return packet;
}

uint32_t* APS5_VABI sceAgcDcbPopMarker(CommandBuffer* buf) {
    return Agc::Marker::Pop(buf, __func__);
}

uint32_t* APS5_VABI sceAgcDcbPushMarker(CommandBuffer* buf, const char* str, uint32_t color) {
    (void)color;
    return Agc::Marker::Push(buf, str, __func__);
}

uint32_t* APS5_VABI sceAgcDcbPushMarkerSpan(CommandBuffer* buf, const void* text, uint32_t byte_count, uint32_t color) {
    (void)color;
    return Agc::Marker::PushSpan(buf, text, byte_count, __func__);
}

uint32_t* APS5_VABI sceAgcDcbSetMarkerSpan(CommandBuffer* buf, const void* text, uint32_t byte_count) {
    auto* packet = Agc::Marker::PushSpan(buf, text, byte_count, __func__);
    Agc::Marker::Pop(buf, __func__);
    return packet;
}

}
