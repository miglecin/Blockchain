//g++ -std=c++17 -O2 -Iinclude src/avalanche.cpp src/hash.cpp -o avalanche
// ./avalanche
#include <iostream>
#include <iomanip>
#include <random>
#include <array>
#include <string>
#include <vector>
#include <sstream>
#include "hash.h"

// Sugeneruoja atsitiktinį ASCII stringą
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


int main(int argc, char** argv) {
    int N = 100000;      // kiek porų testuosim
    int LEN = 20;        // stringo ilgis
    if (argc >= 2) N = std::stoi(argv[1]);
    if (argc >= 3) LEN = std::stoi(argv[2]);

    std::mt19937_64 rng(12345);

    double bit_min=1.0, bit_max=0.0, bit_sum=0.0;
    double hex_min=1.0, hex_max=0.0, hex_sum=0.0;

    for (int k=0; k<N; k++) {
        std::string s1 = random_ascii(LEN, rng);
        std::string s2 = s1;
        // pakeičiam vieną simbolį (vidurį)
        s2[LEN/2] = (s2[LEN/2] == 'a' ? 'b' : 'a');

    auto seed1 = std::array<uint32_t,8>{
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
    auto h1 = bubble_sort_and_hash(v1, seed1);

    auto seed2 = std::array<uint32_t,8>{
    (uint32_t)s1.length() * 123,
    (uint32_t)(unsigned char)s1.front() * 4567,
    (uint32_t)(unsigned char)s1.back() * 8910,
    (uint32_t)(s1.length() << 16) ^ 0xDEAD,
    0xAAAAAAAAu ^ (uint32_t)s1.length(),
    0x55555555u + (uint32_t)s1.length(),
    0xF0F0F0F0u ^ (unsigned char)s1[0],
    0x0F0F0F0Fu ^ (unsigned char)s1.back()
    };
    std::vector<char> v2(s2.begin(), s2.end());
    auto h2 = bubble_sort_and_hash(v2, seed1);
        

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
    }

    std::cout << "Lavinos efekto testas (" << N << " porų, ilgis=" << LEN << ")\n";
    std::cout << "Bitų lygmuo: min=" << bit_min*100 << "%, max=" << bit_max*100
              << "%, vidurkis=" << (bit_sum/N)*100 << "%\n";
    std::cout << "Hex lygmuo: min=" << hex_min*100 << "%, max=" << hex_max*100
              << "%, vidurkis=" << (hex_sum/N)*100 << "%\n";

    return 0;
}