#ifndef POSTMACHINE
#define POSTMACHINE

#include <cstring>
#include <fstream>

class PostMachine {
    char* _tape;
    const size_t _size;
    std::ifstream _progr;
    int _idx;
public:
    PostMachine(const size_t& size, const char* path);
    std::string calc(const std::string& init, int maxsteps);
    const int& idx() const { return _idx; }
    ~PostMachine() {
        delete[] _tape;
        _progr.close();
    }
};
#endif // POSTMACHINE