/**
 * @file game_event.hpp
 * @brief Header file defining game event types.
 * 
 * Contains enumeration of all possible game events used for communication
 * between game components through the observer pattern.
 */

#ifndef GAME_EVENT_HPP
#define GAME_EVENT_HPP

namespace game_event {

/**
 * @enum event_t
 * @brief Enumeration of all possible game event types.
 * 
 * These events are used to notify observers about changes in the game state.
 * Each event corresponds to a specific action or state change in the game.
 */
enum class event_t : int;

/**
 * @enum event_t
 * @brief Enumeration of all possible game event types (implementation).
 */
enum class event_t : int {
    /**
     * @brief Entire game field has been cleared.
     */
    FIELD_CLEAR = 0,
    
    /**
     * @brief A specific cell in the field has been cleared.
     */
    CELL_CLEAR_IN_FIELD,
    
    /**
     * @brief A creature has been killed in the field.
     */
    CREATURE_KILL_IN_FIELD,
    
    /**
     * @brief A creature has been revived in the field.
     */
    CREATURE_REVIVE_IN_FIELD,
    
    /**
     * @brief User requested to close the game.
     */
    USER_ASKED_CLOSE,
    
    /**
     * @brief User requested to restart the game.
     */
    USER_ASKED_RESTART,
    
    /**
     * @brief User requested to set a creature on the field.
     */
    USER_ASKED_SET_CREATURE,
};

} // namespace game_event

#endif // GAME_EVENT_HPP