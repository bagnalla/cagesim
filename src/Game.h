//
// Created by alex on 9/1/16.
//

#ifndef CAGESIM_GAME_H
#define CAGESIM_GAME_H

#include <functional>
#include <vector>
#include "Player.h"

namespace cagesim
{
    class Strategy;

    struct GameData
    {
        // per round, per player, per strategy
        std::vector<std::vector<std::vector<float>>> strategyCosts;
        std::vector<std::vector<std::vector<double>>> strategyWeights;

        void AddRound(size_t numPlayers, size_t numStrategies);
    };

    class Game
    {
    public:
        Game(Strategy *strat, float e);

//        ~Game()
//        {
//            delete strategy;
//        }

        void Next();

        void PrintStuff();

        GameData gameData;

    private:
        Game() {}

        Strategy *strategy;
        std::vector<Player> players;

        size_t t; // round counter
//        GameData gameData;

//        void initNextRoundGameData()
//        {
//            gameData.strategyCosts.push_back(std::vector<std::vector<float> >(players.size()));
//            auto round = &gameData.strategyCosts[gameData.strategyCosts.size() - 1];
//            for (auto it = round->begin(); it != round->end(); ++it) {
//                *it = (std::vector<float>(strategy->GetNumStrategies()));
//            }
//
//            gameData.strategyWeights.push_back(std::vector<std::vector<float> >(players.size()));
//            round = &gameData.strategyWeights[gameData.strategyWeights.size() - 1];
//            for (auto it = round->begin(); it != round->end(); ++it) {
//                *it = (std::vector<float>(strategy->GetNumStrategies()));
//            }
//        }
    };
}

#endif //CAGESIM_GAME_H
