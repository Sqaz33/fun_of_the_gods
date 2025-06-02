/**
 * @file player.hpp
 * @brief Header file for the Player class.
 * 
 * Defines the Player class which represents a game participant with
 * control over a specific game field area and creature type.
 */

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <memory>
#include <string>

#include "user_input.hpp"
#include "observer.hpp"
#include "game_event.hpp"
#include "game_field_area.hpp"

namespace player {

/**
 * @class Player
 * @brief Represents a game player with associated field area and creatures.
 * 
 * Implements the Observer pattern to respond to game events and manages
 * the player's assigned game field area and creature type.
 */
class Player : public std::enable_shared_from_this<Player> {
    using IGameFieldArea = game_field_area::IGameFieldArea;

public:
    Player(int id, const std::string& name);
 
public:
    /**
     * @brief Assigns a game field area to the player.
     * @param area Unique pointer to the IGameFieldArea to assign
     */
    void setFieldArea(std::unique_ptr<IGameFieldArea> area);
    
    /**
     * @brief Gets the player's assigned game field area.
     * @return Reference to the player's IGameFieldArea
     * @throws std::runtime_error if no area has been assigned
     */
    IGameFieldArea& fieldArea();
    
    int id() const noexcept;

    const std::string& name() const;

    void setCreature(int x, int y);

    std::shared_ptr<Player> slf();

private:   
    std::unique_ptr<IGameFieldArea> area_;  ///< Player's assigned game field area
    int id_;                                ///< Player's identifier
    std::string name_;
};

} // namespace player

#endif // PLAYER_HPP