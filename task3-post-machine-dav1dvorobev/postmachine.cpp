#include "postmachine.h"

PostMachine::PostMachine(const size_t& size, const char* path) : _size(size), _progr(path), _idx(0) {
    _tape = new char[_size + 1];
    _tape[_size] = '\0';
    memset(_tape, '0', _size);
}
std::string PostMachine::calc(const std::string& init, int maxsteps) {
    strncpy(_tape, init.c_str(), init.size());
    int dummy[1024] = {0};
    int expect = 1024;
    while (!_progr.eof()) {
        int buf = 0;
        if (_progr >> buf) {
            expect = std::min(expect, buf);
            dummy[buf] = 1;
        } else {
            _progr.clear();
        }
        _progr.ignore(256, '\n');
    }
    _progr.seekg(0);

    while (--maxsteps > 0) {
        int buf = 0;
        if (_progr >> buf && buf == expect) {
            _progr.ignore(1);
            char command = '\0';
            _progr >> command >> expect;
            switch (command) {
                case 'V': _tape[_idx] = '1'; break;
                case 'X': _tape[_idx] = '0'; break;
                case 'L':
                    --_idx;
                    if (_idx == -1) { _idx += static_cast<int>(_size); }
                    break;
                case 'R':
                    ++_idx;
                    if (_idx == static_cast<int>(_size)) { _idx = 0; }
                    break;
                case '?':
                    if (_tape[_idx] == '1') { _progr >> expect; }
                    break;
            }
            if (command == '!') { break; }
        }
        _progr.clear();
        _progr.ignore(256, '\n');
        if (_progr.eof()) {
            _progr.seekg(0);
            if (dummy[expect] == 0) { return "Error"; }
        }
    }
    if (maxsteps == 0) { return "Not applicable"; }
    return std::string(_tape);
}