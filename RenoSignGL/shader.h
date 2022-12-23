#ifndef SHADER_H
#define SHADER_H
#include <GL/glew.h>        // GLEW library
//#include <GLFW/glfw3.h>     // GLFW library

class Shader {
public:

    // Declare Mesh create and destroy functions
    bool static CreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
    void static DestroyShaderProgram(GLuint programId);
};


#endif