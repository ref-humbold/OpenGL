#include "GameController.hpp"
#include <algorithm>

GameController::GameController(int rows, int columns)
    : fieldsCount{rows * columns},
      size{std::make_pair(rows, columns)},
      colours_distrib{0, coloursCount - 1},
      signs_distrib{0, signsCount - 1},
      fields_distrib{0, fieldsCount - 1}
{
    restart();
}

void GameController::restart()
{
    cards.clear();

    int sameCardsRatio = 1 + fieldsCount / (coloursCount * signsCount);

    for(int i = 0; i < fieldsCount; i += 2)
    {
        std::pair<int, int> place1, place2;
        Colour colour;
        Sign sign;

        do
        {
            colour = static_cast<Colour>(colours_distrib(rand_eng));
            sign = static_cast<Sign>(signs_distrib(rand_eng));
        } while(sameCardsRatio < std::count_if(
                        std::begin(cards), std::end(cards),
                        [=](const auto & c) { return c.second.matches(colour, sign); }));

        do
        {
            int ix = fields_distrib(rand_eng);
            place1 = std::make_pair(ix / size.second, ix % size.second);
        } while(cards.find(place1) != cards.end());

        do
        {
            int ix = fields_distrib(rand_eng);
            place2 = std::make_pair(ix / size.second, ix % size.second);
        } while(cards.find(place2) != cards.end() || place2 == place1);

        cards.emplace(place1, GameCard(size, place1, colour, sign));
        cards.emplace(place2, GameCard(size, place2, colour, sign));
    }
}

void GameController::drawGame(GLuint programID, const std::pair<int, int> & currentPlace,
                              const VisiblePlaces & visiblePlaces)
{
    for(auto && e : cards)
        e.second.draw(programID, visiblePlaces.contains(e.first), e.first == currentPlace);
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

std::pair<int, int> GameController::moveFrame(Key key, const std::pair<int, int> & currentPlace)
{
    std::pair<int, int> nextPlace;
    int row = currentPlace.first, column = currentPlace.second;

    switch(key)
    {
        case Key::MoveUp:
            nextPlace = std::make_pair((row + 1) % size.first, column);
            break;

        case Key::MoveDown:
            nextPlace = std::make_pair((row - 1 + size.first) % size.first, column);
            break;

        case Key::MoveLeft:
            nextPlace = std::make_pair(row, (column - 1 + size.second) % size.second);
            break;

        case Key::MoveRight:
            nextPlace = std::make_pair(row, (column + 1) % size.second);
            break;

        default:
            break;
    }

    return nextPlace;
}

bool GameController::checkSame(const VisiblePlaces & visiblePlaces)
{
    return cards.at(*visiblePlaces.first).matches(cards.at(*visiblePlaces.second));
}
