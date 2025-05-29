/**
 * @file game_model.hpp
 * @brief Header file for the game model interfaces and implementations.
 * 
 * Defines the game model interface and implementation for computing
 * game state transitions and determining game outcomes. The model follows
 * the observer pattern to notify about state changes and implements
 * core game logic including creature interactions and win conditions.
 */

#ifndef GAME_MODEL_HPP
#define GAME_MODEL_HPP

#include <memory>
#include <utility>
#include <unordered_map>

#include "creature_factory.hpp"
#include "game_field_area_factory.hpp"
#include "game_field_area.hpp"
#include "observer.hpp"
#include "player.hpp"

namespace game_model {

/**
 * @interface IGameModel
 * @brief Interface for game models that compute game state transitions.
 * 
 * Provides the basic operation for computing one iteration of the game model
 * and accessing current game state information.
 */
struct IGameModel {
    /**
     * @brief Executes one full game iteration (round/turn).
     */
    virtual void game() = 0;

    /**
     * @brief Gets the current active player ID.
     * @return ID of the current player (0 or 1 typically)
     */
    virtual int curPlayer() const noexcept = 0;
    
    /**
     * @brief Gets the winner player ID if game has concluded.
     * @return ID of the winning player, or undefined if game not over
     */
    virtual int winnerPlayer() const noexcept = 0;

    /**
     * @brief Gets remaining moves in current turn/phase.
     * @return Number of moves remaining
     */
    virtual int movesRemained() const noexcept = 0;

    /**
     * @brief Gets remaining expected result calculations.
     * @return Number of ER calculations remaining
     */
    virtual int erRemained() const noexcept = 0;
    
    /**
     * @brief Virtual destructor for polymorphic behavior.
     */
    virtual ~IGameModel() = default;
};

/**
 * @class GameModel
 * @brief Concrete implementation of the game model.
 * 
 * Implements game logic for computing creature interactions and determining
 * game outcomes based on the current state of the game field area. Acts as
 * both observer and subject in the observer pattern.
 */
class GameModel : 
    public IGameModel,
    public observer::IObserver,
    public subject::ISubject,
    public std::enable_shared_from_this<GameModel>
{
    using IGameFieldArea = game_field_area::IGameFieldArea;
    using IGameFieldAreaCurryFactory = factory::IGameFieldAreaCurryFactory;

public:
    /**
     * @brief Constructs a new GameModel instance.
     * @param creatNumberFirstTime Initial number of creatures per player
     * @param creatNumber Regular number of creatures per player
     * @param erCount Number of expected result calculations per turn
     * @param area Game field area implementation
     * @param areaFactory Factory for creating game field areas
     * @param players List of player instances
     */
    GameModel(
        int creatNumberFirstTime,
        int creatNumber,
        int erCount,
        std::unique_ptr<IGameFieldArea> area, 
        std::unique_ptr<IGameFieldAreaCurryFactory> areaFactory,
        const std::vector<std::shared_ptr<player::Player>>& players);
    
public: 
    /**
     * @brief Attaches an observer to a specific event type.
     * @param obs Observer to attach
     * @param event_t Event type to observe
     */
    void attach(
        std::shared_ptr<observer::IObserver> obs, int event_t) override;

    /**
     * @brief Detaches an observer from a specific event type.
     * @param obs Observer to detach (weak reference)
     * @param event_t Event type to stop observing
     */
    void detach(
        std::weak_ptr<observer::IObserver> obs, int event_t) override;

private:
    /**
     * @brief Notifies all observers of a specific event.
     * @param event_t Event type to notify about
     * @param slf Weak reference to this subject
     */
    void notify(int event_t, std::weak_ptr<ISubject> slf) override;

public:
    /**
     * @brief Handles updates from observed subjects.
     * @param subj Subject that triggered the update
     * @param event_t Event type that occurred
     */
    void update(
        std::weak_ptr<subject::ISubject> subj, int event_t) override;

public:
    /**
     * @brief Gets a shared pointer to this instance.
     * @return Shared pointer to this GameModel
     */
    std::shared_ptr<GameModel> slf();

public:
    // IGameModel interface implementation
    void game() override;
    int curPlayer() const noexcept override;
    int winnerPlayer() const noexcept override;
    int movesRemained() const noexcept override;
    int erRemained() const noexcept override;

private:
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

    /**
     * @brief Computes initial expected results.
     * @param erCount Number of ER calculations to perform
     */
    void computeErs_(int erCount);
#ifdef TEST
public:
#endif
    // Model compute methods
    /**
     * @brief Computes one iteration of the game model.
     * @return Pair containing:
     *         - bool: true if game has concluded (win/draw)
     *         - int: winning player ID if game concluded
     * 
     * @note The computation follows these rules:
     *       1. Any live creature with fewer than two live neighbors dies
     *       2. Any live creature with two or three live neighbors lives on
     *       3. Any live creature with more than three live neighbors dies
     *       4. Any dead cell with exactly three live neighbors becomes alive
     */
    std::pair<bool, int> computeEr_();
#ifdef TEST
private:
#endif
    /**
     * @brief Counts neighboring creatures for a given cell.
     * @param xidx X coordinate of the cell
     * @param yidx Y coordinate of the cell
     * @return Map of creature IDs to their neighbor counts
     */
    std::unordered_map<int, int> countNeighbors_(int xidx, int yidx) const;
    
    /**
     * @brief Computes next state changes without applying them.
     */
    void computeAside_();
    
    /**
     * @brief Applies computed state changes and clears the aside buffer.
     */
    void applyNClearAside_();
    
    /**
     * @brief Counts alive creatures in the area by player.
     * @return Map of player IDs to alive status (true if any creatures alive)
     */
    std::unordered_map<int, bool> countAliveCreatureInArea_();

    /**
     * @brief Restarts the game model to initial state.
     */
    void restartModel_();

    // Event firing methods
    void fireWinnerDeterminate_();
    void fireThereWasDraw_();
    void firePlayerBetsCreatures_();
    void fireGameModelCalculatedEr_();
    void fireUserInputRequired();

private:
    // Configuration
    const int creatNumberFirstTime_;                          ///< Initial creature count per player
    const int creatNumber_;                                   ///< Regular creature count per player
    const int erCount_;                                       ///< ER calculations per turn
    std::unique_ptr<IGameFieldAreaCurryFactory> areaFactory_; ///< Area factory
    std::unique_ptr<IGameFieldArea> area_;                    ///< Game field area being modeled
    
    // Computation state
    std::vector<std::tuple<int, bool, int, int>> aside_;   ///< State transition buffer
    std::vector<std::shared_ptr<player::Player>> players_; ///< List of players

    // Game state
    bool roundIsOver_ = false;            ///< Whether winner is determined
    bool askedRestart_ = false;           ///< Whether restart was requested
    bool askedClose_ = false;             ///< Whether close was requested
    bool setupPhase_ = false;             ///< Whether in setup phase
    
    // Player state
    int curPlayer_;                       ///< Current active player ID
    int winnerPlayer_;                    ///< Winning player ID if game over
    int curPlayerCreatNumber_;            ///< Current player's creature count
    int erRemained_;                      ///< Remaining ER calculations
};

} // namespace game_model

#endif // GAME_MODEL_HPP