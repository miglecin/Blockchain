#include <iostream>
#include <string>
#include <vector>
#include <utility>

//bubble sort (rikiuoja pagal baito reikšmę)
void bubble_sort(std::vector<char>& arr) {
    int n = (int)arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - 1 - i; ++j) {
            if (arr[j] > arr[j+1]) {
                std::swap(arr[j], arr[j+1]);
            }
        }
    }
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

    //isrusiuojam su bubble sort
    bubble_sort(data);

    //atspausdinam surikiuota stringa
    std::cout << "Original: " << msg << "\n";
    std::cout << "Sorted:   ";
    for (char c : data) std::cout << c;
    std::cout << "\n";

    //atspausdinam surikiuoto stringo ASCII reiksmes
    std::cout << "Sorted ASCII:   ";
    for (char c : data) std::cout << (int)c << " ";
    std::cout << "\n";

    //Paprasta suma kaip HASH
    int sum= 0;
    for (char c : data) sum+= (int)c;

    std::cout <<"Simple hash (sum of ASCII): " << sum << "\n";


    return 0;
}
