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
    struct CongestionData
    {
        std::vector<std::vector<std::vector<float>>> resourceWeights;
        std::vector<std::vector<std::vector<float>>> resourceProbs;
        std::vector<std::vector<std::vector<float>>> resourceExpectedNumPlayers;

        void AddRound(size_t numPlayers, size_t numResources)
        {
            resourceWeights.push_back(std::vector<std::vector<float> >(numPlayers));
            auto round = &resourceWeights[resourceWeights.size() - 1];
            for (auto it = round->begin(); it != round->end(); ++it) {
                *it = (std::vector<float>(numResources));
            }

            resourceProbs.push_back(std::vector<std::vector<float> >(numPlayers));
            round = &resourceProbs[resourceProbs.size() - 1];
            for (auto it = round->begin(); it != round->end(); ++it) {
                *it = (std::vector<float>(numResources));
            }

            resourceExpectedNumPlayers.push_back(std::vector<std::vector<float> >(numPlayers));
            round = &resourceExpectedNumPlayers[resourceExpectedNumPlayers.size() - 1];
            for (auto it = round->begin(); it != round->end(); ++it) {
                *it = (std::vector<float>(numResources));
            }
        }
    };

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

        std::function<float(size_t, size_t, const std::vector<size_t> &)> GetCostFunction() const
        { return cost; }

        void AddData(const GameData &d)
        {
            cData.AddRound(numPlayers, numResources);

            size_t latest_d_round = d.strategyWeights.size() - 1;
            size_t current_round = cData.resourceWeights.size() - 1;
            std::vector<float> weight_sums = std::vector<float>(numPlayers, 0.0f);

            // resource weights
            if (latest_d_round >= 0) {
                // per player
                for (size_t i = 0; i < d.strategyWeights[latest_d_round].size(); ++i) {
                    // per strategy
                    for (size_t j = 0; j < d.strategyWeights[latest_d_round][i].size(); ++j) {
                        // per resource
                        for (size_t k = 0; k < numResources; ++k) {
                            if ((j & (1 << k)) != 0) { // if resource is in the strategy
                                // add weight to resource
                                cData.resourceWeights[current_round][i][k] += d.strategyWeights[latest_d_round][i][j];
                                weight_sums[i] += d.strategyWeights[latest_d_round][i][j];
                            }
                        }
                    }
                }
            }

            // resource probabilities
            if (latest_d_round >= 0) {
                // per player
                for (size_t i = 0; i < cData.resourceProbs[current_round].size(); ++i) {
                    // per resource
                    for (size_t j = 0; j < cData.resourceProbs[current_round][i].size(); ++j) {
                        cData.resourceProbs[current_round][i][j] =
                                cData.resourceWeights[current_round][i][j] / weight_sums[i];
                    }
                }
            }

            // resource expected number of players
            if (latest_d_round >= 0) {
                // per player
                for (size_t i = 0; i < cData.resourceProbs[current_round].size(); ++i) {
                    // per resource
                    for (size_t j = 0; j < cData.resourceProbs[current_round][i].size(); ++j) {
                        cData.resourceExpectedNumPlayers[current_round][i][j] +=
                                cData.resourceProbs[current_round][i][j];
                    }
                }
            }
        }

        CongestionData cData;

    private:
        size_t numResources;

        std::function<float(size_t, size_t, const std::vector<size_t> &)> cost =
                [this](size_t id, size_t strat, const std::vector<size_t> &s) {
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
