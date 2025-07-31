#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Vertex shader source
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

// Fragment shader source    
const char *fragmentShaderSource = "#version 330 core\n"
         "out vec4 FragColor;\n"
         "void main()\n"
         "{\n"
         "FragColor = vec4(0.3f, 0.3f, 0.9f, 1.0f);\n"
         "}\0";

// fuction to keep input alogic organized
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Function to deal with after start resizeing of the window
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    // Instantiate the GLFW window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window objet
    GLFWwindow *window = glfwCreateWindow(800, 600, "GlfwWindow", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to creat GLFW window!\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    int success;
    char infoLog[512];

    // framebuffer_size_callback function call
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // Creating and compiling the vertexShader from the source code
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    
    // Error loging vertexShader
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
	    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
	    std::cout<<"ERROR: VertexShader compilation failed!\n" << infoLog <<std::endl;
    }
   
    // Creating and compiling the fragmentShader from the source code
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Error loging fragmentShader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
	glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
	std::cout<< "ERROR: FragmentShader compilation failed!\n" << infoLog << std::endl;
    }

    // Creating shaderProgram
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    
    // Linking shaders to shaderProgram
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Error loging shaderProgram
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
	glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	std::cout<< "ERROR: Linking Shaders into ShaderProgram failed!\n" << std::endl;
    }

    // Running shaderProgram and deleting the now unnecessery shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
   
    // Creating verticies array with cordinates for three points
    float verticies[] ={
	    -0.5f, -0.5f, 0.0f, // point one
	     0.5f, -0.5f, 0.5f, // point two
	     0.0f,  0.5f, 0.0f  // point three
    };

    // Creating VertexArrayObject and VertexBufferObject variables
    unsigned int VAO, VBO;

    // giving VBO, VAO variables the right functionality
    glGenVertexArrays(1, &VAO);       
    glGenBuffers(1, &VBO);

    // basically running/starting VertexArrayObject
    glBindVertexArray(VAO);

    // Loading our data(verticies) into VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

    // Telling openGL how to use these raw verticies
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // creating render loop
    while (!glfwWindowShouldClose(window))
    {
        // input calls
        processInput(window);

        // render background color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

	// Running shaderProgram
	glUseProgram(shaderProgram);

	// Starting VAO 
	glBindVertexArray(VAO);

	//Drawing the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

        // check and call events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clean up all the resources
    glfwTerminate();
    return 0;
}
