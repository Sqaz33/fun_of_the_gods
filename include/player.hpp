#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "user_input.hpp"
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
    using ISubject = subject::ISubject;

public:
    Player(std::unique_ptr<IGameFieldArea> area,
           creature_t playerCreature) : 
        area_(std::move(area))
        , creature_(playerCreature)
    {}

    Player() = default;
    
public:
    void update(
        std::weak_ptr<subject::ISubject> subj, int event_t) override
    { 
        auto evt = static_cast<game_event::event_t>(event_t);
        if (game_event::event_t::USER_ASKED_SET_CREATURE == evt) {
            auto lk = subj.lock();
            auto input = 
                std::dynamic_pointer_cast<user_input::IUserInput>(lk)
            auto [suc, x, y] = input->lastCoordInput()
            if (suc) setOneCell_(x, y);
        }
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

    void setCreature(const creature_t& cr) {
        creature_ = cr;
    }
    
    
    decltype(auto) fieldArea() {
        return *area_;
    }

    void setFieldArea(std::unique_ptr<IGameFieldArea> area) {
        area_ = std::move(area);
    }
    
    auto slf() { return shared_from_this(); }

private:
    void setOneCell_(int x, int y) {
        if (area_->isCellAvailable(x, y)) {
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
    std::unique_ptr<IGameFieldArea> area_;
};

} // namespace player

#endif // PLAYER_HPP