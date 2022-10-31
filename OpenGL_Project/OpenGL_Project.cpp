#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <iostream>
#include <string>
#include "Shader.h"

//enum SHADER_TYPE { FRAGMENT = GL_FRAGMENT_SHADER, VERTEX = GL_VERTEX_SHADER};

//
//std::string read_file(const char* filepath)
//{
//    // r: readonly; t: text file
//    FILE* file;
//    errno_t err;
//
//    if ((err = fopen_s(&file, filepath, "rt")) != 0)
//    {
//        return nullptr;
//    }
//
//    // go to end of file
//    fseek(file, 0, SEEK_END);
//
//    // length of file in bytes
//    unsigned long length = ftell(file);
//    char* data = new char[length + 1];
//
//    // set everything to 0
//    memset(data, 0, length + 1);
//
//    // go back to beginning of file
//    fseek(file, 0, SEEK_SET);
//
//    fread(data, 1, length, file);
//    fclose(file);
//
//    // copy data to string
//    std::string result(data);
//    delete[] data;
//    return result;
//}

int main()
{
    float rect_vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int rect_indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    float triangle_vertices[] = {
        // positions         // colors
         0.25f, -0.25f, 0.0f,  0.0f, 1.0f, 1.0f,   // bottom right
        -0.25f, -0.25f, 0.0f,  1.0f, 0.0, 1.0f,   // bottom left
         0.0f,  0.25f, 0.0f,  1.0f, 1.0f, 0.0f    // top 
    };

    unsigned int triangle_indices[] = {  // note that we start from 0!
        0, 1, 2,   // first triangle
    };

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewInit();

    Shader test("./test.vert", "./test.frag");

    unsigned int vertex_array_obj, vertex_buffer_obj, element_buffer_obj;
    glGenBuffers(1, &vertex_buffer_obj); //generate a buffer ID
    glGenBuffers(1, &element_buffer_obj); //generate a buffer ID
    glGenVertexArrays(1, &vertex_array_obj);

    glBindVertexArray(vertex_array_obj);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj); //binds buffer, any buffer calls will affect the currently bound buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangle_indices), triangle_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(vertex_array_obj);

    float target_time = 2;
    float timer = 0;
    float delta = 0;
    float prev_time = 0;
    float a[4] = { 1,0,1,1 };
    float b[4] = { 0,1,1,1 };

    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 0.0f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float time_value = glfwGetTime();
        delta = time_value - prev_time;
        timer += delta;
        prev_time = time_value;

        test.setFloat("xOffset", cos(time_value) * 0.5);
        test.setFloat("yOffset", sin(time_value) * 0.5);
        test.setFloat("sizeMod", (sin(time_value) + 2)*0.5);
        test.use();

        glBindVertexArray(vertex_array_obj);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(vertex_array_obj);
    }
    return 0;
}