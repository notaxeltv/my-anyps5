#include "prx/libc/include/general/VabiMacros.hpp"
#include <cstddef>
#include <cstdlib>

extern "C" {
int APS5_VABI sceSslInit_nid_postfix(std::size_t);
int APS5_VABI sceSslGetCaCerts(int, void*);
int APS5_VABI sceSslFreeCaCerts(int, void*);
int APS5_VABI sceSslGetSerialNumber(void*, const char**, unsigned*);
int APS5_VABI sceSslGetMemoryPoolStats(void*);
int APS5_VABI sceSslFreeSslCertName(void*);
void* APS5_VABI sceSslGetIssuerName(void*);
void* APS5_VABI sceSslGetSubjectName(void*);
int APS5_VABI sceSslGetNameEntryCount(void*);
}

static void Require(bool value) { if (!value) std::abort(); }

struct SslCaCerts {
    void* certs;
    std::size_t num;
    void* pool;
};

int main() {
    constexpr int notFound = static_cast<int>(0x8095F004);
    constexpr int invalidArg = static_cast<int>(0x8095177A);
    int marker = 0;

    const int context = sceSslInit_nid_postfix(0x10000);
    Require(context > 0);
    Require(sceSslGetCaCerts(context, nullptr) == invalidArg);
    Require(sceSslFreeCaCerts(context, nullptr) == invalidArg);

    SslCaCerts certs{&marker, 3, &marker};
    Require(sceSslGetCaCerts(context, &certs) == notFound);
    Require(certs.certs == nullptr && certs.num == 0 && certs.pool == nullptr);

    certs = {&marker, 3, &marker};
    Require(sceSslFreeCaCerts(context, &certs) == 0);
    Require(certs.certs == nullptr && certs.num == 0 && certs.pool == nullptr);

    const char* serial = reinterpret_cast<const char*>(&marker);
    unsigned serialLen = 9;
    Require(sceSslGetSerialNumber(nullptr, &serial, &serialLen) == invalidArg);
    Require(sceSslGetSerialNumber(&marker, nullptr, &serialLen) == invalidArg);
    Require(sceSslGetSerialNumber(&marker, &serial, nullptr) == invalidArg);
    Require(sceSslGetSerialNumber(&marker, &serial, &serialLen) == notFound);
    Require(serial == nullptr && serialLen == 0);

    unsigned stats[4] = {1, 2, 3, 4};
    Require(sceSslGetMemoryPoolStats(nullptr) == invalidArg);
    Require(sceSslGetMemoryPoolStats(stats) == 0);
    Require(stats[0] == 0 && stats[1] == 0 && stats[2] == 0 && stats[3] == 0);

    Require(sceSslGetIssuerName(nullptr) == nullptr);
    Require(sceSslGetIssuerName(&marker) == nullptr);
    Require(sceSslGetSubjectName(&marker) == nullptr);
    Require(sceSslGetNameEntryCount(nullptr) == invalidArg);
    Require(sceSslGetNameEntryCount(&marker) == 0);
    Require(sceSslFreeSslCertName(nullptr) == invalidArg);
    Require(sceSslFreeSslCertName(&marker) == 0);
}
