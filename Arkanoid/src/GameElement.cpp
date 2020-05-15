#include "GameElement.hpp"

GLuint createVertexBuffer(const GLfloat vbData[], size_t size)
{
    GLuint bufferID;

    glGenBuffers(1, &bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    glBufferData(GL_ARRAY_BUFFER, size, vbData, GL_STATIC_DRAW);
    return bufferID;
}

void loadBuffer(GLuint vertexBufferID, GLuint colourBufferID)
{
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colourBufferID);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
}

#pragma region GameBoard

// 0.57735 ~~ sqrt(3) / 3
// 0.34641 ~~ sqrt(3) / 5

GameBoard::GameBoard()
    : vbDataHexagon{0.0f,  0.0f, 0.4f,  0.0f,      0.2f, 0.34641f,  -0.2f, 0.34641f,
                    -0.4f, 0.0f, -0.2f, -0.34641f, 0.2f, -0.34641f, 0.4f,  0.0f},
      cbDataHexagon{0.0f, 0.6f, 0.6f,  0.0f, 0.0f, 0.25f, 0.0f, 0.0f, 0.25f, 0.0f, 0.0f, 0.25f,
                    0.0f, 0.0f, 0.25f, 0.0f, 0.0f, 0.25f, 0.0f, 0.0f, 0.25f, 0.0f, 0.0f, 0.25f},
      vbDataTriangle{0.0f, 0.0f, 0.0f, 1.0f, 0.57735f, 0.0f},
      cbDataTriangle{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
      scaleMatrix{glm::mat2(glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 1.0f))},
      rotateMatrix{glm::mat2(glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 1.0f))},
      transformVector{glm::vec2(0.0f, 0.0f)}
{
    vertexBufferHexagon = createVertexBuffer(vbDataHexagon, sizeof(vbDataHexagon));
    colorBufferHexagon = createVertexBuffer(cbDataHexagon, sizeof(cbDataHexagon));
    vertexBufferTriangle = createVertexBuffer(vbDataTriangle, sizeof(vbDataTriangle));
    colorBufferTriangle = createVertexBuffer(cbDataTriangle, sizeof(cbDataTriangle));
    normalVectors.resize(5);
}

void GameBoard::drawBackground(GLuint pID)
{
    scaleMatrix = glm::mat2(glm::vec2(0.1f, 0.0f), glm::vec2(0.0f, 0.1f));

    loadBuffer(vertexBufferHexagon, colorBufferHexagon);

    GLint scaleMat = glGetUniformLocation(pID, "scaleMat");
    GLint rotateMat = glGetUniformLocation(pID, "rotateMat");
    GLint transformVec = glGetUniformLocation(pID, "transformVec");

    for(int j = -30; j <= 30; ++j)
        for(int i = -9; i <= 9; ++i)
        {
            transformVector =
                    scaleMatrix * glm::vec2(1.2f * i + 0.6f * std::abs(j % 2), 0.34641f * j);

            glUniformMatrix2fv(scaleMat, 1, GL_FALSE, &scaleMatrix[0][0]);
            glUniformMatrix2fv(rotateMat, 1, GL_FALSE, &rotateMatrix[0][0]);
            glUniform2fv(transformVec, 1, &transformVector[0]);

            glDrawArrays(GL_TRIANGLE_FAN, 0, 8);
        }

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

void GameBoard::drawBorderTriangles(GLuint pID)
{
    // left bottom
    scaleMatrix = glm::mat2(glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 1.0f));
    rotateMatrix = glm::mat2(glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 1.0f));
    transformVector = glm::vec2(-1.0f, -1.0f);
    drawOneTriangle(pID);

    // left top
    scaleMatrix = glm::mat2(glm::vec2(-1.0f, 0.0f), glm::vec2(0.0f, 1.0f));
    rotateMatrix = glm::mat2(glm::vec2(-1.0f, 0.0f), glm::vec2(0.0f, -1.0f));
    transformVector = glm::vec2(-1.0f, 1.0f);
    drawOneTriangle(pID);

    // right top
    scaleMatrix = glm::mat2(glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 1.0f));
    rotateMatrix = glm::mat2(glm::vec2(-1.0f, 0.0f), glm::vec2(0.0f, -1.0f));
    transformVector = glm::vec2(1.0f, 1.0f);
    drawOneTriangle(pID);

    // right bottom
    scaleMatrix = glm::mat2(glm::vec2(-1.0f, 0.0f), glm::vec2(0.0f, 1.0f));
    rotateMatrix = glm::mat2(glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 1.0f));
    transformVector = glm::vec2(1.0f, -1.0f);
    drawOneTriangle(pID);
}

