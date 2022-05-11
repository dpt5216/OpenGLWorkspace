#include "renderer.h"
#include "vertexBuffer.h"
#include "indexBuffer.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std;

struct ShaderProgramSource{
    string vs;
    string fs;
};

static ShaderProgramSource ParseShader(const string& filepath){
    //doing this the c++ way, which is slower but safer than the c way

    ifstream stream(filepath);

    enum class ShaderType{
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    string line;
    stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)){
        if (line.find("#shader") != string::npos){
            if (line.find("vertex") != string::npos){
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != string::npos){
                type = ShaderType::FRAGMENT;
            }
        }
        else{
            ss[(int) type] << line << "\n";
        }
        
    }

    return {ss[0].str(), ss[1].str()};
}

static unsigned int CompileShader(unsigned int type, const string& source){

    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    GL_CALL(glShaderSource(id, 1, &src, nullptr)); //nullptr tells OpenGL that our program string is null-terminated
    GL_CALL(glCompileShader(id));

    //Error handling
    int result;
    GL_CALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result)); 
    if (result == GL_FALSE){
        // fetch the error message
        int length;
        GL_CALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char message[length]; //if c++ doesnt like growing the stack in this way, you can use alloca(length)
        GL_CALL(glGetShaderInfoLog(id, length, &length, message));

        cout << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " shader compilation error" << endl;
        cout << message << endl;

        GL_CALL(glDeleteShader(id));
        return 0;
    }


    return id;
}

static unsigned int CreateShader(const string& vertexShader, const string& fragmentShader){ // note: & denotes a "reference", which is like a pointer that is never allowed to be NULL
    
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    // add something to catch if CompileShader failed

    GL_CALL(glAttachShader(program, vs));
    GL_CALL(glAttachShader(program, fs));
    GL_CALL(glLinkProgram(program));
    GL_CALL(glValidateProgram(program));

    // shaders are now linked, we can delete our intermediate shaders
    GL_CALL(glDeleteShader(vs));
    GL_CALL(glDeleteShader(fs));

    return program;
}

void changeColor(float currColor[4], int u_location){
    
    currColor[0] += 0.01 - ((currColor[0] + 0.01) > 1);
    currColor[1] += 0.01 - ((currColor[1] + 0.10) > 1);
    currColor[2] += 0.01 - ((currColor[2] + 0.15) > 1);
    //currColor[3] += 0.01 - ((currColor[3] + 0.01) > 1);


    
    GL_CALL(glUniform4f(u_location, currColor[0], currColor[1], currColor[2], currColor[3]));
    

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
        cout << "glewInit error\n";
        glfwTerminate();
        return -1;
    }

    cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;

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

    unsigned int vao;
    GL_CALL(glGenVertexArrays(1,&vao));
    GL_CALL(glBindVertexArray(vao));

 //   vertexArray va;

    vertexBuffer vb(positions, 4 * 2 * sizeof(float));

   /* va.addBuffer(vb);

    bufferLayout layout;
    layout.push<float>(3);
    va.addLayout(layout);*/


    /*// lets define a buffer out here 
    unsigned int buffer;
    GL_CALL(glGenBuffers(1, &buffer)); 
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, buffer)); 
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW)); //GL_STATIC_DRAW is just a hint for the implementation, can do other behavior with it but may be slow
    */
    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*) 0)); // tell OpenGl how to read our buffer

    indexBuffer ib(indices, 6);

    

    /*unsigned int ibo;
    GL_CALL(glGenBuffers(1, &ibo)); 
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo)); 
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW)); //HAS TO BE UNSIGNED*/

    // cool so we have data, now we have to tell openGL what to do with it in the form of some shaders
    
    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

    unsigned int shader = CreateShader(source.vs, source.fs);
    GL_CALL(glUseProgram(shader));

    // Uniform
    float currColor[] = {0.2f, 0.3f, 0.4f, 1.0f};

    int u_location = glGetUniformLocation(shader, "u_Color");

    
    GL_CALL(glUniform4f(u_location, currColor[0], currColor[1], currColor[2], currColor[3]));
    
    


    // render loop
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)){
        /* Render here */
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));


        changeColor(currColor, u_location);

//            va.bind();
        GL_CALL(glBindVertexArray(vao));
        ib.bind();

        
        GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); 
        


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    GL_CALL(glDeleteProgram(shader));

    glfwTerminate();
    return 0;
}
