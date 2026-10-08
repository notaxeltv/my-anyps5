#include <cstdint>
#include <cstddef>
#include "SceTypes.hpp"
#include "prx/libc/include/General.hpp"
#include <atomic>

// No network is emulated: contexts, templates and requests can be created, but any request
// that would touch the network fails with the library's network error.
static constexpr int ERROR_NETWORK = static_cast<int>(0x80435001);
static std::atomic<int> g_nextHandle{1};

namespace {

constexpr int ERROR_NOT_FOUND = static_cast<int>(0x8095F004);
constexpr int ERROR_INVALID_ARG = static_cast<int>(0x8095177A);

struct SslData {
    char* ptr;
    size_t size;
};

struct SslCaCerts {
    SslData* certs;
    size_t num;
    void* pool;
};

struct SslMemoryPoolStats {
    unsigned pool_size;
    unsigned max_inuse_size;
    unsigned current_inuse_size;
    int reserved;
};

}

extern "C" {

int APS5_VABI sceSslFreeCaCerts(int ssl_ctx_id, void* ca_certs) {
    (void)ssl_ctx_id;
    if (!ca_certs) return ERROR_INVALID_ARG;
    *static_cast<SslCaCerts*>(ca_certs) = {};
    return 0;
}

int APS5_VABI sceSslGetCaCerts(int ssl_ctx_id, void* ca_certs) {
    (void)ssl_ctx_id;
    if (!ca_certs) return ERROR_INVALID_ARG;
    *static_cast<SslCaCerts*>(ca_certs) = {};
    return ERROR_NOT_FOUND;
}

int APS5_VABI sceSslInit_nid_postfix(uint64_t pool_size) {
    (void)pool_size;
    return g_nextHandle.fetch_add(1, std::memory_order_relaxed);
}

int APS5_VABI sceSslTerm_nid_postfix(int ssl_ctx_id) {
    (void)ssl_ctx_id;
    return 0;
}

int APS5_VABI sceSslClose(int ssl_ctx_id) {
    (void)ssl_ctx_id;
    return 0;
}

int APS5_VABI sceSslGetSerialNumber(void* ssl_cert, const char** serial, unsigned* serial_len) {
    if (!ssl_cert || !serial || !serial_len) return ERROR_INVALID_ARG;
    *serial = nullptr;
    *serial_len = 0;
    return ERROR_NOT_FOUND;
}

int APS5_VABI sceSslLoadCert() {
    NotImplemented_nid_no_patch(__func__);
    return 0;
}

int APS5_VABI sceSslGetMemoryPoolStats(SslMemoryPoolStats* stats) {
    if (!stats) return ERROR_INVALID_ARG;
    *stats = {};
    return 0;
}

int APS5_VABI sceSslFreeSslCertName(void* cert_name) {
    if (!cert_name) return ERROR_INVALID_ARG;
    return 0;
}

void* APS5_VABI sceSslGetIssuerName(void* ssl_cert) {
    if (!ssl_cert) return nullptr;
    return nullptr;
}

int APS5_VABI sceSslGetNameEntryCount(void* cert_name) {
    if (!cert_name) return ERROR_INVALID_ARG;
    return 0;
}

int APS5_VABI sceSslGetNameEntryInfo(void* cert_name, int entry_num, char* oidname, unsigned max_oidname_len, char* value, unsigned max_value_len, unsigned* value_len) {
    (void)entry_num;
    (void)oidname;
    (void)max_oidname_len;
    (void)value;
    (void)max_value_len;
    if (!cert_name || !value_len) return ERROR_INVALID_ARG;
    *value_len = 0;
    return ERROR_NOT_FOUND;
}

int APS5_VABI sceSslGetPem() {
    NotImplemented_nid_no_patch(__func__);
    return 0;
}

void* APS5_VABI sceSslGetSubjectName(void* ssl_cert) {
    if (!ssl_cert) return nullptr;
    return nullptr;
}

}
