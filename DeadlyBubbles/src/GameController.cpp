#include "GameController.hpp"

using namespace glm;

GameController::GameController(GLFWwindow * window, bool training)
    : points{1.0},
      cameraPos{vec3(0.0f, 0.0f, 3.0f)},
      cameraDir{vec3(0.0f, 0.0f, 0.0f)},
      viewInside{false},
      training{training},
      fov{PI_CONST / 4},
      persBegin{1.0f},
      persStep{4.0f}
{
    restartMoves();

    lightSource = vec4(0.0f, aqua.getSide(), 0.0f, 1.0f);

    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    view = lookAt(cameraPos, cameraDir, vec3(0.0f, 1.0f, 0.0f));
    proj = perspective(fov, (1.0f * windowWidth) / windowHeight, persBegin, persBegin + persStep);
}

void GameController::drawGame(GLuint pID)
{
    player.drawPlayer(pID, view, proj, lightSource);
    bubble.drawAllBubbles(pID, view, proj, lightSource);
    aqua.drawCube(pID, view, proj, lightSource);
}

void GameController::restart()
{
    player.restart(0.9f * aqua.getSide());
    bubble.elements.clear();
    setCamera();
}

void GameController::restartMoves()
{
    playerMoves.clear();
    playerMoves.emplace(Key::MoveFront, vec4(0.0f, 0.0f, -1.0f, 0.0f));
    playerMoves.emplace(Key::MoveBack, vec4(0.0f, 0.0f, 1.0f, 0.0f));
    playerMoves.emplace(Key::MoveUp, vec4(0.0f, 1.0f, 0.0f, 0.0f));
    playerMoves.emplace(Key::MoveDown, vec4(0.0f, -1.0f, 0.0f, 0.0f));
    playerMoves.emplace(Key::MoveLeft, vec4(-1.0f, 0.0f, 0.0f, 0.0f));
    playerMoves.emplace(Key::MoveRight, vec4(1.0f, 0.0f, 0.0f, 0.0f));
}

void GameController::setCamera()
{
    if(viewInside)
    {
        vec3 position = player.getRadPos().second;

        cameraPos = vec3(position[0], position[1], position[2] + 0.1f);
        cameraDir = vec3(position[0], position[1], position[2]);
        persBegin = 0.05f;
        persStep = 2.0f;
    }
    else
    {
        cameraPos = vec3(0.0f, 0.0f, 3.0f);
        cameraDir = vec3(0.0f, 0.0f, 0.0f);
        persBegin = 1.0f;
        persStep = 4.0f;
    }

    fov = PI_CONST / 4;
    view = lookAt(cameraPos, cameraDir, vec3(0.0f, 1.0f, 0.0f));
    proj = perspective(fov, (1.0f * windowWidth) / windowHeight, persBegin, persBegin + persStep);
    restartMoves();
}

void GameController::changeCamera()
{
    viewInside = !viewInside;
    setCamera();
}

void GameController::viewScale(GLfloat zoom)
{
    fov *= zoom;
    fov = min(fov, PI_CONST / 2);
    fov = max(fov, PI_CONST / 6);
    proj = perspective(fov, (1.0f * windowWidth) / windowHeight, persBegin, persBegin + persStep);
}

void GameController::viewRotate(GLfloat angleRad, vec3 axis)
{
    if(viewInside)
    {
        mat4 tr1 = mat4(vec4(1.0f, 0.0f, 0.0f, 0.0f), vec4(0.0f, 1.0f, 0.0f, 0.0f),
                        vec4(0.0f, 0.0f, 1.0f, 0.0f), vec4(0.0f, 0.0f, 0.1f, 1.0f));
        mat4 rt = toMat4(angleAxis(angleRad, axis));
        mat4 rtMv = toMat4(angleAxis(-angleRad, axis));
        mat4 tr2 = mat4(vec4(1.0f, 0.0f, 0.0f, 0.0f), vec4(0.0f, 1.0f, 0.0f, 0.0f),
                        vec4(0.0f, 0.0f, 1.0f, 0.0f), vec4(0.0f, 0.0f, -0.1f, 1.0f));

        for(auto & move : playerMoves)
            move.second = rtMv * move.second;

        view = tr2 * rt * tr1 * view;
    }
    else
    {
        vec4 axisObj4 = inverse(view) * vec4(axis[0], axis[1], axis[2], 0.0f);
        vec3 axisObj = vec3(axisObj4[0], axisObj4[1], axisObj4[2]);

        view = view * toMat4(angleAxis(angleRad, axisObj));
    }
}

