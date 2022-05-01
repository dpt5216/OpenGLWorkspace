
#include <iostream>

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <fstream>
#include <sstream>

struct ShaderProgramSource{
    std::string vs;
    std::string fs;
};

static ShaderProgramSource ParseShader(const std::string& filepath){
    //doing this the c++ way, which is slower but safer than the c way

    std::ifstream stream(filepath);

    enum class ShaderType{
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)){
        if (line.find("#shader") != std::string::npos){
            if (line.find("vertex") != std::string::npos){
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos){
                type = ShaderType::FRAGMENT;
            }
        }
        else{
            ss[(int) type] << line << "\n";
        }
        
    }

    return {ss[0].str(), ss[1].str()};
}

static unsigned int CompileShader(unsigned int type, const std::string& source){

    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr); //nullptr tells OpenGL that our program string is null-terminated
    glCompileShader(id);

    //Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); 
    if (result == GL_FALSE){
        // fetch the error message
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char message[length]; //if c++ doesnt like growing the stack in this way, you can use alloca(length)
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " shader compilation error" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }


    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader){ // note: & denotes a "reference", which is like a pointer that is never allowed to be NULL
    
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    // add something to catch if CompileShader failed

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    // shaders are now linked, we can delete our intermediate shaders
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


int main(void){
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Icon
    GLFWimage pepe;
    pepe.pixels = stbi_load("Assets/pepe.png", &pepe.height, &pepe.width, 0, 4);
    glfwSetWindowIcon(window, 1, &pepe);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // initialize glew. Note: has to happen in a valid OpenGL context (which is created by glfwMakeContextCurrent())
    if (glewInit() != GLEW_OK){
        std::cout << "glewInit error\n";
        glfwTerminate();
        return -1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    // positions we feed into the buffer. If we were doing this for real we should make a struct and use offsets for attributes
    float positions[] = { 
        -0.5f, -0.5f, // 0
         0.5f, -0.5f, // 1
         0.5f,  0.5f, // 2
        -0.5f, 0.5f   // 3
    };
    
    // indices we use to tell OpenGL which vertices to draw
    unsigned int indices[] = { //HAS TO BE UNSIGNED
        0, 1, 2,
        2, 3, 0
    };

    // lets define a buffer out here 
    unsigned int buffer;
    glGenBuffers(1, &buffer); 
    glBindBuffer(GL_ARRAY_BUFFER, buffer); 
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW); //GL_STATIC_DRAW is just a hint for the implementation, can do other behavior with it but may be slow

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*) 0); // tell OpenGl how to read our buffer

    unsigned int ibo;
    glGenBuffers(1, &ibo); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); 
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW); //HAS TO BE UNSIGNED

    // cool so we have data, now we have to tell openGL what to do with it in the form of some shaders
    
    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

    unsigned int shader = CreateShader(source.vs, source.fs);
    glUseProgram(shader);

    // render loop
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)){
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); 


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}
