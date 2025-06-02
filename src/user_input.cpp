#include "user_input.hpp"

#include <iostream>
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
    while (auto evt = window_->pollEvent()) {
        if (evt->is<sf::Event::Closed>()) {
            fireUserAskedClose_();
        } else if (
            auto mouse = evt->getIf<sf::Event::MouseButtonPressed>(); 
            mouse && mouse->button == sf::Mouse::Button::Left) 
        {   
            auto pos = mouse->position;
            computeCoord_(pos.x, pos.y);
            fireUserAskedSetCreature_();
        } else if (
            auto key = evt->getIf<sf::Event::KeyPressed>(); 
            key && key->scancode == sf::Keyboard::Scancode::Escape) 
        {
            fireUserAskedRestart_();
        }
    }
}

std::tuple<bool, int, int> UserInput::lastCoordInput() noexcept {
    return lastCoordInput_;
}

void UserInput::computeCoord_(int x, int y) {
    int col = 
        (x - startX_) / (cellWidth_ + gridThickness_);
    int row = 
        (y - startY_) / (cellHeight_ + gridThickness_);

    if (col >= 0 && row >= 0) {
        lastCoordInput_ = {true, col, row};
    } else {
        lastCoordInput_ = {false, 0, 0};
    }
}

void UserInput::fireUserAskedClose_() {
    int evt = static_cast<int>(
        game_event::event_t::USER_ASKED_CLOSE);
    notify(evt);
} 

void UserInput::fireUserAskedSetCreature_() {
    int evt = static_cast<int>(
        game_event::event_t::USER_ASKED_SET_CREATURE);
    notify(evt);
}

void UserInput::fireUserAskedRestart_() {
    int evt = static_cast<int>(
        game_event::event_t::USER_ASKED_RESTART);
    notify(evt);
}

} // namespace user_input