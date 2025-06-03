#include "game_model.hpp"

#include <algorithm>
#include <numeric>
#include <array>
#include <tuple>
#include <iterator>
#include <chrono>
#include <thread>
#include <ranges>
#include <random>

namespace {
    using IGameFieldArea = game_field_area::IGameFieldArea;
    using IGameFieldAreaCurryFactory = factory::IGameFieldAreaCurryFactory;


    bool computeLiveStatusConwayGame(bool isAlive, int neighborsCount) {
        return (neighborsCount == 2 && isAlive) || neighborsCount == 3;
    }
} // namespace 

namespace game_model {

GameModel::GameModel(
        int creatNumberFirstTime,
        int creatNumber,
        int erCount,
        std::unique_ptr<IGameFieldArea> area, 
        std::unique_ptr<IGameFieldAreaCurryFactory> areaFactory,
        const std::vector<std::shared_ptr<player::Player>>& players):
    creatNumberFirstTime_(creatNumberFirstTime)
    , creatNumber_(creatNumber)
    , erCount_(erCount)
    , area_(std::move(area))
    , areaFactory_(std::move(areaFactory))
    , players_(players)
{
#ifndef TEST
    giveAreasForTwoPlayers_();
#endif
}

void GameModel::attach(
    std::shared_ptr<observer::IObserver> obs, int event_t) 
{ subject::ISubject::attach(obs, event_t); }

void GameModel::detach(
    std::weak_ptr<observer::IObserver> obs, int event_t) 
{ subject::ISubject::detach(obs, event_t); }

void GameModel::notify(int event_t) 
{ subject::ISubject::notify(event_t); }

void GameModel::update(int event_t)
{
    using evt_t = game_event::event_t;
    auto evt = static_cast<evt_t>(event_t);
    switch (evt) {
        case evt_t::USER_ASKED_CLOSE: {
            askedClose_ = true;
            break;
        }
        case evt_t::USER_ASKED_RESTART: { 
            askedRestart_ = true;
            restartModel_();
            break;
        }
        case evt_t::CREATURE_KILL_IN_FIELD: {
            if (setupPhase_) {
                ++curPlayerCreatNumber_;
            }   
            break;
        }
        case evt_t::CREATURE_REVIVE_IN_FIELD: { 
            if (setupPhase_) {
                --curPlayerCreatNumber_;
            }
        }
    }
}

void GameModel::game() {
    while (!askedClose_) {
        askedRestart_ = false;
        setupField_(creatNumberFirstTime_);
        while (!askedClose_ && !askedRestart_ && !roundIsOver_) {    
            computeErs_(erCount_);
            if (!roundIsOver_) setupField_(creatNumber_);
        }
        roundIsOver_ = false;
        while ((!askedRestart_) && (!askedClose_)) {
            fireUserInputRequired();
        }
    } 
}

std::shared_ptr<player::Player> 
GameModel::curPlayer() const noexcept {
    return curPlayer_;
}

std::shared_ptr<player::Player> 
GameModel::winnerPlayer() const noexcept {
    return winnerPlayer_;
}

int GameModel::movesRemained() const noexcept {
    return curPlayerCreatNumber_;
}

int GameModel::erRemained() const noexcept  {
    return erRemained_;
}

void GameModel::giveAreasForTwoPlayers_() {
    std::pair<int, int> ul1 = {0, 0};
    std::pair<int, int> lr1 = {area_->width() / 2 - 1, 
                                    area_->height() - 1};
    auto area1 = areaFactory_->createArea(ul1, lr1);
    players_[0]->setFieldArea(std::move(area1));
    
    std::pair<int, int> ul2 = {area_->width() / 2, 0};
    std::pair<int, int> lr2 = {area_->width() - 1, 
                                    area_->height() - 1};
    auto area2 = areaFactory_->createArea(ul2, lr2);
    players_[1]->setFieldArea(std::move(area2));
}

void GameModel::setupField_(int creatureNumber) {
    for (auto&& p : players_) {
        setupFieldForPlayer_(creatNumber_, p);
    }
}

void GameModel::setupFieldForPlayer_(int creatureNumber, 
    std::shared_ptr<player::Player> player) 
{
    setupPhase_ = true;
    curPlayer_ = player;
    auto&& area = player->fieldArea();
    curPlayerCreatNumber_ = creatureNumber;
    area.unlock();
    while (curPlayerCreatNumber_ && !askedClose_ && !askedRestart_) {
        firePlayerBetsCreatures_();
        fireUserInputRequired();
    } 
    area.lock();
    setupPhase_ = false;
}

void GameModel::computeErs_(int erCount) {
    while (!roundIsOver_ && erCount) {
        erRemained_ = erCount--;
        fireGameModelCalculatedEr_();
        auto [suc, win, player] = computeEr_(); 
        if (!suc) {
            roundIsOver_ = true;
            winnerPlayer_ = player;
            if (!win) {
                fireThereWasDraw_();
            } else {
                fireWinnerDeterminate_();
            }
        } else {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(250));
        }
    }
}

