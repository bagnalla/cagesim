//
// Created by alex on 9/1/16.
//

#ifndef CAGESIM_STRATEGY_H
#define CAGESIM_STRATEGY_H

namespace cagesim
{
    class Strategy
    {
    public:
        Strategy(size_t nPlayers) { numPlayers = nPlayers; }
        size_t GetNumPlayers() { return numPlayers; }
        size_t GetNumStrategies() { return numStrategies; }
        virtual std::function<float(size_t, size_t, const std::vector<size_t>&)> GetCostFunction() = 0;
    protected:
        size_t numPlayers;
        size_t numStrategies;
        float min_cost;
        float max_cost;
    };
}

#endif //CAGESIM_STRATEGY_H
