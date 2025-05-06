#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "observer.hpp"
#include "game_event.hpp"
#include "game_field_area.hpp"

namespace player {

template <class cell_t>
class Player : public observer::IObserver {
    using IGameFieldArea = game_field_area::IGameFieldArea;

public:
    Player(std::shared_ptr<IGameFieldArea> area, cell_t playerCell) : 
        area_(area)
        , playerCell_(playerCell)

    {}
    
public:
    void update(
        std::weak_ptr<subject::ISubject> subj, int event_t) override final 
    { 
        auto evt = static_cast<game_event::event_t>(event_t);
        if (game_event::event_t::USER_INPUT == evt) 
            setOneCell_();
    }
    
public:
    void setCells(int k) {
        input_->clearInput(); // TODO
        while (k--) {
            input_->readInput(); // TODO
        }   
    }

private:
    void setOneCell_() {
        auto [x, y] = input_->lastInput(); // TODO
        if (area_->isCellAvailable(x, y)) {
            area_->setCell(x, y, playerCell_);
        }
    }

private:   
    cell_t playerCell_;
    std::shared_ptr<IUserInput> input_; // TODO
    std::shared_ptr<IGameFieldArea> area_;
};

} // namespace player

#endif // PLAYER_HPP