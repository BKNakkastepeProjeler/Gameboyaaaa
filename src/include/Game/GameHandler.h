#pragma once

#include "Game/BaseGame.h"

namespace GameHandler
{
    void Init();

    void Periodic();

    void EndGame(WinState state);
}