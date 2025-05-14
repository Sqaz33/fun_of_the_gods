#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <memory>

#include "user_input.hpp"
#include "observer.hpp"
#include "game_event.hpp"
#include "game_field_area.hpp"

namespace player {

class Player : public observer::IObserver {
    using IGameFieldArea = game_field_area::IGameFieldArea;

public:
    void update(
        std::weak_ptr<subject::ISubject> subj, int event_t) override;
 
public:
    void setFieldArea(
        std::unique_ptr<IGameFieldArea> area);
    IGameFieldArea& fieldArea();
    void setCreatId(int id) noexcept;
    int creatId() const noexcept;

private:
    void tapOnCreature_(int x, int y);

private:   
    std::unique_ptr<IGameFieldArea> area_;
    int creatId_;
};

} // namespace player

#endif // PLAYER_HPP