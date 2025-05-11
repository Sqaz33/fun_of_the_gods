#include "user_input.hpp"

#include <cmath>

#include "game_event.hpp"

namespace user_input {

UserInput::UserInput(       
        std::shared_ptr<sf::Window> window, 
        float startX, 
        float startY,
        float cellWidth,
        float cellHeight):
    window_(window)
    , startX_(startX)
    , startY_(startY)
    , cellWidth_(cellWidth)
    , cellHeight_(cellHeight)
{}

void UserInput::readInput() {
    if (auto evt = window_->pollEvent()) {
        if (evt->is<sf::Event::Closed>()) {
            fireUserAskedClose_();
        } else if (
            auto mouse = evt->getIf<sf::Event::MouseButtonPressed>(); 
            mouse->button == sf::Mouse::Button::Right) 
        {   
            auto pos = mouse->position;
            computeCoord_(pos.x, pos.y);
            fireUserAskedSetCreature_();
        } else if(
            auto key = evt->getIf<sf::Event::KeyPressed>(); 
            key->scancode == sf::Keyboard::Scancode::Escape) 
        {
            fireUserAskedRestart_();
        }
    }
}

std::tuple<bool, int, int> UserInput::lastCoordInput() noexcept {
    return lastCoordInput_;
}

void UserInput::computeCoord_(float x, float y) {
    decltype(lastCoordInput_) res;
    if (x >= startX_ && y >= startY_) {
        x -= startX_; 
        y -= startY_;
        res = {
            true,
            static_cast<int>(std::ceil(x / cellWidth_)),
            static_cast<int>(std::ceil(y / cellHeight_)),
        };
    }
    res = {false, 0, 0};
}

std::shared_ptr<UserInput> UserInput::slf() {
    return shared_from_this();
}

void UserInput::fireUserAskedClose_() {
    int evt = static_cast<int>(
        game_event::event_t::USER_ASKED_CLOSE);
    notify(evt, slf());
} 

void UserInput::fireUserAskedSetCreature_() {
    int evt = static_cast<int>(
        game_event::event_t::USER_ASKED_SET_CREATURE)
    notify(evt, slf());
}

void UserInput::fireUserAskedRestart_() {
    int evt = static_cast<int>(
        game_event::event_t::USER_ASKED_RESTART)
    notify(evt, slf());
}

} // namespace user_input