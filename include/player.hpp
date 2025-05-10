#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "observer.hpp"
#include "subject.hpp"
#include "game_event.hpp"
#include "game_field_area.hpp"

namespace player {

template <class creature_t, template <class> class cell_t>
class Player : 
    public observer::IObserver 
    , public subject::ISubject
    , std::enable_shared_from_this<Player>
{
    using IGameFieldArea 
        = game_field_area::IGameFieldArea<cell_t<creature_t>>;
    using IUserInput = user_input::IUserInput;
    using ISubject = subject::ISubject;

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
        if (game_event::event_t::USER_ASKED_SET_CREATURE == evt) 
            setOneCell_();
    }

public:
    void attach(
        std::shared_ptr<observer::IObserver> obs, int event_t)
    { ISubject::attach(obs, event_t); }

    void detach(
        std::weak_ptr<observer::IObserver> obs, int event_t) 
    { ISubject::detach(obs, event_t); }
    
public:
    const creature_t& creature() const noexcept {
        return creature;
    }

    auto slf() { return shared_from_this(); }

private:
    void setOneCell_() {
        auto [suc, x, y] = input_->lastCoordInput();
        if (suc && area_->isCellAvailable(x, y)) {
            auto&& cell = area_->getCell();
            auto&& cr = cell.creature();
            if (cr.isAlive() && cr.id() == creature_.id()) {
                cr.kill();
                firePlayerKillCreature_();
            } else {
                cell.setCreature(creature_); 
            }
        }
    }

    void firePlayerKillCreature_() {
        notify(
            game_event::event_t::PLAYER_KILL_CREATURE, 
            slf());
    }

private:   
    creature_t creature_;
    std::shared_ptr<IUserInput> input_; 
    std::shared_ptr<IGameFieldArea> area_;
};

} // namespace player

#endif // PLAYER_HPP