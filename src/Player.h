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
        Player(uint i, double e, Strategy *strat);
        uint ChooseStrategy() const;
        void Update(const std::vector<uint>& s, GameData& gd);
        void PrintDist() const;
    private:
        uint id;
        double epsilon;
        Strategy *strategy;
        std::vector<double> weights;
    };
}

#endif //CAGESIM_PLAYER_H
