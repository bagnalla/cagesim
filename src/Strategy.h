//
// Created by alex on 9/1/16.
//

#ifndef CAGESIM_STRATEGY_H
#define CAGESIM_STRATEGY_H

#include "Game.h"

namespace cagesim
{
    class Strategy
    {
    public:
        Strategy(size_t nPlayers) { numPlayers = nPlayers; }
        size_t GetNumPlayers() const { return numPlayers; }
        size_t GetNumStrategies() const { return numStrategies; }
        virtual std::function<float(uint, uint, const std::vector<uint>&)> GetCostFunction() const = 0;
        virtual void AddData(const GameData& d) = 0;
    protected:
        size_t numPlayers;
        size_t numStrategies;
        float min_cost;
        float max_cost;
    };
}

#endif //CAGESIM_STRATEGY_H
