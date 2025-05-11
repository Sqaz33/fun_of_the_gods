// #include "game_model.hpp"

// #include <algorithm>
// #include <numeric>
// #include <array>
// #include <tuple>

// namespace {
//     using Creature = creature::Creature;
//     using Cell = cell::Cell<Creature>;
//     using IGameFieldArea = game_field_area::IGameFieldArea<Cell>;

//     bool computeLiveStatusConwayGame(bool isAlive, int neighborsCount) {
//         return (neighborsCount > 2 && isAlive) || neighborsCount == 3;
//     }
// } // namespace 

// namespace game_model {

// GameModel::GameModel(
//     std::unique_ptr<IGameFieldArea> area, int playerCount) :
//     area_(std::move(area))
//     , playerCount_(playerCount)
// {}

// std::pair<bool, int> GameModel::compute() {
//     computeAside_();
//     applyNClearAside_();
//     auto count = countCreatureInArea_();
//     if (count.size() < 2) {
//         if (count.size() == 1) {
//             return {false, count.begin()->first};
//         } 
//         return {false, -1};
//     }
//     return {true, 0};
// } 

// std::map<int, int> GameModel::countNeighbors_(int xidx, int yidx) const {
//     constexpr std::array<const std::pair<int, int>, 8> neighbors {{
//         {-1,  -1}, {0, -1}, {1, -1},
//         {-1,   0},          {1,  0},
//         {-1,   1}, {0,  1}, {1,  1}  
//     }};

//     std::map<int, int> res;
//     for (auto&& d : neighbors) {
//         auto x = xidx + d.first;
//         auto y = yidx + d.second;
//         if (area_->isCellAvailable(x, y)) {
//             auto&& cell = area_->getCell(x, y);
//             auto&& cr = cell.creature();
//             if (cr.isAlive()) {
//                 auto it = res.find(cr.id());
//                 if (it != res.end()) {
//                     ++it->second;
//                 } else {
//                     res[cr.id()] = 1;
//                 }
//             }
    
//         }
//     }

//     return res;
// }

// void GameModel::computeAside_() {
//     auto corner = area_->getUpperLeftCorner();
//     auto limW = area_->width() + corner.first;
//     auto limH = area_->height() + corner.second;
//     for (auto y = corner.second; y < limH; ++y) {
//         for (auto x = corner.first; x < limW; ++x) {
//             if (area_->isCellAvailable(x, y)) {
//                 auto ne = countNeighbors_(x, y);
//                 int neSum = std::accumulate(ne.begin(), ne.end(), 
//                                 0, [] (int i, auto&& p) { return i + p.second; });
//                 auto&& cell = area_->getCell(x, y);
//                 auto&& cr = cell.creature();
//                 bool isAlive = cr.isAlive();
//                 if (computeLiveStatusConwayGame(isAlive, neSum)) {
//                     if (!isAlive) {
//                         auto max = std::max_element(ne.begin(), ne.end(), 
//                                                     [] (auto&& p) { return p.second; });
//                         int id = max->first;
//                         aside_.emplace_back(id, true, x, y);
//                     }
//                 } else {
//                     aside_.emplace_back(0, false, x, y);
//                 }
//             }
//         }
//     }
// }

// void GameModel::applyNClearAside_() {
//     for (auto&& as : aside_) {
//         auto [id, isAlive, x, y] = as;
//         auto&& cell = area_->getCell(x, y);
//         auto&& cr = cell.creature();
//         cr.setID(id);
//         if (isAlive) {
//             cr.revive();
//         } else {
//             cr.kill(); 
//         }
//     }
//     aside_.clear();
// }

// std::map<int, int> GameModel::countCreatureInArea_() {
//     std::map<int, int> res;
//     auto corner = area_->getUpperLeftCorner();
//     auto limW = area_->width() + corner.first;
//     auto limH = area_->height() + corner.second;
//     for (auto y = corner.second; y < limH; ++y) {
//         for (auto x = corner.first; x < limW; ++x) {
//             if (area_->isCellAvailable(x, y)) {
//                 auto&& cell = area_->getCell(x, y);
//                 auto&& cr = cell.creature();
//                 auto it = res.find(cr.id());
//                 if (it != res.end()) {
//                     ++it->second;
//                 } else {
//                     res[cr.id()] = 0;
//                 }
//             }
//         }
//     }
//     return res;
// }

// } // namespace game_model
