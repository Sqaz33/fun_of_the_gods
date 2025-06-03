#ifndef FIGURE_HPP
#define FIGURE_HPP 

namespace figure {

struct IFigure {
    virtual bool isPointInFigure(float x, float y) = 0;
    virtual ~IFigure() = default;
};

class Romb : public IFigure {
public:
    Romb(float diagonalA, float diagonalB);

public:
    bool isPointInFigure(float x, float y) override;

private:
    float diagonalA_; 
    float diagonalB_;
};

class DummyFigure : public IFigure {
public:
    bool isPointInFigure(float, float) override;
};

} // namespace figure

#endif // FIGURE_HPP