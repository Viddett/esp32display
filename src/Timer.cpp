#include <Timer.hpp>
#include <esp32-hal-timer.h>

void Timer::start() {
    _startTime = millis();
    _isRunning = true;
}

void Timer::stop() {
    _isRunning = false;
    _startTime = -1;
}

void Timer::run() {
    if (!_isRunning) {
        start();
    }
}

int Timer::elapsedMs() {
    return _isRunning ? millis() - _startTime : -1;
}

int Timer::elapsed() {
    return _isRunning ? elapsedMs()/1000 : -1;
}

bool Timer::isElapsedMs(int ms) {
    if (_isRunning) {
        return _startTime + ms <= millis();
    }
    else {
        return false;
    }
}

bool Timer::isElapsed(int s) {
    return isElapsedMs(1000*s);
}

bool Timer::isRunning() {
    return _isRunning;
}