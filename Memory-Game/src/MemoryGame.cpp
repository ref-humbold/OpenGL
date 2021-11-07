#include <cstdlib>
#include <iostream>
#include "GLinit.hpp"
#include "GameController.hpp"
#include "Parameters.hpp"

struct GameState
{
    explicit GameState(const GameController & ctrl) : cardsCount{ctrl.fieldsCount}
    {
        restart();
    }

    void restart()
    {
        round = 1;
        cardPairsLeft = cardsCount / 2;
        currentPlace = std::make_pair(0, 0);
        visiblePlaces = VisiblePlaces();
        std::cout << "\n## NEW GAME ##\n";
    }

    void printRound()
    {
        std::cout << "** Round " << round << "\n";
    }

    const int cardsCount;
    int round, cardPairsLeft;
    std::pair<int, int> currentPlace;
    VisiblePlaces visiblePlaces;
};

int main(int argc, char * argv[])
{
    parameters params(argc, argv);
    GLProgram program = initializeGL();

    GameController ctrl(params.rows(), params.columns());
    GameState state(ctrl);

    state.printRound();

    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(program.programID);
        ctrl.drawGame(program.programID, state.currentPlace, state.visiblePlaces);
        glfwSwapBuffers(program.window);
        glfwPollEvents();

        if(state.cardPairsLeft > 0)
        {
            Key key = ctrl.checkKeyPress(program.window);

            if(key != Key::None)
            {
                ctrl.checkKeyRelease(program.window, key);

                if(state.visiblePlaces.second)
                    state.visiblePlaces = VisiblePlaces();
            }

            if(key == Key::Select && !ctrl.cardAt(state.currentPlace).visible)
            {
                if(!state.visiblePlaces.first)
                    state.visiblePlaces.first = state.currentPlace;
                else if(state.visiblePlaces.first.value() != state.currentPlace)
                {
                    state.visiblePlaces.second = state.currentPlace;

                    if(ctrl.checkSame(state.visiblePlaces))
                    {
                        ctrl.cardAt(state.visiblePlaces.first.value()).visible = true;
                        ctrl.cardAt(state.visiblePlaces.second.value()).visible = true;
                        --state.cardPairsLeft;
                        std::cout << "-- Matched! --\n";
                    }

                    if(state.cardPairsLeft == 0)
                        std::cout << "You won in " << state.round << " rounds.\n";
                    else
                    {
                        ++state.round;
                        state.printRound();
                    }
                }
            }
            else if(key != Key::None && key != Key::Select)
                state.currentPlace = ctrl.moveFrame(key, state.currentPlace);
        }
        else
        {
            Key key = ctrl.checkKeyPress(program.window);

            if(key == Key::Select)
            {
                ctrl.checkKeyRelease(program.window, key);
                ctrl.restart();
                state.restart();
            }
        }
    } while(glfwGetKey(program.window, GLFW_KEY_ESCAPE) != GLFW_PRESS
            && glfwWindowShouldClose(program.window) == 0);

    if(state.cardPairsLeft > 0)
        std::cout << "Game was interrupted.\n\n";

    return 0;
}
