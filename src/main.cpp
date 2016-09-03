#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>
#include <vector>
#include "CongestionStrategy.h"
#include "Game.h"

#define NUM_PLAYERS 4
#define NUM_RESOURCES 4
#define NUM_GAMES 100
#define NUM_ROUNDS 1000

void mergeData(const std::vector<cagesim::Game> games, cagesim::GameData *data)
{
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
        for (auto it = games.begin(); it != games.end(); ++it) {
            it->Next();
        }

        averageData.AddRound(NUM_PLAYERS, strat->GetNumStrategies());
        mergeData(games, &averageData);
    }
}

int main()
{
    std::srand(static_cast<unsigned int>(std::time(0)));

    cagesim::GameData averageData;

    std::cout << "begin" << std::endl;

    std::thread simThread (runSim, std::ref(averageData));

    simThread.join();

    std::cout << "end" << std::endl;
    return 0;
}