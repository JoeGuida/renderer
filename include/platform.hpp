#ifndef RENDERER_PLATFORM_HPP
#define RENDERER_PLATFORM_HPP

#if defined(_WIN64) || defined(_WIN32)
    #include <platform/win32.hpp>
#elif defined(__linux__)
    #include <platform/linux.hpp>
#endif

#endif
