//
// Created by alex on 9/2/16.
//

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <stdexcept>
#include "Game.h"
#include "Strategy.h"

namespace cagesim
{
    Player::Player(uint i, double e, Strategy *strat)
    {
        id = i;
        epsilon = e;
        strategy = strat;
        weights = std::vector<double>(strat->GetNumStrategies(), 1.0f);
    }

    uint Player::ChooseStrategy() const
    {
        double sum_weights = 0.0;
        for (size_t i = 0; i < weights.size(); ++i) {
            sum_weights += weights[i];
        }

        double r = static_cast<double>(std::rand()) / RAND_MAX;
        double acc = 0.0;

        for (uint i = 0; i < weights.size(); ++i) {
            double p = weights[i] / sum_weights;
            if (acc <= r && r <= acc + p) {
                return i;
            }
            acc += p;
        }

        throw std::runtime_error("player " + std::to_string(id) + " unable to choose a strategy." +
                                 " r = " + std::to_string(r) + ", acc = " + std::to_string(acc));
    }

    void Player::Update(const std::vector<uint>& s, GameData& gd)
    {
        auto costFun = strategy->GetCostFunction();

        for (uint i = 0; i < weights.size(); ++i) {
            auto c = costFun(id, i, s);
            weights[i] *= pow(1.0 - epsilon, c);

            // update gameData with cost and weight
            //auto dfgdfg = gd.strategyCosts.size();
            //assert(0 < gd.strategyCosts.size() && gd.strategyCosts.size() < UINT32_MAX);
            gd.strategyCosts[gd.strategyCosts.size() - 1][id][i] = c;
            gd.strategyWeights[gd.strategyCosts.size() - 1][id][i] = weights[i];
        }
    }

    void Player::PrintDist() const
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
