#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <sstream>
#include <iomanip>

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

//salt (16 baitų)
std::vector<uint8_t> make_salt(const std::string& msg) { 
    std::vector<uint8_t> salt(16, 0); //sukuriam tusc salt
    for (size_t i = 0; i < msg.size(); i++) {
        //kiekviena raida imaisom i viena is 16 baitu (ASCII + pozicija*13), 0xFF kad nevirsytu 255
        salt[i % 16] = (salt[i % 16] + (uint8_t)msg[i] + (i * 13)) & 0xFF;
    }
    return salt;
}

int main() {
    std::string msg ="slaptazodis jfvsdj";

    //i simboliu vekt
    std::vector<char> data(msg.begin(), msg.end());
    
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
    auto h = bubble_sort_and_hash(data, seed);

    //atspausdinam originalu stringa
    std::cout << "Original: " << msg << "\n";

    // spausdinam galutinį 256-bit hash
    std::ostringstream ss;
    for (uint32_t part : h) {
        ss << std::hex << std::setfill('0') << std::setw(8) << part;
    }
    std::cout << "Custom hash (256-bit hex): " << ss.str() << "\n";

    return 0;
}
