#include "figure.hpp"

#include <cmath>

namespace figure {

Romb::Romb(float diagonalA, float diagonalB) :
    diagonalA_(diagonalA)
    , diagonalB_(diagonalB)
{}

bool Romb::isPointInFigure(float x, float y) {
    x -= diagonalA_;
    y -= diagonalB_;
    return 
        (std::abs(x) * diagonalA_ 
            + std::abs(y) * diagonalB_) <= diagonalA_ * diagonalB_;
}

bool DummyFigure::isPointInFigure(float, float)
{ return true; } 

} // namespace figure