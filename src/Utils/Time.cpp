#include "Time.hpp"

Utils::Timer::Timer(): Timer(0) {}

Utils::Timer::Timer(double seconds) : _period_sec(seconds) {
    clock_gettime(CLOCK_MONOTONIC, &_last_time);
}

bool Utils::Timer::isTriggered(bool reset) {
    return isTriggered(_period_sec, reset);
}

bool Utils::Timer::isTriggered(double seconds, bool reset) {
    timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);

    double delta_time =
               static_cast<double>(current_time.tv_sec - _last_time.tv_sec) +
               static_cast<double>(current_time.tv_nsec - _last_time.tv_nsec) /
               1000000000.0;

    bool     triggered;
    if (delta_time >= seconds) {
        if(reset) this->reset();
        triggered = true;
    } else {
        triggered = false;
    }

    return triggered;
}

void Utils::Timer::reset() {
    clock_gettime(CLOCK_MONOTONIC, &_last_time);
}

void Utils::StopWatch::start() {
    clock_gettime(CLOCK_MONOTONIC, &_start);
}

void Utils::StopWatch::stop() {
    clock_gettime(CLOCK_MONOTONIC, &_end);
}

double Utils::StopWatch::getElapsedSecs() {
    double elapsed_time =
               static_cast<double>(_end.tv_sec - _start.tv_sec) +
               static_cast<double>(_end.tv_nsec - _start.tv_nsec) /
               1000000000.0;
    return elapsed_time;
}
