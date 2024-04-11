#include <fstream>
#include <iostream>

#include "postmachine.h"

int main(int argc, char** argv) {
    std::ifstream input(argv[2]);
    if (!input) {
        std::cerr << "File opening error" << std::endl;
        return -1;
    }
    std::ofstream output(argv[3]);
    size_t size = 0;
    int maxsteps = 0;
    std::string init;
    input >> size >> maxsteps >> init;
    PostMachine progr(size, argv[1]);
    std::string result = progr.calc(init, maxsteps);
    if (result == "Error" || result == "Not applicable") {
        output << result;
        return 0;
    }
    int idx = progr.idx();
    while (true) {
        --idx;
        if (idx == -1) { idx += static_cast<int>(result.size()); }
        if (result[idx] == '1') { break; }
    }
    for (int i = progr.idx();; ++i) {
        if (i == static_cast<int>(result.size())) { i = 0; }
        output << result[i];
        if (i == idx) { break; }
    }
    input.close();
    output.close();
    return 0;
}