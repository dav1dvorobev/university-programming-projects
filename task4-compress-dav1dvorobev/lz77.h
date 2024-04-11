#ifndef LZ77
#define LZ77

#include <fstream>
#include <iostream>
#include <sstream>

struct token {
    int offset;
    int length;
    char next;
};
std::ostream& operator<<(std::ostream&, const token&);

void lz77_compress(std::ifstream&, std::ofstream&);
void lz77_decompress(std::ifstream&, std::ofstream&);
#endif // LZ77