#ifndef GAME_EVENT_HPP
#define GAME_EVENT_HPP

namespace game_event {

enum class event_t : int;

enum class event_t : int {
    FIELD_CLEAR = 0,
    CELL_CLEAR_IN_FIELD,
    CREATURE_KILL_IN_FIELD,
    CREATURE_REVIVE_IN_FIELD,
    USER_ASKED_CLOSE,
    USER_ASKED_RESTART,
    USER_ASKED_SET_CREATURE,
};

} // game_event 

#endif // GAME_EVENT_HPP