
#include <iostream>

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>



#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main(void)
{
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
    float positions[6] = {-0.5f, -0.5f,
                           0.0f,  0.5f,
                           0.5f, -0.5f };

    // lets define a buffer out here 
    unsigned int buffer;
    glGenBuffers(1, &buffer); 
    glBindBuffer(GL_ARRAY_BUFFER, buffer); 
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW); //GL_STATIC_DRAW is just a hint for the implementation, can do other behavior with it but may be slow

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*) 0); // tell OpenGl how to read our buffer

    // cool so we have data, now we have to tell openGL what to do with it


    // render loop
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


        glDrawArrays(GL_TRIANGLES, 0, 3); //draws from array data (which we bound with glBindBuffer(GL_ARRAY_BUFFER...))



        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
