//g++ -Xpreprocessor -fopenmp \
    -I/usr/local/opt/libomp/include \
    -L/usr/local/opt/libomp/lib \
   -lomp -Iinclude -O3 \
    src/collision.cpp src/hash.cpp -o collision_test -lcrypto -Wno-deprecated-declarations
// ./collision_test 100000        
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <array>
#include <random>
#include <chrono>
#include <sstream>
#include <cstdint>

#include <omp.h>        //OpenMP header
#include <openssl/md5.h>
#include <openssl/sha.h>
#include "hash.h"   
    

//Sugeneruoja atsitiktini ASCII stringa [32..126]
static std::string random_ascii(size_t len, std::mt19937_64& rng) {
    std::uniform_int_distribution<int> dist(32, 126);
    std::string s; 
    s.resize(len);
    for (size_t i = 0; i < len; ++i) {
        s[i] = static_cast<char>(dist(rng));
    }
    return s;
}

//Mano hash (256-bit)
static std::array<uint32_t,8> hash256(const std::string& msg) {
    auto salt = make_salt(msg);
    std::vector<char> data;
    data.reserve(salt.size() + msg.size());
    for (uint8_t b : salt) data.push_back((char)b);
    data.insert(data.end(), msg.begin(), msg.end());

    std::array<uint32_t, 8> seed = {
        (uint32_t)msg.length() * 123,
        (uint32_t)(unsigned char)msg.front() * 4567,
        (uint32_t)(unsigned char)msg.back() * 8910,
        (uint32_t)(msg.length() << 16) ^ 0xDEAD,
        0xAAAAAAAAu ^ (uint32_t)msg.length(),
        0x55555555u + (uint32_t)msg.length(),
        0xF0F0F0F0u ^ (unsigned char)msg[0],
        0x0F0F0F0Fu ^ (unsigned char)msg.back()
    };

    return bubble_sort_and_hash(data, seed);
}

// Hash helper: convert raw bytes to std::string
static std::string bytes_to_string(const unsigned char* data, size_t len) {
    return std::string(reinterpret_cast<const char*>(data), len);
}

// MD5
static std::string md5_hash(const std::string& input) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5((unsigned char*)input.c_str(), input.size(), digest);
    return bytes_to_string(digest, MD5_DIGEST_LENGTH);
}

// SHA-1
static std::string sha1_hash(const std::string& input) {
    unsigned char digest[SHA_DIGEST_LENGTH];
    SHA1((unsigned char*)input.c_str(), input.size(), digest);
    return bytes_to_string(digest, SHA_DIGEST_LENGTH);
}

// SHA-256
static std::string sha256_hash(const std::string& input) {
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)input.c_str(), input.size(), digest);
    return bytes_to_string(digest, SHA256_DIGEST_LENGTH);
}

struct Result { 
    size_t len; 
    uint64_t pairs; 
    uint64_t collisions; 
    double secs; 
};

// Tikrina kolizijas pasirinktam algoritmui
template<typename F>
static Result test_len(const std::string& name, F hash_func, size_t len, uint64_t pairs, uint64_t seed0) {
    auto t0 = std::chrono::high_resolution_clock::now();
    uint64_t coll = 0;

    #pragma omp parallel
    {
        std::mt19937_64 rng(seed0 ^ (0x9E3779B97F4A7C15ull * (omp_get_thread_num()+1)));
        uint64_t local_coll = 0;

        #pragma omp for schedule(dynamic, 8)
        for (int64_t i = 0; i < (int64_t)pairs; ++i) {
            std::string a = random_ascii(len, rng);
            std::string b = random_ascii(len, rng);

            auto ha = hash_func(a);
            auto hb = hash_func(b);

            if (ha == hb) ++local_coll;
        }

        #pragma omp atomic
        coll += local_coll;
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> d = t1 - t0;
    return Result{len, pairs, coll, d.count()};
}

int main(int argc, char** argv) {
    // Naudojimas: ./collision_test <pairs_per_len> [seed]
    uint64_t pairs = 2000; 
    uint64_t seed0 = 123456789;

    if (argc >= 2) pairs = std::stoull(argv[1]);
    if (argc >= 3) seed0 = std::stoull(argv[2]);

    std::vector<size_t> lengths = {10, 100, 500, 1000};

    std::cout << "Collision search (pairs per length = " << pairs
              << ", seed=" << seed0 << ")\n";
    std::cout << "algo,len,pairs,collisions,rate,seconds\n";

    for (size_t L : lengths) {
        // Mano hash
        auto r1 = test_len("MyHash", [](const std::string& s){
            return bytes_to_string((unsigned char*)hash_to_hex(hash256(s)).c_str(), 64);
        }, L, pairs, seed0 + L);
        std::cout << "MyHash," << r1.len << "," << r1.pairs << "," << r1.collisions << ","
                  << std::setprecision(8) << (double)r1.collisions/r1.pairs << "," << r1.secs << "\n";

        // MD5
        auto r2 = test_len("MD5", md5_hash, L, pairs, seed0 + L);
        std::cout << "MD5," << r2.len << "," << r2.pairs << "," << r2.collisions << ","
                  << std::setprecision(8) << (double)r2.collisions/r2.pairs << "," << r2.secs << "\n";

        // SHA-1
        auto r3 = test_len("SHA1", sha1_hash, L, pairs, seed0 + L);
        std::cout << "SHA1," << r3.len << "," << r3.pairs << "," << r3.collisions << ","
                  << std::setprecision(8) << (double)r3.collisions/r3.pairs << "," << r3.secs << "\n";

        // SHA-256
        auto r4 = test_len("SHA256", sha256_hash, L, pairs, seed0 + L);
        std::cout << "SHA256," << r4.len << "," << r4.pairs << "," << r4.collisions << ","
                  << std::setprecision(8) << (double)r4.collisions/r4.pairs << "," << r4.secs << "\n";
    }
    return 0;
}