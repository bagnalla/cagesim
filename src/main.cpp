#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>
#include <vector>
#include <LightSource.h>
#include <Shader.h>
#include "Camera.h"
#include "Cube.h"
#include "CongestionStrategy.h"
#include "Game.h"
#include "Plot.h"
#include "Puddi.h"

#define NUM_PLAYERS 6
#define NUM_RESOURCES 6
#define NUM_GAMES 8
#define NUM_ROUNDS 1000

using namespace puddi;

cagesim::GameData averageData;
bool simRunning = true;

void mergeData(const std::vector<cagesim::Game> games, cagesim::GameData *data)
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
    std::cout << "begin" << std::endl;

    cagesim::CongestionStrategy *strat = new cagesim::CongestionStrategy(NUM_PLAYERS, NUM_RESOURCES);
    float epsilon = 0.5f;
    std::vector<cagesim::Game> games;
    for (size_t i = 0; i < NUM_GAMES; ++i) {
        games.push_back(cagesim::Game(strat, epsilon));
    }

    for (size_t i = 0; i < NUM_ROUNDS; ++i)
    {
#pragma omp parallel for
        for (size_t j = 0; j < games.size(); ++j) {
            games[j].Next();
        }

        averageData.AddRound(NUM_PLAYERS, strat->GetNumStrategies());
        if (simRunning) {
            mergeData(games, &averageData);
        }
    }
    std::cout << "end" << std::endl;
}

std::thread simThread (runSim, std::ref(averageData));

Cube *cube;

void init()
{
    //world light
    LightSource *lightSource = LightSource::ObtainLightSource();
    lightSource->ambient = vec4(1.0, 1.0, 1.0, 1.0);
    lightSource->diffuse = vec4(1.0, 1.0, 1.0, 1.0);
    lightSource->specular = vec4(1.0, 1.0, 1.0, 1.0);
    lightSource->position = vec4(-250.0f, -500.0f, 500.0f, 0.0f);
    lightSource->UpdateMatrix();
    LightSource::UpdateLightSourceMatricesInShaders();


    // HUD light
    mat4 hudLight = mat4(vec4(1.0, 1.0, 1.0, 1.0), // ambient
                         vec4(1.0, 1.0, 1.0, 1.0), // diffuse
                         vec4(1.0, 1.0, 1.0, 1.0), // specular
                         vec4(-2.5f, 5.0f, -5.0f, 0.0f)); // position
    Shader::SetOrthoLightSource(hudLight);

    cube = new Cube(engine::GetRootObject());
//    cube->SetEmissive(true);
//    cube->SetEmissionColor(vec4(1.0f, 0.0f, 0.0f, 1.0f));
    cube->SetMaterial(Material::RedRubber());
    //cube->SetIsHUDElement(true);
    cube->Translate(vec4(0.0f, 5.0f, 0.0f, 0.0f));
//    cube->RotateX(0.1f);
//    cube->RotateY(0.1f);
}

bool holdingMouseClick = false;
int update()
{
    SDL_Event ev;
    while (SDL_PollEvent(&ev))
    {
        // send event to camera
        engine::MainCamera->InputEvent(ev);

        // quit
        if (ev.type == SDL_QUIT)
        {
            return 1;
        }
            // key press
        else if (ev.type == SDL_KEYDOWN)
        {
            switch (ev.key.keysym.sym)
            {
                // EXIT PROGRAM
                case SDLK_ESCAPE:
                    simRunning = false;
                    simThread.detach();
                    return 1;
                    break;
                    // ENABLE FULLSCREEN
                case SDLK_f:
                    engine::ToggleFullScreen();
                    break;
                    /*case SDLK_1:
                        break;*/
                case SDLK_r:
                    break;
                case SDLK_MINUS:
                case SDLK_UNDERSCORE:
                    break;
                case SDLK_PLUS:
                case SDLK_EQUALS:
                    break;
                case SDLK_1:
                    cube->RotateX(-0.1f);
                    break;
                case SDLK_2:
                    cube->RotateX(0.1f);
                    break;
                case SDLK_3:
                    cube->RotateY(-0.1f);
                    break;
                case SDLK_4:
                    cube->RotateY(0.1f);
                    break;
                case SDLK_5:
                    cube->RotateZ(-0.1f);
                    break;
                case SDLK_6:
                    cube->RotateZ(0.1f);
                    break;
            }
        }
            // mouse click
        else if (ev.type == SDL_MOUSEBUTTONDOWN)
        {
            holdingMouseClick = true;
        }
            // mouse release
        else if (ev.type == SDL_MOUSEBUTTONUP)
        {
            holdingMouseClick = false;
        }
            // mouse motion
        else if (ev.type == SDL_MOUSEMOTION)
        {
            //
        }
            // window event
        else if (ev.type == SDL_WINDOWEVENT)
        {
            engine::UpdateProjectionMatrixAndViewport();
        }
    }
    return 0;
}

int main()
{
    std::srand(static_cast<unsigned int>(std::time(0)));

//    auto begin = std::chrono::steady_clock::now();

//    simThread.join();

//    auto end = std::chrono::steady_clock::now();
//
//    std::cout << "end. time elapsed: " <<
//              std::chrono::duration <double, std::milli> (end - begin).count() <<
//              " ms" << std::endl;
    if (int initStatus = engine::Init(100.0f) != 0)
        return initStatus;

    //std::thread simThread (runSim, std::ref(averageData));
    init();

    engine::RegisterUpdateFunction(update);

    return engine::Run();
}