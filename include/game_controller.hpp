/**
 * @file game_controller.hpp
 * @brief Game Controller class - central coordinator of game components
 * 
 * Defines the main controller class responsible for managing:
 * - Game lifecycle and flow control
 * - Coordination between Model, View and Input systems
 * - Event handling and state transitions
 * - User interaction processing
 * 
 * Implements the Observer pattern to react to game state changes.
 */

#ifndef GAME_CONTROLLER_HPP
#define GAME_CONTROLLER_HPP

#include <unordered_map>
#include "game_field_area_factory.hpp"
#include "player.hpp"
#include "user_input.hpp"
#include "view.hpp"
#include "game_model.hpp"

namespace game_controller {

/**
 * @class GameController
 * @brief Central game management system implementing MVC architecture
 * 
 * Responsibilities include:
 * - Initializing and managing game components
 * - Processing user input and translating to game actions
 * - Updating model and view states
 * - Handling game events and transitions
 * - Managing render loop and frame updates
 * 
 * @note Inherits from IObserver to receive game state notifications
 */
class GameController : public observer::IObserver {
    using IGameField = game_field::IGameField;
    using IGameFieldArea = game_field_area::IGameFieldArea;
    using IGameModel = game_model::IGameModel;
    using IUserInput = user_input::IUserInput;
    using IGameFieldAreaCurryFactory = factory::IGameFieldAreaCurryFactory;
    using DrawableGridCanvas = view::DrawableGridCanvas;

public:
    /**
     * @brief Constructs the GameController with all required dependencies
     * @param area Game field implementation
     * @param model Shared pointer to game model
     * @param view Composite view component
     * @param input User input handler
     * @param window SFML render window
     * @param creatureColors Mapping of creature IDs to display colors
     */
    GameController(
        std::unique_ptr<IGameFieldArea> area,
        std::shared_ptr<IGameModel> model,
        std::shared_ptr<view::IDrawableComposite> view,
        std::shared_ptr<IUserInput> input,
        std::shared_ptr<sf::RenderWindow> window,
        const std::unordered_map<int, sf::Color>& creatureColors);

    /**
     * @brief Handles notifications from observed subjects
     * @param subj Weak pointer to originating subject
     * @param event_t Type of event that occurred (from game_event::event_t)
     * 
     * @details Processes game events including:
     * - Field/cell state changes
     * - Game phase transitions
     * - Player actions
     * - Win/draw conditions
     */
    void update(std::weak_ptr<subject::ISubject> subj, int event_t) override;

    /**
     * @brief Main game execution loop
     * 
     * Manages the complete game lifecycle:
     * 1. Initial setup and configuration
     * 2. Main gameplay loop
     * 3. Post-game states
     * 4. Cleanup and restart handling
     * 
     * @note Runs until game window is closed
     */
    void game();

private:
    // Rendering methods
    /**
     * @brief Updates and redraws the game window
     * 
     * Performs:
     * - View component updates
     * - Window clearing
     * - Drawing of all elements
     * - Display buffer swap
     */
    void redrawWindowNDisplay_();
    
    /**
     * @brief Updates specific grid cell visualization
     * @param xidx X coordinate (0-based)
     * @param yidx Y coordinate (0-based)
     */
    void updateCellInGridCanvasInView_(int xidx, int yidx);
    
    /**
     * @brief Retrieves the grid canvas component from view
     * @return Shared pointer to DrawableGridCanvas
     */
    std::shared_ptr<DrawableGridCanvas> getCanvasComp_();
    
    /**
     * @brief Resets the grid canvas to empty state
     */
    void clearGridCanvas_();

    /**
     * @brief Resets controller state for new game
     * 
     * Clears all transient state and prepares for:
     * - New game initialization
     * - Model reset
     * - View reset
     */
    void restartController_();

    // Notification handlers
    /**
     * @brief Handles winner determination event
     * @param player Winning player ID (0 or 1)
     */
    void notifyAboutWinner_(int player);
    
    /**
     * @brief Handles draw condition event
     */
    void notifyAboutDraw_();
    
    /**
     * @brief Processes player participation updates
     * @param player Player ID (0 or 1)
     * @param n Participation state code
     */
    void notifyAboutPlayerParticipation_(int player, int n);
    
    /**
     * @brief Handles model computation progress updates
     * @param n Current computation iteration
     */
    void notifyAboutModelComputing_(int n);
    
    /**
     * @brief Updates text display component
     * @param txt Text to display
     */
    void setTextOnTextComp_(const std::string& txt);

private:
    // Component dependencies
    std::unique_ptr<IGameFieldArea> area_;          ///< Game field data and logic
    std::shared_ptr<IGameModel> model_;             ///< Game rules and state
    std::shared_ptr<view::IDrawableComposite> view_;///< Visual representation
    std::shared_ptr<IUserInput> input_;             ///< User input processing
    std::shared_ptr<sf::RenderWindow> window_;      ///< Render target

    // Configuration
    std::unordered_map<int, sf::Color> creatureColors_; ///< Creature visualization colors

    // State flags
    bool gameModelSetupPhase_ = false; ///< Tracks initialization phase status
};

} // namespace game_controller

#endif // GAME_CONTROLLER_HPP