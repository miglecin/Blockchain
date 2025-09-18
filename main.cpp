#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <sstream>
#include <iomanip>

//bubble sort (rikiuoja pagal baito reikšmę) su hash skaiciavimu per swap'us
unsigned int bubble_sort_and_hash(std::vector<char>& arr, unsigned int seed) {
    unsigned int h = seed;
    int n = (int)arr.size();

    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - 1 - i; ++j) {
            if (arr[j] > arr[j+1]) {
                // kai sukeičiam elementus, atnaujinam hash
                unsigned int a = (unsigned char)arr[j];
                unsigned int b = (unsigned char)arr[j+1];
                h = (h << 3) + (h >> 2) + (a * 17 + b * 31 + j * 13);

                std::swap(arr[j], arr[j+1]);
            }
        }
    }
    return h;
}

int main() {
    std::string msg ="slaptazodis";

    //i simboliu vekt
    std::vector<char> data(msg.begin(), msg.end());

    //atspausdinam pradinio stringo ASCII reikšmes
    std::cout << "Original: " << msg << "\n";
    std::cout << "Original ASCII: ";
    for (char c : data) std::cout << (int)c << " ";
    std::cout << "\n";

    //paleidziam bubble sort su hash skaiciavimu
    unsigned int seed = msg.length() * 123; //seed priklauso nuo teksto ilgio
    unsigned int h = bubble_sort_and_hash(data, seed);

    //atspausdinam surikiuota stringa
    std::cout << "Original: " << msg << "\n";
    std::cout << "Sorted:   ";
    for (char c : data) std::cout << c;
    std::cout << "\n";

    //atspausdinam surikiuoto stringo ASCII reiksmes
    std::cout << "Sorted ASCII:   ";
    for (char c : data) std::cout << (int)c << " ";
    std::cout << "\n";

    //Paprasta suma kaip HASH(palyginimui)
    int sum= 0;
    for (char c : data) sum+= (int)c;

    std::cout <<"Simple hash (sum of ASCII): " << sum << "\n";

    //custom hash priklausantis nuo bubble sort swapu
    std::cout << "Custom hash (decimal): " << h << "\n";

    
    return 0;
}
