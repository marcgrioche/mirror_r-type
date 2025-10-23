#include "../../../shared/include/GameInputs.hpp"
#include "RTypeClient.hpp"

using namespace Client;

void RTypeClient::handleInputs(const InputManager& t_inputs)
{
    if (!m_isConnected) {
        return;
    }

    const auto& currentActions = t_inputs.getActions();
    std::vector<std::pair<GameInput, bool>> changedInputs;

    for (const auto& [action, currentState] : currentActions) {
        if (action == GameAction::QUIT)
            continue;

        auto prevIt = m_previousInputStates.find(action);
        bool previousState = (prevIt != m_previousInputStates.end()) ? prevIt->second : false;

        if (currentState != previousState) {
            changedInputs.emplace_back(static_cast<GameInput>(action), currentState);
        }
    }

    for (const auto& [action, previousState] : m_previousInputStates) {
        if (previousState && currentActions.find(action) == currentActions.end()) {
            if (action != GameAction::QUIT) {
                changedInputs.emplace_back(static_cast<GameInput>(action), false);
            }
        }
    }

    if (changedInputs.empty()) {
        return;
    }

    Message msg(MessageType::INPUT);
    msg.write(static_cast<uint32_t>(getCurrentTick()));
    msg.write(static_cast<uint8_t>(changedInputs.size()));
    for (const auto& [input, state] : changedInputs) {
        msg.write(static_cast<uint8_t>(input));
        msg.write(static_cast<uint8_t>(state ? 1 : 0));
    }
    sendMessage(msg);

    m_previousInputStates.clear();
    for (const auto& [action, state] : currentActions) {
        m_previousInputStates[action] = state;
    }
}

void RTypeClient::handleAllInputs(const InputManager& t_inputs)
{
    const auto actions = t_inputs.getActions();

    if (actions.empty())
        return;
    Message msg(MessageType::INPUT);
    msg.write(1u); // placeholder for now as the ticks are not implemented on client side
    msg.write(static_cast<uint8_t>(actions.size()));
    for (const auto& [fst, snd] : actions) {
        msg.write(static_cast<uint8_t>(fst));
        msg.write(static_cast<uint8_t>(snd));
    }
    sendMessage(msg);
}

void RTypeClient::sendCurrentInputState(const std::vector<std::pair<GameInput, bool>>& inputs)
{
    if (inputs.empty() || !m_isConnected) {
        return;
    }

    Message msg(MessageType::INPUT);
    msg.write(static_cast<uint32_t>(getCurrentTick()));
    msg.write(static_cast<uint8_t>(inputs.size()));

    for (const auto& [input, state] : inputs) {
        msg.write(static_cast<uint8_t>(input));
        msg.write(static_cast<uint8_t>(state ? 1 : 0));
    }

    sendMessage(msg);
}

void RTypeClient::sendCurrentInputStateWithMouse(const std::vector<std::pair<GameInput, bool>>& inputs, float mouseX, float mouseY)
{
    if (inputs.empty() || !m_isConnected) {
        return;
    }

    Message msg(MessageType::INPUT);
    msg.write(static_cast<uint32_t>(getCurrentTick()));
    msg.write(static_cast<uint8_t>(inputs.size()));

    for (const auto& [input, state] : inputs) {
        msg.write(static_cast<uint8_t>(input));
        msg.write(static_cast<uint8_t>(state ? 1 : 0));
    }

    // Append mouse coordinates at the end
    msg.write(mouseX);
    msg.write(mouseY);

    sendMessage(msg);
}
