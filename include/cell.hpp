/**
 * @file cell.hpp
 * @brief Header file for the Cell interface and implementation.
 */

#ifndef CELL_HPP
#define CELL_HPP

#include <memory>
#include <map>
#include <vector>

#include "creature.hpp"

namespace cell {

/**
 * @interface ICell
 * @brief Interface representing a cell that can contain a creature.
 * 
 * This interface defines the basic operations that any cell implementation
 * should provide to interact with the creature it contains.
 */
struct ICell {
    /**
     * @brief Get a const reference to the creature in the cell.
     * @return const reference to the ICreature object.
     */
    virtual const creature::ICreature& creature() const = 0;
    
    /**
     * @brief Get a reference to the creature in the cell.
     * @return reference to the ICreature object.
     */
    virtual creature::ICreature& creature() = 0;
    
    /**
     * @brief Set or replace the creature in the cell.
     * @param creat Unique pointer to the new ICreature object.
     */
    virtual void setCreature(std::unique_ptr<creature::ICreature> creat) = 0;

    virtual bool hasCreature() const = 0;

    virtual void removeCreature() = 0;

    virtual void addNeighbors(const ICell* ne) = 0;

    virtual std::map<const std::shared_ptr<player::Player>, int> 
        countNeighborsCreatures() const = 0;

    /**
     * @brief Virtual destructor for proper polymorphic destruction.
     */
    virtual ~ICell() = default;
};

/**
 * @class Cell
 * @brief Concrete implementation of the ICell interface.
 * 
 * This class provides a basic implementation of a cell that can contain
 * a single creature. The creature is managed using a unique pointer.
 */
class Cell : public ICell {
public:
    /**
     * @brief Get a const reference to the creature in the cell.
     * @return const reference to the ICreature object.
     * @throws std::runtime_error if no creature is set.
     */
    const creature::ICreature& creature() const override;
    
    /**
     * @brief Get a reference to the creature in the cell.
     * @return reference to the ICreature object.
     * @throws std::runtime_error if no creature is set.
     */
    creature::ICreature& creature() override;
    
    /**
     * @brief Set or replace the creature in the cell.
     * @param creat Unique pointer to the new ICreature object.
     *        If nullptr is passed, the current creature is removed.
     */
    void setCreature(std::unique_ptr<creature::ICreature> creat) override;

    bool hasCreature() const override;

    void removeCreature() override;

    void addNeighbors(const ICell* ne) override;

    std::map<const std::shared_ptr<player::Player>, int> 
        countNeighborsCreatures() const override;

private:
    std::unique_ptr<creature::ICreature> creat_; ///< Unique pointer to the contained creature
    std::vector<const ICell*> neighbors_;
};

} // namespace cell

#endif // CELL_HPP