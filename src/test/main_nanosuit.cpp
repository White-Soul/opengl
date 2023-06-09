#include "tool.h"
#include "utils.h"
#include "Shader.h"
#include "Model.h"
#include <boost/filesystem.hpp>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "assimp-vc141-mtd.lib")
#endif

#ifdef DEBUG
#define PREFIX std::string("../")
#else
#define PREFIX std::string("./")
#endif

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main() {
    auto window = init();

    if (window == nullptr) std::exit(-1);

    Shader ourShader(
        {boost::filesystem::path("./resource/model_loading.vert").string(),
         boost::filesystem::path("./resource/model_loading.frag").string()});
    ourShader.Compile();

    Model ourModel(
        boost::filesystem::path("./resource/nanosuit/nanosuit.obj").string());

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        glm::mat4 projecton = glm::perspective(
            glm::radians(camera.zoom()), (float)width / height, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setUniform("projection", projecton);
        ourShader.setUniform("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(
            model,
            glm::vec3(
                0.0f));  // translate it down so it's at the center of the scene
        model = glm::scale(
            model,
            glm::vec3(
                1.0f));  // it's a bit too big for our scene, so scale it down
        ourShader.setUniform("model", model);
        ourModel.Draw(ourShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
