#ifndef TIMER_H
#define TIMER_H

#include <SDL2/SDL.h>  // Include SDL header for SDL functions

class Timer {
public:
    Timer();  // Constructor declaration
    void start();  // Start timer
    void stop();   // Stop timer
    void reset();  // Reset timer
    Uint32 getTicks() const;  // Get elapsed time
    bool isStarted() const;  // Check if the timer has started
    bool isStopped() const;  // Check if the timer has stopped

private:
    Uint32 startTicks;  // Time when the timer was started
    Uint32 pausedTicks;  // Time when the timer was paused
    bool started;  // Flag to check if the timer has started
    bool stopped;  // Flag to check if the timer has stopped
};

#endif