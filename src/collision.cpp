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

// Apskaičiuoja 256-bit hash (kaip 8x uint32_t) pagal funkcijas
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

struct Result { 
    size_t len; 
    uint64_t pairs; 
    uint64_t collisions; 
    double secs; 
};

static Result test_len(size_t len, uint64_t pairs, uint64_t seed0) {
    //Bubble sort O(n^2)
    auto t0 = std::chrono::high_resolution_clock::now();

    uint64_t coll = 0; // skaitiklis koliziju

    // Pasidalinam darbą tarp gijų
    #pragma omp parallel
    {
        std::mt19937_64 rng(seed0 ^ (0x9E3779B97F4A7C15ull * (omp_get_thread_num()+1)));
        uint64_t local_coll = 0;

        #pragma omp for schedule(dynamic, 8)
        for (int64_t i = 0; i < (int64_t)pairs; ++i) {
            std::string a = random_ascii(len, rng);
            std::string b = random_ascii(len, rng);

            auto ha = hash256(a);
            auto hb = hash256(b);

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
    std::cout << "len,pairs,collisions,rate,seconds\n";

    for (size_t L : lengths) {
        auto r = test_len(L, pairs, seed0 + L);
        double rate = (r.pairs == 0) ? 0.0 : (double)r.collisions / (double)r.pairs;
        std::cout << r.len << "," << r.pairs << "," << r.collisions << ","
                  << std::setprecision(8) << rate << "," << r.secs << "\n";
    }
    return 0;
}
