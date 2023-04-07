#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "settings.h"
#include "shader.h"
#include "buffer.h"
#include "texture.h"
#include "quad.h"
#include "cube.h"

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = lastX - xpos;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    if (camera.enableCameraMovement)
        camera.ProcessMouseMovement(xoffset, yoffset);

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (camera.enableCameraMovement)
        camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // TODO: Fix resizing issues
    glViewport(0, 0, width, height);
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
}

void processInput(GLFWwindow* window, float dt)
{
    //TODO: Fix button presses

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (camera.enableCameraMovement)
    {
        if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
            camera.MovementSpeed = camera.MovementSpeed + 0.05f;
        if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
            camera.MovementSpeed = camera.MovementSpeed > 1.5f ? camera.MovementSpeed - 0.05f : camera.MovementSpeed;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(CameraMovement::FORWARD, dt);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(CameraMovement::BACKWARD, dt);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(CameraMovement::LEFT, dt);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(CameraMovement::RIGHT, dt);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            camera.ProcessKeyboard(CameraMovement::UP, dt);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            camera.ProcessKeyboard(CameraMovement::DOWN, dt);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            camera.enableCameraMovement = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
    else
    {
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            glfwSetCursorPos(window, WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
            camera.enableCameraMovement = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
}

GLFWwindow* Init()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Clouds", NULL, NULL);
    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return nullptr;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    return window;
}

int main()
{
    GLFWwindow* window = Init();
    if (window == nullptr)
        return -1;

    Quad2D quad;
    Cube cube;
    Texture col(1.0, 0.8, 0.5);

    FrameBuffer fbo;
    Texture framebufferImage(SCR_WIDTH, SCR_HEIGHT);
    fbo.BindTexture(framebufferImage.ID);
    Texture depthImage(SCR_WIDTH, SCR_HEIGHT);
    fbo.BindTexture(depthImage.ID, GL_COLOR_ATTACHMENT1);
    RenderBuffer rbo(SCR_WIDTH, SCR_HEIGHT);
    fbo.BindRenderBufferObject(rbo.ID);
    
    Shader worldShader(flatVertPath.string().c_str(), flatFragPath.string().c_str());
    Shader screenShader(screenVertPath.string().c_str(), screenFragPath.string().c_str());

    float lastFrame = 0.f;
    float dt = 0.f;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        dt = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, dt);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), aspectRatio, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.f);

        fbo.BindFrameBuffer();
        GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        fbo.DrawBuffers(buffers, 2);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.f, 0.f, 0.f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        worldShader.use();
        worldShader.setMat4("projection", projection);
        worldShader.setMat4("view", view);
        worldShader.setMat4("model", model);
        worldShader.setInt("colorTexture", 0);
        cube.BindTexture(GL_TEXTURE0, col.ID);
        cube.Draw();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); 
        screenShader.use();
        screenShader.setInt("screenTexture", 0);
        quad.BindTexture(GL_TEXTURE0, framebufferImage.ID);
        quad.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

	return 0;
}