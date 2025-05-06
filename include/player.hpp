#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "observer.hpp"
#include "game_event.hpp"
#include "game_field_area.hpp"

namespace player {

template <class creature_t, template <class> class cell_t>
class Player : public observer::IObserver {
    using IGameFieldArea 
        = game_field_area::IGameFieldArea<cell_t<creature_t>>;
    using IUserInput = user_input::IUserInput;

public:
    Player(std::shared_ptr<IGameFieldArea> area,
           std::shared_ptr<IUserInput> input, 
           creature_t playerCreature) : 
        area_(area)
        , creature_(playerCreature)
        , input_(input)
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

    const creature_t& creature() const noexcept {
        return creature;
    }

private:
    void setOneCell_() {
        auto [x, y] = input_->lastInput(); // TODO
        if (area_->isCellAvailable(x, y)) {
            auto&& cell = area_->getCell();
            cell.setCreature(creature_); 
        }
    }

private:   
    creature_t creature_;
    std::shared_ptr<IUserInput> input_; // TODO
    std::shared_ptr<IGameFieldArea> area_;
};

} // namespace player

#endif // PLAYER_HPP