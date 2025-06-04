#include "creature_strategy.hpp"

namespace creature_strategy {

bool ConwayCreatureStrategy::computeLiveStatus(
    int neighborsCount, bool isAlive) 
{
    return (neighborsCount == 2 && isAlive) || neighborsCount == 3;
}


} // namespace creature_strategy