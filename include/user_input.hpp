/**
 * @file user_input.hpp
 * @brief Header file for user input handling in the game.
 * 
 * Defines interfaces and implementations for processing user input events
 * and translating them into game coordinates and actions.
 */

#ifndef USER_INPUT_HPP
#define USER_INPUT_HPP

#include <utility>
#include <tuple>
#include <memory>

#include <SFML/Window.hpp>

#include "subject.hpp"

namespace user_input {

/**
 * @interface IUserInput
 * @brief Interface for user input handling systems.
 * 
 * Extends ISubject to allow observation of input events.
 * Provides methods for reading input and querying the last input coordinates.
 */
struct IUserInput : subject::ISubject {
    /**
     * @brief Reads and processes input events from the window.
     */
    virtual void readInput() = 0;
    
    /**
     * @brief Gets the last valid coordinate input from the user.
     * @return Tuple containing:
     *         - bool: true if input was valid, false otherwise
     *         - int: x coordinate in grid cells
     *         - int: y coordinate in grid cells
     */
    virtual std::tuple<bool, int, int> lastCoordInput() noexcept = 0;
    
    /**
     * @brief Virtual destructor for polymorphic behavior.
     */
    virtual ~IUserInput() = default;
};

/**
 * @class UserInput
 * @brief Concrete implementation of user input handling using SFML.
 * 
 * Translates SFML window events into game coordinates and actions,
 * notifying observers of relevant input events.
 */
class UserInput : 
    public IUserInput,
    public std::enable_shared_from_this<UserInput>
{
public:
    /**
     * @brief Constructs a UserInput handler.
     * @param window SFML window to monitor for input
     * @param startX X position of grid origin in pixels
     * @param startY Y position of grid origin in pixels
     * @param cellWidth Width of each grid cell in pixels
     * @param cellHeight Height of each grid cell in pixels
     * @param gridThickness Thickness of grid lines in pixels
     */
    UserInput(
        std::shared_ptr<sf::Window> window, 
        float startX, 
        float startY,
        float cellWidth,
        float cellHeight,
        float gridThickness);

    // IUserInput interface implementation
    void readInput() override;
    std::tuple<bool, int, int> lastCoordInput() noexcept override;


    // ISubject interface forwarding
    void attach(std::shared_ptr<observer::IObserver> obs, int event_t) override
    { subject::ISubject::attach(obs, event_t); }

    void detach(std::weak_ptr<observer::IObserver> obs, int event_t) override
    { subject::ISubject::detach(obs, event_t); }

protected:
    void notify(int event_t) override
    { subject::ISubject::notify(event_t); }

private:
    /**
     * @brief Notifies observers of window close request.
     */
    void fireUserAskedClose_();
    
    /**
     * @brief Notifies observers of creature placement request.
     */
    void fireUserAskedSetCreature_();
    
    /**
     * @brief Notifies observers of game restart request.
     */
    void fireUserAskedRestart_();
    
    /**
     * @brief Converts screen coordinates to grid coordinates.
     * @param x Screen x coordinate in pixels
     * @param y Screen y coordinate in pixels
     */
    void computeCoord_(int x, int y);

private:
    std::shared_ptr<sf::Window> window_;      ///< SFML window to monitor
    float startX_;                           ///< Grid origin x position
    float startY_;                           ///< Grid origin y position
    float cellWidth_;                        ///< Width of grid cells
    float cellHeight_;                       ///< Height of grid cells
    float gridThickness_;                    ///< Thickness of grid lines
    std::tuple<bool, int, int> lastCoordInput_ = {false, -1, -1}; ///< Last input coordinates
};

} // namespace user_input

#endif // USER_INPUT_HPP