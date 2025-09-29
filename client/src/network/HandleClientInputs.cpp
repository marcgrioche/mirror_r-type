#include "RTypeClient.hpp"

using namespace Client;

void RTypeClient::handleInputs(const InputManager& t_inputs)
{
    std::vector<GameAction> actions;

    if (t_inputs.isUp()) {
        actions.emplace_back(GameAction::MOVE_UP);
    }
    if (t_inputs.isDown()) {
        actions.emplace_back(GameAction::MOVE_DOWN);
    }
    if (t_inputs.isLeft()) {
        actions.emplace_back(GameAction::MOVE_LEFT);
    }
    if (t_inputs.isRight()) {
        actions.emplace_back(GameAction::MOVE_RIGHT);
    }
    if (t_inputs.isShoot()) {
        actions.emplace_back(GameAction::SHOOT);
    }
    if (actions.empty())
        return;
    Message msg(MessageType::INPUT);
    msg.write(1u); // placeholder for now as the ticks are not implemented on client side
    msg.write(static_cast<uint8_t>(actions.size()));
    for (const auto& action : actions) {
        msg.write(static_cast<uint8_t>(action));
        msg.write(static_cast<uint8_t>(1));
    }
    sendMessage(msg);
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
