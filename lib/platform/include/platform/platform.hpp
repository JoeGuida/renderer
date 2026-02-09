#ifndef PLATFORM_PLATFORM_HPP
#define PLATFORM_PLATFORM_HPP

#ifdef WINDOWS
    #include <platform/windows/win32.hpp>
#elifdef APPLE
    #include <platform/mac/mac.hpp>
#endif

#endif
