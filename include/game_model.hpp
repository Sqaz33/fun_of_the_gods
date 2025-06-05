#ifndef GAME_MODEL_HPP
#define GAME_MODEL_HPP

#include <memory>
#include <utility>
#include <tuple>
#include <map>

#include "creature_factory.hpp"
#include "game_field_area_factory.hpp"
#include "game_field_area.hpp"
#include "observer.hpp"
#include "player.hpp"
#include "creature_strategy.hpp"

namespace game_model {

struct IGameModel {
    virtual void game() = 0;
    virtual std::shared_ptr<player::Player> curPlayer() const noexcept = 0;
    virtual std::shared_ptr<player::Player> winnerPlayer() const noexcept = 0;
    virtual int movesRemained() const noexcept = 0;
    virtual int erRemained() const noexcept = 0;
    
    virtual ~IGameModel() = default;
};

class GameModel : 
    public IGameModel,
    public observer::IObserver,
    public subject::ISubject,
    public std::enable_shared_from_this<GameModel>
{
    using IGameFieldArea = game_field_area::IGameFieldArea;
    using IGameFieldAreaCurryFactory = factory::IGameFieldAreaCurryFactory;
    using ICreatureStrategy = creature_strategy::ICreatureStrategy;

public:
    GameModel(
        int creatNumberFirstTime,
        int creatNumber,
        int erCount,
        std::unique_ptr<IGameFieldArea> area, 
        std::unique_ptr<IGameFieldAreaCurryFactory> areaFactory,
        const std::vector<std::shared_ptr<player::Player>>& players,
        std::unique_ptr<ICreatureStrategy> creatStrategy);
    
public: 
    void attach(
        std::shared_ptr<observer::IObserver> obs, int event_t) override;
    void detach(
        std::weak_ptr<observer::IObserver> obs, int event_t) override;

private:
    void notify(int event_t) override;

public:
    void update(int event_t) override;

public:
    void game() override;
    std::shared_ptr<player::Player> curPlayer() const noexcept override;
    std::shared_ptr<player::Player> winnerPlayer() const noexcept override;
    int movesRemained() const noexcept override;
    int erRemained() const noexcept override;

private:
    void giveAreasForTwoPlayers_();
    void setupField_(int N);
    void setupFieldForPlayer_(int creatureNumber, 
            std::shared_ptr<player::Player> player);
    void computeErs_(int erCount);
#ifdef TEST
public:
#endif
    std::tuple<bool, bool, std::shared_ptr<player::Player>> 
        computeEr_();

#ifdef TEST
private:
#endif
    void computeAside_();
    void applyNClearAside_();
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
    std::unique_ptr<IGameFieldArea> area_;                    
    std::unique_ptr<ICreatureStrategy> creatStrategy_;
    
    std::vector<
        std::tuple< std::shared_ptr<player::Player>, bool, int, int>> aside_;
    std::vector<std::shared_ptr<player::Player>> players_; 

    bool roundIsOver_ = false;            
    bool askedRestart_ = false;           
    bool askedClose_ = false;             
    bool setupPhase_ = false;             
    
    std::shared_ptr<player::Player> curPlayer_;          
    std::shared_ptr<player::Player> winnerPlayer_;       
    int curPlayerCreatNumber_;                           
    int erRemained_;                                     
};

} // namespace game_model

#endif // GAME_MODEL_HPP