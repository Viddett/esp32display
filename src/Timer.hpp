#ifndef TIMER_HPP
#define TIMER_HPP


class Timer {
    public:
    void start();
    void stop();
    void run();
    int elapsedMs();
    int elapsed();
    bool isElapsedMs(int ms);
    bool isElapsed(int s);
    bool isRunning();

    private:
    bool _isRunning {false};
    int _startTime {-1};
};



#endif