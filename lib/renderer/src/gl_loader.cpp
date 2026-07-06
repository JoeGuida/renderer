#include "renderer/api/gl_loader.hpp"

PFNGLATTACHSHADERPROC             glAttachShader             = nullptr;
PFNGLBINDBUFFERPROC               glBindBuffer               = nullptr;
PFNGLBINDBUFFERBASEPROC           glBindBufferBase           = nullptr;
PFNGLBINDVERTEXARRAYPROC          glBindVertexArray          = nullptr;
PFNGLBUFFERDATAPROC               glBufferData               = nullptr;
PFNGLBUFFERSUBDATAPROC            glBufferSubData            = nullptr;
PFNGLCLEARPROC                    glClear                    = nullptr;
PFNGLCLEARCOLORPROC               glClearColor               = nullptr;
PFNGLCOMPILESHADERPROC            glCompileShader            = nullptr;
PFNGLCREATEPROGRAMPROC            glCreateProgram            = nullptr;
PFNGLCREATESHADERPROC             glCreateShader             = nullptr;
PFNGLDELETEPROGRAMPROC            glDeleteProgram            = nullptr;
PFNGLDELETESHADERPROC             glDeleteShader             = nullptr;
PFNGLDETACHSHADERPROC             glDetachShader             = nullptr;
PFNGLDRAWARRAYSINSTANCEDPROC      glDrawArraysInstanced      = nullptr;
PFNGLDRAWELEMENTSINSTANCEDPROC    glDrawElementsInstanced    = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray  = nullptr;
PFNGLGENBUFFERSPROC               glGenBuffers               = nullptr;
PFNGLGENVERTEXARRAYSPROC          glGenVertexArrays          = nullptr;
PFNGLGETBUFFERPARAMETERIVPROC     glGetBufferParameteriv     = nullptr;
PFNGLGETPROGRAMIVPROC             glGetProgramiv             = nullptr;
PFNGLGETPROGRAMINFOLOGPROC        glGetProgramInfoLog        = nullptr;
PFNGLGETSHADERIVPROC              glGetShaderiv              = nullptr;
PFNGLGETSHADERINFOLOGPROC         glGetShaderInfoLog         = nullptr;
PFNGLGETUNIFORMLOCATIONPROC       glGetUniformLocation       = nullptr;
PFNGLLINKPROGRAMPROC              glLinkProgram              = nullptr;
PFNGLSHADERSOURCEPROC             glShaderSource             = nullptr;
PFNGLUNIFORM1FPROC                glUniform1f                = nullptr;
PFNGLUNIFORM1UIPROC               glUniform1ui               = nullptr;
PFNGLUNIFORM3FVPROC               glUniform3fv               = nullptr;
PFNGLUNIFORMMATRIX4FVPROC         glUniformMatrix4fv         = nullptr;
PFNGLUSEPROGRAMPROC               glUseProgram               = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC      glVertexAttribPointer      = nullptr;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;

