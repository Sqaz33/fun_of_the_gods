#include "user_input.hpp"

#include <cmath>

#include "game_event.hpp"

namespace user_input {

UserInput::UserInput(       
        std::shared_ptr<sf::Window> window, 
        float startX, 
        float startY,
        int cellHeight
        int cellWidth) :
    window_(window)
    , startX_(startX)
    , startY_(startY)
    , cellHeight_(cellHeight)
    , cellWidth_(cellWidth)
{}

void UserInput::readInput() {
    if (auto evt = window_->pollEvent()) {
        if (evt->is<sf::Event::Closed>()) {
            fireUserAskedClose_();
        } else if (
            auto mouse = evt->getIf<sf::Event::MouseButtonPressed>(); 
            mouse->button == sf::Mouse::Button::Rigth) 
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

std::pair<int, int> UserInput::lastCoordInput() noexcept {
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
    notify(
        game_event::event_t::USER_ASKED_CLOSE, slf());
} 

void UserInput::fireUserAskedSetCreature_() {
    notify(
        game_event::event_t::USER_ASKED_SET_CREATURE, slf());
}

void UserInput::fireUserAskedRestart_() {
    notify(
        game_event::event_t::USER_ASKED_RESTART, slf());
}

} // namespace user_input