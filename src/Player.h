//
// Created by alex on 9/1/16.
//

#ifndef CAGESIM_PLAYER_H
#define CAGESIM_PLAYER_H

#include <functional>
#include <vector>
#include "Strategy.h"

namespace cagesim
{
    class GameData;

    class Player
    {
    public:
        Player(size_t i, float e, Strategy *strat, GameData *gd);
        size_t ChooseStrategy();
        void Update(const std::vector<size_t>& s);
        void PrintDist();
    private:
        size_t id;
        float epsilon;
        Strategy *strategy;
        std::vector<double> weights;
        GameData *gameData; // all players have pointer to the same GameData object
    };
}

#endif //CAGESIM_PLAYER_H
