//
// Created by alex on 9/1/16.
//

#ifndef CAGESIM_GAME_H
#define CAGESIM_GAME_H

#include <functional>
#include <vector>
#include "Player.h"
#include "Strategy.h"

namespace cagesim
{
    struct GameData
    {
        // per round, per player, per strategy
        std::vector<std::vector<std::vector<float>>> strategyCosts;
        std::vector<std::vector<std::vector<float>>> strategyWeights;

        void AddRound(size_t numPlayers, size_t numStrategies)
        {
            strategyCosts.push_back(std::vector<std::vector<float> >(numPlayers));
            auto round = &strategyCosts[strategyCosts.size() - 1];
            for (auto it = round->begin(); it != round->end(); ++it) {
                *it = (std::vector<float>(numStrategies));
            }

            strategyWeights.push_back(std::vector<std::vector<float> >(numPlayers));
            round = &strategyWeights[strategyWeights.size() - 1];
            for (auto it = round->begin(); it != round->end(); ++it) {
                *it = (std::vector<float>(numStrategies));
            }
        }
    };

    class Game
    {
    public:
        Game(Strategy *strat, float e)
        {
            strategy = strat;
            for (size_t i = 0; i < strat->GetNumPlayers(); ++i) {
                players.push_back(Player(i, e, strat));
            }
            t = 0;

            gameData.AddRound(strat->GetNumPlayers(), strat->GetNumStrategies());
        }

//        ~Game()
//        {
//            delete strategy;
//        }

        void Next()
        {
            //gameData.AddRound(players.size(), strategy->GetNumStrategies());

            std::vector<size_t> s(players.size());

            for (size_t i = 0; i < players.size(); ++i) {
                s[i] = players[i].ChooseStrategy();
            }

            for (auto it = players.begin(); it != players.end(); ++it) {
                it->Update(s, gameData);
            }

            ++t;
        }

        void PrintStuff()
        {
            for (auto it = players.begin(); it != players.end(); ++it) {
                it->PrintDist();
            }
        }

        //GameData gameData;

    private:
        Game() {}

        Strategy *strategy;
        std::vector<Player> players;

        size_t t; // round counter
        GameData gameData;

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
