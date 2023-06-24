#ifndef LTYPE_UTILS_TIME_H
#define LTYPE_UTILS_TIME_H

#include <time.h>

namespace Utils {
    /**
     * A periodic timer
     * Initialize with period on construction and check if period passed with
     * isTriggered()
     */
    class Timer {
        double   _period_sec;
        timespec _last_time{};

    public:
        /// A timer with no period, use in combination with isTriggered(seconds)
        Timer();
        /// A timer with a period, use in combination with isTriggered()
        explicit Timer(double seconds);
        /**
         * Checks if the timer has been triggered
         * @param reset: sets wether or not we set the timer at 0 when it is
         * triggered
         * @return true if the timer was triggered else false
         */
        bool isTriggered(bool reset = true);
        /**
         * Checks if the timer has been triggered
         * @param seconds: number of seconds  since the last trigger
         * @param reset: sets wether or not we set the timer at 0 when it is
         * triggered
         * @return true if the timer was triggered else false
         */
        bool isTriggered(double seconds, bool reset = true);
        void reset();
    };

    /**
     * A stopwatch
     * Use start() and stop() to start and stop the stopwatch, then use
     * getElapsedSecs() to get the time that passed
     */
    class StopWatch {
        timespec _start;
        timespec _end;

    public:
        StopWatch() = default;
        /// Starts the stop watch
        void start();
        /// Record the time
        void stop();
        /// last call to stop() - last call to start()
        double getElapsedSecs();
    };
}

#endif