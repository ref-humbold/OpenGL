#ifndef GAME_CONTROLLER_HPP_
#define GAME_CONTROLLER_HPP_

#include <cstdlib>
#include <iostream>
#include <map>
#include <optional>
#include <random>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "GameCard.hpp"

enum class Key : int
{
    None = GLFW_KEY_UNKNOWN,
    Select = GLFW_KEY_SPACE,
    MoveUp = GLFW_KEY_UP,
    MoveDown = GLFW_KEY_DOWN,
    MoveLeft = GLFW_KEY_LEFT,
    MoveRight = GLFW_KEY_RIGHT
};

struct VisiblePlaces
{
    VisiblePlaces() : first{std::nullopt}, second{std::nullopt}
    {
    }

    bool contains(const std::pair<int, int> & place) const
    {
        return (first && place == first.value()) || (second && place == second.value());
    }

    std::optional<std::pair<int, int>> first;
    std::optional<std::pair<int, int>> second;
};

class GameController
{
public:
    GameController(int rows, int columns);

    GameCard & cardAt(const std::pair<int, int> place)
    {
        return cards.at(place);
    }

    void restart();
    void drawGame(GLuint programID, const std::pair<int, int> & currentPlace,
                  const VisiblePlaces & visiblePlaces);
    Key checkKeyPress(GLFWwindow * window);
    void checkKeyRelease(GLFWwindow * window, Key key);
    std::pair<int, int> moveFrame(Key key, const std::pair<int, int> & currentPlace);
    bool checkSame(const VisiblePlaces & visiblePlaces);

    const int fieldsCount;

private:
    static constexpr int coloursCount = 8, signsCount = 4;
    const std::pair<int, int> size;
    std::map<std::pair<int, int>, GameCard> cards;
    std::default_random_engine rand_eng;
    std::uniform_int_distribution<int> colours_distrib;
    std::uniform_int_distribution<int> signs_distrib;
    std::uniform_int_distribution<int> fields_distrib;
};

#endif
