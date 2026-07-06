#ifndef RENDERER_API_GL_LOADER_HPP
#define RENDERER_API_GL_LOADER_HPP

#include <expected>
#include <string>

#include "window/win32.hpp"

#include "opengl/glcorearb.h"
#include "opengl/wglext.h"

extern PFNGLATTACHSHADERPROC             glAttachShader;
extern PFNGLBINDBUFFERPROC               glBindBuffer;
extern PFNGLBINDBUFFERBASEPROC           glBindBufferBase;
extern PFNGLBUFFERDATAPROC               glBufferData;
extern PFNGLBUFFERSUBDATAPROC            glBufferSubData;
extern PFNGLBINDVERTEXARRAYPROC          glBindVertexArray;
extern PFNGLCLEARPROC                    glClear;
extern PFNGLCLEARCOLORPROC               glClearColor;
extern PFNGLCOMPILESHADERPROC            glCompileShader;
extern PFNGLCREATEPROGRAMPROC            glCreateProgram;
extern PFNGLCREATESHADERPROC             glCreateShader;
extern PFNGLDELETEPROGRAMPROC            glDeleteProgram;
extern PFNGLDELETESHADERPROC             glDeleteShader;
extern PFNGLDETACHSHADERPROC             glDetachShader;
extern PFNGLDRAWARRAYSINSTANCEDPROC      glDrawArraysInstanced;
extern PFNGLDRAWELEMENTSINSTANCEDPROC    glDrawElementsInstanced;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray;
extern PFNGLGENBUFFERSPROC               glGenBuffers;
extern PFNGLGENVERTEXARRAYSPROC          glGenVertexArrays;
extern PFNGLGETBUFFERPARAMETERIVPROC     glGetBufferParameteriv;
extern PFNGLGETPROGRAMIVPROC             glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC        glGetProgramInfoLog;
extern PFNGLGETSHADERIVPROC              glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC         glGetShaderInfoLog;
extern PFNGLGETUNIFORMLOCATIONPROC       glGetUniformLocation;
extern PFNGLLINKPROGRAMPROC              glLinkProgram;
extern PFNGLSHADERSOURCEPROC             glShaderSource;
extern PFNGLUNIFORM3FVPROC               glUniform3fv;
extern PFNGLUNIFORM1FPROC                glUniform1f;
extern PFNGLUNIFORM1UIPROC               glUniform1ui;
extern PFNGLUNIFORMMATRIX4FVPROC         glUniformMatrix4fv;
extern PFNGLUSEPROGRAMPROC               glUseProgram;
extern PFNGLVERTEXATTRIBPOINTERPROC      glVertexAttribPointer;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

inline bool loaded = false;

template <typename T>
auto load_gl_function(const char* name) {
    void* f = (void*)wglGetProcAddress(name);
    if(!f) { // fallback to opengl32.dll for core functions
        static HMODULE module = LoadLibraryA("opengl32.dll");
        f = (void*)GetProcAddress(module, name);
    }

    return reinterpret_cast<T>(f);
}

std::expected<void, std::string> load_gl_functions();

#endif

