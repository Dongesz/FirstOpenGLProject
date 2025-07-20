#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// function to deal with after start resizeing of the window
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    // instantiate the GLFW window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window objet
    GLFWwindow *window = glfwCreateWindow(800, 600, "GlfwWindow", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to creat GLFW window!\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    // setting up rendering window size
    glViewport(0, 0, 800, 600);

    // framebuffer_size_callback function call
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // creating render loop
    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clean up all the resources
    glfwTerminate();
    return 0;
}
