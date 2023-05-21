#include "Cryptography.hpp"

Cryptography::Cryptography(/* args */)
{
}

Cryptography::~Cryptography()
{
}

auto Cryptography::GenerateKey() -> void
{
    CryptoPP::AutoSeededRandomPool prng;
    prng.GenerateBlock(key, sizeof(key));

    if (!(m_EncryptCTX = EVP_CIPHER_CTX_new()))
        std::cout << "EVP_CIPHER_CTX_new m_EncryptCTX failed" << std::endl;

    if (!(EVP_EncryptInit_ex(m_EncryptCTX, EVP_aes_128_cfb8(), nullptr, key, key)))
        std::cout << "EVP_EncryptInit_ex m_EncryptCTX failed" << std::endl;

    if (!(m_DecryptCTX = EVP_CIPHER_CTX_new()))
        std::cout << "EVP_CIPHER_CTX_new m_DecryptCTX failed" << std::endl;

    if (!(EVP_DecryptInit_ex(m_DecryptCTX, EVP_aes_128_cfb8(), nullptr, key, key)))
        std::cout << "EVP_DecryptInit_ex m_DecryptCTX failed" << std::endl;

    m_BlockSize = EVP_CIPHER_block_size(EVP_aes_128_cfb8());
}

auto Cryptography::SetPublicKey(std::vector<unsigned char> publickey) -> void
{
    m_publicKey = publickey;
}

auto Cryptography::GetEncryptedSharedSecret() -> std::vector<unsigned char>
{
    return EncryptRSA(std::vector<unsigned char>(key, key + sizeof(key)));
}
void performTwosCompliment(unsigned char *buffer, int length)
{
    bool carry = true;
    int i;
    unsigned char newByte, value;
    for (i = length - 1; i >= 0; --i)
    {
        value = buffer[i];
        newByte = ~value & 0xff;
        if (carry)
        {
            carry = newByte == 0xff;
            buffer[i] = newByte + 1;
        }
        else
        {
            buffer[i] = newByte;
        }
    }
}
auto Cryptography::GenerateAuthHash(std::string serverid) -> std::string
{

    CryptoPP::SHA1 hash;
    hash.Update((const CryptoPP::byte *)serverid.data(), serverid.size());
    hash.Update((const CryptoPP::byte *)key, sizeof(key));
    hash.Update((const CryptoPP::byte *)m_publicKey.data(), m_publicKey.size());
    /*     digest.resize(hash.DigestSize()); */

    unsigned char digest[20];

    memset(digest, 0, sizeof(digest));

    hash.Final((CryptoPP::byte *)digest);
    bool negative = static_cast<int>(digest[0]) < 0;
    if (negative)
    {
        performTwosCompliment(digest, sizeof(digest));
    }
    char digest_final[sizeof(digest) * 2 + 1];
    for (int i = 0; i < sizeof(digest); i++)
    {
        sprintf(&digest_final[i * 2], "%02x", static_cast<unsigned int>(digest[i]));
    }
    std::string result(digest_final);
    result.erase(0, result.find_first_not_of('0'));
    if (negative)
    {
        result = "-" + result;
    }
    return result;
}

auto Cryptography::EncryptRSA(std::vector<unsigned char> data) -> std::vector<unsigned char>
{
    CryptoPP::AutoSeededRandomPool rng;

    CryptoPP::RSA::PublicKey publicKey;
    CryptoPP::ArraySource as(m_publicKey.data(), m_publicKey.size(), true);
    publicKey.Load(as);

    std::string buffer;

    CryptoPP::RSAES_PKCS1v15_Encryptor e(publicKey);
    CryptoPP::StringSource(data.data(), data.size(), true, new CryptoPP::PK_EncryptorFilter(rng, e, new CryptoPP::StringSink(buffer)));

    return std::vector<unsigned char>(buffer.begin(), buffer.end());
}

auto Cryptography::EncryptAES(std::vector<unsigned char> data) -> std::vector<unsigned char>
{

    std::vector<unsigned char> result;
    int size = 0;

    result.resize(data.size() + m_BlockSize);
    EVP_EncryptUpdate(m_EncryptCTX, &result[0], &size, &data[0], data.size());
    result.resize(size);

    return result;
}

auto Cryptography::DecryptAES(std::vector<unsigned char> data) -> std::vector<unsigned char>
{
    std::vector<unsigned char> result;
    int size = 0;

    result.resize(data.size() + m_BlockSize);
    EVP_DecryptUpdate(m_DecryptCTX, &result[0], &size, &data[0], data.size());
    result.resize(size);
    return result;
}