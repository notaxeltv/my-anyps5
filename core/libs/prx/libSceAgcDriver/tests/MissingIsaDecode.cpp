#include "RdnaDecoder/RdnaInstructionDecoder.hpp"
#include <cstdio>
#include <exception>
#include <span>

using namespace ShaderRecompiler;

namespace {

bool checkOpcode(std::span<const std::uint32_t> words, RdnaOpcode expected, const char* name) {
    try {
        const auto instruction = DecodeRdnaInstruction(0, words, 0);
        if (instruction.op != expected) {
            std::fprintf(stderr, "%s decoded to %u\n", name, static_cast<unsigned>(instruction.op));
            return false;
        }
        return true;
    } catch (const std::exception& error) {
        std::fprintf(stderr, "%s: %s\n", name, error.what());
        return false;
    }
}

}

int main() {
    const std::uint32_t swappc[]{0xBE802100u};
    const std::uint32_t setpc[]{0xBEFD2100u};
    const std::uint32_t interpP1ll[]{0xD6680000u, 0};
    const std::uint32_t interpP1lv[]{0xD6690000u, 0};
    const std::uint32_t interpP2[]{0xD66A0000u, 0};
    bool ok = true;
    ok &= checkOpcode(swappc, RdnaOpcode::SSwappcB64, "s_swappc_b64");
    ok &= checkOpcode(setpc, RdnaOpcode::SSetpcB64, "s_setpc_b64 via s_swappc null dest");
    ok &= checkOpcode(interpP1ll, RdnaOpcode::VInterpP1llF16, "v_interp_p1ll_f16");
    ok &= checkOpcode(interpP1lv, RdnaOpcode::VInterpP1lvF16, "v_interp_p1lv_f16");
    ok &= checkOpcode(interpP2, RdnaOpcode::VInterpP2F16, "v_interp_p2_f16");
    return ok ? 0 : 1;
}
