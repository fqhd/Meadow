#include "ShadowMap.hpp"

const int width = 4096;

void ShadowMap::init() {
    lightPos = glm::vec3(15, 18, 16);
	glGenFramebuffers(1, &framebuffer);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, width, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    shader.load("depthmap");

    float near_plane = 0.1f, far_plane = 32.0f;
    float scale = 15.0f;
    glm::mat4 lightProjection = glm::ortho(-(scale - 3.5f), scale - 3.5f, -(scale - 2.5f), scale + 0.5f, near_plane, far_plane);
    
    glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(4.0, 0.0f, 4.0), glm::vec3(0.0f, 1.0f, 0.0f));

    lightSpaceMatrix = lightProjection * lightView;
}

void ShadowMap::generateShadowMap(World* world) {
    shader.bind();
    glViewport(0, 0, width, width);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClear(GL_DEPTH_BUFFER_BIT);

    shader.loadUniform("lightSpaceMatrix", lightSpaceMatrix);
    
    world->chunk.render();
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}