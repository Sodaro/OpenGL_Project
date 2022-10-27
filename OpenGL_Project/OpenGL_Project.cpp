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
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
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

    //unsigned int vertex_shader, fragment_shader;
    //vertex_shader = get_compiled_shader("./test.vert", SHADER_TYPE::VERTEX);
    //fragment_shader = get_compiled_shader("./test.frag", SHADER_TYPE::FRAGMENT);

    //unsigned int shader_program;
    //shader_program = glCreateProgram();
    //glAttachShader(shader_program, vertex_shader);
    //glAttachShader(shader_program, fragment_shader);
    //glLinkProgram(shader_program);
    //if (!test_program(shader_program))
    //    return 1;

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

   

    //glDeleteShader(vertex_shader);
    //glDeleteShader(fragment_shader);


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
        float time_value = glfwGetTime();
        delta = time_value - prev_time;
        timer += delta;
        prev_time = time_value;


        //int vertex_color_location = glGetUniformLocation(shader_program, "ourColor"); //gets uniform variable
        test.use();
        
        //if (timer <= target_time)
        //{
        //    test.setVec4("color_a", a[0], a[1], a[2], a[3]);
        //    test.setVec4("color_b", b[0], b[1], b[2], b[3]);
        //    test.setFloat("lerp_value", timer / target_time);
        //}
        //else
        //{
        //    std::swap(a, b);
        //    timer = 0;
        //}
        glBindVertexArray(vertex_array_obj);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(vertex_array_obj);
    }



    return 0;
}