//
// Created by alex on 9/2/16.
//

#include <cmath>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <stdexcept>
#include "Game.h"

namespace cagesim
{
    Player::Player(size_t i, float e, Strategy *strat, GameData *gd)
    {
        id = i;
        epsilon = e;
        strategy = strat;
        gameData = gd;
        weights = std::vector<double>(strat->GetNumStrategies(), 1.0f);
    }

    size_t Player::ChooseStrategy()
    {
        double sum_weights = 0.0;
        for (size_t i = 0; i < weights.size(); ++i) {
            sum_weights += weights[i];
        }

        float r = static_cast<float>(std::rand()) / RAND_MAX;
        float acc = 0.0f;

        for (size_t i = 0; i < weights.size(); ++i) {
            double p = weights[i] / sum_weights;
            if (acc <= r && r <= acc + p) {
                return i;
            }
            acc += p;
        }

        throw std::runtime_error("player " + std::to_string(id) + " unable to choose a strategy.");
    }

    void Player::Update(const std::vector<size_t>& s)
    {
        auto costFun = strategy->GetCostFunction();

        for (size_t i = 0; i < weights.size(); ++i) {
            auto c = costFun(id, i, s);
            weights[i] *= pow(1.0 - epsilon, c);

            // update gameData with cost and weight
            gameData->strategyCosts[gameData->strategyCosts.size() - 1][id][i] = c;
            gameData->strategyWeights[gameData->strategyCosts.size() - 1][id][i] = weights[i];
        }
    }

    void Player::PrintDist()
    {
        double sum_weights = 0.0;
        for (size_t i = 0; i < weights.size(); ++i) {
            sum_weights += weights[i];
        }

        std::cout << "player " << std::to_string(id) << " dist: [";
        for (size_t i = 0; i < weights.size(); ++i) {
            std::cout << weights[i] / sum_weights;
            if (i < weights.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "]\n";

//        std::cout << "player " << std::to_string(id) << " weights: [";
//        for (size_t i = 0; i < weights.size(); ++i) {
//            std::cout << weights[i];
//            if (i < weights.size() - 1) {
//                std::cout << ", ";
//            }
//        }
//        std::cout << "]\n";
    }
}
