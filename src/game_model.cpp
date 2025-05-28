#include "game_model.hpp"

#include <algorithm>
#include <numeric>
#include <array>
#include <tuple>
#include <iterator>
#include <chrono>
#include <thread>

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
    giveCreatureIdPlayers_();
}

void GameModel::attach(
    std::shared_ptr<observer::IObserver> obs, int event_t) 
{ subject::ISubject::attach(obs, event_t); }

void GameModel::detach(
    std::weak_ptr<observer::IObserver> obs, int event_t) 
{ subject::ISubject::detach(obs, event_t); }

void GameModel::notify(
    int event_t, std::weak_ptr<ISubject> slf) 
{ subject::ISubject::notify(event_t, slf); }

void GameModel::update(
    std::weak_ptr<subject::ISubject> subj, int event_t)
{
    using evt_t = game_event::event_t;
    auto evt = static_cast<evt_t>(event_t);
    auto lk = subj.lock();
    auto field = 
        std::dynamic_pointer_cast<game_field::IGameField>(lk);
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

std::shared_ptr<GameModel> GameModel::slf() {
    return shared_from_this();
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

int GameModel::curPlayer() const noexcept {
    return curPlayer_;
}

int GameModel::winnerPlayer() const noexcept {
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

void GameModel::giveCreatureIdPlayers_() {
    for (auto&& p : players_) {
        auto id = creature::nextCreatureId();
        p->setCreatId(id);
    }
}

void GameModel::setupField_(int creatureNumber) {
    for (int i = 0; i < players_.size(); ++i) {
        setupFieldForPlayer_(creatNumber_, i);
    }
}

void GameModel::setupFieldForPlayer_(int creatureNumber, int player) {
    setupPhase_ = true;
    auto p = players_[player];
    auto&& area = p->fieldArea();
    curPlayer_ = player;
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
    while (!askedClose_ && !askedRestart_ && erCount) {
        erRemained_ = erCount--;
        fireGameModelCalculatedEr_();
        auto [suc, id] = computeEr_(); 
        if (!suc) {
            roundIsOver_ = true;
            winnerPlayer_ = id;
            if (id == -1) {
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

std::pair<bool, int> GameModel::computeEr_() {
    computeAside_();
    applyNClearAside_();
    auto count = countAliveCreatureInArea_();
    if (count.size() < 2) {
        if (count.size() == 1) {
            return {false, count.begin()->first};
        } 
        return {false, -1};
    }
    return {true, 0};
} 

std::unordered_map<int, int> GameModel::countNeighbors_(int xidx, int yidx) const {
    constexpr std::array<const std::pair<int, int>, 8> neighbors {{
        {-1,  -1}, {0, -1}, {1, -1},
        {-1,   0},          {1,  0},
        {-1,   1}, {0,  1}, {1,  1}  
    }};

    std::unordered_map<int, int> res;
    for (auto&& d : neighbors) {
        auto x = xidx + d.first;
        auto y = yidx + d.second;
        if (area_->isCellAvailable(x, y)) {
            auto&& cr = area_->getCreatureByCell(x, y);
            if (cr.isAlive()) {
                auto it = res.find(cr.id());
                if (it != res.end()) {
                    ++it->second;
                } else {
                    res[cr.id()] = 1;
                }
            }
    
        }
    }

    return res;
}

void GameModel::computeAside_() {
    auto luCorner = area_->upperLeftCorner();
    auto rdCorner = area_->lowerRightCorner();
    
    for (auto y = luCorner.second; y <= rdCorner.second; ++y) {
        for (auto x = luCorner.first; x <= rdCorner.first; ++x) {
            if (area_->isCellAvailable(x, y)) {
                auto ne = countNeighbors_(x, y);
                int neSum = std::accumulate(ne.begin(), ne.end(), 
                                0, [] (int i, auto&& p) { return i + p.second; });
                auto&& cr = area_->getCreatureByCell(x, y);
                bool isAlive = cr.isAlive();
                if (computeLiveStatusConwayGame(isAlive, neSum)) {
                    if (!isAlive) {
                        auto max = std::max_element(ne.begin(), ne.end(), 
                                                    [] (auto&& l, auto&& p) 
                                                    { return l.second < p.second; });
                        int id = max->first;
                        aside_.emplace_back(id, true, x, y);
                    }
                } else if (isAlive) {
                    aside_.emplace_back(0, false, x, y);
                }
            }
        }
    }
}

void GameModel::applyNClearAside_() {
    for (auto&& as : aside_) {
        auto [id, rev, x, y] = as;
        if (rev) {
            area_->reviveCreatureInCell(x, y, id);
        } else {
            area_->killCreatureInCell(x, y);
        }
    }
    aside_.clear();
}

std::unordered_map<int, bool> GameModel::countAliveCreatureInArea_() {
    std::unordered_map<int, bool> res;
    auto corner = area_->upperLeftCorner();
    auto limW = area_->width() + corner.first;
    auto limH = area_->height() + corner.second;
    for (auto y = corner.second; y < limH; ++y) {
        for (auto x = corner.first; x < limW; ++x) {
            if (area_->isCellAvailable(x, y)) {
                auto&& cr = area_->getCreatureByCell(x, y);
                if (cr.isAlive()) {
                    res[cr.id()] = true;
                }
            }
        }
    }
    return res;
}

void GameModel::restartModel_() {
    area_->clear();
}

void GameModel::fireWinnerDeterminate_() {
    int evt = static_cast<int>(
        game_event::event_t::WINNER_DETERMINATE);
    notify(evt, slf());   
}

void GameModel::fireThereWasDraw_() {
    int evt = static_cast<int>(
        game_event::event_t::DRAW_DETERMINATE);
    notify(evt, slf());   
}

void GameModel::firePlayerBetsCreatures_() {
    int evt = static_cast<int>(
        game_event::event_t::PLAYER_BETS_CREATURES);
    notify(evt, slf());   
}

void GameModel::fireGameModelCalculatedEr_() {
    int evt = static_cast<int>(
        game_event::event_t::GAME_MODEL_CALCULATED_ER);
    notify(evt, slf());   
}

void GameModel::fireUserInputRequired() {
    int evt = static_cast<int>(
        game_event::event_t::USER_INPUT_REQUIRED);
    notify(evt, slf());   
}


} // namespace game_model
