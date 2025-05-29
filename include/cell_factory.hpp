#ifndef CELL_FACTORY_HPP
#define CELL_FACTORY_HPP

#include <memory>

#include "cell.hpp"

namespace factory {

/**
 * @interface ICellFactory
 * @brief Interface for cell factory implementations.
 * 
 * This interface defines the contract for factories that create cell objects.
 * Implementations should provide concrete creation logic for specific cell types.
 */
struct ICellFactory {
    /**
     * @brief Creates a new cell instance.
     * @return std::unique_ptr<cell::ICell> A unique pointer to the newly created cell object.
     * 
     * This pure virtual function must be implemented by derived classes to provide
     * specific cell creation functionality.
     */
    virtual std::unique_ptr<cell::ICell> createCell() const = 0;
    
    /**
     * @brief Virtual destructor for proper polymorphic destruction.
     */
    virtual ~ICellFactory() = default;
};

/**
 * @class CellFactory
 * @brief Concrete implementation of the cell factory.
 * 
 * This class provides a concrete implementation of the ICellFactory interface,
 * creating instances of cell objects. The exact type of cell created depends
 * on the implementation details of this factory.
 */
class CellFactory : public ICellFactory {
public:
    /**
     * @brief Creates a new cell instance.
     * @return std::unique_ptr<cell::ICell> A unique pointer to the newly created cell object.
     * 
     * This function overrides the base interface method to provide concrete
     * cell creation functionality.
     */
    std::unique_ptr<cell::ICell> createCell() const override;
};

} // namespace factory

#endif // CELL_FACTORY_HPP