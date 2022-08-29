#ifndef LOG_HPP
#define LOG_HPP

#include <string.h>

class Log {
public:
    Log() {}
    ~Log() {
        Serial.println();
    };
    template <class T>
    Log& operator << (const T &print) {
        Serial.print(print);
        Serial.print(" ");
        return *this;
    }
private:

};

#endif