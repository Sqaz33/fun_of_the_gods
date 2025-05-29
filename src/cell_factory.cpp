#include "cell_factory.hpp"

namespace factory {

std::unique_ptr<cell::ICell> CellFactory::createCell() const 
{ return std::make_unique<cell::Cell>(); }


} // namespace factory