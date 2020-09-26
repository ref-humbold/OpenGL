#include "GameCard.hpp"

GameCard::GameCard(const std::pair<int, int> & size, const std::pair<int, int> & place,
                   Colour colour, Sign sign)
    : visible{false},
      vertexBufferData{
              -0.9f, -0.9f, 0.9f,  -0.9f, 0.9f, 0.9f,  -0.9f, 0.9f,  // card
              -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f, 1.0f,  -1.0f, 1.0f,  // frame
              0.0f,  0.7f,  0.0f,  -0.7f,  // pipe
              -0.7f, -0.7f, 0.7f,  0.7f,  0.7f, -0.7f, -0.7f, 0.7f,  // cross
              0.0f,  0.7f,  -0.7f, -0.7f, 0.7f, -0.7f,  // triangle
              -0.7f, 0.0f,  0.0f,  0.7f,  0.7f, 0.0f,  0.0f,  -0.7f  // square
      },
      scaleVector{0.8f / size.second, 0.8f / size.first},
      transformVector{1 - size.second + 2 * place.second, 1 - size.first + 2 * place.first},
      colour{colour},
      sign{sign}
{
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);
}

void GameCard::draw(GLuint programID, bool isCurrent, bool hasFrame) const
{
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

    if(hasFrame)
        drawFrame(programID);

    drawCard(programID, visible || isCurrent);

    glDisableVertexAttribArray(0);
}

void GameCard::drawCard(GLuint programID, bool isVisible) const
{
    GLint scale = glGetUniformLocation(programID, "scale");
    GLint transform = glGetUniformLocation(programID, "transform");
    GLint fragmentColor = glGetUniformLocation(programID, "fragmentColor");

    glUniform2f(scale, scaleVector[0], scaleVector[1]);
    glUniform2f(transform, transformVector[0], transformVector[1]);

    if(!isVisible)
    {
        glUniform3f(fragmentColor, 0.0f, 0.0f, 0.0f);  // black
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        return;
    }

    switch(colour)
    {
        case Colour::Red:
            glUniform3f(fragmentColor, 1.0f, 0.0f, 0.0f);
            break;

        case Colour::Green:
            glUniform3f(fragmentColor, 0.0f, 1.0f, 0.0f);
            break;

        case Colour::Blue:
            glUniform3f(fragmentColor, 0.0f, 0.0f, 1.0f);
            break;

        case Colour::Cyan:
            glUniform3f(fragmentColor, 0.0f, 1.0f, 1.0f);
            break;

        case Colour::Magenta:
            glUniform3f(fragmentColor, 1.0f, 0.0f, 1.0f);
            break;

        case Colour::Yellow:
            glUniform3f(fragmentColor, 1.0f, 1.0f, 0.0f);
            break;

        case Colour::Orange:
            glUniform3f(fragmentColor, 1.0f, 0.66f, 0.0f);
            break;

        case Colour::Purple:
            glUniform3f(fragmentColor, 0.66f, 0.0f, 0.66f);
            break;
    }

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    drawSign(programID);
}

void GameCard::drawSign(GLuint programID) const
{
    GLint scale = glGetUniformLocation(programID, "scale");
    GLint transform = glGetUniformLocation(programID, "transform");
    GLint fragmentColor = glGetUniformLocation(programID, "fragmentColor");

    glUniform2f(scale, scaleVector[0], scaleVector[1]);
    glUniform2f(transform, transformVector[0], transformVector[1]);
    glUniform3f(fragmentColor, 0.0f, 0.0f, 0.0f);

    switch(sign)
    {
        case Sign::Pipe:
            glDrawArrays(GL_LINES, 8, 2);
            break;

        case Sign::Cross:
            glDrawArrays(GL_LINES, 10, 2);
            glDrawArrays(GL_LINES, 12, 2);
            break;

        case Sign::Triangle:
            glDrawArrays(GL_LINE_LOOP, 14, 3);
            break;

        case Sign::Square:
            glDrawArrays(GL_LINE_LOOP, 17, 4);
            break;
    }
}

void GameCard::drawFrame(GLuint programID) const
{
    GLint scale = glGetUniformLocation(programID, "scale");
    GLint transform = glGetUniformLocation(programID, "transform");
    GLint fragmentColor = glGetUniformLocation(programID, "fragmentColor");

    glUniform2f(scale, scaleVector[0], scaleVector[1]);
    glUniform2f(transform, transformVector[0], transformVector[1]);

    glUniform3f(fragmentColor, 0.5f, 0.5f, 0.5f);  // gray
    glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
}
