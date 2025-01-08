#include "timer.h"  // Include the header for the Timer class
#include <SDL2/SDL.h>  // Include SDL header for SDL functions

// Constructor definition
Timer::Timer() : startTicks(0), pausedTicks(0), started(false), stopped(false) {}

// Start the timer
void Timer::start() {
    started = true;
    stopped = false;
    startTicks = SDL_GetTicks();
    pausedTicks = 0;
}

// Stop the timer
void Timer::stop() {
    if (started && !stopped) {
        stopped = true;
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

// Reset the timer
void Timer::reset() {
    started = false;
    stopped = false;
    startTicks = 0;
    pausedTicks = 0;
}

// Get the time elapsed
Uint32 Timer::getTicks() const {
    Uint32 time = 0;

    if (started) {
        if (stopped) {
            time = pausedTicks;
        } else {
            time = SDL_GetTicks() - startTicks;
        }
    }

    return time;
}

// Check if the timer has started
bool Timer::isStarted() const {
    return started;
}

// Check if the timer is stopped
bool Timer::isStopped() const {
    return stopped;
}
