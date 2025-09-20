#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <sstream>
#include <iomanip>
#include <fstream>

//bubble sort (rikiuoja pagal baito reikšmę) su hash skaiciavimu per swap'us (256 bit)
std::array<uint32_t, 8> bubble_sort_and_hash(std::vector<char>& arr, std::array<uint32_t, 8> seed) {
    int n = (int)arr.size();

    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - 1 - i; ++j) {
            if (arr[j] > arr[j+1]) {
                //kai sukeiciam elemntus, atnaujinam hash
                unsigned int a = (unsigned char)arr[j];
                unsigned int b = (unsigned char)arr[j+1];

                int idx = j % 8; //pasirenkam, kuri 32-bit bloka keisti
                seed[idx] = (seed[idx] << 5) + (seed[idx] >> 3) + (a * 17 + b * 31 + j * 13);

                std::swap(arr[j], arr[j+1]);
            }
        }
    }
    return seed;
}

//salt (16 baitu)
std::vector<uint8_t> make_salt(const std::string& msg) { 
    std::vector<uint8_t> salt(16, 0); //sukuriam tusc salt
    for (size_t i = 0; i < msg.size(); i++) {
        //kiekviena raida imaisom i viena is 16 baitu (ASCII + pozicija*13), 0xFF kad nevirsytu 255
        salt[i % 16] = (salt[i % 16] + (uint8_t)msg[i] + (i * 13)) & 0xFF;
    }
    return salt;
}

//hex spausdinimas
std::string bytes_to_hex(const std::vector<uint8_t>& v) {
    std::ostringstream ss;
    ss << std::hex << std::setfill('0');
    for (uint8_t b : v) ss << std::setw(2) << (int)b;
    return ss.str();
}

//hash pavertimas i hex
std::string hash_to_hex(const std::array<uint32_t, 8>& h) {
    std::ostringstream ss;
    ss << std::hex << std::setfill('0');
    for (uint32_t part : h) {
        ss<< std::setw(8) << part;
    }
    return ss.str();
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
    0x55555555u + (uint32_t)msg.length(),        // ita konstanta
    0xF0F0F0F0u ^ (unsigned char)msg[0],         //pirmo simbolio itaka
    0x0F0F0F0Fu ^ (unsigned char)msg.back()      //paskutinio simbolio itaka
    };

    //paleidziam bubble sort su hash skaiciavimu
    auto h= bubble_sort_and_hash(data, seed);

std::ofstream out("results.txt", std::ios::app); //append
if (!out) {
    std::cerr << "Klaida: nepavyko sukurti results.txt\n";
    return 1;
}

out << "Input file: " << infile << "\n";
out << "Original (from file): " << msg << "\n";
out << "Generated salt (hex): " << bytes_to_hex(salt) << "\n";
out << "Custom hash (256-bit hex): " << hash_to_hex(h) << "\n";
out << "\n";

return 0;

}