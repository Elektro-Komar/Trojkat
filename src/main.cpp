#include <iostream>
using namespace std;
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
// Window dimensions
const GLuint WIDTH = 1920, HEIGHT = 1280;

// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"uniform vec3 Pozycja;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x + Pozycja.x, position.y + Pozycja.y, position.z, 1.0);\n"
"}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"uniform vec4 TrianCol;\n"
"void main()\n"
"{\n"
"color = TrianCol;\n"
"}\n\0";
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
// The MAIN function, from here we start the application and run the game loop
GLfloat locationx, locationy, locationz;
GLuint VBO, VAO, VBOQ, VAOQ, VBOS, VAOS, VBOS2, VAOS2;
GLuint shaderProgram;
int color = 1;
bool Quad = false, Star = false;
int TriangleColor;
#define SHORTCUT(a, b) glfwGetKey(window, a) == GLFW_PRESS && glfwGetKey(window, b) == GLFW_PRESS
#define KEY_CLICKED(key) glfwGetKey(window, key) == GLFW_PRESS && glfwGetKey(window, key) != GLFW_REPEAT && glfwGetKey(window, key) != GLFW_RELEASE
#define MOUSE_BUTTON_CLICKED(key) glfwGetMouseButton(window, key) == GLFW_PRESS && glfwGetMouseButton(window, key) != GLFW_RELEASE && glfwGetMouseButton(window, key) != GLFW_REPEAT



int main()
{
    // Init GLFW
    glfwInit();

    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Trojkat", glfwGetPrimaryMonitor(), nullptr);
   
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, screenWidth, screenHeight);


    // Build and compile our shader program
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] =
    {
        -0.5f, -0.5f, 0.0f, // Left
        0.5f, -0.5f, 0.0f, // Right
        0.0f,  0.5f, 0.0f  // Top
    };
    GLfloat quadvertices[] = {
                // first triangle
        0.5f, 0.5f, 0.0f, // top right
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, 0.5f, 0.0f, // top left
        // second triangle
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f // top left
    };
    GLfloat starvertices[] = {
        0.5f, 0.25f, 0.0f,
        0.0f, -0.75f, 0.0f,
        -0.5f, 0.25f, 0.0f,
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)





    //Quad
    glGenVertexArrays(1, &VAOQ);
    glGenBuffers(1, &VBOQ);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAOQ);

    glBindBuffer(GL_ARRAY_BUFFER, VBOQ);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadvertices), quadvertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


    //Star
    glGenVertexArrays(1, &VAOS);
    glGenBuffers(1, &VBOS);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAOS);

    glBindBuffer(GL_ARRAY_BUFFER, VBOS);
    glBufferData(GL_ARRAY_BUFFER, sizeof(starvertices), starvertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


    glfwSetKeyCallback(window, KeyCallback);
    // Game loop
    int width, height;

    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw our first triangle
        glUseProgram(shaderProgram);

        //Zmienianie Pozycji strzałkami: UP, DOWN, LEFT, RIGHT
        bool isTri = true;
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        int vertexLocation = glGetUniformLocation(shaderProgram, "Pozycja");
        glUniform3f(vertexLocation, ((xpos / WIDTH) - 0.5f) * 2.0f, ((-ypos / HEIGHT) + 0.46f) * 2.0f, locationz);
        TriangleColor = glGetUniformLocation(shaderProgram, "TrianCol");

        if (!Quad)
        {
            if (MOUSE_BUTTON_CLICKED(GLFW_MOUSE_BUTTON_LEFT))
            {
                glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLES, 0, 3);
                glBindVertexArray(0);
            }
            if (KEY_CLICKED(GLFW_KEY_E))
            {
                glUniform3f(vertexLocation, 0.0f, 0.0f, 0.0f);
                glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLES, 0, 3);
                glBindVertexArray(0);
            }
        }
        else {
            if (MOUSE_BUTTON_CLICKED(GLFW_MOUSE_BUTTON_LEFT))
            {
                glBindVertexArray(VAOQ);
                glDrawArrays(GL_TRIANGLES, 0, 6);
                glBindVertexArray(0);
            }
            if (KEY_CLICKED(GLFW_KEY_E))
            {
                glUniform3f(vertexLocation, 0.0f, 0.0f, 0.0f);
                glUseProgram(shaderProgram);
                glBindVertexArray(VAOQ);
                glDrawArrays(GL_TRIANGLES, 0, 6);
                glBindVertexArray(0);
            }

        }
        if (Star && !Quad)
        {
            if (MOUSE_BUTTON_CLICKED(GLFW_MOUSE_BUTTON_LEFT))
            {
                glBindVertexArray(VAOS);
                glDrawArrays(GL_TRIANGLES, 0, 3);
                glBindVertexArray(0);
            }
            if (KEY_CLICKED(GLFW_KEY_E))
            {
                glUniform3f(vertexLocation, 0.0f, 0.0f, 0.0f);
                glUseProgram(shaderProgram);
                glBindVertexArray(VAOS);
                glDrawArrays(GL_TRIANGLES, 0, 6);
                glBindVertexArray(0);
            }
        }
        // Swap the screen buffer
        glfwSwapBuffers(window);
    }

    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAOQ);
    glDeleteBuffers(1, &VBOQ);

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();

    return EXIT_SUCCESS;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (SHORTCUT(GLFW_KEY_LEFT_SHIFT, GLFW_KEY_E)) // E jak escape
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (KEY_CLICKED(GLFW_KEY_C))
    {
        switch (color)
        {
        case 1:
            glUniform4f(TriangleColor, 0.3f, 0.0f, 0.5f, 1.0f);
            color++;
            break;
        case 2:
            glUniform4f(TriangleColor, 0.9f, 0.5f, 0.5f, 1.0f);
            color++;
            break;
        case 3:
            glUniform4f(TriangleColor, 0.0f, 0.5f, 0.5f, 1.0f);
            color = 1;
            break;
        }
    }
    if (KEY_CLICKED(GLFW_KEY_W))
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (SHORTCUT(GLFW_KEY_LEFT_SHIFT, GLFW_KEY_W))
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (KEY_CLICKED(GLFW_KEY_Q))
        Quad = !Quad;
    if (KEY_CLICKED(GLFW_KEY_S))
        Star = !Star;
}