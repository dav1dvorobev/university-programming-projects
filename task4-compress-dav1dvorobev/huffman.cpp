#include "huffman.h"

void delete_tree(node* root) {
    if (root == nullptr) { return; }
    delete_tree(root->left);
    delete_tree(root->right);
    delete root;
}
void sort(node* arr[], const int& start, const int& end) {
    for (int i = start; i < end; ++i) {
        int sorted = i - 1;
        while (sorted > start - 1 && arr[sorted]->counter > arr[sorted + 1]->counter) {
            std::swap(arr[sorted], arr[sorted + 1]);
            --sorted;
        }
    }
}
std::string code;
std::string table[256];
void encoding(node* root) {
    if (root->left != nullptr) {
        code += '0';
        encoding(root->left);
    }
    if (root->right != nullptr) {
        code += '1';
        encoding(root->right);
    }
    if (root->left == nullptr && root->right == nullptr) {
        if (code.empty()) { code = "0"; }
        table[static_cast<unsigned char>(root->symbol)] = code;
    }
    code.pop_back();
}
node* create_tree(const unsigned int* dict) {
    node* arr[256];
    int size = 0;
    for (int i = 0; i < 256; ++i) {
        if (dict[i] != 0) {
            arr[size] = new node;
            arr[size]->symbol = static_cast<char>(i);
            arr[size]->counter = dict[i];
            ++size;
        }
    }
    int start = 0;
    while (start != size - 1) {
        sort(arr, start, size);
        node* left = arr[start];
        arr[start++] = nullptr;
        node* right = arr[start];
        arr[start] = new node(left, right); // parent
    }
    if (size == 1) {
        return arr[0]; // parent
    }
    return arr[start];
}
void huffman_compress(std::ifstream& input, std::ofstream& output) {
    unsigned int dict[256] = {0};
    while (!input.eof()) {
        int cur = input.get();
        if (cur != -1) { ++dict[cur]; }
    }
    for (int i = 0; i < 256; ++i) { output << dict[i] << '|'; }
    node* root = create_tree(dict);
    encoding(root);
    input.clear();
    input.seekg(0);
    std::string str;
    std::string temp;
    char cur = '\0';
    while (!input.eof()) {
        int cur = input.get();
        if (cur != -1) {
            for (char x : table[cur]) {
                temp += x;
                if (temp.size() == 8) {
                    str += static_cast<char>(stoi(temp, 0, 2));
                    temp.clear();
                }
            }
        }
    }
    output << temp.size() << temp;
    if (!str.empty()) { output << str; }
    delete_tree(root);
}
void huffman_decompress(std::ifstream& input, std::ofstream& output) {
    unsigned int dict[256] = {0};
    for (int i = 0; i < 256; ++i) {
        int counter = 0;
        input >> counter;
        dict[i] = counter;
        input.clear();
        input.ignore(1);
    }
    node* root = create_tree(dict);
    node* cur_root = root;
    std::string tail;
    int temp = input.get() - 48;
    for (int i = 0; i < temp; ++i) { tail += static_cast<char>(input.get()); }
    std::string bits;
    while ((temp = input.get()) != EOF) {
        std::bitset<8> buff(temp);
        bits += buff.to_string();
    }
    bits += tail;
    if (cur_root->left == nullptr && cur_root->right == nullptr) {
        for (int i = 0; i < bits.size(); ++i) { output << cur_root->symbol; }
    } else {
        for (char x : bits) {
            cur_root = (x == '1') ? cur_root->right : cur_root->left;
            if (cur_root->left == nullptr && cur_root->right == nullptr) {
                output << cur_root->symbol;
                cur_root = root;
            }
        }
    }
    delete_tree(root);
}