#include "Game/BaseGame.h"

#include "Game/Games/SnakeGame.h"
#include "Game/Games/TurretGame.h"

#include "Display/MatrixDisplay.h"

namespace GameHandler
{
    BaseGame* Games[] = {
        new SnakeGame(),
        new TurretGame()
    };
    
    const uint8_t GameCount = sizeof(Games) / sizeof(BaseGame*);

    uint8_t SelectedGame;

    bool Playing = false;
    
    void SelectGame(int8_t newGame)
    {
        if(newGame < 0) SelectedGame = GameCount - 1;
        else if(newGame >= GameCount) SelectedGame = 0;
        else SelectedGame = newGame;

        MatrixDisplay::Set(Games[SelectedGame]->GetIcon());
    }

    void Init()
    {
        SelectGame(0);
    }

    void PlayGame()
    {
        if(Playing) return;

        Playing = true;

        Games[SelectedGame]->Begin();
    }

    void EndGame(WinState state)
    {
        Games[SelectedGame]->EndAnimation(state);
        
        Playing = false;

        SelectGame(SelectedGame);

    }

    void Periodic()
    {
        if(Playing)
        {
            Games[SelectedGame]->Periodic();
            return;
        }

        Point dir = Input::GetDirectionVector();
        if(dir.X == 0)
        {
            if(dir.Y != 0) PlayGame();

            return;
        }

        SelectGame(SelectedGame + dir.X);
        
    }
}