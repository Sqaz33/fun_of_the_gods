#ifndef GAME_MODEL_HPP
#define GAME_MODEL_HPP

#include <memory>
#include <utility>
#include <map>

#include "creature.hpp"
#include "cell.hpp"
#include "game_field_area.hpp"

namespace game_model {

struct IGameModel {
    virtual std::pair<bool, int> compute() = 0;
    virtual ~IGameModel() {}
};

// template <class creature_t, template<class> class cell_t>
class GameModel : public IGameModel {
    using Creature = creature::Creature;
    using Cell = cell::Cell<Creature>;
    using IGameFieldArea = game_field_area::IGameFieldArea<Cell>;

public:
    GameModel(std::unique_ptr<IGameFieldArea> area, int playerCount);    
    std::pair<bool, int> compute() override;

private:
    std::map<int, int> countNeighbors_(int xidx, int yidx) const;
    void computeAside_();
    void applyNClearAside_();
    std::map<int, int> countCreatureInArea_();

private:
    std::unique_ptr<IGameFieldArea> area_;
    std::vector<std::tuple<int, bool, int, int>> aside_;
    int playerCount_;
};

} // namespace game_model

#endif 
