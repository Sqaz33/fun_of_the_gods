/**
 * @file game_controller.hpp
 * @brief Header file for the Game Controller class.
 * 
 * Defines the main controller that manages game flow, player interactions,
 * and coordinates between model, view, and input components.
 */

#ifndef GAME_CONTROLLER_HPP
#define GAME_CONTROLLER_HPP

#include <chrono>
#include <thread>
#include <sstream>
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
     * @brief Constructs a GameController with all necessary dependencies.
     * @param K Number of creatures for the first time
     * @param T Number of model computation iterations
     * @param N Number of creatures
     * @param area Game field area implementation
     * @param areaFactory Factory for creating game field areas
     * @param model Game model implementation
     * @param view Composite view component
     * @param players List of players
     * @param input User input handler
     * @param window SFML render window
     * @param creatureColors Mapping of creature IDs to their colors
     */
    GameController(
        int K,
        int T,
        int N,
        std::unique_ptr<IGameFieldArea> area,
        std::unique_ptr<IGameFieldAreaCurryFactory> areaFactory,
        std::unique_ptr<IGameModel> model,
        std::shared_ptr<view::IDrawableComposite> view,
        const std::vector<std::shared_ptr<player::Player>>& players,
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

    // Game setup methods
    /**
     * @brief Distributes game areas to the two players.
     */
    void giveAreasForTwoPlayers_();
    
    /**
     * @brief Assigns creature IDs to players.
     */
    void giveCreatureIdPlayers_();
    
    /**
     * @brief Sets up the initial game field.
     * @param N Size of the field to setup
     */
    void setupField_(int N);
    
    /**
     * @brief Sets up the field for a specific player.
     * @param N Size of the field
     * @param player Player number (0 or 1)
     */
    void setupFieldForPlayer_(int N, int player);

    // Game logic methods
    /**
     * @brief Computes the game model for T iterations.
     * @param T Number of iterations to compute
     */
    void computeModel_(int T);
    
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
    std::unique_ptr<IGameFieldAreaCurryFactory> areaFactory_; ///< Factory for game field areas
    std::unique_ptr<IGameModel> model_;                     ///< Game model implementation
    std::shared_ptr<view::IDrawableComposite> view_;        ///< Composite view component
    std::vector<std::shared_ptr<player::Player>> players_;  ///< List of players
    std::shared_ptr<IUserInput> input_;                     ///< User input handler
    std::shared_ptr<sf::RenderWindow> window_;              ///< SFML render window

    // Configuration
    std::unordered_map<int, sf::Color> creatureColors_;     ///< Creature ID to color mapping
    int K_;                                                 ///< Number of cells needed to win
    int T_;                                                 ///< Number of model iterations
    int N_;                                                 ///< Initial field size
    int curN_;                                              ///< Current field size

    // Game state
    bool winnerDeterminate_ = false;                        ///< Whether winner is determined
    bool askedRestart_ = false;                             ///< Whether restart was requested
    bool askedClose_ = false;                               ///< Whether close was requested
    bool setupPhase_ = false;                               ///< Whether in setup phase
};

} // namespace game_controller

#endif // GAME_CONTROLLER_HPP