#ifndef GL_RENDERER_CLOCK_HPP
#define GL_RENDERER_CLOCK_HPP

#include <chrono>

using time_point = std::chrono::time_point<std::chrono::system_clock>;

struct Clock {
    time_point delta_time;
    time_point last_frame_time;
};

#endif
