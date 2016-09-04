#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>
#include <vector>
#include "CongestionStrategy.h"
#include "Game.h"

//#ifdef _OPENMP
//#include <omp.h>
//#endif

#define NUM_PLAYERS 8
#define NUM_RESOURCES 8
#define NUM_GAMES 64
#define NUM_ROUNDS 1000

void mergeData(const std::vector<cagesim::Game> games, cagesim::GameDatgia *data)
{
    auto numPlayers = games[0].gameData.strategyCosts[0].size();
    auto numStrategies = games[0].gameData.strategyCosts[0][0].size();
    size_t l = data->strategyCosts.size() - 1; // current round

#pragma omp parallel for
    for (size_t i = 0; i < numPlayers; ++i) {
        for (size_t j = 0; j < numStrategies; ++j) {
            float cost_sum = 0.0f;
            float weight_sum = 0.0f;
            for (size_t k = 0; k < games.size(); ++k) {
                cost_sum += games[k].gameData.strategyCosts[0][i][j];
                weight_sum += games[k].gameData.strategyWeights[0][i][j];
            }

            data->strategyCosts[l][i][j] = cost_sum / games.size();
            data->strategyWeights[l][i][j] = weight_sum / games.size();
        }
    }
}

void runSim(cagesim::GameData& averageData)
{
    cagesim::CongestionStrategy *strat = new cagesim::CongestionStrategy(NUM_PLAYERS, NUM_RESOURCES);
    float epsilon = 0.5f;
    std::vector<cagesim::Game> games;
    for (size_t i = 0; i < NUM_GAMES; ++i) {
        games.push_back(cagesim::Game(strat, epsilon));
    }

    for (size_t i = 0; i < NUM_ROUNDS; ++i)
    {
#pragma omp parallel for
        //for (auto it = games.begin(); it != games.end(); ++it) {
        for (size_t i = 0; i < games.size(); ++i) {
//            it->Next();
            games[i].Next();
        }

        averageData.AddRound(NUM_PLAYERS, strat->GetNumStrategies());
        mergeData(games, &averageData);
    }
}

int main()
{
    std::srand(static_cast<unsigned int>(std::time(0)));

    cagesim::GameData averageData;

    std::cout << "begin" << std::endl;;

    std::thread simThread (runSim, std::ref(averageData));
    simThread.join();

    std::cout << "end" << std::endl;
    return 0;
}