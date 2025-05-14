#include "player.hpp"

namespace {
    
    using IGameFieldArea = game_field_area::IGameFieldArea;

} // namespace

namespace player {
    
void Player::update(
    std::weak_ptr<subject::ISubject> subj, int event_t) 
{ 
    auto evt = static_cast<game_event::event_t>(event_t);
    if (game_event::event_t::USER_ASKED_SET_CREATURE == evt) {
        auto lk = subj.lock();
        auto input = 
            std::dynamic_pointer_cast<user_input::IUserInput>(lk);
        auto [suc, x, y] = input->lastCoordInput();
        if (suc) tapOnCreature_(x, y);
    }
}

void Player::setFieldArea(
        std::unique_ptr<IGameFieldArea> area)
{ area_ = std::move(area); }

IGameFieldArea& Player::fieldArea(){
    return *area_;
}

void Player::setCreatId(int id) noexcept {
    creatId_ = id;
}

int Player::creatId() const noexcept {
    return creatId_;
}

void Player::tapOnCreature_(int x, int y) {
    if (area_->isCellAvailable(x, y)) {
        auto&& cr = area_->getCreatureByCell(x, y);
        if (cr.isAlive() && cr.id() == creatId_) {
            area_->killCreatureInCell(x, y);
        } else if (!cr.isAlive()) {
            area_->reviveCreatureInCell(x, y, creatId_);
        }
    }
}

} // namespace player
