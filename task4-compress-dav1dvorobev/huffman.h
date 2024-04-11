#ifndef HUFFMAN
#define HUFFMAN

#include <bitset>
#include <fstream>
#include <iostream>

struct node {
    char symbol;
    unsigned long counter;
    node* left;
    node* right;
    node() : left(nullptr), right(nullptr){};
    node(node* _left, node* _right) {
        left = _left;
        right = _right;
        counter = _left->counter + _right->counter;
    }
};

void encoding(node*);
void sort(node* arr[], const int&, const int&);
node* create_tree(const unsigned int*);
void huffman_compress(std::ifstream&, std::ofstream&);
void huffman_decompress(std::ifstream&, std::ofstream&);
#endif // HUFFMAN