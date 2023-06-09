#include "tool.h"
#include "utils.h"
#include "Shader.h"
#include "Model.h"
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

    Shader shader("./resource/shader/cubemaps.vert", "./resource/shader/cubemaps.frag");
    shader.Compile();
    Shader skyboxShader{"./resource/shader/skybox.vert", "./resource/shader/skybox.frag"};
    skyboxShader.Compile();
    Shader ourShader(
        {"./resource/shader/model_loading.vert",
         "./resource/shader/model_loading.frag"});
    ourShader.Compile();
    Model ourModel(
        "./resource/model/nanosuit/nanosuit.obj");
    // cube VAO
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*)0);

    // load textures
    // -------------
    unsigned int cubeTexture =
        loadTexture("./resource/texture/container.jpg");

    std::vector<std::string> faces{
        "./resource/texture/skybox/right.jpg",
        "./resource/texture/skybox/left.jpg",
        "./resource/texture/skybox/top.jpg",
        "./resource/texture/skybox/bottom.jpg",
        "./resource/texture/skybox/front.jpg",
        "./resource/texture/skybox/back.jpg"};
    unsigned int cubemapTexture = loadCubemap(faces);

    // shader configuration
    // --------------------
    shader.use();
    shader.setUniform("texture1", 0);

    skyboxShader.use();
    skyboxShader.setUniform("skybox", 0);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw scene as normal
        shader.use();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection =
            glm::perspective(glm::radians(camera.zoom()),
                             (float)width / (float)height, 0.1f, 100.0f);
        shader.setUniform("model", model);
        shader.setUniform("view", view);
        shader.setUniform("projection", projection);
        // cubes
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        ourShader.use();

        projection = glm::perspective(
            glm::radians(camera.zoom()), (float)width / height, 0.1f, 100.0f);
        view = camera.GetViewMatrix();
        ourShader.setUniform("projection", projection);
        ourShader.setUniform("view", view);

        model = glm::mat4(1.0f);
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

        // draw skybox as last
        glDepthFunc(
            GL_LEQUAL);  // change depth function so depth test passes when
                         // values are equal to depth buffer's content
        skyboxShader.use();
        view = glm::mat4(glm::mat3(
            camera
                .GetViewMatrix()));  // remove translation from the view matrix
        skyboxShader.setUniform("view", view);
        skyboxShader.setUniform("projection", projection);

        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);  // set depth function back to default
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
