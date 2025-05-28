/**
 * @file game_controller.hpp
 * @brief Header file for the Game Controller class.
 * 
 * Defines the main controller that manages game flow, player interactions,
 * and coordinates between model, view, and input components.
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
 * @brief Main controller class that manages the game lifecycle.
 * 
 * Coordinates between model, view, and input systems to implement game logic.
 * Implements the Observer pattern to respond to game events.
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
     * @brief 
     */
    GameController(
        std::unique_ptr<IGameFieldArea> area,
        std::shared_ptr<IGameModel> model,
        std::shared_ptr<view::IDrawableComposite> view,
        std::shared_ptr<IUserInput> input,
        std::shared_ptr<sf::RenderWindow> window,
        const std::unordered_map<int, sf::Color>& creatureColors);

    /**
     * @brief Observer pattern update method.
     * @param subj Weak pointer to the subject being observed
     * @param event_t Type of event that occurred
     */
    void update(std::weak_ptr<subject::ISubject> subj, int event_t) override;

    /**
     * @brief Main game loop entry point.
     * 
     * Manages the complete game lifecycle including setup, gameplay,
     * and post-game states until the game is closed.
     */
    void game();

private:
    // Rendering methods
    /**
     * @brief Redraws the window and displays the updated view.
     */
    void redrawWindowNDisplay_();
    
    /**
     * @brief Updates a specific cell in the grid canvas.
     * @param xidx X coordinate of the cell
     * @param yidx Y coordinate of the cell
     */
    void updateCellInGridCanvasInView_(int xidx, int yidx);
    
    /**
     * @brief Gets the grid canvas component from the view.
     * @return Shared pointer to the DrawableGridCanvas
     */
    std::shared_ptr<DrawableGridCanvas> getCanvasComp_();
    
    /**
     * @brief Clears the grid canvas in preparation for new rendering.
     */
    void clearGridCanvas_();

    /**
     * @brief Restarts the controller for a new game.
     */
    void restartController_();

    // Notification methods
    /**
     * @brief Notifies about a game winner.
     * @param player Winning player number
     */
    void notifyAboutWinner_(int player);
    
    /**
     * @brief Notifies about a game draw.
     */
    void noitfyAboutDraw_();
    
    /**
     * @brief Notifies about player participation.
     * @param player Player number
     * @param n Participation information
     */
    void notifyAboutPlayerParticipation_(int player, int n);
    
    /**
     * @brief Notifies about model computation progress.
     * @param n Current iteration number
     */
    void notifyAboutModelComputing_(int n);
    
    /**
     * @brief Sets text on the text component in the view.
     * @param txt Text to display
     */
    void setTextOnTextComp_(const std::string& txt);

private:
    // Dependencies
    std::unique_ptr<IGameFieldArea> area_;                  ///< Game field area implementation
    std::shared_ptr<IGameModel> model_;                     ///< Game model implementation
    std::shared_ptr<view::IDrawableComposite> view_;        ///< Composite view component
    std::shared_ptr<IUserInput> input_;                     ///< User input handler
    std::shared_ptr<sf::RenderWindow> window_;              ///< SFML render window

    // Configuration
    std::unordered_map<int, sf::Color> creatureColors_;     ///< Creature ID to color mapping

    bool gameModelSetupPhase_ = false;

};

} // namespace game_controller

#endif // GAME_CONTROLLER_HPP