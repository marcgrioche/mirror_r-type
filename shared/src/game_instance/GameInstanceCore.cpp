#include "../../include/game_instance/GameInstanceCore.hpp"

GameInstanceCore::GameInstanceCore(uint32_t lobbyId)
    : _lobbyId(lobbyId)
    , _isRunning(false)
    , _currentTick(0)
    , _stateChanged(false)
    , _gameWon(false)
    , _gameLost(false)
{
}

bool GameInstanceCore::hasStateChanged()
{
    bool changed = _stateChanged;
    _stateChanged = false;
    return changed;
}
