#include <iostream>
#include "./Constants.h"
#include "./Game.h"
#include "./AssetManager.h"
#include "./Components/TransformComponent.h"
#include "./Components/SpriteComponent.h"
#include "./Components/KeyboardControlComponent.h"
#include "../lib/glm/glm.hpp"

EntityManager manager;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Renderer* Game::renderer;
SDL_Event Game::event;


Game::Game() {
    this -> isRunning = false;
}

Game::~Game() {

}

bool Game::IsRunning() const {
    return this -> isRunning;
}

void Game::Initialize(int width, int height) {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL" << std::endl;
        return;
    }
    window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_BORDERLESS);
    if (!window) {
         std::cerr << "Error creating SDL window" << std::endl;
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer) {
        std::cerr << "Error creating SDL rendererS" << std::endl;
        return;
    }

    LoadLevel(0);

    isRunning = true;
    return;
}

void Game::LoadLevel(int levelNumber) {
    assetManager -> AddTexture("tank-image", std::string("./assets/images/tank-big-right.png").c_str());
    assetManager -> AddTexture("chopper-image", std::string("./assets/images/chopper-spritesheet.png").c_str());
    assetManager -> AddTexture("radar-image", std::string("./assets/images/radar.png").c_str());

    Entity& chopperEntity(manager.AddEntity("chopper"));
    chopperEntity.AddComponent<TransformComponent>(240, 106, 0, 0, 32, 32, 1);
    chopperEntity.AddComponent<SpriteComponent>("chopper-image", 2, 90, true, false);
    chopperEntity.AddComponent<KeyboardControlComponent>("up", "right", "down", "left", "space");

    Entity& tankEntity(manager.AddEntity("tank"));
    tankEntity.AddComponent<TransformComponent>(0, 0, 20, 20, 32, 32, 1);
    tankEntity.AddComponent<SpriteComponent>("tank-image");

    Entity& radarEntity(manager.AddEntity("Radar"));
    radarEntity.AddComponent<TransformComponent>(720, 15, 0, 0, 64, 64, 1);
    radarEntity.AddComponent<SpriteComponent>("radar-image", 8, 150, false, true);


}

void Game::ProcessInput() {
    SDL_PollEvent(&event);
    switch(event.type) {
        case SDL_QUIT: {
            isRunning = false;
            break;
        }
        case SDL_KEYDOWN: {
            if(event.key.keysym.sym == SDLK_ESCAPE){
                isRunning = false;
            }
        }
        default: {
            break;
        }
    }
}



void Game::Update() {
    int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - ticksLastFrame);
    if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME ) {
        SDL_Delay(timeToWait);
    }

    float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;
    deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;
    ticksLastFrame = SDL_GetTicks();

    manager.Update(deltaTime);
}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    if(manager.HasNoEntities()){
        return;
    }

    manager.Render();

    SDL_RenderPresent(renderer);
}

void Game::Destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_QUIT;
}

