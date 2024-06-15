#include "MbedTlsEncryptionHandler.h"

using namespace tcremote;

MbedTlsEncryptionHandler::MbedTlsEncryptionHandler(const uint8_t* keyIn, const uint8_t* initialIv) {
    memcpy(key, keyIn, sizeof key);
    memcpy(ivDec, initialIv, sizeof ivDec);
    memcpy(ivEnc, initialIv, sizeof ivEnc);
}

void MbedTlsEncryptionHandler::initialise() {
    mbedtls_aes_init(&aes);
}

int roundToNearest(int in) {
    auto remainder = in % 8;
    if(remainder == 0) return in;
    return in + (8-remainder);
}

int MbedTlsEncryptionHandler::encryptData(const uint8_t *plainText, int bytesIn, uint8_t *buffer, size_t buffLen) {
    mbedtls_aes_setkey_enc(&aes, key, 256);
    int totalIn = roundToNearest(bytesIn);
    if(totalIn > buffLen) return 0;
    bool ok = mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, bytesIn, ivEnc, plainText, buffer) == 0;
    return ok ? totalIn : 0;

}

int MbedTlsEncryptionHandler::decryptData(const uint8_t *encoded, int bytesIn, uint8_t *buffer, size_t buffLen) {
    mbedtls_aes_setkey_dec(&aes, key, 256);
    bool ok = mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, bytesIn, ivDec, encoded, buffer) == 0;
    return ok ? bytesIn : 0;
}
