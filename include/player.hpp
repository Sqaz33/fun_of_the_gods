#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <memory>

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
    , public std::enable_shared_from_this<Player<creature_t, cell_t>>
{
    using IGameFieldArea 
        = game_field_area::IGameFieldArea<cell_t<creature_t>>;
    using ISubject = subject::ISubject;
    using sh_base_t = 
        std::enable_shared_from_this<Player<creature_t, cell_t>>;

public:
    Player(std::unique_ptr<IGameFieldArea> area,
           creature_t playerCreature) : 
        area_(std::move(area))
        , creature_(playerCreature)
    { creature_.revive(); }

    Player() = default;
    
public:
    void update(
        std::weak_ptr<subject::ISubject> subj, int event_t) override
    { 
        auto evt = static_cast<game_event::event_t>(event_t);
        if (game_event::event_t::USER_ASKED_SET_CREATURE == evt) {
            auto lk = subj.lock();
            auto input = 
                std::dynamic_pointer_cast<user_input::IUserInput>(lk);
            auto [suc, x, y] = input->lastCoordInput();
            if (suc) setOneCell_(x, y);
        }
    }

public:
    void attach(
        std::shared_ptr<observer::IObserver> obs, int event_t) override
    { ISubject::attach(obs, event_t); }

    void detach(
        std::weak_ptr<observer::IObserver> obs, int event_t) override
    { ISubject::detach(obs, event_t); }
    

private:
    void notify(int event_t, std::weak_ptr<ISubject> slf) override {
        ISubject::notify(event_t, slf);
    }

public:
    const creature_t& creature() const noexcept {
        return creature_;
    }

    void setCreature(const creature_t& cr) {
        creature_ = cr;
        creature_.revive();
    }
    
    decltype(auto) fieldArea() {
        return *area_;
    }

    void setFieldArea(std::unique_ptr<IGameFieldArea> area) {
        area_ = std::move(area);
    }
    
    auto slf() { return sh_base_t::shared_from_this(); }

private:
    void setOneCell_(int x, int y) {
        if (area_->isCellAvailable(x, y)) {
            auto&& cell = area_->getCell(x, y);
            auto&& cr = cell.creature();
            if (cr.isAlive() && cr.id() == creature_.id()) {
                cr.kill();
                firePlayerKillCreature_();
            } else if (!cr.isAlive()) {
                cell.setCreature(creature_); 
                area_->setCell(x, y, cell);
            }
        }
    }

    void firePlayerKillCreature_() {
        int evt = static_cast<int>(
            game_event::event_t::PLAYER_KILL_CREATURE);
        notify(evt, slf());
    }

private:   
    creature_t creature_;
    std::unique_ptr<IGameFieldArea> area_;
};

} // namespace player

#endif // PLAYER_HPP