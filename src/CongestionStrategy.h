//
// Created by alex on 9/1/16.
//

#ifndef CAGESIM_CONGESTIONSTRATEGY_H
#define CAGESIM_CONGESTIONSTRATEGY_H

#include <cmath>
#include <functional>
#include <iostream>
#include <vector>
#include "Strategy.h"

namespace cagesim
{
    class CongestionStrategy : public Strategy
    {
    public:
        CongestionStrategy(size_t nPlayers, size_t numResources) : Strategy(nPlayers)
        {
            this->numResources = numResources;
            this->numStrategies = static_cast<size_t>(pow(2, numResources) - 1);
            min_cost = 1.0f;
            max_cost = numResources * nPlayers;
        }

        std::function<float(size_t, size_t, const std::vector<size_t>&)> GetCostFunction() const { return cost; }
    private:
        size_t numResources;

        std::function<float(size_t, size_t, const std::vector<size_t>&)> cost =
            [this](size_t id, size_t strat, const std::vector<size_t>& s)
            {
                float c = 0.0f;

                strat += 1;
                std::vector<bool> resources(numResources);
                for (size_t i = 0; i < numResources; ++i) {
                    resources[i] = static_cast<bool>(strat & (1 << i));
                    c += resources[i];
                }

                for (size_t i = 0; i < s.size(); ++i) {
                    if (i != id) {
                        for (size_t j = 0; j < numResources; ++j) {
                            c += resources[j] * ((s[i] + 1) & (1 << j)) != 0;
                        }
                    }
                }

//                std::cout << "s: [";
//                for (size_t i = 0; i < resources.size(); ++i) {
//                    std::cout << s[i] + 1;
//                    if (i < resources.size() - 1) {
//                        std::cout << ", ";
//                    }
//                }
//
//                std::cout << "]. player " << std::to_string(id) << ", strat " << std::to_string(strat) << ": [";
//                for (int i = static_cast<int>(resources.size() - 1); i >= 0; --i) {
//                    std::cout << resources[i];
//                    if (i > 0) {
//                        std::cout << ", ";
//                    }
//                }
//                std::cout << "]. cost: " << std::to_string(c / max_cost) << std::endl;

                return c / max_cost;
            };
    };
}

#endif //CAGESIM_CONGESTIONSTRATEGY_H
