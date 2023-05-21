#ifndef CRYPTOGRAPHY_HPP
#define CRYPTOGRAPHY_HPP

#include "stdafx.h"

#define SIGN_BIT(x) ((x) & (1UL << 63))

class Cryptography
{
private:

    CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];

    std::vector<unsigned char> m_publicKey;

    EVP_CIPHER_CTX* m_EncryptCTX;
    EVP_CIPHER_CTX* m_DecryptCTX;

    unsigned int m_BlockSize;

public:
    Cryptography();
    ~Cryptography();

    auto GenerateKey() -> void;
    auto SetPublicKey(std::vector<unsigned char> publickey) -> void;
    auto GetEncryptedSharedSecret() -> std::vector<unsigned char>;
    auto GenerateAuthHash(std::string serverid) -> std::string;

    auto EncryptRSA(std::vector<unsigned char> data) -> std::vector<unsigned char>;

    auto EncryptAES(std::vector<unsigned char> data) -> std::vector<unsigned char>;
    auto DecryptAES(std::vector<unsigned char> data) -> std::vector<unsigned char>;
};


#endif