#include "GameController.hpp"
#include <algorithm>

GameController::GameController(int rows, int columns)
    : fieldsCount{rows * columns},
      size{std::make_pair(rows, columns)},
      vertexBufferData{
              -0.9f, -0.9f, 0.9f,  -0.9f, 0.9f, 0.9f,  -0.9f, 0.9f,  // card
              -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f, 1.0f,  -1.0f, 1.0f,  // frame
              0.0f,  0.7f,  0.0f,  -0.7f,  // pipe
              -0.7f, -0.7f, 0.7f,  0.7f,  0.7f, -0.7f, -0.7f, 0.7f,  // cross
              0.0f,  0.7f,  -0.7f, -0.7f, 0.7f, -0.7f,  // triangle
              -0.7f, 0.0f,  0.0f,  0.7f,  0.7f, 0.0f,  0.0f,  -0.7f  // square
      },
      colours_distrib{0, coloursCount - 1},
      signs_distrib{0, signsCount - 1},
      fields_distrib{0, fieldsCount - 1}
{
    for(int y = -rows + 1; y <= rows; y += 2)
        for(int x = -columns + 1; x <= columns; x += 2)
            transforms.push_back(glm::vec2(x, y));

    restart();

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);
}

void GameController::restart()
{
    visible.clear();
    cards.clear();

    int sameCardsRatio = 1 + fieldsCount / (coloursCount * signsCount);

    for(int i = 0; i < fieldsCount / 2; ++i)
    {
        int index1, index2;
        Colour colour;
        Sign sign;

        do
        {
            colour = static_cast<Colour>(colours_distrib(rand_eng));
            sign = static_cast<Sign>(signs_distrib(rand_eng));
        } while(sameCardsRatio < std::count_if(
                        std::begin(cards), std::end(cards),
                        [=](const auto & c) {
                            return c.second.first == colour && c.second.second == sign;
                        }));

        do
            index1 = fields_distrib(rand_eng);
        while(cards.find(index1) != cards.end());

        do
            index2 = fields_distrib(rand_eng);
        while(cards.find(index2) != cards.end() || index2 == index1);

        cards.emplace(index1, std::make_pair(colour, sign));
        cards.emplace(index2, std::make_pair(colour, sign));
    }
}

void GameController::drawGame(GLuint programID, int currentIndex,
                              const std::pair<int, int> & visibleIndices)
{
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

    drawCards(programID, Colour::Gray, transforms[currentIndex], 4);

    for(int i = 0; i < size.first * size.second; i++)
        if(isVisible(i) || i == visibleIndices.first || i == visibleIndices.second)
        {
            drawCards(programID, cards.at(i).first, transforms[i], 0);
            drawSign(programID, cards.at(i).second, transforms[i]);
        }
        else
            drawCards(programID, Colour::Black, transforms[i], 0);

    glDisableVertexAttribArray(0);
}

Key GameController::checkKeyPress(GLFWwindow * window)
{
    int action = GLFW_PRESS;

    if(glfwGetKey(window, GLFW_KEY_SPACE) == action)
        return Key::Select;

    if(glfwGetKey(window, GLFW_KEY_UP) == action)
        return Key::MoveUp;

    if(glfwGetKey(window, GLFW_KEY_DOWN) == action)
        return Key::MoveDown;

    if(glfwGetKey(window, GLFW_KEY_LEFT) == action)
        return Key::MoveLeft;

    if(glfwGetKey(window, GLFW_KEY_RIGHT) == action)
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
            currentIndex = ((row + 1) % size.first) * size.second + column;
            break;

        case Key::MoveDown:
            currentIndex = ((row - 1 + size.first) % size.first) * size.second + column;
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
    return cards.at(visibleIndices.first) == cards.at(visibleIndices.second);
}

void GameController::drawCards(GLuint programID, Colour colour, glm::vec2 transformation,
                               int frameOffset)
{
    GLint scale = glGetUniformLocation(programID, "scale");
    GLint transform = glGetUniformLocation(programID, "transform");
    GLint fragmentColor = glGetUniformLocation(programID, "fragmentColor");

    glUniform2f(scale, 0.8f / size.second, 0.8f / size.first);
    glUniform2f(transform, transformation[0], transformation[1]);

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

        case Colour::Black:
            glUniform3f(fragmentColor, 0.0f, 0.0f, 0.0f);
            break;

        case Colour::Gray:
            glUniform3f(fragmentColor, 0.5f, 0.5f, 0.5f);
            break;
    }

    glDrawArrays(GL_TRIANGLE_FAN, frameOffset, 4);
}

void GameController::drawSign(GLuint programID, Sign sign, glm::vec2 transformation)
{
    GLint scale = glGetUniformLocation(programID, "scale");
    GLint transform = glGetUniformLocation(programID, "transform");
    GLint fragmentColor = glGetUniformLocation(programID, "fragmentColor");

    glUniform2f(scale, 0.8f / size.second, 0.8f / size.first);
    glUniform2f(transform, transformation[0], transformation[1]);
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
