//make
//./run_all.sh
//./hash_program input/random1.txt
//./hash_program input/negriztamas.txt
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <sstream>
#include <iomanip>
#include <fstream>
#include "hash.h"
#include "timer.h"

#include <openssl/md5.h>
#include <openssl/sha.h>

// Konvertuoja baitus į hex stringą
static std::string bytes_to_hex_openssl(const unsigned char* data, size_t len) {
    std::ostringstream ss;
    ss << std::hex << std::setfill('0');
    for (size_t i = 0; i < len; i++) {
        ss << std::setw(2) << (int)data[i];
    }
    return ss.str();
}

// MD5
static std::string md5_hash(const std::string& input) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5((unsigned char*)input.c_str(), input.size(), digest);
    return bytes_to_hex_openssl(digest, MD5_DIGEST_LENGTH);
}

// SHA-1
static std::string sha1_hash(const std::string& input) {
    unsigned char digest[SHA_DIGEST_LENGTH];
    SHA1((unsigned char*)input.c_str(), input.size(), digest);
    return bytes_to_hex_openssl(digest, SHA_DIGEST_LENGTH);
}

// SHA-256
static std::string sha256_hash(const std::string& input) {
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)input.c_str(), input.size(), digest);
    return bytes_to_hex_openssl(digest, SHA256_DIGEST_LENGTH);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Naudojimas: " << argv[0] << " input-file\n";
        return 1;
    }

    std::string infile = argv[1];
    std::ifstream in(infile, std::ios::binary);
    if (!in) {
        std::cerr << "Nepavyko atidaryti failo: " << infile << "\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << in.rdbuf(); //visa turini i bufferi
    std::string msg = buffer.str(); //paverciam i string

    //salt
    auto salt= make_salt(msg);

    //data = salt + msg
    std::vector<char> data;
    data.reserve(salt.size() + msg.size());
    for (uint8_t b : salt) data.push_back((char)b);
    data.insert(data.end(), msg.begin(), msg.end());
    
    //seed: 8 reiksmes po 32 bitus
    std::array<uint32_t, 8> seed = {
        (uint32_t)msg.length() * 123,            //ilgio seed
    (uint32_t)(unsigned char)msg.front() * 4567, //pirmo simbolio ASCII * konstanta
    (uint32_t)(unsigned char)msg.back() * 8910,  //paskutinio simbolio ASCII * konstanta
    (uint32_t)(msg.length() << 16) ^ 0xDEAD,     //ilgis pastumtas
    0xAAAAAAAAu ^ (uint32_t)msg.length(),        //XOR su ilgiu
    0x55555555u + (uint32_t)msg.length(),        //kita konstanta
    0xF0F0F0F0u ^ (unsigned char)msg[0],         //pirmo simbolio itaka
    0x0F0F0F0Fu ^ (unsigned char)msg.back()      //paskutinio simbolio itaka
    };

    //paleidziam bubble sort su hash skaiciavimu
    Timer t;  //paleidziam laikmati
    auto h= bubble_sort_and_hash(data, seed);
    double duration = t.elapsed_ms();  // paimam laika ms

    // Standartiniai hash
    std::string md5 = md5_hash(msg);
    std::string sha1 = sha1_hash(msg);
    std::string sha256 = sha256_hash(msg);


std::ofstream out("results.txt", std::ios::app); //append
if (!out) {
    std::cerr << "Klaida: nepavyko sukurti results.txt\n";
    return 1;
}

    out << "Input file: " << infile << "\n";
    out << "Original (from file): " << msg << "\n";
    out << "Generated salt (hex): " << bytes_to_hex(salt) << "\n";
    out << "Custom hash (256-bit hex): " << hash_to_hex(h) << "\n";
    out << "Hash calculation time: " << duration << " ms\n";
    out << "MD5: " << md5 << "\n";
    out << "SHA-1: " << sha1 << "\n";
    out << "SHA-256: " << sha256 << "\n";
    out << "\n";

return 0;

}