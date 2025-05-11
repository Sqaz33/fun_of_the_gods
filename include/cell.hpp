#ifndef CELL_HPP
#define CELL_HPP

namespace cell {

template <class creature_t>
struct ICell {
    virtual creature_t& creature() noexcept = 0;
    virtual void setCreature(const creature_t& creat) = 0;
    virtual ~ICell() {}
}; 

template <class creature_t>
class Cell : public ICell<creature_t> {
public:
    Cell(const creature_t& init) :
        creature_(init)
    {}

public:
    creature_t& creature() noexcept override {
        return creature_;
    }

    void setCreature(const creature_t& cr) override{
        creature_ = cr;
    }

private:
    creature_t creature_;
};

} // namespace cell

#endif 