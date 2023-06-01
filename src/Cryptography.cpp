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

    /*CryptoPP::SHA1 hash;
    hash.Update((const CryptoPP::byte *)serverid.data(), serverid.size());
    hash.Update((const CryptoPP::byte *)key, sizeof(key));
    hash.Update((const CryptoPP::byte *)m_publicKey.data(), m_publicKey.size());
    digest.resize(hash.DigestSize());

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
    return result;*/

    SHA_CTX ctx_;

    SHA1_Init(&ctx_);

    SHA1_Update(&ctx_, serverid.data(), serverid.size());
    SHA1_Update(&ctx_, key, sizeof(key));
    SHA1_Update(&ctx_, m_publicKey.data(), m_publicKey.size());

    auto result = std::string();

    auto buf = std::array< std::uint8_t, 20 >();
    SHA1_Final(buf.data(), &ctx_);

    // convert has to bignum
    BIGNUM *bn = BN_bin2bn(buf.data(), buf.size(), nullptr);

    // reset the hasher for next use
    SHA1_Init(&ctx_);

    // check for "negative" value
    if (BN_is_bit_set(bn, 159))
    {
        result += '-';

        // perform 1's compliment on the bignum's bits
        auto tmp = std::vector< unsigned char >(BN_num_bytes(bn));
        BN_bn2bin(bn, tmp.data());
        std::transform(tmp.begin(), tmp.end(), tmp.begin(), [](unsigned char b) { return ~b; });
        BN_bin2bn(tmp.data(), tmp.size(), bn);

        // add 1 "as-if" 2's compliment

        BN_add_word(bn, 1);
    }

    // convert to hex
    auto hex = BN_bn2hex(bn);

    // remove any leading zeroes except the last
    auto view = std::string_view(hex);
    while (view.size() && view[0] == '0')
        view = view.substr(1);

    // append the hex to the result
    result.append(view.begin(), view.end());
    OPENSSL_free(hex);
    BN_free(bn);

    // convert the hex to lower case
    std::transform(result.begin(), result.end(), result.begin(),
    [](unsigned char c){ return std::tolower(c); });
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
auto Cryptography::Compress(std::vector<unsigned char> data) -> std::vector<unsigned char>
{
    std::vector<unsigned char> compressedData;
    unsigned long size = data.size();
    compressedData.resize(size);

    compress((Bytef *)&compressedData[0], &size, (const Bytef *)data.data(), data.size());
    compressedData.resize(size);

    return compressedData;
}
auto Cryptography::Decompress(std::vector<unsigned char> data, size_t decompressSize) -> std::vector<unsigned char>
{
    std::vector<unsigned char> decompressedData;
    decompressedData.resize(decompressSize);
    unsigned long size = decompressedData.size();
    uncompress((Bytef *)&decompressedData[0], &size, (const Bytef *)data.data(), data.size());

    return decompressedData;
}