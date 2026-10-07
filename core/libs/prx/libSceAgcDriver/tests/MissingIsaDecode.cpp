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
    const std::uint32_t rfe[]{0xBE802200u};
    const std::uint32_t call[]{0xBA000000u};
    const std::uint32_t sethalt[]{0xBF8D0000u};
    const std::uint32_t sendmsghalt[]{0xBF910000u};
    const std::uint32_t gwsRelease[]{0xD8600000u, 0};
    const std::uint32_t gwsInit[]{0xD8640000u, 0};
    const std::uint32_t gwsSemaV[]{0xD8680000u, 0};
    const std::uint32_t gwsSemaBr[]{0xD86C0000u, 0};
    const std::uint32_t gwsSemaP[]{0xD8700000u, 0};
    const std::uint32_t gwsBarrier[]{0xD8740000u, 0};
    const std::uint32_t ordered[]{0xDAD00000u, 0};
    const std::uint32_t condxchg[]{0xD9F40000u, 0};
    const std::uint32_t interpP1ll[]{0xD6680000u, 0};
    const std::uint32_t interpP1lv[]{0xD6690000u, 0};
    const std::uint32_t interpP2[]{0xD66A0000u, 0};
    const std::uint32_t program[]{0xBE802100u, 0xBF9D0000u};
    bool ok = true;
    ok &= checkOpcode(swappc, RdnaOpcode::SSwappcB64, "s_swappc_b64");
    ok &= checkOpcode(rfe, RdnaOpcode::SRfeB64, "s_rfe_b64");
    ok &= checkOpcode(call, RdnaOpcode::SCallB64, "s_call_b64");
    ok &= checkOpcode(sethalt, RdnaOpcode::SSethalt, "s_sethalt");
    ok &= checkOpcode(sendmsghalt, RdnaOpcode::SSendmsghalt, "s_sendmsghalt");
    ok &= checkOpcode(gwsRelease, RdnaOpcode::DsGwsSemaReleaseAll, "ds_gws_sema_release_all");
    ok &= checkOpcode(gwsInit, RdnaOpcode::DsGwsInit, "ds_gws_init");
    ok &= checkOpcode(gwsSemaV, RdnaOpcode::DsGwsSemaV, "ds_gws_sema_v");
    ok &= checkOpcode(gwsSemaBr, RdnaOpcode::DsGwsSemaBr, "ds_gws_sema_br");
    ok &= checkOpcode(gwsSemaP, RdnaOpcode::DsGwsSemaP, "ds_gws_sema_p");
    ok &= checkOpcode(gwsBarrier, RdnaOpcode::DsGwsBarrier, "ds_gws_barrier");
    ok &= checkOpcode(ordered, RdnaOpcode::DsOrderedCount, "ds_ordered_count");
    ok &= checkOpcode(condxchg, RdnaOpcode::DsCondxchg32RtnB64, "ds_condxchg32_rtn_b64");
    ok &= checkOpcode(interpP1ll, RdnaOpcode::VInterpP1llF16, "v_interp_p1ll_f16");
    ok &= checkOpcode(interpP1lv, RdnaOpcode::VInterpP1lvF16, "v_interp_p1lv_f16");
    ok &= checkOpcode(interpP2, RdnaOpcode::VInterpP2F16, "v_interp_p2_f16");
    try {
        const auto decoded = RdnaInstructionDecoder{}.Decode(program);
        if (decoded.instructions.size() != 2 || decoded.instructions[0].op != RdnaOpcode::SSwappcB64 ||
            decoded.instructions[1].op != RdnaOpcode::SCodeEnd) {
            std::fprintf(stderr, "program with s_code_end decoded incorrectly\n");
            ok = false;
        }
    } catch (const std::exception& error) {
        std::fprintf(stderr, "s_code_end program: %s\n", error.what());
        ok = false;
    }
    return ok ? 0 : 1;
}
