#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <iostream>
#include <string>
#include "Shader.h"

void processInput(GLFWwindow* window);

float mix_value = 0.2f;

unsigned int create_texture(const char* path, bool clamp_to_edge, GLint internal_format, GLenum format)
{
    unsigned int texture;
    GLenum param = clamp_to_edge ? GL_CLAMP_TO_EDGE : GL_REPEAT;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); //subsequent texture commands will affect bound texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int width, height, nr_channels;
    unsigned char* pixel_data = stbi_load(path, &width, &height, &nr_channels, 0);
    if (pixel_data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, pixel_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "failed to load texture data" << std::endl;
        return 1;
    }
    stbi_image_free(pixel_data);
    return texture;
}

int main()
{
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f    // top left 
    };
    unsigned int rect_indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
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

    stbi_set_flip_vertically_on_load(true);
    Shader texture_shader("./texture_sh.vert", "./texture_sh.frag");

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); //generate a buffer ID
    glGenBuffers(1, &EBO); //generate a buffer ID

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); //binds buffer, any buffer calls will affect the currently bound buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rect_indices), rect_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int texture1, texture2;

    texture1 = create_texture("assets/container.jpg", true, GL_RGB, GL_RGB);
    texture2 = create_texture("assets/bloodtrail.png", false, GL_RGBA, GL_RGBA);

    texture_shader.use();
    texture_shader.setInt("texture1", 0);
    texture_shader.setInt("texture2", 1);

    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = trans * vec;
    std::cout << vec.x << " " << vec.y << vec.z << std::endl;

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        texture_shader.setFloat("mixValue", mix_value);
        texture_shader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(VAO);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mix_value += 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if (mix_value >= 1.0f)
            mix_value = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mix_value -= 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if (mix_value <= 0.0f)
            mix_value = 0.0f;
    }
}
