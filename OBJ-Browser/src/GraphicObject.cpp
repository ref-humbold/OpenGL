#include "GraphicObject.hpp"

void GraphicObject::createBuffers()
{
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vbData.size() * sizeof(glm::vec3), &vbData[0], GL_STATIC_DRAW);

    glGenBuffers(1, &textureBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
    glBufferData(GL_ARRAY_BUFFER, tbData.size() * sizeof(glm::vec2), &tbData[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, nbData.size() * sizeof(glm::vec3), &nbData[0], GL_STATIC_DRAW);
}

void GraphicObject::draw(GLuint pID, glm::mat4 cameraMat, glm::mat4 clipMat)
{
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    GLint worldToCameraMat = glGetUniformLocation(pID, "worldToCameraMat");
    GLint cameraToClipMat = glGetUniformLocation(pID, "cameraToClipMat");

    glUniformMatrix4fv(worldToCameraMat, 1, GL_FALSE, &cameraMat[0][0]);
    glUniformMatrix4fv(cameraToClipMat, 1, GL_FALSE, &clipMat[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, vbData.size());

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}
