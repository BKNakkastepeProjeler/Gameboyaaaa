#include "Game/BaseGame.h"

#include "Game/Games/SnakeGame.h"

#include "Display/MatrixDisplay.h"

namespace GameHandler
{
    BaseGame* Games[] = {
        new SnakeGame()
    };
    
    void Init()
    {
        Games[0]->Begin();
        MatrixDisplay::Set(Games[0]->GetIcon());

        Games[0]->Begin();
    }

    void Periodic()
    {
        Games[0]->Periodic();
    }
}