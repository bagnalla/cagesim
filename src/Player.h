//
// Created by alex on 9/1/16.
//

#ifndef CAGESIM_PLAYER_H
#define CAGESIM_PLAYER_H

#include <functional>
#include <vector>

namespace cagesim
{
    class GameData;
    class Strategy;

    class Player
    {
    public:
        Player(size_t i, float e, Strategy *strat);
        size_t ChooseStrategy() const;
        void Update(const std::vector<size_t>& s, GameData& gd);
        void PrintDist() const;
    private:
        size_t id;
        float epsilon;
        Strategy *strategy;
        std::vector<double> weights;
    };
}

#endif //CAGESIM_PLAYER_H
