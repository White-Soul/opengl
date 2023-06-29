
#include "header/tool.h"
#include "header/utils.h"
#include "header/Shader.h"
#include "header/Model.h"
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "assimp-vc143-mtd.lib")
#endif

#ifdef DEBUG
#define PREFIX std::string("../")
#else
#define PREFIX std::string("./")
#endif

const float vex[] = {
    0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9,
};

int main(int argc, char** argv) {
    auto window = init();

    if (window == nullptr) {
        std::cout << "init error\n";
        std::exit(-1);
    }

    unsigned int VAO, VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vex), &vex, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float),
                          (void*)(1 * sizeof(float)));

    Shader shader{"./resource/shader/game.vert", "./resource/shader/game.geom",
                  "./resource/shader/game.frag"};
    shader.Compile();
    shader.use();

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO);
        glDrawArrays(GL_POINT, 0, 9);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    glfwTerminate();

    return 0;
}
