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
#include <vector>
#include "Shader.h"
#include "Model.h"

const int forward_keys[2] = { GLFW_KEY_UP, GLFW_KEY_W };
const int back_keys[2] = { GLFW_KEY_DOWN, GLFW_KEY_S };
const int right_keys[2] = { GLFW_KEY_RIGHT, GLFW_KEY_D};
const int left_keys[2] = { GLFW_KEY_LEFT, GLFW_KEY_A };
const int view_width = 800;
const int view_height = 600;

struct Input
{
    float horizontal = 0;
    float vertical = 0;
};

void process_input(GLFWwindow* window, Input& input)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    input.vertical = (glfwGetKey(window, forward_keys[0]) | glfwGetKey(window, forward_keys[1])) - 
        (glfwGetKey(window, back_keys[0]) | glfwGetKey(window, back_keys[1]));
    input.horizontal = (glfwGetKey(window, right_keys[0]) | glfwGetKey(window, right_keys[1])) - 
        (glfwGetKey(window, left_keys[0]) | glfwGetKey(window, left_keys[1]));
}

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

    stbi_set_flip_vertically_on_load(true); //flip textures, y is inverted
    glEnable(GL_DEPTH_TEST);
    
    Shader monkey_shader("./lighting_sh.vert", "./lighting_sh.frag");
    Shader light_source_shader("./test.vert", "./light_source_sh.frag");
    Model our_model("./assets/monkey.obj");

    unsigned int VAO, VBO, EBO, light_source_VAO;
    glGenVertexArrays(1, &VAO);
    glGenVertexArrays(1, &light_source_VAO);
    glGenBuffers(1, &VBO); //generate a buffer ID
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindVertexArray(light_source_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); //binds buffer, any buffer calls will affect the currently bound buffer

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    monkey_shader.use();
    monkey_shader.set_vec3("objectColor", 1.0f, 1.0f, 1.0f);
    monkey_shader.set_vec3("lightColor", 1.0f, 1.0f, 1.0f);

    float time, previous_time, delta;
    time = previous_time = delta = 0;

    const glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, 0.0f); //look at world origin
    glm::vec3 camera_forward = glm::normalize(camera_pos - camera_target); //actually looks in opposite direction
    glm::vec3 camera_right = glm::normalize(glm::cross(world_up, camera_forward));
    glm::vec3 camera_up = glm::normalize(glm::cross(camera_right, camera_forward));

    Input input;
    const float rotation_speed = 45.f;
    const float movement_speed = 5.f;
    const float camera_pitch = 1.f;
    const float fov = 90;
    float camera_yaw = 0.f;
    
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    while (!glfwWindowShouldClose(window))
    {
        process_input(window, input);

        glClearColor(0.0f, 0.0f, 0.0f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        time = glfwGetTime();

        delta = time - previous_time;
        previous_time = time;

        
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f); 
        
        camera_pos += input.vertical * camera_forward * delta * movement_speed;
        camera_yaw += -input.horizontal * delta * rotation_speed;

        camera_forward = glm::normalize(glm::vec3(
            cos(glm::radians(camera_yaw)) * cos(glm::radians(camera_pitch)),
            sin(glm::radians(camera_pitch)),
            sin(glm::radians(camera_yaw)) * cos(glm::radians(camera_pitch))));
        camera_right = glm::normalize(glm::cross(world_up, camera_forward));
        camera_up = glm::normalize(glm::cross(camera_right, camera_forward));

        view = glm::lookAt(camera_pos, camera_pos + camera_forward, camera_up);
        projection = glm::perspective(glm::radians(fov), (float)view_width / (float)view_height, 0.1f, 100.0f);

        //monkey
        model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(180.f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(0,-1,0));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

        monkey_shader.use();
        monkey_shader.set_uniform_mat4fv("model", 1, false, model);
        monkey_shader.set_uniform_mat4fv("view", 1, false, view);
        monkey_shader.set_uniform_mat4fv("projection", 1, false, projection);
        monkey_shader.set_vec3("lightPos", lightPos);
        monkey_shader.set_vec3("viewPos", camera_pos);
        monkey_shader.set_float("time", time);
        our_model.Draw(monkey_shader);

        //lightsource
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube

        light_source_shader.use();
        light_source_shader.set_uniform_mat4fv("model", 1, false, model);
        light_source_shader.set_uniform_mat4fv("view", 1, false, view);
        light_source_shader.set_uniform_mat4fv("projection", 1, false, projection);
        our_model.Draw(light_source_shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}