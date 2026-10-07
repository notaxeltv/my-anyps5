#include "prx/libSceAgcDriver/Submit/include/Acb.hpp"
#include "prx/libSceAgcDriver/Execution/include/Driver.hpp"
#include "prx/libSceAgcDriver/Execution/include/GuestMemory.hpp"

#include <cstdint>
#include <cstddef>
#include <string>
#include "SceTypes.hpp"
#include "prx/libc/include/General.hpp"

extern "C" {

int APS5_VABI sceAgcDriverSubmitAcb(uint32_t queue, const Packet* packet) {
    if (queue < 0x20 || queue >= 0x58) {
        throw std::runtime_error(std::string(__func__) + ": unsupported compute queue");
    }
    AgcDriver::Submit(packet, queue);
    return 0;
}

int APS5_VABI sceAgcDriverSubmitMultiAcbs(uint32_t queue, uint32_t* const* acbs, const uint32_t* sizes_in_dwords, uint32_t count) {
    if (queue < 0x20 || queue >= 0x58) {
        throw std::runtime_error(std::string(__func__) + ": unsupported compute queue");
    }
    if (count == 0) return 0;
    if (!acbs || !sizes_in_dwords) throw std::runtime_error(std::string(__func__) + ": null multi-ACB arrays");
    AgcDriver::GuestMemory::CheckRange(acbs, count * sizeof(*acbs), alignof(std::uint32_t*));
    AgcDriver::GuestMemory::CheckRange(sizes_in_dwords, count * sizeof(*sizes_in_dwords), alignof(std::uint32_t));
    for (std::uint32_t i = 0; i < count; ++i) {
        const Packet packet{acbs[i], sizes_in_dwords[i], 0, {}};
        AgcDriver::Submit(&packet, queue);
    }
    return 0;
}

}
