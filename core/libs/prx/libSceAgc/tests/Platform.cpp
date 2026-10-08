#include "prx/libc/include/general/VabiMacros.hpp"
#include "prx/libc/include/Shutdown.hpp"
#include "SceShaders.hpp"

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <vector>

extern "C" int APS5_VABI sceAgcGetIsTrinityMode(bool* isTrinityMode);
extern "C" int APS5_VABI sceAgcSetSemaphoreMemory(void* memory, std::uint64_t size_in_bytes);
extern "C" int APS5_VABI sceAgcSetAmmSemaphoreMemory(void* memory, std::uint64_t size_in_bytes);
extern "C" int APS5_VABI sceAgcGetSemaphoreLabel(std::uint32_t index, void** label_out);
extern "C" int APS5_VABI sceAgcSetShaderInstrumentation(std::uint32_t flags);
extern "C" std::uint32_t APS5_VABI sceAgcGetShaderInstrumentation();
extern "C" int APS5_VABI sceAgcDebugRaiseException();
extern "C" int APS5_VABI sceAgcGetGsPrimPayload(std::uint32_t* payload_out, const Shader* shader);

namespace {

void check(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

template <typename TAction>
void expectFailure(TAction action) {
    try {
        action();
    } catch (const std::runtime_error& error) {
        check(error.what()[0] != '\0', "empty exception message");
        return;
    }
    throw std::runtime_error("expected an exception");
}

void testTrinityMode() {
    std::array<bool, 3> flags{true, true, true};
    check(sceAgcGetIsTrinityMode(&flags[1]) == 0, "Trinity mode query did not return 0");
    check(!flags[1], "base PS5 GPU reported as Trinity");
    check(flags[0] && flags[2], "Trinity mode query wrote past its one-byte flag");
}

void testRejections() {
    expectFailure([] { sceAgcGetIsTrinityMode(nullptr); });
}

void testSemaphoreMemory() {
    constexpr int already = static_cast<int>(0x8A6C0048);
    constexpr int notInit = static_cast<int>(0x8A6C0049);
    constexpr int invalidValue = static_cast<int>(0x8A6C000B);
    constexpr int invalidAlign = static_cast<int>(0x8A6C0002);
    void* label = &label;
    check(sceAgcGetSemaphoreLabel(0, &label) == notInit, "label before init");
    check(sceAgcSetSemaphoreMemory(nullptr, 0x4000) == invalidAlign, "null semaphore memory");
    std::vector<std::uint8_t> tooSmall(0x1000, 1);
    check(sceAgcSetAmmSemaphoreMemory(tooSmall.data(), tooSmall.size()) == invalidAlign, "unaligned semaphore size");
    std::vector<std::uint8_t> pool(0x4000 * 3);
    auto* memory = reinterpret_cast<std::uint8_t*>((reinterpret_cast<std::uintptr_t>(pool.data()) + 0x3fffu) & ~std::uintptr_t{0x3fffu});
    std::memset(memory, 0xa5, 0x4000);
    check(sceAgcSetSemaphoreMemory(memory, 0x4000) == 0, "semaphore memory init");
    check(memory[0] == 0 && memory[0x3fff] == 0, "semaphore memory was not cleared");
    check(sceAgcSetAmmSemaphoreMemory(memory, 0x4000) == 0, "repeated identical init");
    auto* other = memory + 0x4000;
    check(sceAgcSetAmmSemaphoreMemory(other, 0x4000) == already, "second semaphore region");
    check(sceAgcGetSemaphoreLabel(0, nullptr) == invalidAlign, "null label out");
    check(sceAgcGetSemaphoreLabel(0x4000u / 32u, &label) == invalidValue, "label past the pool");
    check(sceAgcGetSemaphoreLabel(0, &label) == 0 && label == memory, "first label");
    check(sceAgcGetSemaphoreLabel(1, &label) == 0 && label == memory + 32, "second label");
}

void testInstrumentationAndGsPrim() {
    check(sceAgcGetShaderInstrumentation() == 0, "instrumentation default");
    check(sceAgcSetShaderInstrumentation(5u) == 0 && sceAgcGetShaderInstrumentation() == 5u, "instrumentation round trip");
    check(sceAgcDebugRaiseException() == 0, "debug raise");
    std::uint32_t payload = 7;
    Shader shader{};
    check(sceAgcGetGsPrimPayload(&payload, &shader) == 0 && payload == 0, "empty shader payload");
    ShaderRegister cx{0x1C2u, 2u};
    shader.cx_registers = &cx;
    shader.num_cx_registers = 1;
    check(sceAgcGetGsPrimPayload(&payload, &shader) == 0 && payload == 8u, "triangle gs prim payload");
    expectFailure([] { sceAgcGetGsPrimPayload(nullptr, nullptr); });
}

}

int main() {
    try {
        testTrinityMode();
        testRejections();
        testSemaphoreMemory();
        testInstrumentationAndGsPrim();
        LibcRunShutdown_nid_postfix();
        std::puts("AGC platform tests passed");
        return 0;
    } catch (const std::exception& error) {
        std::fprintf(stderr, "%s\n", error.what());
        try { LibcRunShutdown_nid_postfix(); }
        catch (const std::exception& shutdown) { std::fprintf(stderr, "shutdown: %s\n", shutdown.what()); }
        return 1;
    }
}
