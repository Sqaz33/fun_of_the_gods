#ifndef CREATURE_STRATEGY_HPP
#define CREATURE_STRATEGY_HPP

namespace creature_strategy {

struct ICreatureStrategy {
    virtual bool computeLiveStatus(int neighborsCount, bool isAlive) = 0; 
};

class ConwayCreatureStrategy :
    public ICreatureStrategy
{
public:
    bool computeLiveStatus(int neighborsCount, bool isAlive) override;
};

} // namespace creature_strategy


#endif // CREATURE_STRATEGY_HPP