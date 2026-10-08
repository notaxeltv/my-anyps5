#include "prx/libc/include/general/VabiMacros.hpp"
#include <array>
#include <cstdint>
#include <cstdlib>

extern "C" {
std::int32_t APS5_VABI scePsmlMfsrGetContextBufferRequirement1100(void* requirement, const void* param);
std::int32_t APS5_VABI scePsmlMfsrCreateContext1100(void** context, const void* param);
std::int32_t APS5_VABI scePsmlMfsrGetDispatchMfsrPacket1100(void* context, void* commandBuffer, const void* param);
int APS5_VABI scePsmlMfsrInit();
int APS5_VABI scePsmlMfsrReleaseContext(void* context);
int APS5_VABI scePsmlMfsrGetDispatchMfsrPacketSizeInDwords(void* context, std::uint32_t* dwords_out);
int APS5_VABI scePsmlMfsrGetSharedResourcesInitRequirement();
int APS5_VABI scePsmlMfsrCreateSharedResources();
int APS5_VABI scePsmlMfsrReleaseSharedResources();
int APS5_VABI scePsmlMfsrGetMipmapBias();
int APS5_VABI scePsmlMfsr2Init();
int APS5_VABI scePsmlMfsr2CreateContext();
int APS5_VABI scePsmlMfsr2ReleaseContext();
int APS5_VABI scePsmlMfsr2GetContextInitRequirement();
int APS5_VABI scePsmlMfsr2GetDispatchPackets();
int APS5_VABI scePsmlMfsr2GetDispatchPacketsSizeInDwords();
int APS5_VABI scePsmlMfsr2CreateSharedResources();
int APS5_VABI scePsmlMfsr2GetSharedResourcesInitRequirement();
int APS5_VABI scePsmlMfsr2ReleaseSharedResources();
}

namespace {

void Require(bool value) { if (!value) std::abort(); }

constexpr std::int32_t kErrNotInitialized = static_cast<std::int32_t>(0x8A810001);

struct CommandBuffer {
    std::uint32_t* cursor;
    std::uint32_t sizeInDwords;
};

}

int main() {
    std::array<std::uint8_t, 0x158> param{};

    std::array<std::uint8_t, 0x18> requirement;
    requirement.fill(0xa5);
    const auto untouchedRequirement = requirement;
    Require(scePsmlMfsrGetContextBufferRequirement1100(requirement.data(), param.data()) == kErrNotInitialized);
    Require(requirement == untouchedRequirement);
    Require(scePsmlMfsrGetContextBufferRequirement1100(nullptr, nullptr) == kErrNotInitialized);

    std::uint64_t contextStorage = 0;
    void* const sentinel = &contextStorage;
    void* context = sentinel;
    Require(scePsmlMfsrCreateContext1100(&context, param.data()) == kErrNotInitialized);
    Require(context == sentinel);
    Require(scePsmlMfsrCreateContext1100(nullptr, nullptr) == kErrNotInitialized);

    std::array<std::uint32_t, 64> dwords;
    dwords.fill(0xa5a5a5a5u);
    const auto untouchedDwords = dwords;
    CommandBuffer commandBuffer{dwords.data(), static_cast<std::uint32_t>(dwords.size())};
    Require(scePsmlMfsrGetDispatchMfsrPacket1100(&contextStorage, &commandBuffer, param.data()) == kErrNotInitialized);
    Require(commandBuffer.cursor == dwords.data());
    Require(commandBuffer.sizeInDwords == dwords.size());
    Require(dwords == untouchedDwords);
    Require(scePsmlMfsrGetDispatchMfsrPacket1100(nullptr, &commandBuffer, param.data()) == kErrNotInitialized);
    Require(commandBuffer.cursor == dwords.data());
    Require(dwords == untouchedDwords);
    Require(scePsmlMfsrInit() == kErrNotInitialized);
    Require(scePsmlMfsrReleaseContext(&contextStorage) == kErrNotInitialized);
    std::uint32_t sizeDwords = 9;
    Require(scePsmlMfsrGetDispatchMfsrPacketSizeInDwords(&contextStorage, &sizeDwords) == kErrNotInitialized);
    Require(sizeDwords == 9);
    Require(scePsmlMfsrGetSharedResourcesInitRequirement() == kErrNotInitialized);
    Require(scePsmlMfsrCreateSharedResources() == kErrNotInitialized);
    Require(scePsmlMfsrReleaseSharedResources() == kErrNotInitialized);
    Require(scePsmlMfsrGetMipmapBias() == kErrNotInitialized);
    Require(scePsmlMfsr2Init() == kErrNotInitialized);
    Require(scePsmlMfsr2CreateContext() == kErrNotInitialized);
    Require(scePsmlMfsr2ReleaseContext() == kErrNotInitialized);
    Require(scePsmlMfsr2GetContextInitRequirement() == kErrNotInitialized);
    Require(scePsmlMfsr2GetDispatchPackets() == kErrNotInitialized);
    Require(scePsmlMfsr2GetDispatchPacketsSizeInDwords() == kErrNotInitialized);
    Require(scePsmlMfsr2CreateSharedResources() == kErrNotInitialized);
    Require(scePsmlMfsr2GetSharedResourcesInitRequirement() == kErrNotInitialized);
    Require(scePsmlMfsr2ReleaseSharedResources() == kErrNotInitialized);
}
