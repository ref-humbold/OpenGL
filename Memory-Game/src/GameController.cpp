#include "GameController.hpp"

using namespace glm;

GameController::GameController(int rows, int columns)
    : vertexBufferData{
            -0.9f, -0.9f,  0.9f, -0.9f, 0.9f,  0.9f, -0.9f,  0.9f,  // card
            -1.0f, -1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f,  1.0f,  // frame
             0.0f,  0.7f,  0.0f, -0.7f,  // pipe
            -0.7f, -0.7f,  0.7f,  0.7f, 0.7f, -0.7f, -0.7f,  0.7f,  // cross
             0.0f,  0.7f, -0.7f, -0.7f, 0.7f, -0.7f,  // triangle
            -0.7f,  0.0f,  0.0f,  0.7f, 0.7f,  0.0f,  0.0f, -0.7f  // square
        },
      fieldsCount{rows * columns},
      size{std::make_pair(rows, columns)}
{
    std::vector<bool> isSet(fieldsCount, false);

    srand(time(nullptr));
    visible.resize(fieldsCount, false);
    colours.resize(fieldsCount);
    signs.resize(fieldsCount);

    for(float i = -rows + 1; i <= rows; i += 2)
        for(float j = -columns + 1; j <= columns; j += 2)
            transforms.push_back(std::make_pair(i, j));

    for(size_t e = 0; e != isSet.size(); e += 2)
    {
        int i, j;
        Colour colour = static_cast<Colour>(rand() % 6);
        Sign sign = static_cast<Sign>(rand() % 4);

        do
            i = rand() % isSet.size();
        while(isSet[i]);

        do
            j = rand() % isSet.size();
        while(isSet[j] || j == i);

        colours[i] = colour;
        signs[i] = sign;
        colours[j] = colour;
        signs[j] = sign;
        isSet[i] = true;
        isSet[j] = true;
    }

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);
}

void GameController::drawGame(GLuint pID, int currentIndex,
                              const std::pair<int, int> & visibleIndices)
{
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

    drawCards(pID, Colour::Gray, transforms[currentIndex], 4);

    for(int i = 0; i < size.first * size.second; i++)
        if(visible[i] || i == visibleIndices.first || i == visibleIndices.second)
        {
            drawCards(pID, colours[i], transforms[i], 0);
            drawSign(pID, signs[i], transforms[i]);
        }
        else
            drawCards(pID, Colour::Black, transforms[i], 0);

    glDisableVertexAttribArray(0);
}

Key GameController::checkKeyPress(GLFWwindow * window)
{
    int action = GLFW_PRESS;

    if(glfwGetKey(window, GLFW_KEY_SPACE) == action)
        return Key::Select;
    else if(glfwGetKey(window, GLFW_KEY_UP) == action)
        return Key::MoveUp;
    else if(glfwGetKey(window, GLFW_KEY_DOWN) == action)
        return Key::MoveDown;
    else if(glfwGetKey(window, GLFW_KEY_LEFT) == action)
        return Key::MoveLeft;
    else if(glfwGetKey(window, GLFW_KEY_RIGHT) == action)
        return Key::MoveRight;

    return Key::None;
}

void GameController::checkKeyRelease(GLFWwindow * window, Key key)
{
    if(key != Key::None)
        do
            glfwPollEvents();
        while(glfwGetKey(window, static_cast<int>(key)) != GLFW_RELEASE);
}

int GameController::moveFrame(Key key, int currentIndex)
{
    int row = currentIndex / size.second, column = currentIndex % size.second;

    switch(key)
    {
        case Key::MoveUp:
            currentIndex = (row + 1) % size.first * size.second + column;
            break;

        case Key::MoveDown:
            currentIndex = (row - 1 + size.first) % size.first * size.second + column;
            break;

        case Key::MoveLeft:
            currentIndex = row * size.second + (column - 1 + size.second) % size.second;
            break;

        case Key::MoveRight:
            currentIndex = row * size.second + (column + 1) % size.second;
            break;

        default:
            break;
    }

    return currentIndex;
}

bool GameController::checkSame(const std::pair<int, int> & visibleIndices)
{
    return colours[visibleIndices.first] == colours[visibleIndices.second]
           && signs[visibleIndices.first] == signs[visibleIndices.second];
}

void GameController::drawCards(GLuint pID, Colour colour, std::pair<int, int> transformation,
                               int frameOffset)
{
    GLint scale = glGetUniformLocation(pID, "scale");
    GLint transform = glGetUniformLocation(pID, "transform");
    GLint color = glGetUniformLocation(pID, "fragmentColor");

    glUniform2f(scale, 0.8f / size.second, 0.8f / size.first);
    glUniform2f(transform, transformation.second, transformation.first);

    switch(colour)
    {
        case Colour::Red:
            glUniform3f(color, 1.0f, 0.0f, 0.0f);  // red
            break;

        case Colour::Green:
            glUniform3f(color, 0.0f, 1.0f, 0.0f);  // green
            break;

        case Colour::Blue:
            glUniform3f(color, 0.0f, 0.0f, 1.0f);  // blue
            break;

        case Colour::Cyan:
            glUniform3f(color, 0.0f, 1.0f, 1.0f);  // cyan
            break;

        case Colour::Magenta:
            glUniform3f(color, 1.0f, 0.0f, 1.0f);  // magenta
            break;

        case Colour::Yellow:
            glUniform3f(color, 1.0f, 1.0f, 0.0f);  // yellow
            break;

        case Colour::Black:
            glUniform3f(color, 0.0f, 0.0f, 0.0f);  // black
            break;

        case Colour::Gray:
            glUniform3f(color, 0.5f, 0.5f, 0.5f);  // grey
            break;
    }

    glDrawArrays(GL_TRIANGLE_FAN, frameOffset, 4);
}

void GameController::drawSign(GLuint pID, Sign sign, std::pair<int, int> transformation)
{
    GLint scale = glGetUniformLocation(pID, "scale");
    GLint transform = glGetUniformLocation(pID, "transform");
    GLint color = glGetUniformLocation(pID, "fragmentColor");

    glUniform2f(scale, 0.8f / size.second, 0.8f / size.first);
    glUniform2f(transform, transformation.second, transformation.first);
    glUniform3f(color, 0.0f, 0.0f, 0.0f);

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
