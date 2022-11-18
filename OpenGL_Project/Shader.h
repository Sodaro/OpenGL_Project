#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glm/glm.hpp"

class Shader
{
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    // use/activate the shader
    void use();
    // utility uniform functions
    void set_bool(const std::string& name, bool value) const;
    void set_int(const std::string& name, int value) const;
    void set_float(const std::string& name, float value) const;
    void set_vec4(const std::string& name, float x, float y, float z, float w) const;
    void set_vec3(const std::string& name, float x, float y, float z) const;
    void set_vec3(const std::string& name, const glm::vec3& vec) const;
    void set_uniform_mat4fv(const std::string& name, int count, bool transpose, const glm::mat4& value) const;
};
