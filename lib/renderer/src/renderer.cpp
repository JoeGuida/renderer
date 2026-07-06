#include "renderer/core/renderer.hpp"

#include "renderer/api/gl_loader.hpp"

#include <array>

std::expected<void, std::string> initialize_opengl(PlatformWindow* handle) {
    PIXELFORMATDESCRIPTOR pfd {
        .nSize = sizeof(PIXELFORMATDESCRIPTOR),
        .nVersion = 1,
        .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        .iPixelType = PFD_TYPE_RGBA,
        .cColorBits = 32,
        .cDepthBits = 24,
        .cStencilBits = 8,
        .iLayerType = PFD_MAIN_PLANE
    };

    int pixel_format = ChoosePixelFormat(handle->hdc, &pfd);
    if(!pixel_format) {
        return std::unexpected("error choosing pixel format!");
    }
    if(!SetPixelFormat(handle->hdc, pixel_format, &pfd)) {
        return std::unexpected("error setting pixel format!");
    }

    std::array<int, 7> attribs = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 6,
        WGL_CONTEXT_PROFILE_MASK_ARB,
        WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    HGLRC temp_context = wglCreateContext(handle->hdc);
    if(!temp_context) {
        return std::unexpected("failed to create temporary gl context");
    }

    wglMakeCurrent(handle->hdc, temp_context);
    wglCreateContextAttribsARB = load_gl_function<PFNWGLCREATECONTEXTATTRIBSARBPROC>("wglCreateContextAttribsARB");
    if(!wglCreateContextAttribsARB) {
        return std::unexpected("failed to load gl function: wglCreateContextAttribsARB");
    }

    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(temp_context);

    HGLRC hglrc = wglCreateContextAttribsARB(handle->hdc, nullptr, attribs.data());
    if(!hglrc) {
        return std::unexpected("error creating gl context");
    }

    if(!wglMakeCurrent(handle->hdc, hglrc)) {
        return std::unexpected("failed to make gl context current");
    }

    auto functions = load_gl_functions();
    if(!loaded) {
        return std::unexpected(functions.error());
    }

    handle->hglrc = hglrc;
    return {};
}

