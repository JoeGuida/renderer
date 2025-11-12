#ifndef GL_RENDERER_CLOCK_HPP
#define GL_RENDERER_CLOCK_HPP

#include <chrono>

using Period = std::chrono::system_clock::period;
using Rep = std::chrono::system_clock::rep;
using time_point = std::chrono::time_point<std::chrono::system_clock>;
using Duration = std::chrono::duration<Rep, Period>;

struct Clock {
    Duration delta_time;
    time_point last_frame_time;
};

void update_time(Clock& clock);

#endif
