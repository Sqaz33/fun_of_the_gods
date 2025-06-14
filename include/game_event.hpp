#ifndef GAME_EVENT_HPP
#define GAME_EVENT_HPP

namespace game_event {

enum class event_t : int {
    FIELD_CLEAR = 0,
    CREATURE_REMOVE_IN_FIELD,
    CREATURE_SET_IN_FIELD,
    USER_ASKED_CLOSE,
    USER_ASKED_RESTART,
    USER_ASKED_SET_CREATURE,
    WINNER_DETERMINATE,
    DRAW_DETERMINATE,
    PLAYER_BETS_CREATURES,
    GAME_MODEL_CALCULATED_ER,
    USER_INPUT_REQUIRED
};

} // namespace game_event

#endif // GAME_EVENT_HPP