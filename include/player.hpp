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
class Player : public observer::IObserver {
    using IGameFieldArea = game_field_area::IGameFieldArea;

public:
    /**
     * @brief Handles update notifications from observed subjects.
     * @param subj Weak pointer to the subject triggering the update
     * @param event_t Type of event that occurred (from game_event::event_t)
     * 
     * Processes game events relevant to the player, particularly user input events
     * for creature placement in the player's assigned area.
     */
    void update(std::weak_ptr<subject::ISubject> subj, int event_t) override;
 
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
    
    /**
     * @brief Sets the player's creature type identifier.
     * @param id Creature identifier to assign
     */
    void setCreatId(int id) noexcept;
    
    /**
     * @brief Gets the player's creature type identifier.
     * @return The creature identifier
     */
    int creatId() const noexcept;

private:
    /**
     * @brief Handles creature placement at specified coordinates.
     * @param x X coordinate in the game field
     * @param y Y coordinate in the game field
     * 
     * Attempts to place the player's creature type at the specified location
     * within their assigned game field area.
     */
    void tapOnCreature_(int x, int y);

private:   
    std::unique_ptr<IGameFieldArea> area_;  ///< Player's assigned game field area
    int creatId_;                           ///< Player's creature type identifier
};

} // namespace player

#endif // PLAYER_HPP