void GameBoard::countNormalVectors()
{
    glm::vec2 normal = glm::vec2(1.0f, 0.57735f);

    // left bottom
    scaleMatrix = glm::mat2(glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 1.0f));
    rotateMatrix = glm::mat2(glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 1.0f));
    normalVectors[0] = glm::normalize(rotateMatrix * scaleMatrix * normal);

    // left top
    scaleMatrix = glm::mat2(glm::vec2(-1.0f, 0.0f), glm::vec2(0.0f, 1.0f));
    rotateMatrix = glm::mat2(glm::vec2(-1.0f, 0.0f), glm::vec2(0.0f, -1.0f));
    normalVectors[1] = glm::normalize(rotateMatrix * scaleMatrix * normal);

    // top
    normalVectors[2] = glm::normalize(glm::vec2(0.0f, -1.0f));

    // right top
    scaleMatrix = glm::mat2(glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 1.0f));
    rotateMatrix = glm::mat2(glm::vec2(-1.0f, 0.0f), glm::vec2(0.0f, -1.0f));
    normalVectors[3] = glm::normalize(rotateMatrix * scaleMatrix * normal);

    // right bottom
    scaleMatrix = glm::mat2(glm::vec2(-1.0f, 0.0f), glm::vec2(0.0f, 1.0f));
    rotateMatrix = glm::mat2(glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 1.0f));
    normalVectors[4] = glm::normalize(rotateMatrix * scaleMatrix * normal);
}

