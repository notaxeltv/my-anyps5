#include <cstdint>
#include <cstddef>
#include "SceTypes.hpp"
#include "prx/libc/include/General.hpp"

struct TlsIndex {
    std::uintptr_t ti_module;
    std::uintptr_t ti_offset;
};

extern "C" {

int APS5_VABI sceCoredumpWriteUserData() {
    NotImplemented_nid_no_patch(__func__);
    return 0;
}

void* APS5_VABI __tls_get_addr_nid_postfix(TlsIndex* index) {
    if (index == nullptr) throw std::invalid_argument("__tls_get_addr: tls_index is null");
    throw std::runtime_error("__tls_get_addr: general-dynamic TLS is not modelled");
}

}
