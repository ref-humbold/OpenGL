#include "GameElement.hpp"

using namespace glm;

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

GameBoard::GameBoard()
    : vbDataHexagon{0.0f,  0.0f, 0.4f,  0.0f,      0.2f, 0.34641f,  -0.2f, 0.34641f,
                    -0.4f, 0.0f, -0.2f, -0.34641f, 0.2f, -0.34641f, 0.4f,  0.0f},
      cbDataHexagon{0.0f, 0.6f, 0.6f,  0.0f, 0.0f, 0.25f, 0.0f, 0.0f, 0.25f, 0.0f, 0.0f, 0.25f,
                    0.0f, 0.0f, 0.25f, 0.0f, 0.0f, 0.25f, 0.0f, 0.0f, 0.25f, 0.0f, 0.0f, 0.25f},
      vbDataTriangle{0.0f, 0.0f, 0.0f, 1.0f, 0.57735f, 0.0f},
      cbDataTriangle{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
      scaleMatrix{mat2(vec2(1.0f, 0.0f), vec2(0.0f, 1.0f))},
      rotateMatrix{mat2(vec2(1.0f, 0.0f), vec2(0.0f, 1.0f))},
      transformVector{vec2(0.0f, 0.0f)}
{
    vertexBufferHexagon = createVertexBuffer(vbDataHexagon, sizeof(vbDataHexagon));
    colorBufferHexagon = createVertexBuffer(cbDataHexagon, sizeof(cbDataHexagon));
    vertexBufferTriangle = createVertexBuffer(vbDataTriangle, sizeof(vbDataTriangle));
    colorBufferTriangle = createVertexBuffer(cbDataTriangle, sizeof(cbDataTriangle));
    normVecs.resize(5);
}

void GameBoard::drawBackground(GLuint pID)
{
    scaleMatrix = mat2(vec2(0.1f, 0.0f), vec2(0.0f, 0.1f));

    loadBuffer(vertexBufferHexagon, colorBufferHexagon);

    GLint scaleMat = glGetUniformLocation(pID, "scaleMat");
    GLint rotateMat = glGetUniformLocation(pID, "rotateMat");
    GLint transformVec = glGetUniformLocation(pID, "transformVec");

    for(int j = -30; j <= 30; ++j)
        for(int i = -9; i <= 9; ++i)
        {
            transformVector = scaleMatrix * vec2(1.2f * i + 0.6f * abs(j % 2), 0.34641f * j);

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

    scaleMatrix = mat2(vec2(1.0f, 0.0f), vec2(0.0f, 1.0f));
    rotateMatrix = mat2(vec2(1.0f, 0.0f), vec2(0.0f, 1.0f));
    transformVector = vec2(-1.0f, -1.0f);

    drawOneTriangle(pID);

    // left top

    scaleMatrix = mat2(vec2(-1.0f, 0.0f), vec2(0.0f, 1.0f));
    rotateMatrix = mat2(vec2(-1.0f, 0.0f), vec2(0.0f, -1.0f));
    transformVector = vec2(-1.0f, 1.0f);

    drawOneTriangle(pID);

    // right top

    scaleMatrix = mat2(vec2(1.0f, 0.0f), vec2(0.0f, 1.0f));
    rotateMatrix = mat2(vec2(-1.0f, 0.0f), vec2(0.0f, -1.0f));
    transformVector = vec2(1.0f, 1.0f);

    drawOneTriangle(pID);

    // right bottom

    scaleMatrix = mat2(vec2(-1.0f, 0.0f), vec2(0.0f, 1.0f));
    rotateMatrix = mat2(vec2(1.0f, 0.0f), vec2(0.0f, 1.0f));
    transformVector = vec2(1.0f, -1.0f);

    drawOneTriangle(pID);
}

void GameBoard::countNormalVectors()
{
    vec2 normal = vec2(1.0f, 0.57735f);

    // left bottom

    scaleMatrix = mat2(vec2(1.0f, 0.0f), vec2(0.0f, 1.0f));
    rotateMatrix = mat2(vec2(1.0f, 0.0f), vec2(0.0f, 1.0f));

    normVecs[0] = normalize(rotateMatrix * scaleMatrix * normal);

    // left top

    scaleMatrix = mat2(vec2(-1.0f, 0.0f), vec2(0.0f, 1.0f));
    rotateMatrix = mat2(vec2(-1.0f, 0.0f), vec2(0.0f, -1.0f));

    normVecs[1] = normalize(rotateMatrix * scaleMatrix * normal);

    // top

    normVecs[2] = normalize(vec2(0.0f, -1.0f));

    // right top

    scaleMatrix = mat2(vec2(1.0f, 0.0f), vec2(0.0f, 1.0f));
    rotateMatrix = mat2(vec2(-1.0f, 0.0f), vec2(0.0f, -1.0f));

    normVecs[3] = normalize(rotateMatrix * scaleMatrix * normal);

    // right bottom

    scaleMatrix = mat2(vec2(-1.0f, 0.0f), vec2(0.0f, 1.0f));
    rotateMatrix = mat2(vec2(1.0f, 0.0f), vec2(0.0f, 1.0f));

    normVecs[4] = normalize(rotateMatrix * scaleMatrix * normal);
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
    : vbDataRect{0.5f, 0.25f, 0.5f, -0.25f, -0.5f, -0.25f, -0.5f, 0.25f},
      cbDataRect{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
      cbDataRectBorder{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
      scaleMatrix{mat2(vec2(0.1f, 0.0f), vec2(0.0f, 0.1f))},
      rotateMatrix{mat2(vec2(1.0f, 0.0f), vec2(0.0f, 1.0f))},
      transformVector{vec2(0.0f, 0.0f)}
{
    vertexBufferRect = createVertexBuffer(vbDataRect, sizeof(vbDataRect));
    colorBufferRect = createVertexBuffer(cbDataRect, sizeof(cbDataRect));
    colorBufferRectBorder = createVertexBuffer(cbDataRectBorder, sizeof(cbDataRectBorder));

    bricksLeft = 74;
    isVisible.resize(6);

    for(auto & vc : isVisible)
        vc.resize(13, true);

    isVisible[4][0] = false;
    isVisible[4][12] = false;
    isVisible[5][0] = false;
    isVisible[5][12] = false;
}

void GameBrick::drawAllBricks(GLuint pID)
{
    for(int col = 0; col <= 5; ++col)
        for(int i = 0; i <= 12; ++i)
        {
            transformVector = vec2(0.1f * (i - 6), 0.6f + 0.05f * col);

            if(isVisible[col][i])
            {
                drawRect(pID, col + 1);
                drawRectBorder(pID);
            }
        }
}

void GameBrick::drawRect(GLuint pID, int col)
{
    switch(col)
    {
        case 1:  // blue
            for(int i = 0; i < 12; ++i)
                cbDataRect[i] = i % 3 == 2 ? 1.0f : 0.0f;

            break;

        case 2:  // green
            for(int i = 0; i < 12; ++i)
                cbDataRect[i] = i % 3 == 1 ? 1.0f : 0.0f;

            break;

        case 3:  // cyan
            for(int i = 0; i < 12; ++i)
                cbDataRect[i] = i % 3 != 0 ? 1.0f : 0.0f;

            break;

        case 4:  // red
            for(int i = 0; i < 12; ++i)
                cbDataRect[i] = i % 3 == 0 ? 1.0f : 0.0f;

            break;

        case 5:  // magenta
            for(int i = 0; i < 12; ++i)
                cbDataRect[i] = i % 3 != 1 ? 1.0f : 0.0f;

            break;

        case 6:  // yellow
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
      scaleMatrix{mat2(vec2(0.1f, 0.0f), vec2(0.0f, 0.1f))},
      rotateMatrix{mat2(vec2(1.0f, 0.0f), vec2(0.0f, 1.0f))},
      transformVector{vec2(0.0f, -0.95f)},
      velocity{1.0f}
{
    vertexBufferPaddle = createVertexBuffer(vbDataPaddle, sizeof(vbDataPaddle));
    colorBufferPaddle = createVertexBuffer(cbDataPaddle, sizeof(cbDataPaddle));
}

void GamePaddle::restart()
{
    transformVector = vec2(0.0f, -0.95f);
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
    transformVector[0] = max(-0.4f, transformVector[0] - velocity * delta);
}

void GamePaddle::moveRight(GLfloat delta)
{
    transformVector[0] = min(0.4f, transformVector[0] + velocity * delta);
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
      scaleMatrix{mat2(vec2(0.08f, 0.0f), vec2(0.0f, 0.08f))},
      rotateMatrix{mat2(vec2(1.0f, 0.0f), vec2(0.0f, 1.0f))},
      transformVector{vec2(0.0f, -0.9f)},
      vNorm{vec2(0.0f, 0.0f)},
      angleMult{0},
      radius{length(scaleMatrix * vec2(0.13660254f, 0.13660254f))},
      separator{1.25f * radius},
      velocityDistance{50.0f * radius},
      startingShot{true}
{
    vertexBufferBall = createVertexBuffer(vbDataBall, sizeof(vbDataBall));
    colorBufferBall = createVertexBuffer(cbDataBall, sizeof(cbDataBall));
    vertexBufferCross = createVertexBuffer(vbDataCross, sizeof(vbDataCross));
    colorBufferCross = createVertexBuffer(cbDataCross, sizeof(cbDataCross));

    velocity = velocityDistance * normalize(vec2((-10.0f + rand() % 21) / 10.0f, 1.0f));
    collided.resize(7);

    for(int i = 0; i < 5; ++i)
        collided[i].resize(13, std::make_pair(false, false));

    collided[5].resize(5, std::make_pair(false, false));
    collided[6].resize(1, std::make_pair(false, false));
}

void GameBall::restart()
{
    transformVector = vec2(0.0f, -0.9f);
    startingShot = true;
    velocity = velocityDistance * normalize(vec2((-10.0f + rand() % 21) / 10.0f, 1.0f));
    angleMult = 0;
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

GLfloat GameBall::countDistance(vec2 pt, vec2 nl, vec2 pl)
{
    vec2 nvl = normalize(nl), ptd = pl - pt;
    GLfloat ilosk = dot(ptd, nvl);

    return length(ptd - nvl * ilosk);
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
    if(countDistance(transformVector, vec2(0.57735f, -1.0f), vec2(-1.0f, 0.0f)) <= separator
       && !collided[5][0].first)
    {
        vNorm += board.normVecs[0];
        collided[5][0].second = true;
    }
    else if(countDistance(transformVector, vec2(0.57735f, 1.0f), vec2(-1.0f, 0.0f)) <= separator
            && !collided[5][1].first)
    {
        vNorm += board.normVecs[1];
        collided[5][1].second = true;
    }
    else if(abs(0.975f - transformVector[1]) <= separator && !collided[5][2].first)
    {
        vNorm += board.normVecs[2];
        collided[5][2].second = true;
    }
    else if(countDistance(transformVector, vec2(-0.57735f, 1.0f), vec2(1.0f, 0.0f)) <= separator
            && !collided[5][3].first)
    {
        vNorm += board.normVecs[3];
        collided[5][3].second = true;
    }
    else if(countDistance(transformVector, vec2(-0.57735f, -1.0f), vec2(1.0f, 0.0f)) <= separator
            && !collided[5][4].first)
    {
        vNorm += board.normVecs[4];
        collided[5][4].second = true;
    }
}

void GameBall::checkCollisionPaddle(GamePaddle & paddle)
{
    GLfloat padPosX = paddle.getPosX(), dist = abs(paddle.getSurf() - transformVector[1]);

    if(isInRange(transformVector[0], padPosX - 0.06f, padPosX + 0.06f) && dist <= separator
       && !collided[6][0].first)
    {
        GLfloat rnd = rand() % 2 == 0 ? (1 + rand() % 10) * 0.005f : -(1 + rand() % 8) * 0.005f;

        vNorm += normalize(vec2(rnd, 1.0f));
        collided[6][0].second = true;
    }
}

void GameBall::checkCollisionBrick(GameBrick & brick)
{
    std::vector<std::pair<GLfloat, GLfloat>> bricksHit;

    auto getBrickPos = [](vec2 v) -> vec2 {
        return vec2(floor(10 * v[0] + 0.5f) * 0.1f, floor(20 * v[1] + 0.5f) * 0.05f);
    };

    auto brickScored = [](GameBrick & brick, int br, int bc,
                          std::vector<std::pair<GLfloat, GLfloat>> & bricksHit) -> void {
        --brick.bricksLeft;
        bricksHit.push_back(std::make_pair(br, bc));
        std::cout << "TRAFIŁEŚ CEGŁĘ!! Zostało: " << brick.bricksLeft << "...\n\n";
    };

    if(transformVector[1] >= 0.525f)
    {
        GLfloat brickBorders[4];
        vec2 brickCorners[4];
        GLfloat brickDist[8];
        vec2 brPos = getBrickPos(transformVector);

        brickBorders[0] = brPos[1] + 0.025f;  // up
        brickBorders[1] = brPos[1] - 0.025f;  // down
        brickBorders[2] = brPos[0] - 0.05f;  // left
        brickBorders[3] = brPos[0] + 0.05f;  // right

        brickCorners[0] = vec2(brickBorders[2], brickBorders[0]);  // left up
        brickCorners[1] = vec2(brickBorders[2], brickBorders[1]);  // left down
        brickCorners[2] = vec2(brickBorders[3], brickBorders[0]);  // right up
        brickCorners[3] = vec2(brickBorders[3], brickBorders[1]);  // right down

        brickDist[0] = abs(brickBorders[0] - transformVector[1]);
        brickDist[1] = abs(brickBorders[1] - transformVector[1]);
        brickDist[2] = abs(brickBorders[2] - transformVector[0]);
        brickDist[3] = abs(brickBorders[3] - transformVector[0]);

        brickDist[4] = distance(brickCorners[0], transformVector);
        brickDist[5] = distance(brickCorners[1], transformVector);
        brickDist[6] = distance(brickCorners[2], transformVector);
        brickDist[7] = distance(brickCorners[3], transformVector);

        if(brPos[1] > 0.55f)
        {
            int rw = (int)floor((brPos[1] - 0.6f) * 20.0f + 0.5);
            int cl = (int)floor((brPos[0] + 0.6f) * 10.0f + 0.5);

            if(isInRange(rw, 0, 5) && isInRange(cl, 0, 12) && !collided[rw][cl].first
               && brick.isVisible[rw][cl])
            {
                GLfloat minim = 1.0f;
                int minidx = 0;

                for(int i = 0; i < 4; ++i)
                    if(brickDist[i] < minim)
                        minidx = i;

                switch(minidx)
                {
                    case 0:
                        vNorm += normalize(vec2(0.0f, 1.0f));
                        break;

                    case 1:
                        vNorm += normalize(vec2(0.0f, -1.0f));
                        break;

                    case 2:
                        vNorm += normalize(vec2(-1.0f, 0.0f));
                        break;

                    case 3:
                        vNorm += normalize(vec2(1.0f, 0.0f));
                        break;
                }

                collided[rw][cl].second = true;
                brickScored(brick, rw, cl, bricksHit);
            }
        }

        if(brickDist[0] <= separator)  // up
        {
            vec2 brPosC = brPos + vec2(0.0f, 0.05f);

            int rw = (int)floor((brPosC[1] - 0.6f) * 20.0f + 0.5);
            int cl = (int)floor((brPosC[0] + 0.6f) * 10.0f + 0.5);

            if(isInRange(rw, 0, 5) && isInRange(cl, 0, 12) && !collided[rw][cl].first
               && brick.isVisible[rw][cl])
            {
                vNorm += normalize(vec2(0.0f, -1.0f));
                collided[rw][cl].second = true;
                brickScored(brick, rw, cl, bricksHit);
            }
        }

        if(brickDist[1] <= separator)  // down
        {
            vec2 brPosC = brPos + vec2(0.0f, -0.05f);

            int rw = (int)floor((brPosC[1] - 0.6f) * 20.0f + 0.5);
            int cl = (int)floor((brPosC[0] + 0.6f) * 10.0f + 0.5);

            if(isInRange(rw, 0, 5) && isInRange(cl, 0, 12) && !collided[rw][cl].first
               && brick.isVisible[rw][cl])
            {
                vNorm += normalize(vec2(0.0f, 1.0f));
                collided[rw][cl].second = true;
                brickScored(brick, rw, cl, bricksHit);
            }
        }

        if(brickDist[2] <= separator)  // left
        {
            vec2 brPosC = brPos + vec2(-0.1f, 0.0f);

            int rw = (int)floor((brPosC[1] - 0.6f) * 20.0f + 0.5);
            int cl = (int)floor((brPosC[0] + 0.6f) * 10.0f + 0.5);

            if(isInRange(rw, 0, 5) && isInRange(cl, 0, 12) && !collided[rw][cl].first
               && brick.isVisible[rw][cl])
            {
                vNorm += normalize(vec2(1.0f, 0.0f));
                collided[rw][cl].second = true;
                brickScored(brick, rw, cl, bricksHit);
            }
        }

        if(brickDist[3] <= separator)  // right
        {
            vec2 brPosC = brPos + vec2(0.1f, 0.0f);

            int rw = (int)floor((brPosC[1] - 0.6f) * 20.0f + 0.5);
            int cl = (int)floor((brPosC[0] + 0.6f) * 10.0f + 0.5);

            if(isInRange(rw, 0, 5) && isInRange(cl, 0, 12) && !collided[rw][cl].first
               && brick.isVisible[rw][cl])
            {
                vNorm += normalize(vec2(-1.0f, 0.0f));
                collided[rw][cl].second = true;
                brickScored(brick, rw, cl, bricksHit);
            }
        }

        if(brickDist[4] <= radius)  // left up
        {
            vec2 brPosC = brPos + vec2(-0.1f, 0.05f);

            int rw = (int)floor((brPosC[1] - 0.6f) * 20.0f + 0.5);
            int cl = (int)floor((brPosC[0] + 0.6f) * 10.0f + 0.5);

            if((rw == 0 && isInRange(cl, 0, 11) && !collided[rw][cl].first
                && brick.isVisible[rw][cl] && !brick.isVisible[rw][cl + 1])
               || (isInRange(rw, 1, 5) && isInRange(cl, 0, 11) && !collided[rw][cl].first
                   && brick.isVisible[rw][cl] && !brick.isVisible[rw][cl + 1]
                   && !brick.isVisible[rw - 1][cl]))
            {
                vNorm += velocity[1] < 0 ? normalize(vec2(1.0f, 0.0f))
                                         : normalize(vec2(1.0f, -1.0f));
                collided[rw][cl].second = true;
                brickScored(brick, rw, cl, bricksHit);
            }
        }

        if(brickDist[5] <= radius)  // left down
        {
            vec2 brPosC = brPos + vec2(-0.1f, -0.05f);

            int rw = (int)floor((brPosC[1] - 0.6f) * 20.0f + 0.5);
            int cl = (int)floor((brPosC[0] + 0.6f) * 10.0f + 0.5);

            if((rw == 5 && isInRange(cl, 0, 11) && !collided[rw][cl].first
                && brick.isVisible[rw][cl] && !brick.isVisible[rw][cl + 1])
               || (isInRange(rw, 0, 4) && isInRange(cl, 0, 11) && !collided[rw][cl].first
                   && brick.isVisible[rw][cl] && !brick.isVisible[rw][cl + 1]
                   && !brick.isVisible[rw + 1][cl]))
            {
                vNorm +=
                        velocity[1] > 0 ? normalize(vec2(1.0f, 0.0f)) : normalize(vec2(1.0f, 1.0f));
                collided[rw][cl].second = true;
                brickScored(brick, rw, cl, bricksHit);
            }
        }

        if(brickDist[6] <= radius)  // right up
        {
            vec2 brPosC = brPos + vec2(0.1f, 0.05f);

            int rw = (int)floor((brPosC[1] - 0.6f) * 20.0f + 0.5);
            int cl = (int)floor((brPosC[0] + 0.6f) * 10.0f + 0.5);

            if((rw == 0 && isInRange(cl, 1, 12) && !collided[rw][cl].first
                && brick.isVisible[rw][cl] && !brick.isVisible[rw][cl - 1])
               || (isInRange(rw, 1, 5) && isInRange(cl, 1, 12) && !collided[rw][cl].first
                   && brick.isVisible[rw][cl] && !brick.isVisible[rw][cl - 1]
                   && !brick.isVisible[rw - 1][cl]))
            {
                vNorm += velocity[1] < 0 ? normalize(vec2(-1.0f, 0.0f))
                                         : normalize(vec2(-1.0f, -1.0f));
                collided[rw][cl].second = true;
                brickScored(brick, rw, cl, bricksHit);
            }
        }

        if(brickDist[7] <= radius)  // right down
        {
            vec2 brPosC = brPos + vec2(0.1f, -0.05f);

            int rw = (int)floor((brPosC[1] - 0.6f) * 20.0f + 0.5);
            int cl = (int)floor((brPosC[0] + 0.6f) * 10.0f + 0.5);

            if((rw == 5 && isInRange(cl, 1, 12) && !collided[rw][cl].first
                && brick.isVisible[rw][cl] && !brick.isVisible[rw][cl - 1])
               || (isInRange(rw, 0, 4) && isInRange(cl, 1, 12) && !collided[rw][cl].first
                   && brick.isVisible[rw][cl] && !brick.isVisible[rw][cl - 1]
                   && !brick.isVisible[rw + 1][cl]))
            {
                vNorm += velocity[1] > 0 ? normalize(vec2(-1.0f, 0.0f))
                                         : normalize(vec2(-1.0f, 1.0f));
                collided[rw][cl].second = true;
                brickScored(brick, rw, cl, bricksHit);
            }
        }
    }

    for(auto & b : bricksHit)
        brick.isVisible[b.first][b.second] = false;
}

void GameBall::moveBall(GLfloat delta)
{
    angleMult = (angleMult + 1) % 8;

    float rad = angleMult * M_PI / 8.0f;

    rotateMatrix = mat2(vec2(cos(rad), sin(rad)), vec2(-sin(rad), cos(rad)));

    if(startingShot)
    {
        delta = 0.0f;
        startingShot = false;
    }

    if(vNorm[0] != 0.0f || vNorm[1] != 0.0f)
        velocity = reflect(velocity, normalize(vNorm));

    transformVector += velocity * delta;
    vNorm = vec2(0.0f, 0.0f);

    for(auto & vc : collided)
        for(auto & e : vc)
        {
            e.first = e.second;
            e.second = false;
        }
}

#pragma endregion