void GameBoard::drawOneTriangle(GLuint pID)
{
    loadBuffer(vertexBufferTriangle, colorBufferTriangle);

    GLint scaleMat = glGetUniformLocation(pID, "scaleMat");
    GLint rotateMat = glGetUniformLocation(pID, "rotateMat");
    GLint transformVec = glGetUniformLocation(pID, "transformVec");

    glUniformMatrix2fv(scaleMat, 1, GL_FALSE, &scaleMatrix[0][0]);
    glUniformMatrix2fv(rotateMat, 1, GL_FALSE, &rotateMatrix[0][0]);
    glUniform2fv(transformVec, 1, &transformVector[0]);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

#pragma endregion
#pragma region GameBrick

GameBrick::GameBrick()
    : isVisible{{true, true, true, true, true, true, true, true, true, true, true, true, true},
                {true, true, true, true, true, true, true, true, true, true, true, true, true},
                {true, true, true, true, true, true, true, true, true, true, true, true, true},
                {true, true, true, true, true, true, true, true, true, true, true, true, true},
                {false, true, true, true, true, true, true, true, true, true, true, true, false},
                {false, true, true, true, true, true, true, true, true, true, true, true, false}},
      bricksLeft{74},
      vbDataRect{0.5f, 0.25f, 0.5f, -0.25f, -0.5f, -0.25f, -0.5f, 0.25f},
      cbDataRect{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
      cbDataRectBorder{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
      scaleMatrix{glm::mat2(glm::vec2(0.1f, 0.0f), glm::vec2(0.0f, 0.1f))},
      rotateMatrix{glm::mat2(glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 1.0f))},
      transformVector{glm::vec2(0.0f, 0.0f)}
{
    vertexBufferRect = createVertexBuffer(vbDataRect, sizeof(vbDataRect));
    colorBufferRect = createVertexBuffer(cbDataRect, sizeof(cbDataRect));
    colorBufferRectBorder = createVertexBuffer(cbDataRectBorder, sizeof(cbDataRectBorder));
}

void GameBrick::drawAllBricks(GLuint pID)
{
    Colour rowColours[6] = {Colour::Blue, Colour::Green,   Colour::Cyan,
                            Colour::Red,  Colour::Magenta, Colour::Yellow};

    for(int i = 0; i < 6; ++i)
        for(int j = 0; j < 13; ++j)
        {
            transformVector = glm::vec2(0.1f * (j - 6), 0.6f + 0.05f * i);

            if(isVisible[i][j])
            {
                drawRect(pID, rowColours[i]);
                drawRectBorder(pID);
            }
        }
}

void GameBrick::drawRect(GLuint pID, Colour colour)
{
    switch(colour)
    {
        case Colour::Blue:
            for(int i = 0; i < 12; ++i)
                cbDataRect[i] = i % 3 == 2 ? 1.0f : 0.0f;

            break;

        case Colour::Green:
            for(int i = 0; i < 12; ++i)
                cbDataRect[i] = i % 3 == 1 ? 1.0f : 0.0f;

            break;

        case Colour::Cyan:
            for(int i = 0; i < 12; ++i)
                cbDataRect[i] = i % 3 != 0 ? 1.0f : 0.0f;

            break;

        case Colour::Red:
            for(int i = 0; i < 12; ++i)
                cbDataRect[i] = i % 3 == 0 ? 1.0f : 0.0f;

            break;

        case Colour::Magenta:
            for(int i = 0; i < 12; ++i)
                cbDataRect[i] = i % 3 != 1 ? 1.0f : 0.0f;

            break;

        case Colour::Yellow:
            for(int i = 0; i < 12; ++i)
                cbDataRect[i] = i % 3 != 2 ? 1.0f : 0.0f;

            break;
    }

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferRect);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferRect);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cbDataRect), cbDataRect, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    GLint scaleMat = glGetUniformLocation(pID, "scaleMat");
    GLint rotateMat = glGetUniformLocation(pID, "rotateMat");
    GLint transformVec = glGetUniformLocation(pID, "transformVec");

    glUniformMatrix2fv(scaleMat, 1, GL_FALSE, &scaleMatrix[0][0]);
    glUniformMatrix2fv(rotateMat, 1, GL_FALSE, &rotateMatrix[0][0]);
    glUniform2fv(transformVec, 1, &transformVector[0]);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

void GameBrick::drawRectBorder(GLuint pID)
{
    loadBuffer(vertexBufferRect, colorBufferRectBorder);

    GLint scaleMat = glGetUniformLocation(pID, "scaleMat");
    GLint rotateMat = glGetUniformLocation(pID, "rotateMat");
    GLint transformVec = glGetUniformLocation(pID, "transformVec");

    glUniformMatrix2fv(scaleMat, 1, GL_FALSE, &scaleMatrix[0][0]);
    glUniformMatrix2fv(rotateMat, 1, GL_FALSE, &rotateMatrix[0][0]);
    glUniform2fv(transformVec, 1, &transformVector[0]);

    glDrawArrays(GL_LINE_LOOP, 0, 4);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

#pragma endregion
#pragma region GamePaddle

GamePaddle::GamePaddle()
    : vbDataPaddle{0.0f, 0.0f, 0.4f, -0.05f, 0.6f, 0.1f, -0.6f, 0.1f, -0.4f, -0.05f},
      cbDataPaddle{1.0f, 1.0f, 1.0f, 0.8f, 0.8f, 0.8f, 0.2f, 0.2f,
                   0.2f, 0.2f, 0.2f, 0.2f, 0.8f, 0.8f, 0.8f},
      scaleMatrix{glm::mat2(glm::vec2(0.1f, 0.0f), glm::vec2(0.0f, 0.1f))},
      rotateMatrix{glm::mat2(glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 1.0f))},
      transformVector{glm::vec2(0.0f, -0.95f)},
      velocity{1.0f}
{
    vertexBufferPaddle = createVertexBuffer(vbDataPaddle, sizeof(vbDataPaddle));
    colorBufferPaddle = createVertexBuffer(cbDataPaddle, sizeof(cbDataPaddle));
}

