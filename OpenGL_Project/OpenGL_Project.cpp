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
#include <map>
#include "Shader.h"

const int forward_keys[2] = { GLFW_KEY_UP, GLFW_KEY_W };
const int back_keys[2] = { GLFW_KEY_DOWN, GLFW_KEY_S };
const int right_keys[2] = { GLFW_KEY_RIGHT, GLFW_KEY_D};
const int left_keys[2] = { GLFW_KEY_LEFT, GLFW_KEY_A };

struct Input
{
    float horizontal = 0;
    float vertical = 0;
};

void processInput(GLFWwindow* window, Input& input);

float mix_value = 0.2f;

unsigned int create_texture(const char* path, bool clamp_to_edge, GLint internal_format, GLenum format)
{
    unsigned int texture;
    GLenum param = clamp_to_edge ? GL_CLAMP_TO_EDGE : GL_REPEAT;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); //subsequent texture commands will affect bound texture

    //wrapping params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);

    //filtering params
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
    int view_width = 800;
    int view_height = 600;
    float vertices[] = {
         // positions          // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
    };


    unsigned int rect_indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    float cube_vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  -0.0f,  0.0f),
        glm::vec3(2.0f,  -5.0f, -15.0f),
        glm::vec3(-1.5f, -7.2f, -2.5f),
        glm::vec3(-3.8f, -7.0f, -12.3f),
        glm::vec3(2.4f, -8.4f, -3.5f),
        glm::vec3(-1.7f,  -3.0f, -7.5f),
        glm::vec3(1.3f, -9.0f, -2.5f),
        glm::vec3(1.5f,  -2.0f, -2.5f),
        glm::vec3(1.5f,  -0.2f, -1.5f),
        glm::vec3(-1.3f,  -1.0f, -1.5f)
    };

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(view_width, view_height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewInit();

    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);

    Shader texture_shader("./texture_sh.vert", "./texture_sh.frag");

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); //generate a buffer ID
    glGenBuffers(1, &EBO); //generate a buffer ID

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); //binds buffer, any buffer calls will affect the currently bound buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rect_indices), rect_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int texture1, texture2;
    texture1 = create_texture("assets/container.jpg", true, GL_RGB, GL_RGB);
    texture2 = create_texture("assets/bloodtrail.png", false, GL_RGBA, GL_RGBA);

    texture_shader.use();
    texture_shader.setInt("texture1", 0);
    texture_shader.setInt("texture2", 1);

    float time, previous_time, delta;
    time = previous_time = delta = 0;

    float rotation = 0.f;

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f); //look at world origin

    glm::vec3 cameraForward = glm::normalize(cameraPos - cameraTarget); //actually looks in opposite direction
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraForward));
    glm::vec3 cameraUp = glm::normalize(glm::cross(cameraRight, cameraForward));

    Input input;

    const float camera_pitch = 1.f;
    float camera_yaw = 0.f;
    const float rotation_speed = 45.f;
    const float movement_speed = 5.f;

    while (!glfwWindowShouldClose(window))
    {
        processInput(window, input);

        glClearColor(0.0f, 0.0f, 0.0f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        time = glfwGetTime();
        delta = time - previous_time;
        previous_time = time;

        float fov = 90;
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        
        cameraPos += input.vertical * cameraForward * delta * movement_speed;
        camera_yaw += -input.horizontal * delta * rotation_speed;

        cameraForward = glm::normalize(glm::vec3(
            cos(glm::radians(camera_yaw)) * cos(glm::radians(camera_pitch)),
            sin(glm::radians(camera_pitch)),
            sin(glm::radians(camera_yaw)) * cos(glm::radians(camera_pitch))));

        cameraRight = glm::normalize(glm::cross(up, cameraForward));
        cameraUp = glm::normalize(glm::cross(cameraRight, cameraForward));
        //cameraPos += input.horizontal * cameraRight * delta * 10.f;

        view = glm::lookAt(cameraPos, cameraPos + cameraForward, cameraUp);
        projection = glm::perspective(glm::radians(fov), (float)view_width / (float)view_height, 0.1f, 100.0f);

        texture_shader.use();
        texture_shader.setFloat("mixValue", mix_value);
        //texture_shader.setUniformMat4fv("model", 1, false, model);
        texture_shader.setUniformMat4fv("view", 1, false, view);
        texture_shader.setUniformMat4fv("projection", 1, false, projection);

        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            texture_shader.setUniformMat4fv("model", 1, false, model);

            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}

const auto key = glfwGetKey;
void processInput(GLFWwindow* window, Input& input)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    input.vertical = (key(window, forward_keys[0]) | key(window, forward_keys[1])) - (key(window, back_keys[0]) | key(window, back_keys[1]));
    input.horizontal = (key(window, right_keys[0]) | key(window, right_keys[1])) - (key(window, left_keys[0]) | key(window, left_keys[1]));
}
