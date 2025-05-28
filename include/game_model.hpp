/**
 * @file game_model.hpp
 * @brief Header file for the game model interfaces and implementations.
 * 
 * Defines the game model interface and implementation for computing
 * game state transitions and determining game outcomes.
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
// TODO dox
namespace game_model {

/**
 * @interface IGameModel
 * @brief Interface for game models that compute game state transitions.
 * 
 * Provides the basic operation for computing one iteration of the game model.
 */
struct IGameModel {
    /**
     * @brief 
     */
    virtual void game() = 0;

    /*...*/
    virtual int curPlayer() const noexcept = 0;
    
    /*...*/
    virtual int winnerPlayer() const noexcept = 0;

    /*...*/
    virtual int movesRemained() const noexcept  = 0;

    /*...*/
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
 * game outcomes based on the current state of the game field area.
 */
class GameModel : 
    public IGameModel
    , public observer::IObserver
    , public subject::ISubject
    , public std::enable_shared_from_this<GameModel>
{
    using IGameFieldArea = game_field_area::IGameFieldArea;
    using IGameFieldAreaCurryFactory = factory::IGameFieldAreaCurryFactory;

public:
    /**
     * @brief 
     */
    GameModel(
        int creatNumberFirstTime,
        int creatNumber,
        int erCount,
        std::unique_ptr<IGameFieldArea> area, 
        std::unique_ptr<IGameFieldAreaCurryFactory> areaFactory,
        const std::vector<std::shared_ptr<player::Player>>& players);
    
public: 
    void attach(
        std::shared_ptr<observer::IObserver> obs, int event_t) override;

    void detach(
        std::weak_ptr<observer::IObserver> obs, int event_t) override;

private:
    void notify(int event_t, std::weak_ptr<ISubject> slf) override;

public:
    void update(
        std::weak_ptr<subject::ISubject> subj, int event_t) override;

public:
    std::shared_ptr<GameModel> slf();

public:
    void game() override;
    /*...*/
    int curPlayer() const noexcept override;
    
    /*...*/
    int winnerPlayer() const noexcept override;

    /*...*/
    int movesRemained() const noexcept override;

    /*...*/
    int erRemained() const noexcept  override;

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

    void computeErs_(int erCount);

    // Mdodel compute methods
    /**
     * @brief Computes one iteration of the game model.
     * @return std::pair<bool, int> where:
     *         - first: true if game has concluded (win/draw), false otherwise
     *         - second: winning player ID if game concluded, undefined otherwise
     * 
     * @note The computation follows these rules:
     *       1. Any live creature with fewer than two live neighbors dies (underpopulation)
     *       2. Any live creature with two or three live neighbors lives on
     *       3. Any live creature with more than three live neighbors dies (overpopulation)
     *       4. Any dead cell with exactly three live neighbors becomes a live creature (reproduction)
     */
    std::pair<bool, int> computeEr_();

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
     * @return Map of player IDs to whether they have alive creatures
     */
    std::unordered_map<int, bool> countAliveCreatureInArea_();

    void restartModel_();

    void fireWinnerDeterminate_();
    
    void fireThereWasDraw_();

    void firePlayerBetsCreatures_();

    void fireGameModelCalculatedEr_();

    void fireUserInputRequired();

private:
    const int creatNumberFirstTime_;
    const int creatNumber_;
    const int erCount_;
    std::unique_ptr<IGameFieldAreaCurryFactory> areaFactory_;
    std::unique_ptr<IGameFieldArea> area_;  ///< Game field area being modeled
    std::vector<std::tuple<int, bool, int, int>> aside_; ///< Buffer for state transitions
    std::vector<std::shared_ptr<player::Player>> players_;  ///< List of players

    // Game state
    bool roundIsOver_ = false;                        ///< Whether winner is determined
    bool askedRestart_ = false;                             ///< Whether restart was requested
    bool askedClose_ = false;                               ///< Whether close was requested
    bool setupPhase_ = false;                               ///< Whether in setup phase
    
    // ?? TODO:
    int curPlayer_;
    int winnerPlayer_;
    int curPlayerCreatNumber_;
    int erRemained_;

};

} // namespace game_model

#endif // GAME_MODEL_HPP