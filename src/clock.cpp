#include "clock.hpp"

void update_time(Clock& clock) {
    auto now = std::chrono::system_clock::now();
    clock.delta_time = now - clock.last_frame_time;
    clock.last_frame_time = now;
}
