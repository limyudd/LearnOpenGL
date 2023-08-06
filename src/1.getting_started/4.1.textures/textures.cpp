#include <glad/glad.h>
#include <stb_image.h>
#include <GLFW/glfw3.h>
#include <iostream>

struct RenderData {
    GLFWwindow* window;   // ����ָ��
    GLuint fbo;           // ֡�������FrameBuffer Object��ID
    GLuint texture;       // ����ID
    int imageWidth;       // ͼ����
    int imageHeight;      // ͼ��߶�
};

// ����ͼ�񲢴�������
void loadTexture(RenderData& renderData, const char* imagePath)
{
    int width, height, channels;
    unsigned char* image = stbi_load(imagePath, &width, &height, &channels, 3);

    if (!image) {
        std::cerr << "�޷�����ͼ��" << imagePath << std::endl;
        return;
    }

    renderData.imageWidth = width;
    renderData.imageHeight = height;

    glGenTextures(1, &renderData.texture);
    glBindTexture(GL_TEXTURE_2D, renderData.texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(image);
}

// ��Ⱦ������FBO��
void renderScene(RenderData& renderData)
{
    glBindFramebuffer(GL_FRAMEBUFFER, renderData.fbo);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, renderData.texture);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, renderData.texture);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    glfwSwapBuffers(renderData.window);
}

// ��ʼ��OpenGL�ʹ���
void initialize(RenderData& renderData)
{
    if (!glfwInit())
    {
        std::cerr << "GLFW��ʼ��ʧ�ܣ�" << std::endl;
        exit(EXIT_FAILURE);
    }

    // ��������
    renderData.window = glfwCreateWindow(512, 512, "OpenGL FBO Image Display", NULL, NULL);
    if (!renderData.window)
    {
        std::cerr << "���ڴ���ʧ�ܣ�" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(renderData.window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "�޷���ʼ��glad��" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    const char* imagePath = "C://Users//mazhy//Pictures//testfbo.jpg";
    loadTexture(renderData, imagePath);

    glGenFramebuffers(1, &renderData.fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, renderData.fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderData.texture, 0);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "FBO����ʧ�ܣ�" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int main()
{
    RenderData renderData;
    initialize(renderData);

    while (!glfwWindowShouldClose(renderData.window))
    {
        renderScene(renderData);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
