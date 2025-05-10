#ifndef GAME_CONTROLLER_HPP
#define GAME_CONTROLLER_HPP

namespace game_event {

enum class event_t : int;

enum class event_t : int {
    GAME_FIELD_UPDATE = 0,
    USER_ASKED_CLOSE,
    USER_ASKED_RESTART,
    USER_ASKED_SET_CREATURE
};

} // game_event 

#endif // GAME_CONTROLLER_HPP