void GamePaddle::restart()
{
    transformVector = glm::vec2(0.0f, -0.95f);
    velocity = 1.0f;
}

void GamePaddle::drawPaddle(GLuint pID)
{
    loadBuffer(vertexBufferPaddle, colorBufferPaddle);

    GLint scaleMat = glGetUniformLocation(pID, "scaleMat");
    GLint rotateMat = glGetUniformLocation(pID, "rotateMat");
    GLint transformVec = glGetUniformLocation(pID, "transformVec");

    glUniformMatrix2fv(scaleMat, 1, GL_FALSE, &scaleMatrix[0][0]);
    glUniformMatrix2fv(rotateMat, 1, GL_FALSE, &rotateMatrix[0][0]);
    glUniform2fv(transformVec, 1, &transformVector[0]);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 5);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

void GamePaddle::moveLeft(GLfloat delta)
{
    transformVector[0] = std::max(-0.4f, transformVector[0] - velocity * delta);
}

void GamePaddle::moveRight(GLfloat delta)
{
    transformVector[0] = std::min(0.4f, transformVector[0] + velocity * delta);
}

GLfloat GamePaddle::getPosX()
{
    return transformVector[0];
}

GLfloat GamePaddle::getSurf()
{
    return transformVector[1] + 0.01f;
}

#pragma endregion
#pragma region GameBall

// 0.18660254 ~~ (2 + sqrt(3)) / 20
// 0.13660254 ~~ (1 + sqrt(3)) / 20

GameBall::GameBall()
    : vbDataBall{0.0f,         0.0f,         0.05f,        0.18660254f,  0.13660254f,  0.13660254f,
                 0.18660254f,  0.05f,        0.18660254f,  -0.05f,       0.13660254f,  -0.13660254f,
                 0.05f,        -0.18660254f, -0.05f,       -0.18660254f, -0.13660254f, -0.13660254f,
                 -0.18660254f, -0.05f,       -0.18660254f, 0.05f,        -0.13660254f, 0.13660254f,
                 -0.05f,       0.18660254f,  0.05f,        0.18660254f},
      cbDataBall{0.8f,  0.8f,  0.8f,  0.35f, 0.35f, 0.35f, 0.35f, 0.35f, 0.35f, 0.35f, 0.35f,
                 0.35f, 0.35f, 0.35f, 0.35f, 0.35f, 0.35f, 0.35f, 0.35f, 0.35f, 0.35f, 0.35f,
                 0.35f, 0.35f, 0.35f, 0.35f, 0.35f, 0.35f, 0.35f, 0.35f, 0.35f, 0.35f, 0.35f,
                 0.35f, 0.35f, 0.35f, 0.35f, 0.35f, 0.35f, 0.35f, 0.35f, 0.35f},
      vbDataCross{0.13660254f, 0.13660254f,  -0.13660254f, -0.13660254f,
                  0.13660254f, -0.13660254f, -0.13660254f, 0.13660254f},
      cbDataCross{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
      scaleMatrix{glm::mat2(glm::vec2(0.08f, 0.0f), glm::vec2(0.0f, 0.08f))},
      rotateMatrix{glm::mat2(glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 1.0f))},
      transformVector{glm::vec2(0.0f, -0.9f)},
      normalVector{glm::vec2(0.0f, 0.0f)},
      radius{glm::length(scaleMatrix * glm::vec2(0.13660254f, 0.13660254f))},
      separator{1.25f * radius},
      velocityDistance{50.0f * radius},
      startingShot{true}
{
    vertexBufferBall = createVertexBuffer(vbDataBall, sizeof(vbDataBall));
    colorBufferBall = createVertexBuffer(cbDataBall, sizeof(cbDataBall));
    vertexBufferCross = createVertexBuffer(vbDataCross, sizeof(vbDataCross));
    colorBufferCross = createVertexBuffer(cbDataCross, sizeof(cbDataCross));

    velocity = velocityDistance * glm::normalize(glm::vec2((-10.0f + rand() % 21) / 10.0f, 1.0f));
    collided.resize(7);

    for(int i = 0; i < 5; ++i)
        collided[i].resize(13, std::make_pair(false, false));

    collided[5].resize(5, std::make_pair(false, false));
    collided[6].resize(1, std::make_pair(false, false));
}

