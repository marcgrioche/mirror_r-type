/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Game Prediction - Client-side prediction and reconciliation
*/

#include "Game.hpp"
#include "ecs/components/AllComponents.hpp"
#include <cmath>
#include <iostream>

void Game::updateClientPrediction()
{
    if (!m_clientGameInstance || m_isLocalMode) {
        return;
    }

    auto currentInputs = getCurrentInputs();
    uint32_t currentTick = m_clientNetwork->getCurrentTick();

    m_inputHistory.addInputEntry(currentTick, currentInputs);
    m_clientGameInstance->processPlayerInput(m_clientPlayerId, currentTick, currentInputs);
    m_clientGameInstance->update();

    updatePredictedEntityPosition(currentTick);
}

void Game::updatePredictedEntityPosition(uint32_t currentTick)
{
    Entity clientPlayerEntity = findClientPlayerEntity();
    if (clientPlayerEntity.id == 0 || !_registry.has<Position>(clientPlayerEntity)) {
        return;
    }

    auto& clientPos = _registry.get<Position>(clientPlayerEntity);
    auto playerEntities = m_clientGameInstance->getRegistry().view<Position, PlayerTag>();

    for (auto it = playerEntities.begin(); it != playerEntities.end(); ++it) {
        auto [pos, playerTag] = *it;
        clientPos.x = pos.x;
        clientPos.y = pos.y;

        updateSpritePosition(clientPlayerEntity, pos.x, pos.y);
        logPredictedPosition(pos.x, pos.y, currentTick);
        break;
    }
}

void Game::updateSpritePosition(Entity entity, float x, float y)
{
    if (_registry.has<Sprite>(entity)) {
        auto& sprite = _registry.get<Sprite>(entity);
        sprite.dstRect.x = static_cast<int>(x);
        sprite.dstRect.y = static_cast<int>(y);
    }
}

void Game::logPredictedPosition(float x, float y, uint32_t tick)
{
    if (tick % 30 == 0) {
        printf("[CLIENT] Predicted pos: (%.2f, %.2f) tick: %u\n", x, y, tick);
    }
}

void Game::reconcileWithServerState(uint32_t serverTick, const Message& msg)
{
    if (!m_clientGameInstance || m_isLocalMode) {
        return;
    }

    processServerPlayerUpdates(serverTick, msg);
    m_lastServerTick = serverTick;
    m_inputHistory.removeInputsOlderThan(serverTick);
}

void Game::processServerPlayerUpdates(uint32_t serverTick, const Message& msg)
{
    const_cast<Message&>(msg).resetReadPosition();
    msg.readU32();
    uint8_t numPlayers = msg.readU8();

    for (uint8_t i = 0; i < numPlayers; ++i) {
        processServerPlayerState(serverTick, msg);
    }
}

void Game::processServerPlayerState(uint32_t serverTick, const Message& msg)
{
    uint32_t entityId = msg.readU32();
    float serverPosX = msg.readFloat();
    float serverPosY = msg.readFloat();
    uint32_t health = msg.readU32();

    Entity clientPlayerEntity = findClientPlayerEntity();
    if (clientPlayerEntity.id == 0) {
        return;
    }

    checkAndReconcileEntity(clientPlayerEntity, entityId, serverPosX, serverPosY, serverTick);
}

void Game::checkAndReconcileEntity(Entity clientEntity, uint32_t serverId,
    float serverX, float serverY, uint32_t serverTick)
{
    auto view = _registry.view<ServerEntityId>();
    for (auto it = view.begin(); it != view.end(); ++it) {
        Entity entity = it.entity();
        const auto& serverEntityId = _registry.get<ServerEntityId>(entity);

        if (serverEntityId.id == serverId && _registry.has<PredictedEntity>(entity)) {
            reconcileEntityPosition(entity, serverX, serverY, serverTick);
            break;
        }
    }
}

void Game::reconcileEntityPosition(Entity entity, float serverX, float serverY,
    uint32_t serverTick)
{
    if (!_registry.has<Position>(entity)) {
        return;
    }

    auto& position = _registry.get<Position>(entity);
    float errorDistance = calculatePositionError(position.x, position.y, serverX, serverY);

    logReconciliationInfo(serverX, serverY, position.x, position.y, errorDistance, serverTick);

    if (errorDistance > Client::InputHistory::RECONCILIATION_THRESHOLD) {
        performReconciliation(entity, serverX, serverY, serverTick, errorDistance);
    }
}

float Game::calculatePositionError(float clientX, float clientY,
    float serverX, float serverY)
{
    return std::sqrt(
        (clientX - serverX) * (clientX - serverX) + (clientY - serverY) * (clientY - serverY));
}

void Game::logReconciliationInfo(float serverX, float serverY, float clientX, float clientY,
    float error, uint32_t tick)
{
    printf("[CLIENT] Server pos: (%.2f, %.2f) vs Predicted: (%.2f, %.2f) - Error: %.2fpx tick: %u\n",
        serverX, serverY, clientX, clientY, error, tick);
}

void Game::performReconciliation(Entity entity, float serverX, float serverY,
    uint32_t serverTick, float errorDistance)
{
    printf("[CLIENT] RECONCILIATION TRIGGERED! Error %.2fpx > threshold %.2fpx\n",
        errorDistance, Client::InputHistory::RECONCILIATION_THRESHOLD);

    auto& position = _registry.get<Position>(entity);
    position.x = serverX;
    position.y = serverY;

    replayInputsFromTick(serverTick);
}

void Game::replayInputsFromTick(uint32_t fromTick)
{
    if (!m_clientGameInstance) {
        return;
    }

    auto inputsToReplay = m_inputHistory.getInputsSince(fromTick + 1);

    for (const auto& inputEntry : inputsToReplay) {
        m_clientGameInstance->processPlayerInput(m_clientPlayerId, inputEntry.tick, inputEntry.inputs);
        m_clientGameInstance->update();
    }

    updateClientPositionFromPrediction();
}

void Game::updateClientPositionFromPrediction()
{
    Entity clientPlayerEntity = findClientPlayerEntity();
    if (clientPlayerEntity.id == 0 || !_registry.has<Position>(clientPlayerEntity)) {
        return;
    }

    auto& clientPos = _registry.get<Position>(clientPlayerEntity);
    auto playerEntities = m_clientGameInstance->getRegistry().view<Position, PlayerTag>();

    for (auto it = playerEntities.begin(); it != playerEntities.end(); ++it) {
        auto [pos, playerTag] = *it;
        clientPos.x = pos.x;
        clientPos.y = pos.y;
        updateSpritePosition(clientPlayerEntity, pos.x, pos.y);
        break;
    }
}

Entity Game::findClientPlayerEntity()
{
    auto view = _registry.view<PredictedEntity>();
    for (auto it = view.begin(); it != view.end(); ++it) {
        Entity entity = it.entity();
        const auto& predicted = _registry.get<PredictedEntity>(entity);
        if (predicted.playerId == m_clientPlayerId) {
            return entity;
        }
    }
    return Entity { 0, 0 };
}