std::expected<void, std::string> load_gl_functions() {
    if(loaded) { return {}; }

    glAttachShader = load_gl_function<PFNGLATTACHSHADERPROC>("glAttachShader");
    if(!glAttachShader) {
        return std::unexpected("error loading function: glAttachShader");
    }

    glBindBuffer = load_gl_function<PFNGLBINDBUFFERPROC>("glBindBuffer");
    if(!glBindBuffer) {
        return std::unexpected("error loading function: glBindBuffer");
    }

    glBindBufferBase = load_gl_function<PFNGLBINDBUFFERBASEPROC>("glBindBufferBase");
    if(!glBindBufferBase) {
        return std::unexpected("error loading function: glBindBufferBase");
    }

    glBindVertexArray = load_gl_function<PFNGLBINDVERTEXARRAYPROC>("glBindVertexArray");
    if(!glBindVertexArray) {
        return std::unexpected("error loading function: glBindVertexArray");
    }

    glBufferData = load_gl_function<PFNGLBUFFERDATAPROC>("glBufferData");
    if(!glBufferData) {
        return std::unexpected("error loading function: glBufferData");
    }

    glBufferSubData = load_gl_function<PFNGLBUFFERSUBDATAPROC>("glBufferSubData");
    if(!glBufferSubData) {
        return std::unexpected("error loading function: glBufferSubData");
    }

    glClear = load_gl_function<PFNGLCLEARPROC>("glClear");
    if(!glClear) {
        return std::unexpected("error loading function: glClear");
    }

    glClearColor = load_gl_function<PFNGLCLEARCOLORPROC>("glClearColor");
    if(!glClearColor) {
        return std::unexpected("error loading function: glClearColor");
    }

    glCompileShader = load_gl_function<PFNGLCOMPILESHADERPROC>("glCompileShader");
    if(!glCompileShader) {
        return std::unexpected("error loading function: glCompileShader");
    }

    glCreateProgram = load_gl_function<PFNGLCREATEPROGRAMPROC>("glCreateProgram");
    if(!glCreateProgram) {
        return std::unexpected("error loading function: glCreateProgram");
    }

    glCreateShader = load_gl_function<PFNGLCREATESHADERPROC>("glCreateShader");
    if(!glCreateShader) {
        return std::unexpected("error loading function: glCreateShader");
    }

    glDeleteProgram = load_gl_function<PFNGLDELETEPROGRAMPROC>("glDeleteProgram");
    if(!glDeleteProgram) {
        return std::unexpected("error loading function: glDeleteProgram");
    }

    glDeleteShader = load_gl_function<PFNGLDELETESHADERPROC>("glDeleteShader");
    if(!glDeleteShader) {
        return std::unexpected("error loading function: glDeleteShader");
    }

    glDetachShader = load_gl_function<PFNGLDETACHSHADERPROC>("glDetachShader");
    if(!glDetachShader) {
        return std::unexpected("error loading function: glDetachShader");
    }

    glDrawArraysInstanced = load_gl_function<PFNGLDRAWARRAYSINSTANCEDPROC>("glDrawArraysInstanced");
    if(!glDrawArraysInstanced) {
        return std::unexpected("error loading function: glDrawArraysInstanced");
    }

    glDrawElementsInstanced = load_gl_function<PFNGLDRAWELEMENTSINSTANCEDPROC>("glDrawElementsInstanced");
    if(!glDrawElementsInstanced) {
        return std::unexpected("error loading function: glDrawElementsInstanced");
    }

    glEnableVertexAttribArray = load_gl_function<PFNGLENABLEVERTEXATTRIBARRAYPROC>("glEnableVertexAttribArray");
    if(!glEnableVertexAttribArray) {
        return std::unexpected("error loading function: glEnableVertexAttribArray");
    }

    glGenBuffers = load_gl_function<PFNGLGENBUFFERSPROC>("glGenBuffers");
    if(!glGenBuffers) {
        return std::unexpected("error loading function: glGenBuffers");
    }

    glGenVertexArrays = load_gl_function<PFNGLGENVERTEXARRAYSPROC>("glGenVertexArrays");
    if(!glGenVertexArrays) {
        return std::unexpected("error loading function: glGenVertexArrays");
    }

    glGetProgramiv = load_gl_function<PFNGLGETPROGRAMIVPROC>("glGetProgramiv");
    if(!glGetProgramiv) {
        return std::unexpected("error loading function: glGetProgramiv");
    }

    glGetBufferParameteriv = load_gl_function<PFNGLGETBUFFERPARAMETERIVPROC>("glGetBufferParameteriv");
    if(!glGetBufferParameteriv) {
        return std::unexpected("error loading function: glGetBufferParameteriv");
    }

    glGetProgramInfoLog = load_gl_function<PFNGLGETPROGRAMINFOLOGPROC>("glGetProgramInfoLog");
    if(!glGetProgramInfoLog) {
        return std::unexpected("error loading function: glGetProgramInfoLog");
    }

    glGetShaderiv = load_gl_function<PFNGLGETSHADERIVPROC>("glGetShaderiv");
    if(!glGetShaderiv) {
        return std::unexpected("error loading function: glGetShaderiv");
    }

    glGetShaderInfoLog = load_gl_function<PFNGLGETSHADERINFOLOGPROC>("glGetShaderInfoLog");
    if(!glGetShaderInfoLog) {
        return std::unexpected("error loading function: glGetShaderInfoLog");
    }

    glGetUniformLocation = load_gl_function<PFNGLGETUNIFORMLOCATIONPROC>("glGetUniformLocation");
    if(!glGetUniformLocation) {
        return std::unexpected("error loading function: glGetUniformLocation");
    }

    glLinkProgram = load_gl_function<PFNGLLINKPROGRAMPROC>("glLinkProgram");
    if(!glLinkProgram) {
        return std::unexpected("error loading function: glLinkProgram");
    }

    glShaderSource = load_gl_function<PFNGLSHADERSOURCEPROC>("glShaderSource");
    if(!glShaderSource) {
        return std::unexpected("error loading function: glShaderSource");
    }

    glUniform1f = load_gl_function<PFNGLUNIFORM1FPROC>("glUniform1f");
    if(!glUniform1f) {
        return std::unexpected("error loading function: glUniform1f");
    }

    glUniform1ui = load_gl_function<PFNGLUNIFORM1UIPROC>("glUniform1ui");
    if(!glUniform1ui) {
        return std::unexpected("error loading function: glUniform1ui");
    }

    glUniform3fv = load_gl_function<PFNGLUNIFORM3FVPROC>("glUniform3fv");
    if(!glUniform3fv) {
        return std::unexpected("error loading function: glUniform3fv");
    }

    glUniformMatrix4fv = load_gl_function<PFNGLUNIFORMMATRIX4FVPROC>("glUniformMatrix4fv");
    if(!glUniformMatrix4fv) {
        return std::unexpected("error loading function: glUniformMatrix4fv");
    }

    glUseProgram = load_gl_function<PFNGLUSEPROGRAMPROC>("glUseProgram");
    if(!glUseProgram) {
        return std::unexpected("error loading function: glUseProgram");
    }

    glVertexAttribPointer = load_gl_function<PFNGLVERTEXATTRIBPOINTERPROC>("glVertexAttribPointer");
    if(!glVertexAttribPointer) {
        return std::unexpected("error loading function: glVertexAttribPointer");
    }

    loaded = true;
    return {};
}
