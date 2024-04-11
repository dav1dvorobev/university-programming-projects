#include "lz77.h"

std::ostream& operator<<(std::ostream& output, const token& other) {
    output << static_cast<char>(other.offset) << static_cast<char>(other.length) << other.next;
    return output;
}
void lz77_compress(std::ifstream& input, std::ofstream& output) {
    input.seekg(0, std::ios::end);
    int size = static_cast<int>(input.tellg());
    ++size;
    input.clear();
    input.seekg(0);
    char* str = new char[size];
    int idx = 0;
    int counter = 0;
    while (!input.eof()) {
        char cur = static_cast<char>(input.get());
        str[idx++] = cur;
        if (cur == -1) { ++counter; }
    }
    --counter;
    idx = 0;
    while (idx < size) {
        token best = {0, 0, str[idx]};
        for (int i = (idx > 128) ? idx - 128 : 0; i < idx; ++i) {
            int match = 0;
            int j = 0;
            while (str[i + j] == str[idx + match]) {
                ++match;
                ++j;
                if (i + j == idx) { j = 0; }
                if (match == 255) { break; }
            }
            if (match != 0 && match >= best.length) { best = {idx - i, match, str[idx + match]}; }
        }
        idx += best.length + 1;
        if (static_cast<int>(best.next) == -1) {
            if (counter == 0) {
                output << static_cast<char>(best.offset) << static_cast<char>(best.length);
            } else {
                output << best;
                --counter;
            }
        } else {
            output << best;
        }
    }
    delete[] str;
}
void lz77_decompress(std::ifstream& input, std::ofstream& output) {
    std::string str;
    token cur;
    while (!input.eof()) {
        cur.offset = input.get();
        cur.length = input.get();
        cur.next = static_cast<char>(input.get());
        if (cur.length > 0) {
            int start = static_cast<int>(str.size()) - cur.offset;
            for (int i = 0; i < cur.length; ++i) { str += str[start + i]; }
        }
        str += cur.next;
    }
    while (static_cast<int>(str[str.size() - 1]) == -1) { str.pop_back(); }
    output << str;
}