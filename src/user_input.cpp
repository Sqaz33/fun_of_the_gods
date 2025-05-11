#include "user_input.hpp"

#include <cmath>

#include "game_event.hpp"

namespace user_input {

UserInput::UserInput(       
        std::shared_ptr<sf::Window> window, 
        float startX, 
        float startY,
        float cellWidth,
        float cellHeight,
        float gridThickness):
    window_(window)
    , startX_(startX)
    , startY_(startY)
    , cellWidth_(cellWidth)
    , cellHeight_(cellHeight)
    , gridThickness_(gridThickness)
{}

void UserInput::readInput() {
    while (true) {
        if (auto evt = window_->pollEvent()) {
            if (evt->is<sf::Event::Closed>()) {
                fireUserAskedClose_();
                return;
            } else if (
                auto mouse = evt->getIf<sf::Event::MouseButtonPressed>(); 
                mouse && mouse->button == sf::Mouse::Button::Left) 
            {   
                auto pos = mouse->position;
                computeCoord_(pos.x, pos.y);
                fireUserAskedSetCreature_();
                return;
            } else if (
                auto key = evt->getIf<sf::Event::KeyPressed>(); 
                key && key->scancode == sf::Keyboard::Scancode::Escape) 
            {
                fireUserAskedRestart_();
                return;
            }
        }
    }
}

std::tuple<bool, int, int> UserInput::lastCoordInput() noexcept {
    return lastCoordInput_;
}

void UserInput::computeCoord_(int x, int y) {
    decltype(lastCoordInput_) res;
    if (x >= startX_ && y >= startY_) {
        x -= startX_; 
        y -= startY_;
        int cellNGridWidth = 
            cellWidth_ + gridThickness_;
        int cellNGridHeight = 
            cellHeight_ + gridThickness_;
        int z = y / cellNGridWidth;
        res = {
            true,
            x / cellNGridHeight,
            y / cellNGridWidth,
        };
    } else {
        res = {false, 0, 0};
    }
    lastCoordInput_ = res;
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
        game_event::event_t::USER_ASKED_SET_CREATURE);
    notify(evt, slf());
}

void UserInput::fireUserAskedRestart_() {
    int evt = static_cast<int>(
        game_event::event_t::USER_ASKED_RESTART);
    notify(evt, slf());
}

} // namespace user_input