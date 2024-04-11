#include <fstream>
#include <iostream>

#include "huffman.h"
#include "lz77.h"
#include "mtf.h"

int main(int argc, char** argv) {
    std::string inputFILE;
    std::string outputFILE;
    bool mtf = false;
    bool lz77 = false;
    bool huffman = false;
    bool decompress = false;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-mtf") {
            mtf = true;
        } else if (arg == "-lz77") {
            lz77 = true;
        } else if (arg == "-huffman") {
            huffman = true;
        } else if (arg == "-d") {
            decompress = true;
        } else if (arg == "-h" || arg == "?") {
            std::cout << "Usage: compress [options] input output" << std::endl;
            return 0;
        } else {
            if (inputFILE.empty()) {
                inputFILE = arg;
            } else if (outputFILE.empty()) {
                outputFILE = arg;
            }
        }
    }
    std::ifstream input(inputFILE, std::ios::binary);
    std::ofstream output(outputFILE, std::ios::binary);
    std::ofstream tmp;
    if (decompress) {
        char bits = static_cast<char>(input.get());
        if (bits == 1) {
            mtf_decoding(input, output);
        } else if (bits == 2) {
            lz77_decompress(input, output);
        } else if (bits == 4) {
            huffman_decompress(input, output);
        } else if (bits == 3) {
            tmp.open("lz77_decompress.bin", std::ios::binary);
            lz77_decompress(input, tmp);
            input.close();
            tmp.close();
            input.open("lz77_decompress.bin", std::ios::binary);
            mtf_decoding(input, output);
        } else if (bits == 5) {
            tmp.open("huffman_decompress.bin", std::ios::binary);
            huffman_decompress(input, tmp);
            input.close();
            tmp.close();
            input.open("huffman_decompress.bin", std::ios::binary);
            mtf_decoding(input, output);
        } else if (bits == 6) {
            tmp.open("huffman_decompress.bin", std::ios::binary);
            huffman_decompress(input, tmp);
            input.close();
            tmp.close();
            input.open("huffman_decompress.bin", std::ios::binary);
            lz77_decompress(input, output);
        } else if (bits == 7) {
            tmp.open("huffman_decompress.bin", std::ios::binary);
            huffman_decompress(input, tmp);
            input.close();
            tmp.close();
            input.open("huffman_decompress.bin", std::ios::binary);
            tmp.open("lz77_decompress.bin", std::ios::binary);
            lz77_decompress(input, tmp);
            input.close();
            tmp.close();
            input.open("lz77_decompress.bin", std::ios::binary);
            mtf_decoding(input, output);
        }
    } else {
        char bits = 0;
        std::string tmpFILE;
        if (mtf) {
            bits += 1;
            tmpFILE = "mtf_encoding.bin";
            tmp.open(tmpFILE, std::ios::binary);
            mtf_encoding(input, tmp);
            input.close();
            tmp.close();
        }
        if (lz77) {
            bits += 2;
            if (!tmpFILE.empty()) { input.open(tmpFILE, std::ios::binary); }
            tmpFILE = "lz77_compress.bin";
            tmp.open(tmpFILE, std::ios::binary);
            lz77_compress(input, tmp);
            input.close();
            tmp.close();
        }
        if (huffman) {
            bits += 4;
            if (!tmpFILE.empty()) { input.open(tmpFILE, std::ios::binary); }
            tmpFILE = "huffman_compress.bin";
            tmp.open(tmpFILE, std::ios::binary);
            huffman_compress(input, tmp);
            input.close();
            tmp.close();
        }
        input.open(tmpFILE, std::ios::binary);
        output << bits << input.rdbuf();
        input.close();
        output.close();
    }
    return 0;
}