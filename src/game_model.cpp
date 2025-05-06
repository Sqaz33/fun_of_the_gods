#include "game_model.hpp"

#include <algorithm>
#include <numeric>
#include <array>
#include <tuple>

namespace {
    using Creature = creature::Creature;
    using Cell = cell::Cell<Creature>;
    using IGameFieldArea = game_field_area::IGameFieldArea<Cell>;

    bool computeLiveStatusConwayGame(bool isAlive, int neighborsCount) {
        return (neighborsCount > 2 && isAlive) || neighborsCount == 3;
    }
} // namespace 

namespace game_model {

GameModel::GameModel(
    std::unique_ptr<IGameFieldArea> area, int playerCount) :
    area_(std::move(area))
    , playerCount_(playerCount)
{}

std::pair<bool, creature::ICreature> GameModel::compute() {
    std::vector<std::tuple<int, bool, int, int>> aside;

    auto corner = area_->getUpperLeftCorner();
    auto limW = area_->width() + corner.first;
    auto limH = area_->height() + corner.second;
    for (auto y = corner.second; y < limH; ++y) {
        for (auto x = corner.first; x < limW; ++x) {
            if (area_->isCellAvailable(x, y)) {
                auto ne = countNeighbors_(x, y);
                int neSum = std::accumulate(ne.begin(), ne.end(), 
                                0, [] (auto&& p) { return p.second; });
                auto&& cell = area_->getCell(x, y);
                auto&& cr = cell.creature();
                bool isAlive = cr.isAlive();
                if (computeLiveStatusConwayGame(isAlive, neSum)) {
                    if (!isAlive) {
                        auto max = std::max_element(ne.begin(), ne.end(), 
                                                    [] (auto&& p) { return p.second; });
                        int id = max->first;
                        aside.emplace_back(id, true, x, y);
                    }
                } else {
                    aside.emplace_back(0, false, x, y);
                }
            }
        }
    }

    for (auto&& as : aside) {
        auto [id, isAlive, x, y] = as;
        auto&& cell = area_->getCell(x, y);
        auto&& cr = cell.creature();
        cr.setID(id);
        if (isAlive) {
            cr.revive();
        } else {
            cr.kill(); 
        }
    }
} 

std::map<int, int> GameModel::countNeighbors_(int xidx, int yidx) const {
    constexpr std::array<const std::pair<int, int>, 8> neighbors {{
        {-1,  -1}, {0, -1}, {1, -1},
        {-1,   0},          {1,  0},
        {-1,   1}, {0,  1}, {1,  1}  
    }};

    std::map<int, int> res;
    for (auto&& d : neighbors) {
        auto x = xidx + d.first;
        auto y = yidx + d.second;
        if (area_->isCellAvailable(x, y)) {
            auto&& cell = area_->getCell(x, y);
            auto&& cr = cell.creature();
            auto it = res.find(cr.id());
            if (it != res.end()) {
                ++it->second;
            } else {
                res[cr.id()] = 0;
            }
        }
    }

    return res;
}

} // namespace game_model
