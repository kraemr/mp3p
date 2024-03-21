#pragma once
#include <GL/glew.h>
namespace shaders{
    int compiledStatus(GLint shaderID);
    GLint makeVertexShader (const char* shaderSource);
    GLuint makeFragmentShader (const char* shaderSource);
    bool linkedStatus(GLint programID);
    GLint makeShaderProgram (GLint vertexShaderID, GLint fragmentShaderID);
    GLint linkShaderProgram(GLint programID);
}