std::tuple<bool, bool, std::shared_ptr<player::Player>> 
GameModel::computeEr_() 
{
    computeAside_();
    applyNClearAside_();
    auto count = area_->checkCreatureInArea();
    if (count.size() < 2) {
        if (count.size() == 1) {
            return {false, true, *count.begin()};
        } 
        return {false, false, nullptr};
    }
    return {true, false, nullptr};
} 

void GameModel::computeAside_() {
    namespace views = std::ranges::views;
    auto luCorner = area_->upperLeftCorner();
    auto rdCorner = area_->lowerRightCorner();
    
    for (auto y = luCorner.second; y <= rdCorner.second; ++y) {
        for (auto x = luCorner.first; x <= rdCorner.first; ++x) {
            if (area_->isCellAvailable(x, y)) {
                auto ne = area_->countCellNeighborsCreatures(x, y);
                int neSum = std::accumulate(ne.begin(), ne.end(), 
                                0, [] (int i, auto&& p) { return i + p.second; });
                bool isAlive = area_->hasCreatureInCell(x, y);
                if (computeLiveStatusConwayGame(isAlive, neSum)) {
                    if (!isAlive) {
                        auto max = std::max_element(ne.begin(), ne.end(), 
                                                    [] (auto&& l, auto&& p) 
                                                    { return l.second < p.second; });
                        auto matchingMax = ne | 
                            views::filter([&max](auto&& v) 
                                    { return v.second == max->second; });
                        auto szMax = std::distance(matchingMax.begin(), 
                                                    matchingMax.end());
                        std::random_device r;
                        std::default_random_engine e1(r());
                        std::uniform_int_distribution<int> uniform_dist(0, szMax - 1);
                        int mean = uniform_dist(e1);
                        auto resMax = matchingMax.begin();
                        std::advance(resMax, mean);
                        auto player = resMax->first;
                        aside_.emplace_back(player, true, x, y);
                    }
                } else if (isAlive) {
                    aside_.emplace_back(nullptr, false, x, y);
                }
            }
        }
    }
}

void GameModel::applyNClearAside_() {
    for (auto [player, set, x, y] : aside_) {
        if (set) {
            area_->setCreatureInCell(x, y, player);
        } else {
            area_->removeCreatureInCell(x, y);
        }
    }
    aside_.clear();
}

void GameModel::restartModel_() {
    area_->clear();
}

void GameModel::fireWinnerDeterminate_() {
    int evt = static_cast<int>(
        game_event::event_t::WINNER_DETERMINATE);
    notify(evt);   
}

void GameModel::fireThereWasDraw_() {
    int evt = static_cast<int>(
        game_event::event_t::DRAW_DETERMINATE);
    notify(evt);   
}

void GameModel::firePlayerBetsCreatures_() {
    int evt = static_cast<int>(
        game_event::event_t::PLAYER_BETS_CREATURES);
    notify(evt);   
}

void GameModel::fireGameModelCalculatedEr_() {
    int evt = static_cast<int>(
        game_event::event_t::GAME_MODEL_CALCULATED_ER);
    notify(evt);   
}

void GameModel::fireUserInputRequired() {
    int evt = static_cast<int>(
        game_event::event_t::USER_INPUT_REQUIRED);
    notify(evt);   
}


} // namespace game_model
