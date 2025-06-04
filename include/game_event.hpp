/**
 * @file game_event.hpp
 * @brief Header file defining game event types.
 * 
 * Contains enumeration of all possible game events used for communication
 * between game components through the observer pattern. Events represent
 * significant state changes or user actions that need to be communicated
 * across different game systems.
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
 * The events are used within the observer pattern implementation to allow
 * decoupled communication between game systems.
 */
enum class event_t : int {
    /**
     * @brief Entire game field has been cleared (all cells reset).
     */
    FIELD_CLEAR = 0,
    
    /**
     * @brief A specific cell in the field has been cleared.
     */
    CELL_CLEAR_IN_FIELD,
    
    /**
     * @brief A creature has been killed/removed from the field.
     */
    CREATURE_REMOVE_IN_FIELD,
    
    /**
     * @brief A creature has been revived/added to the field.
     */
    CREATURE_SET_IN_FIELD,
    
    /**
     * @brief User requested to close/exit the game.
     */
    USER_ASKED_CLOSE,
    
    /**
     * @brief User requested to restart the game.
     */
    USER_ASKED_RESTART,
    
    /**
     * @brief User requested to place a creature on the field.
     */
    USER_ASKED_SET_CREATURE,

    /**
     * @brief A winner has been determined in the game.
     */
    WINNER_DETERMINATE,
    
    /**
     * @brief The game has ended in a draw.
     */
    DRAW_DETERMINATE,
    
    /**
     * @brief Player has placed bets on creatures (for betting systems).
     */
    PLAYER_BETS_CREATURES,
    
    /**
     * @brief Game model has calculated er.
     */
    GAME_MODEL_CALCULATED_ER,
    
    /**
     * @brief System requires user input to proceed.
     */
    USER_INPUT_REQUIRED
};

} // namespace game_event

#endif // GAME_EVENT_HPP