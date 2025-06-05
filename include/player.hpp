#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <memory>
#include <string>

#include "user_input.hpp"
#include "observer.hpp"
#include "game_event.hpp"
#include "game_field_area.hpp"

namespace player {

class Player : public std::enable_shared_from_this<Player> {
    using IGameFieldArea = game_field_area::IGameFieldArea;

public:
    Player(int id, const std::string& name);
 
public:
    void setFieldArea(std::unique_ptr<IGameFieldArea> area);
    IGameFieldArea& fieldArea();
    int id() const noexcept;
    const std::string& name() const;
    void tapOnCreature(int x, int y);
    std::shared_ptr<Player> slf();

private:   
    std::unique_ptr<IGameFieldArea> area_;  
    int id_;                                
    std::string name_;
};

} // namespace player

#endif // PLAYER_HPP