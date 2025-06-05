#ifndef CELL_FACTORY_HPP
#define CELL_FACTORY_HPP

#include <memory>

#include "cell.hpp"

namespace factory {

struct ICellFactory {
    virtual std::unique_ptr<cell::ICell> createCell() const = 0;
    
    virtual ~ICellFactory() = default;
};

class CellFactory : public ICellFactory {
public:
    std::unique_ptr<cell::ICell> createCell() const override;
};

} // namespace factory

#endif // CELL_FACTORY_HPP