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
const char* fragmentShaderSource = R"(
	#version 330 core
	out vec4 FragColor;
	in vec2 TexCoords;

	// Egyszerű procedurális fa mintázat UV alapján
	void main()
	{
	    float x = TexCoords.x * 10.0;
	    float y = TexCoords.y * 10.0;
	
	    float ring = sin(3.14159 * sqrt(x * x + y * y));
	    float grain = fract(x + y);
	
	    float brightness = 0.3 + 0.3 * ring + 0.4 * grain;
	
	    vec3 woodColor = vec3(0.4, 0.2, 0.05) * brightness;
	
	    FragColor = vec4(woodColor, 1.0);
	}	
	)";

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
   
    float verticies[] ={
	     0.5f,  0.5f, 0.0f, // top right
	     0.5f, -0.5f, 0.0f, // bottom right
	    -0.5f, -0.5f, 0.0f, // bottom left
	    -0.5f,  0.5f, 0.0f	// top left
    };
    unsigned int indicies[] = {

	    0,1,3,
	    1,2,3
    };


    // Creating VertexArrayObject and VertexBufferObject variables
    unsigned int VAO, VBO, EBO;

    // Giving VBO, VAO variables the right functionality
    glGenVertexArrays(1, &VAO);       
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Binfing VAO
    glBindVertexArray(VAO);

    // Loading our data(verticies) into VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

    // Telling openGL how to use these raw verticies
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Creating render loop
    while (!glfwWindowShouldClose(window))
    {
        // Input calls
        processInput(window);

        // Render background color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

	// Running shaderProgram
	glUseProgram(shaderProgram);

	// Starting VAO 
	glBindVertexArray(VAO);

	// Drawing the triangle
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Check and call events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up all the resources
    glfwTerminate();
    return 0;
}