void GameBall::restart()
{
    transformVector = glm::vec2(0.0f, -0.9f);
    startingShot = true;
    velocity = velocityDistance * glm::normalize(glm::vec2((-10.0f + rand() % 21) / 10.0f, 1.0f));
}

void GameBall::drawBall(GLuint pID)
{
    loadBuffer(vertexBufferBall, colorBufferBall);

    GLint scaleMat = glGetUniformLocation(pID, "scaleMat");
    GLint rotateMat = glGetUniformLocation(pID, "rotateMat");
    GLint transformVec = glGetUniformLocation(pID, "transformVec");

    glUniformMatrix2fv(scaleMat, 1, GL_FALSE, &scaleMatrix[0][0]);
    glUniformMatrix2fv(rotateMat, 1, GL_FALSE, &rotateMatrix[0][0]);
    glUniform2fv(transformVec, 1, &transformVector[0]);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 14);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

void GameBall::drawCross(GLuint pID)
{
    loadBuffer(vertexBufferCross, colorBufferCross);

    GLint scaleMat = glGetUniformLocation(pID, "scaleMat");
    GLint rotateMat = glGetUniformLocation(pID, "rotateMat");
    GLint transformVec = glGetUniformLocation(pID, "transformVec");

    glUniformMatrix2fv(scaleMat, 1, GL_FALSE, &scaleMatrix[0][0]);
    glUniformMatrix2fv(rotateMat, 1, GL_FALSE, &rotateMatrix[0][0]);
    glUniform2fv(transformVec, 1, &transformVector[0]);

    glDrawArrays(GL_LINES, 0, 2);
    glDrawArrays(GL_LINES, 2, 2);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

bool GameBall::isInRange(GLfloat value, GLfloat minR, GLfloat maxR)
{
    return minR <= value && value <= maxR;
}

GLfloat GameBall::countDistance(glm::vec2 pt, glm::vec2 nl, glm::vec2 pl)
{
    glm::vec2 nvl = glm::normalize(nl), ptd = pl - pt;
    GLfloat dotProduct = glm::dot(ptd, nvl);

    return glm::length(ptd - nvl * dotProduct);
}

bool GameBall::checkOutside()
{
    return transformVector[1] <= -1.0f || transformVector[1] <= -2.0f * transformVector[0] - 2.0f
           || transformVector[1] >= 2.0f * transformVector[0] + 2.0f
           || transformVector[1] >= -2.0f * transformVector[0] + 2.0f
           || transformVector[1] <= 2.0f * transformVector[0] - 2.0f;
}

void GameBall::checkCollisionBoard(GameBoard & board)
{
    if(countDistance(transformVector, glm::vec2(0.57735f, -1.0f), glm::vec2(-1.0f, 0.0f))
               <= separator
       && !collided[5][0].first)
    {
        normalVector += board.normalVectors[0];
        collided[5][0].second = true;
    }
    else if(countDistance(transformVector, glm::vec2(0.57735f, 1.0f), glm::vec2(-1.0f, 0.0f))
                    <= separator
            && !collided[5][1].first)
    {
        normalVector += board.normalVectors[1];
        collided[5][1].second = true;
    }
    else if(std::abs(0.975f - transformVector[1]) <= separator && !collided[5][2].first)
    {
        normalVector += board.normalVectors[2];
        collided[5][2].second = true;
    }
    else if(countDistance(transformVector, glm::vec2(-0.57735f, 1.0f), glm::vec2(1.0f, 0.0f))
                    <= separator
            && !collided[5][3].first)
    {
        normalVector += board.normalVectors[3];
        collided[5][3].second = true;
    }
    else if(countDistance(transformVector, glm::vec2(-0.57735f, -1.0f), glm::vec2(1.0f, 0.0f))
                    <= separator
            && !collided[5][4].first)
    {
        normalVector += board.normalVectors[4];
        collided[5][4].second = true;
    }
}

void GameBall::checkCollisionPaddle(GamePaddle & paddle)
{
    GLfloat padPosX = paddle.getPosX(), dist = std::abs(paddle.getSurf() - transformVector[1]);

    if(isInRange(transformVector[0], padPosX - 0.06f, padPosX + 0.06f) && dist <= separator
       && !collided[6][0].first)
    {
        GLfloat rnd = rand() % 2 == 0 ? (1 + rand() % 10) * 0.005f : -(1 + rand() % 8) * 0.005f;

        normalVector += glm::normalize(glm::vec2(rnd, 1.0f));
        collided[6][0].second = true;
    }
}

void GameBall::checkCollisionBrick(GameBrick & brick)
{
    std::vector<std::pair<int, int>> bricksHit;

    if(transformVector[1] >= 0.525f)
    {
        GLfloat brickBorders[4];
        glm::vec2 brickCorners[4];
        GLfloat brickDist[8];
        glm::vec2 brPos = glm::vec2(floor(10 * transformVector[0] + 0.5f) * 0.1f,
                                    floor(20 * transformVector[1] + 0.5f) * 0.05f);

        brickBorders[0] = brPos[1] + 0.025f;  // up
        brickBorders[1] = brPos[1] - 0.025f;  // down
        brickBorders[2] = brPos[0] - 0.05f;  // left
        brickBorders[3] = brPos[0] + 0.05f;  // right

        brickCorners[0] = glm::vec2(brickBorders[2], brickBorders[0]);  // left up
        brickCorners[1] = glm::vec2(brickBorders[2], brickBorders[1]);  // left down
        brickCorners[2] = glm::vec2(brickBorders[3], brickBorders[0]);  // right up
        brickCorners[3] = glm::vec2(brickBorders[3], brickBorders[1]);  // right down

        brickDist[0] = std::abs(brickBorders[0] - transformVector[1]);
        brickDist[1] = std::abs(brickBorders[1] - transformVector[1]);
        brickDist[2] = std::abs(brickBorders[2] - transformVector[0]);
        brickDist[3] = std::abs(brickBorders[3] - transformVector[0]);

        brickDist[4] = glm::distance(brickCorners[0], transformVector);
        brickDist[5] = glm::distance(brickCorners[1], transformVector);
        brickDist[6] = glm::distance(brickCorners[2], transformVector);
        brickDist[7] = glm::distance(brickCorners[3], transformVector);

        if(brPos[1] > 0.55f)
        {
            int row = (int)floor((brPos[1] - 0.6f) * 20.0f + 0.5);
            int column = (int)floor((brPos[0] + 0.6f) * 10.0f + 0.5);

            if(isInRange(row, 0, 5) && isInRange(column, 0, 12) && !collided[row][column].first
               && brick.isVisible[row][column])
            {
                int closeBorder = 0;

                for(int i = 0; i < 4; ++i)
                    if(brickDist[i] < 1.0f)
                        closeBorder = i;

                switch(closeBorder)
                {
                    case 0:
                        normalVector += glm::normalize(glm::vec2(0.0f, 1.0f));
                        break;

                    case 1:
                        normalVector += glm::normalize(glm::vec2(0.0f, -1.0f));
                        break;

                    case 2:
                        normalVector += glm::normalize(glm::vec2(-1.0f, 0.0f));
                        break;

                    case 3:
                        normalVector += glm::normalize(glm::vec2(1.0f, 0.0f));
                        break;
                }

                brickScored(brick, row, column, bricksHit);
            }
        }

        if(brickDist[0] <= separator)  // up
        {
            glm::vec2 brPosC = brPos + glm::vec2(0.0f, 0.05f);

            int row = (int)floor((brPosC[1] - 0.6f) * 20.0f + 0.5);
            int column = (int)floor((brPosC[0] + 0.6f) * 10.0f + 0.5);

            if(isInRange(row, 0, 5) && isInRange(column, 0, 12) && !collided[row][column].first
               && brick.isVisible[row][column])
            {
                normalVector += glm::normalize(glm::vec2(0.0f, -1.0f));
                brickScored(brick, row, column, bricksHit);
            }
        }

        if(brickDist[1] <= separator)  // down
        {
            glm::vec2 brPosC = brPos + glm::vec2(0.0f, -0.05f);

            int row = (int)floor((brPosC[1] - 0.6f) * 20.0f + 0.5);
            int column = (int)floor((brPosC[0] + 0.6f) * 10.0f + 0.5);

            if(isInRange(row, 0, 5) && isInRange(column, 0, 12) && !collided[row][column].first
               && brick.isVisible[row][column])
            {
                normalVector += glm::normalize(glm::vec2(0.0f, 1.0f));
                brickScored(brick, row, column, bricksHit);
            }
        }

        if(brickDist[2] <= separator)  // left
        {
            glm::vec2 brPosC = brPos + glm::vec2(-0.1f, 0.0f);

            int row = (int)floor((brPosC[1] - 0.6f) * 20.0f + 0.5);
            int column = (int)floor((brPosC[0] + 0.6f) * 10.0f + 0.5);

            if(isInRange(row, 0, 5) && isInRange(column, 0, 12) && !collided[row][column].first
               && brick.isVisible[row][column])
            {
                normalVector += glm::normalize(glm::vec2(1.0f, 0.0f));
                brickScored(brick, row, column, bricksHit);
            }
        }

        if(brickDist[3] <= separator)  // right
        {
            glm::vec2 brPosC = brPos + glm::vec2(0.1f, 0.0f);

            int row = (int)floor((brPosC[1] - 0.6f) * 20.0f + 0.5);
            int column = (int)floor((brPosC[0] + 0.6f) * 10.0f + 0.5);

            if(isInRange(row, 0, 5) && isInRange(column, 0, 12) && !collided[row][column].first
               && brick.isVisible[row][column])
            {
                normalVector += glm::normalize(glm::vec2(-1.0f, 0.0f));
                brickScored(brick, row, column, bricksHit);
            }
        }

        if(brickDist[4] <= radius)  // left up
        {
            glm::vec2 brPosC = brPos + glm::vec2(-0.1f, 0.05f);

            int row = (int)floor((brPosC[1] - 0.6f) * 20.0f + 0.5);
            int column = (int)floor((brPosC[0] + 0.6f) * 10.0f + 0.5);

            if((row == 0 && isInRange(column, 0, 11) && !collided[row][column].first
                && brick.isVisible[row][column] && !brick.isVisible[row][column + 1])
               || (isInRange(row, 1, 5) && isInRange(column, 0, 11) && !collided[row][column].first
                   && brick.isVisible[row][column] && !brick.isVisible[row][column + 1]
                   && !brick.isVisible[row - 1][column]))
            {
                normalVector += velocity[1] < 0 ? glm::normalize(glm::vec2(1.0f, 0.0f))
                                                : glm::normalize(glm::vec2(1.0f, -1.0f));
                brickScored(brick, row, column, bricksHit);
            }
        }

        if(brickDist[5] <= radius)  // left down
        {
            glm::vec2 brPosC = brPos + glm::vec2(-0.1f, -0.05f);

            int row = (int)floor((brPosC[1] - 0.6f) * 20.0f + 0.5);
            int column = (int)floor((brPosC[0] + 0.6f) * 10.0f + 0.5);

            if((row == 5 && isInRange(column, 0, 11) && !collided[row][column].first
                && brick.isVisible[row][column] && !brick.isVisible[row][column + 1])
               || (isInRange(row, 0, 4) && isInRange(column, 0, 11) && !collided[row][column].first
                   && brick.isVisible[row][column] && !brick.isVisible[row][column + 1]
                   && !brick.isVisible[row + 1][column]))
            {
                normalVector += velocity[1] > 0 ? glm::normalize(glm::vec2(1.0f, 0.0f))
                                                : glm::normalize(glm::vec2(1.0f, 1.0f));
                brickScored(brick, row, column, bricksHit);
            }
        }

        if(brickDist[6] <= radius)  // right up
        {
            glm::vec2 brPosC = brPos + glm::vec2(0.1f, 0.05f);

            int row = (int)floor((brPosC[1] - 0.6f) * 20.0f + 0.5);
            int column = (int)floor((brPosC[0] + 0.6f) * 10.0f + 0.5);

            if((row == 0 && isInRange(column, 1, 12) && !collided[row][column].first
                && brick.isVisible[row][column] && !brick.isVisible[row][column - 1])
               || (isInRange(row, 1, 5) && isInRange(column, 1, 12) && !collided[row][column].first
                   && brick.isVisible[row][column] && !brick.isVisible[row][column - 1]
                   && !brick.isVisible[row - 1][column]))
            {
                normalVector += velocity[1] < 0 ? glm::normalize(glm::vec2(-1.0f, 0.0f))
                                                : glm::normalize(glm::vec2(-1.0f, -1.0f));
                brickScored(brick, row, column, bricksHit);
            }
        }

        if(brickDist[7] <= radius)  // right down
        {
            glm::vec2 brPosC = brPos + glm::vec2(0.1f, -0.05f);

            int row = (int)floor((brPosC[1] - 0.6f) * 20.0f + 0.5);
            int column = (int)floor((brPosC[0] + 0.6f) * 10.0f + 0.5);

            if((row == 5 && isInRange(column, 1, 12) && !collided[row][column].first
                && brick.isVisible[row][column] && !brick.isVisible[row][column - 1])
               || (isInRange(row, 0, 4) && isInRange(column, 1, 12) && !collided[row][column].first
                   && brick.isVisible[row][column] && !brick.isVisible[row][column - 1]
                   && !brick.isVisible[row + 1][column]))
            {
                normalVector += velocity[1] > 0 ? glm::normalize(glm::vec2(-1.0f, 0.0f))
                                                : glm::normalize(glm::vec2(-1.0f, 1.0f));
                brickScored(brick, row, column, bricksHit);
            }
        }
    }

    for(auto & b : bricksHit)
        brick.isVisible[b.first][b.second] = false;
}

void GameBall::moveBall(GLfloat delta)
{
    float rotationAngle = (2 + rand() % 5) * M_PI / 8.0f;

    rotateMatrix = glm::mat2(glm::vec2(cos(rotationAngle), sin(rotationAngle)),
                             glm::vec2(-sin(rotationAngle), cos(rotationAngle)));

    if(startingShot)
    {
        delta = 0.0f;
        startingShot = false;
    }

    if(normalVector[0] != 0.0f || normalVector[1] != 0.0f)
        velocity = glm::reflect(velocity, glm::normalize(normalVector));

    transformVector += velocity * delta;
    normalVector = glm::vec2(0.0f, 0.0f);

    for(auto & vc : collided)
        for(auto & e : vc)
        {
            e.first = e.second;
            e.second = false;
        }
}

void GameBall::brickScored(GameBrick & brick, int row, int column,
                           std::vector<std::pair<int, int>> & bricksHit)
{
    --brick.bricksLeft;
    collided[row][column].second = true;
    bricksHit.push_back(std::make_pair(row, column));
    std::cout << "TRAFIŁEŚ CEGŁĘ!! Zostało: " << brick.bricksLeft << "...\n\n";
}

#pragma endregion
