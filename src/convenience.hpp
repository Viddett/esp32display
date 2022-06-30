#ifndef CONVENIENCE_HPP
#define CONVENIENCE_HPP

inline int rangeLim(int num, int min, int max) {
    return num < min ? min : num > max ? max : num;
}

#endif