#include <iostream>
#include "Player.h"
#include "Game.h"

int main()
{
    std::list<Player*> players;
    for (int i = 0; i < 456; ++i)
    {
        players.push_back(new Player(i+1));
    }
    
    std::vector<Game*> games;
    games.push_back(new RedLightGreenLight(20));
    games.push_back(new RPS());
    games.push_back(new TugOfWar());
    games.push_back(new GlassBridge());
    games.push_back(new Marbles());
    games.push_back(new Ddakji());
    games.push_back(new Pysical_Asia_ship());
    games.push_back(new SquidGame());
    
    for (auto game : games)
    {
        for (auto player : players)
            game->join(player);

        game->play();

        auto alivePlayers = game->getAlivePlayers();

        // 원본 players는 게임에 join할 때만 참조용으로 사용
        // 실제 게임 플레이어는 게임 내부에서 생성/관리됨
        players.clear();

        for (auto player : alivePlayers)
        {
            players.push_back(new Player(*player));
        }
    }

    std::cout << "\n================ Game Summary ================\n";
    std::cout << "| Game | Total | Survivors | Deaths | Death Rate | Notes |\n";
    std::cout << "---------------------------------------------\n";

    for (Game* game : games) {
        game->printSummary();
    }


    for (auto game : games)
        delete game;

    // 마지막 남은 players 정리
    for (auto player : players)
        delete player;
}