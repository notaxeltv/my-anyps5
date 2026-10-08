#include <cstdint>
#include <cstddef>
#include "SceTypes.hpp"
#include "prx/libc/include/General.hpp"
#include <cstring>
#include <string>
#include "SceShaders.hpp"
#include "prx/libSceAgc/Command/include/Packet.hpp"

namespace {

constexpr int kAgcOk = 0;
constexpr int kAgcInvalidValue = static_cast<int>(0x8A6C000B);
constexpr int kAgcInvalidAlignment = static_cast<int>(0x8A6C0002);
constexpr int kAgcAlreadyInitialized = static_cast<int>(0x8A6C0048);
constexpr int kAgcNotInitialized = static_cast<int>(0x8A6C0049);
constexpr std::uint64_t kSemaphoreAlign = 0x4000ull;
constexpr std::uint64_t kSemaphoreLabelBytes = 32u;
constexpr std::uint32_t kGsPrimPayloadRegister = 0x1C2u;

std::uint8_t* g_semaphoreMemory = nullptr;
std::uint64_t g_semaphoreSize = 0;
std::uint32_t g_shaderInstrumentation = 0;

int SetSemaphoreMemory(void* memory, std::uint64_t size_in_bytes) {
    if (g_semaphoreSize != 0) {
        if (g_semaphoreMemory == static_cast<std::uint8_t*>(memory) && g_semaphoreSize == size_in_bytes) return kAgcOk;
        return kAgcAlreadyInitialized;
    }
    if (memory == nullptr || size_in_bytes == 0 ||
        ((reinterpret_cast<std::uintptr_t>(memory) | size_in_bytes) & (kSemaphoreAlign - 1u)) != 0) {
        return kAgcInvalidAlignment;
    }
    std::memset(memory, 0, static_cast<std::size_t>(size_in_bytes));
    g_semaphoreMemory = static_cast<std::uint8_t*>(memory);
    g_semaphoreSize = size_in_bytes;
    return kAgcOk;
}

}

extern "C" {

int APS5_VABI sceAgcAcbPushMarkerSpan() {
 NotImplemented_nid_no_patch(__func__);
 return 0;
}

int APS5_VABI sceAgcAcbSetMarkerSpan() {
 NotImplemented_nid_no_patch(__func__);
 return 0;
}

int APS5_VABI sceAgcAcbSetWorkloadComplete() {
 NotImplemented_nid_no_patch(__func__);
 return 0;
}

int APS5_VABI sceAgcAcbSetWorkloadStreamInactive() {
 NotImplemented_nid_no_patch(__func__);
 return 0;
}

int APS5_VABI sceAgcAcbSetWorkloadsActive() {
 NotImplemented_nid_no_patch(__func__);
 return 0;
}

int APS5_VABI sceAgcAcquireMemSetEngine(std::uint32_t* cmd, std::uint32_t engine) {
    Agc::Command::CheckAddress(reinterpret_cast<std::uintptr_t>(cmd), 4, __func__);
    Agc::Command::CheckBits(engine, 1, __func__);
    Agc::Command::Require(((cmd[0] >> 8u) & 0xffu) == 0x58u, __func__, "not an ACQUIRE_MEM packet");
    cmd[1] = (cmd[1] & 0x7fffffffu) | (engine << 31u);
    return 0;
}


int APS5_VABI sceAgcDcbPushMarkerSpan() {
 NotImplemented_nid_no_patch(__func__);
 return 0;
}

int APS5_VABI sceAgcDcbSetMarkerSpan() {
 NotImplemented_nid_no_patch(__func__);
 return 0;
}

int APS5_VABI sceAgcDcbSetWorkloadStreamInactive() {
 NotImplemented_nid_no_patch(__func__);
 return 0;
}

int APS5_VABI sceAgcDebugRaiseException() {
    return 0;
}


int APS5_VABI sceAgcGetDefaultCxStateFlat() {
 NotImplemented_nid_no_patch(__func__);
 return 0;
}

int APS5_VABI sceAgcGetSemaphoreLabel(std::uint32_t index, void** label_out) {
    const auto end = (static_cast<std::uint64_t>(index) + 1u) * kSemaphoreLabelBytes;
    if (g_semaphoreSize == 0) return kAgcNotInitialized;
    if (end > g_semaphoreSize) return kAgcInvalidValue;
    if (label_out == nullptr) return kAgcInvalidAlignment;
    *label_out = g_semaphoreMemory + static_cast<std::uint64_t>(index) * kSemaphoreLabelBytes;
    return kAgcOk;
}


int APS5_VABI sceAgcSetAmmSemaphoreMemory(void* memory, std::uint64_t size_in_bytes) {
    return SetSemaphoreMemory(memory, size_in_bytes);
}


int APS5_VABI sceAgcSetSemaphoreMemory(void* memory, std::uint64_t size_in_bytes) {
    return SetSemaphoreMemory(memory, size_in_bytes);
}


int APS5_VABI sceAgcCbMemsetExclusive() {
 NotImplemented_nid_no_patch(__func__);
 return 0;
}

int APS5_VABI sceAgcBranchPatchSetThenTarget_0300() {
 NotImplemented_nid_no_patch(__func__);
 return 0;
}

int APS5_VABI sceAgcGetGsPrimPayload(std::uint32_t* payload_out, const Shader* shader) {
    if (payload_out == nullptr || shader == nullptr) {
        throw std::runtime_error(std::string(__func__) + ": null payload or shader");
    }
    *payload_out = 0;
    if (shader->num_cx_registers == 0) return 0;
    if (shader->cx_registers == nullptr) {
        throw std::runtime_error(std::string(__func__) + ": shader context registers are missing");
    }
    for (std::uint32_t i = 0; i < shader->num_cx_registers; ++i) {
        if (shader->cx_registers[i].offset != kGsPrimPayloadRegister) continue;
        if ((shader->cx_registers[i].value & 0xfu) == 2u) *payload_out = 8u;
        break;
    }
    return 0;
}


int APS5_VABI sceAgcSetShaderInstrumentation(std::uint32_t flags) {
    g_shaderInstrumentation = flags;
    return 0;
}


std::uint32_t APS5_VABI sceAgcGetShaderInstrumentation() {
    return g_shaderInstrumentation;
}


int APS5_VABI sceAgcBranchPatchSetElseTarget_0300() {
 NotImplemented_nid_no_patch(__func__);
 return 0;
}

APS5_EXPORT("vieBRwlh1Lw", sceAgcUnknown_vieBRwlh1Lw);
int APS5_VABI sceAgcUnknown_vieBRwlh1Lw(void) {
 NotImplemented_nid_no_patch(__func__);
 return 0;
}

}
