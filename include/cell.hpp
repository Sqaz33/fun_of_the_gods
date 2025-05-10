#ifndef CELL_HPP
#define CELL_HPP

namespace cell {

template <class creature_t>
struct ICell {
    virtual const creature_t& creature() const noexcept = 0;
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
    const creature_t& creature() const noexcept override {
        return creature_;
    }

    void setCreature(const creature_t& cr) {
        creature_ = cr;
    }

private:
    creature_t creature_;
};

} // namespace cell

#endif 