int GameController::checkCollisionBubble()
{
    std::pair<GLfloat, vec3> ply = player.getRadPos();

    for(auto it = bubble.elements.begin(); it != bubble.elements.end(); ++it)
    {
        if(distance(std::get<1>(*it), ply.second) <= std::get<0>(*it) + ply.first)
            return std::get<3>(*it);
    }

    return 0;
}

bool GameController::checkEndRound()
{
    std::pair<GLfloat, vec3> ply = player.getRadPos();
    std::tuple<GLfloat, vec3, vec3> egb = bubble.endGameBubble;

    return distance(std::get<1>(egb), ply.second) <= std::get<0>(egb) + ply.first;
}

void GameController::deletePointedBubble(int ix)
{
    bubble.elements.remove_if(
            [=](std::tuple<GLfloat, vec3, vec3, int> b) { return std::get<3>(b) == ix; });
}

GLfloat GameController::moveBubbles(GLfloat delta, GLfloat counter, int freq)
{
    bubble.move(delta, aqua.getSide());

    if(!training && counter >= 0.2f / freq)
    {
        bubble.showUp(aqua.getSide());
        counter = 0.0f;
    }

    return counter;
}

void GameController::movePlayer(GLfloat delta, const std::vector<Key> & keys)
{
    GLfloat side = aqua.getSide();
    std::pair<GLfloat, vec3> ply = player.getRadPos();
    vec3 moveOfPlayer = vec3(0.0f, 0.0f, 0.0f);

    for(Key k : keys)
    {
        moveOfPlayer[0] += playerMoves.at(k)[0];
        moveOfPlayer[1] += playerMoves.at(k)[1];
        moveOfPlayer[2] += playerMoves.at(k)[2];
    }

    vec3 moveVector = moveOfPlayer * delta;
    vec3 nextPos = ply.second + moveVector;

    if(nextPos[0] > side - ply.first || nextPos[0] < -side + ply.first
       || nextPos[1] > side - ply.first || nextPos[1] < -side + ply.first
       || nextPos[2] > side - ply.first || nextPos[2] < -side + ply.first)
        moveVector = vec3(0.0f, 0.0f, 0.0f);

    if(viewInside)
    {
        player.move(moveVector);
        view = translate(view, -moveVector);
    }

    player.move(moveVector);
}

vec3 GameController::getMousePos(GLFWwindow * window)
{
    double x, y;

    glfwGetCursorPos(window, &x, &y);

    vec3 res = vec3(2.0f * x / windowWidth - 1.0f, -(2.0f * y / windowHeight - 1.0f), 0.0f);
    GLfloat ln = res[0] * res[0] + res[1] * res[1];

    if(ln <= 1.0f)
        res[2] = sqrt(1.0f - ln);

    return res;
}

std::vector<Key> GameController::checkKeyPress(GLFWwindow * window, const std::vector<Key> & keys)
{
    std::vector<Key> pressed;
    auto was_pressed = [=](Key k) -> bool {
        return glfwGetKey(window, static_cast<int>(k)) == GLFW_PRESS;
    };

    std::copy_if(keys.begin(), keys.end(), std::back_inserter(pressed), was_pressed);
    return pressed;
}

bool GameController::checkMouseAction(GLFWwindow * window, int action)
{
    return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == action;
}

void GameController::checkKeyReleased(GLFWwindow * window, Key key)
{
    do
        glfwPollEvents();
    while(glfwGetKey(window, static_cast<int>(key)) != GLFW_RELEASE);
}
