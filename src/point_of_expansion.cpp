#include "point_of_expansion.hpp"

#include <cmath>
#include <numbers>

namespace game_field {
GamefieldWithFigureAndTriangularNeighbors::
GamefieldWithFigureAndTriangularNeighbors(
    int width, int height, 
    std::unique_ptr<factory::ICreatureFactory> creatFactory,
    std::unique_ptr<factory::ICellFactory> cellFactory,
    std::unique_ptr<figure::IFigure> figure):
    GameFieldWithFigure(width, height, 
                        std::move(creatFactory), 
                        std::move(cellFactory),
                        std::move(figure))
{ computeAddNeighbors_(); }

std::map<const std::shared_ptr<player::Player>, int> 
GamefieldWithFigureAndTriangularNeighbors::
countCellNeighborsCreatures(int xidx, int yidx) const {
    using base = GameFieldWithFigure;

    auto curCount =
         base::countCellNeighborsCreatures(
            xidx, yidx);
    
    std::pair<int, int> cur{xidx, yidx};
    // если мы в угле и клека доступна
    if (addNeighbor_.contains(cur)) {   
        // то получить и сложить с текущеми соседями
        auto [nex, ney] = addNeighbor_.at(cur);
        auto neCount = 
            base::countCellNeighborsCreatures(nex, ney);
        for (auto&& [p, c] : neCount) {
            auto it = curCount.find(p);
            if (it != curCount.end()) {
                it->second += c; 
            } else {
                curCount[p] = c;
            }
        }
    }

    return curCount;
}


void 
GamefieldWithFigureAndTriangularNeighbors::
computeAddNeighbors_() {
    namespace numb = std::numbers;
    int w = width();
    int h = height();
    // центр поля
    double cx = w / 2.0;
    double cy = h / 2.0;

    // длина стороны треугольника 
    double side = std::min(w, h) / 2.0;

    // расчёт координат вершин (по часовой стрелке)
    std::pair<int, int> vertices[3];
    for (int i = 0; i < 3; ++i) {
        double angle = numb::pi / 2 + i * 2 * numb::pi / 3;
        int x = static_cast<int>(std::round(cx + side * std::cos(angle)));
        int y = static_cast<int>(std::round(cy - side * std::sin(angle))); 
        vertices[i] = { x, y };
    }

    // для каждой вершины — найти середину противоположной стороны
    for (int i = 0; i < 3; ++i) {
        if (!isExcludedCell(vertices[i].first, vertices[i].second)) {
            std::pair<int, int> a = vertices[(i + 1) % 3];
            std::pair<int, int> b = vertices[(i + 2) % 3];
            std::pair<int, int> mid = { (a.first + b.first) / 2, (a.second + b.second) / 2 };
            if (!isExcludedCell(mid.first, mid.second)) {
                addNeighbor_[vertices[i]] = mid; 
            }
        }

    }
}

} // namespace game_field