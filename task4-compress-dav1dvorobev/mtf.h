#ifndef MTF
#define MTF

#include <fstream>
#include <iostream>

void mtf_encoding(std::ifstream& input, std::ofstream& output) {
    std::string list;
    for (int i = 0; i < 256; ++i) { list += static_cast<char>(i); }
    int cur = 0;
    while ((cur = input.get()) != EOF) {
        output << static_cast<char>(list.find(static_cast<char>(cur)));
        list = static_cast<char>(cur) + list.erase(list.find(static_cast<char>(cur)), 1);
    }
}
void mtf_decoding(std::ifstream& input, std::ofstream& output) {
    std::string list;
    for (int i = 0; i < 256; ++i) { list += static_cast<char>(i); }
    int cur = 0;
    while ((cur = input.get()) != EOF) {
        output << list[cur];
        char temp = list[cur];
        list = temp + list.erase(cur, 1);
    }
}
#endif // MTF