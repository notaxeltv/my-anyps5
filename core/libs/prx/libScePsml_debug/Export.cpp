#include <cstdint>
#include <cstddef>
#include "SceTypes.hpp"
#include "prx/libc/include/General.hpp"

namespace {

constexpr std::int32_t SCE_PSML_ERROR_NOT_INITIALIZED = static_cast<std::int32_t>(0x8A810001);

}

extern "C" {

std::int32_t APS5_VABI scePsmlMfsrGetContextBufferRequirement1100(void* requirement, const void* param) {
 (void)requirement;
 (void)param;
 return SCE_PSML_ERROR_NOT_INITIALIZED;
}

std::int32_t APS5_VABI scePsmlMfsrCreateContext1100(void** context, const void* param) {
 (void)context;
 (void)param;
 return SCE_PSML_ERROR_NOT_INITIALIZED;
}

std::int32_t APS5_VABI scePsmlMfsrGetDispatchMfsrPacket1100(void* context, void* commandBuffer, const void* param) {
 (void)context;
 (void)commandBuffer;
 (void)param;
 return SCE_PSML_ERROR_NOT_INITIALIZED;
}

int APS5_VABI scePsmlMfsrGetSharedResourcesInitRequirement() {
 return SCE_PSML_ERROR_NOT_INITIALIZED;
}

int APS5_VABI scePsmlMfsrInit() {
    return SCE_PSML_ERROR_NOT_INITIALIZED;
}


int APS5_VABI scePsmlMfsrGetDispatchMfsrPacketSizeInDwords(void* context, std::uint32_t* dwords_out) {
    (void)context;
    (void)dwords_out;
    return SCE_PSML_ERROR_NOT_INITIALIZED;
}


int APS5_VABI scePsmlMfsrGetContextBufferRequirement800M3_2() {
 NotImplemented_nid_no_patch(__func__);
 return 0;
}

int APS5_VABI scePsmlMfsrSelectConfig() {
 NotImplemented_nid_no_patch(__func__);
 return 0;
}

int APS5_VABI scePsmlMfsrGetMipmapBias() {
 return SCE_PSML_ERROR_NOT_INITIALIZED;
}

int APS5_VABI scePsmlMfsrRequestCapture() {
 NotImplemented_nid_no_patch(__func__);
 return 0;
}

int APS5_VABI scePsmlMfsrReleaseContext(void* context) {
    (void)context;
    return SCE_PSML_ERROR_NOT_INITIALIZED;
}


int APS5_VABI scePsmlMfsrIsCaptureInProgress() {
 NotImplemented_nid_no_patch(__func__);
 return 0;
}

int APS5_VABI scePsmlMfsrGetDispatchMfsrPacket900() {
 NotImplemented_nid_no_patch(__func__);
 return 0;
}

int APS5_VABI scePsmlMfsrCreateSharedResources() {
 return SCE_PSML_ERROR_NOT_INITIALIZED;
}

int APS5_VABI scePsmlMfsrCreateContext800M3_2() {
 NotImplemented_nid_no_patch(__func__);
 return 0;
}

int APS5_VABI scePsmlMfsrReleaseSharedResources() {
 return SCE_PSML_ERROR_NOT_INITIALIZED;
}

int APS5_VABI scePsmlMfsr2ReleaseSharedResources() {
    return SCE_PSML_ERROR_NOT_INITIALIZED;
}

int APS5_VABI scePsmlMfsr2GetSharedResourcesInitRequirement() {
    return SCE_PSML_ERROR_NOT_INITIALIZED;
}

int APS5_VABI scePsmlMfsr2GetContextInitRequirement() {
    return SCE_PSML_ERROR_NOT_INITIALIZED;
}

int APS5_VABI scePsmlMfsr2ReleaseContext() {
    return SCE_PSML_ERROR_NOT_INITIALIZED;
}

int APS5_VABI scePsmlMfsr2GetDispatchPacketsSizeInDwords() {
    return SCE_PSML_ERROR_NOT_INITIALIZED;
}

int APS5_VABI scePsmlMfsr2CreateSharedResources() {
    return SCE_PSML_ERROR_NOT_INITIALIZED;
}

int APS5_VABI scePsmlMfsr2GetDispatchPackets() {
    return SCE_PSML_ERROR_NOT_INITIALIZED;
}

int APS5_VABI scePsmlMfsr2CreateContext() {
    return SCE_PSML_ERROR_NOT_INITIALIZED;
}

int APS5_VABI scePsmlMfsr2Init() {
    return SCE_PSML_ERROR_NOT_INITIALIZED;
}

}
