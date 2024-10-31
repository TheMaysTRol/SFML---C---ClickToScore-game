#include "Game.h"

using namespace std;

int main()
{
    //Init Game engine
    Game game;

    //Init srand
    srand(static_cast<unsigned>(time(NULL)));

    //Game loop
    while (game.Running())
    {
        //Update
        game.Update();
        //Render
        game.Render();
    }
    //End of application
    return 0;
}