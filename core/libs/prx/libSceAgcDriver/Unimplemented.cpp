#include <cstdint>
#include <cstddef>
#include "SceTypes.hpp"
#include "prx/libc/include/General.hpp"

extern "C" {

int APS5_VABI sceAgcDriverGetShaderDebuggingStatus() {
    return 1;
}

int APS5_VABI sceAgcDriverRegisterMultipleResources() {
 NotImplemented_nid_no_patch(__func__);
 return 0;
}

int APS5_VABI sceAgcDriverRegisterDefaultOwner(uint32_t* owner_handle) {
    (void)owner_handle;
    return static_cast<int>(0x8A6C9018);
}

int APS5_VABI sceAgcDriverSetValidationErrorOutputFrequency() {
    return 0;
}

int APS5_VABI sceAgcDriverSetSubmitValidationMode() {
    return 0;
}

int APS5_VABI sceAgcDriverGetSubmitValidationConfig() {
    return 0;
}

int APS5_VABI sceAgcDriverGetSubmitValidationMode() {
    return 0;
}

int APS5_VABI sceAgcDriverSetSubmitValidationConfig() {
    return 0;
}

}
