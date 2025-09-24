//g++ -std=c++17 -O2 -Iinclude src/avalanche.cpp src/hash.cpp -o avalanche -lcrypto -Wno-deprecated-declarations
// ./avalanche
#include <iostream>
#include <iomanip>
#include <random>
#include <array>
#include <string>
#include <vector>
#include <sstream>
#include "hash.h"
#include <openssl/md5.h>
#include <openssl/sha.h>

//sugeneruoja atsitiktinį ASCII stringą
static std::string random_ascii(size_t len, std::mt19937_64& rng) {
    std::uniform_int_distribution<int> dist(32, 126);
    std::string s(len, ' ');
    for (size_t i = 0; i < len; i++) s[i] = (char)dist(rng);
    return s;
}

// Paverčia hash į bitų vektorių
static std::vector<int> hash_to_bits(const std::array<uint32_t,8>& h) {
    std::vector<int> bits;
    bits.reserve(256);
    for (uint32_t part : h) {
        for (int i = 31; i >= 0; --i) {
            bits.push_back((part >> i) & 1);
        }
    }
    return bits;
}
// Pagalbinė funkcija: baitus paverčia į hex
static std::string bytes_to_hex(const unsigned char* data, size_t len) {
    std::ostringstream oss;
    for (size_t i = 0; i < len; i++) {
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)data[i];
    }
    return oss.str();
}
// MD5
static std::string md5_hash(const std::string& input) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5((unsigned char*)input.c_str(), input.size(), digest);
    return bytes_to_hex(digest, MD5_DIGEST_LENGTH);
}

// SHA-1
static std::string sha1_hash(const std::string& input) {
    unsigned char digest[SHA_DIGEST_LENGTH];
    SHA1((unsigned char*)input.c_str(), input.size(), digest);
    return bytes_to_hex(digest, SHA_DIGEST_LENGTH);
}

// SHA-256
static std::string sha256_hash(const std::string& input) {
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)input.c_str(), input.size(), digest);
    return bytes_to_hex(digest, SHA256_DIGEST_LENGTH);
}


int main(int argc, char** argv) {
    int N = 10000;      // kiek porų testuosim
    int LEN = 20;       // stringo ilgis
    if (argc >= 2) N = std::stoi(argv[1]);
    if (argc >= 3) LEN = std::stoi(argv[2]);

    std::mt19937_64 rng(12345);

    // statistika mano hash'ui
    double bit_min=1.0, bit_max=0.0, bit_sum=0.0;
    double hex_min=1.0, hex_max=0.0, hex_sum=0.0;

    // statistika MD5 / SHA1 / SHA256
    double md5_sum=0.0, sha1_sum=0.0, sha256_sum=0.0;

    for (int k=0; k<N; k++) {
        std::string s1 = random_ascii(LEN, rng);
        std::string s2 = s1;
        // pakeičiam vieną simbolį (vidurį)
        s2[LEN/2] = (s2[LEN/2] == 'a' ? 'b' : 'a');

        //mano hash
        auto seed = std::array<uint32_t,8>{
            (uint32_t)s1.length() * 123,
            (uint32_t)(unsigned char)s1.front() * 4567,
            (uint32_t)(unsigned char)s1.back() * 8910,
            (uint32_t)(s1.length() << 16) ^ 0xDEAD,
            0xAAAAAAAAu ^ (uint32_t)s1.length(),
            0x55555555u + (uint32_t)s1.length(),
            0xF0F0F0F0u ^ (unsigned char)s1[0],
            0x0F0F0F0Fu ^ (unsigned char)s1.back()
        };
        std::vector<char> v1(s1.begin(), s1.end());
        std::vector<char> v2(s2.begin(), s2.end());
        auto h1 = bubble_sort_and_hash(v1, seed);
        auto h2 = bubble_sort_and_hash(v2, seed);

        // Bitų lygmuo
        auto b1 = hash_to_bits(h1);
        auto b2 = hash_to_bits(h2);
        int diff_bits = 0;
        for (size_t i=0; i<b1.size(); i++) if (b1[i]!=b2[i]) diff_bits++;
        double bit_ratio = (double)diff_bits / b1.size();
        bit_sum += bit_ratio;
        if (bit_ratio < bit_min) bit_min = bit_ratio;
        if (bit_ratio > bit_max) bit_max = bit_ratio;

        // Hex lygmuo
        std::string hx1 = hash_to_hex(h1);
        std::string hx2 = hash_to_hex(h2);
        int diff_hex = 0;
        for (size_t i=0; i<hx1.size(); i++) if (hx1[i]!=hx2[i]) diff_hex++;
        double hex_ratio = (double)diff_hex / hx1.size();
        hex_sum += hex_ratio;
        if (hex_ratio < hex_min) hex_min = hex_ratio;
        if (hex_ratio > hex_max) hex_max = hex_ratio;

        // MD5
        std::string md5_1 = md5_hash(s1);
        std::string md5_2 = md5_hash(s2);
        int diff_md5 = 0;
        for (size_t i=0; i<md5_1.size(); i++) if (md5_1[i]!=md5_2[i]) diff_md5++;
        md5_sum += (double)diff_md5 / md5_1.size();

        // SHA-1
        std::string sha1_1 = sha1_hash(s1);
        std::string sha1_2 = sha1_hash(s2);
        int diff_sha1 = 0;
        for (size_t i=0; i<sha1_1.size(); i++) if (sha1_1[i]!=sha1_2[i]) diff_sha1++;
        sha1_sum += (double)diff_sha1 / sha1_1.size();

        // SHA-256
        std::string sha256_1 = sha256_hash(s1);
        std::string sha256_2 = sha256_hash(s2);
        int diff_sha256 = 0;
        for (size_t i=0; i<sha256_1.size(); i++) if (sha256_1[i]!=sha256_2[i]) diff_sha256++;
        sha256_sum += (double)diff_sha256 / sha256_1.size();
    }

    std::cout << "Lavinos efekto testas (" << N << " porų, ilgis=" << LEN << ")\n";

    std::cout << ">>> mano hash:\n";
    std::cout << "Bitų lygmuo: min=" << bit_min*100 << "%, max=" << bit_max*100
              << "%, vidurkis=" << (bit_sum/N)*100 << "%\n";
    std::cout << "Hex lygmuo: min=" << hex_min*100 << "%, max=" << hex_max*100
              << "%, vidurkis=" << (hex_sum/N)*100 << "%\n";

    std::cout << " Hex lygmuo:" << "%\n";
    std::cout << ">>> MD5 vidurkis=" << (md5_sum/N)*100 << "%\n";
    std::cout << ">>> SHA-1 vidurkis=" << (sha1_sum/N)*100 << "%\n";
    std::cout << ">>> SHA-256 vidurkis=" << (sha256_sum/N)*100 << "%\n";

    return 0;
}