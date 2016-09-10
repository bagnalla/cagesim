//
// Created by alex on 9/9/16.
//

#include "Game.h"
#include "Strategy.h"

namespace cagesim
{
    // GameData

    void GameData::AddRound(size_t numPlayers, size_t numStrategies)
    {
        strategyCosts.push_back(std::vector<std::vector<float> >(numPlayers));
        auto round = &strategyCosts[strategyCosts.size() - 1];
        for (auto it = round->begin(); it != round->end(); ++it) {
            *it = (std::vector<float>(numStrategies));
        }

        strategyWeights.push_back(std::vector<std::vector<double> >(numPlayers));
        auto roundD = &strategyWeights[strategyWeights.size() - 1];
        for (auto it = roundD->begin(); it != roundD->end(); ++it) {
            *it = (std::vector<double>(numStrategies));
        }
    }

    // Game

    Game::Game(Strategy *strat, float e)
    {
        strategy = strat;
        for (uint i = 0; i < strat->GetNumPlayers(); ++i) {
            players.push_back(Player(i, e, strat));
        }
        t = 0;

        // only track one round
        gameData.AddRound(strat->GetNumPlayers(), strat->GetNumStrategies());
    }

    void Game::Next()
    {
        std::vector<uint> s(players.size());

        for (size_t i = 0; i < players.size(); ++i) {
            s[i] = players[i].ChooseStrategy();
        }

        for (auto it = players.begin(); it != players.end(); ++it) {
            it->Update(s, gameData);
        }

        strategy->AddData(gameData);

        ++t;
    }

    void Game::PrintStuff()
    {
        for (auto it = players.begin(); it != players.end(); ++it) {
            it->PrintDist();
        }
    }
}