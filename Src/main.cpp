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

	uniform vec3 uColor;
	void main()
	{
	    FragColor = vec4(uColor, 1.0);
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

// Helper function to draw the faces of a cube
void drawFace(int startIndex,int colorLocation ,float r, float g, float b)
{
	glUniform3f(colorLocation, r, g, b);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(uintptr_t)(startIndex * sizeof(unsigned int)));
}

// Function to draw a cube with 6 drawFace calls
void drawCube(unsigned int shaderProgram)
{	
	int colorLocation = glGetUniformLocation(shaderProgram, "uColor");
	//bottom side
	drawFace(30, colorLocation,0.0f, 1.0f, 0.0f);
	//left side
	drawFace(18, colorLocation, 0.0f, 0.0f, 1.0f);
	//back side
	drawFace(6, colorLocation, 1.0f, 0.0f, 0.0f);
	
	//top side
	drawFace(24, colorLocation, 0.0f, 1.0f, 0.0f);
	//right side
	drawFace(12, colorLocation, 0.0f, 0.0f, 1.0f);
	//front side
	drawFace(0, colorLocation, 1.0f, 0.0f, 0.0f);
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
	     //front side
	     0.4f,  0.4f, 0.0f, // top right 
	     0.4f, -0.4f, 0.0f, // bottom righ
	    -0.4f, -0.4f, 0.0f, // bottom left
	    -0.4f,  0.4f, 0.0f,	// top left

	     //back side
	     0.4f+0.2f,  0.4f+0.2f, -0.4f, // top right
	     0.4f+0.2f, -0.4f+0.2f, -0.4f, // bottom right
	    -0.4f+0.2f, -0.4f+0.2f, -0.4f, // bottom left
	    -0.4f+0.2f,  0.4f+0.2f, -0.4f, // top left
					    
	     //right side
	     0.4f+0.2f, 0.4f+0.2f, -0.4f,  // top right
	     0.4f+0.2f, -0.4f+0.2f, -0.4f, // bottom right
	     0.4f, -0.4f, 0.0f,		   // bottom left
	     0.4f, 0.4f, 0.0f,		   // top left

	     //left side
	    -0.4f+0.2f,  0.4f+0.2f, -0.4f, // top right
	    -0.4f+0.2f, -0.4f+0.2f, -0.4f, // bottom right
	    -0.4f, -0.4f, 0.0f, 	   // bottom left
	    -0.4f,  0.4f, 0.0f,	       	   // top left

	     //top side
	     0.4f+0.2f, 0.4f+0.2f, -0.4f, // top right
	     0.4f, 0.4f, 0.0f,		  // bottom right
	    -0.4f, 0.4f , 0.0f,		  // bottom left
	    -0.4f+0.2f, 0.4+0.2f, -0.4f,  // top left

	     //bottom right
	     0.4f+0.2f, -0.4f+0.2f, -0.4f, // top right
	     0.4f, -0.4, 0.0f,       	   // bottom right
	    -0.4f, -0.4f, 0.0f,		   // bottom left
	    -0.4+0.2, -0.4+0.2, -0.4	   // top left

    };
    unsigned int indicies[] = {
	    //front side
	    0,1,3, 
	    1,2,3,
	    //back side
	    4,5,7,
	    5,6,7,
 	    //right side
	    8,9,11,
	    9,10,11,
	    //left side
	    12,13,15,
	    13,14,15,
	    //top side
	    16,17,19,
	    17,18,19,
	    //bottom side
	    20,21,23,
	    21,22,23
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

    //Loading data into EBO
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
	int colorLocation = glGetUniformLocation(shaderProgram, "uColor");

	// Starting VAO 
	glBindVertexArray(VAO);

	// Drawing the cube
	drawCube(shaderProgram);

        // Check and call events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up all the resources
    glfwTerminate();
    return 0;
}
