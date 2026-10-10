#include <cstddef>
#include <cstdint>
#include <cstring>

#include "prx/libc/include/General.hpp"
#include "prx/libSceNgs2.native/include/Ngs2Types.hpp"

#pragma GCC visibility push(default)

extern "C" {

int APS5_VABI sceNgs2GeomApply(const Ngs2GeomListenerWork* listener, const Ngs2GeomSourceParam* source, Ngs2GeomAttribute* out_attrib, uint32_t flags) {
    (void)listener;
    (void)source;
    (void)out_attrib;
    (void)flags;
    NotImplemented_nid_no_patch(__func__);
    return 0;
}

int APS5_VABI sceNgs2GeomCalcListener(const Ngs2GeomListenerParam* param, Ngs2GeomListenerWork* out_work, uint32_t flags) {
    (void)param;
    (void)out_work;
    (void)flags;
    NotImplemented_nid_no_patch(__func__);
    return 0;
}

int APS5_VABI sceNgs2GeomResetListenerParam(Ngs2GeomListenerParam* out_listener_param) {
    if (!out_listener_param) return SCE_NGS2_ERROR_INVALID_OUT_ADDRESS;
    std::memset(out_listener_param, 0, sizeof(*out_listener_param));
    return SCE_NGS2_OK;
}


int APS5_VABI sceNgs2GeomResetSourceParam(Ngs2GeomSourceParam* out_source_param) {
    if (!out_source_param) return SCE_NGS2_ERROR_INVALID_OUT_ADDRESS;
    std::memset(out_source_param, 0, sizeof(*out_source_param));
    return SCE_NGS2_OK;
}


}

#pragma GCC visibility pop
