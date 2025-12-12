#include <iostream>
#include <algorithm>
#include "Game.h"
#include "Player.h"


// Destructor: releases all dynamically allocated Player objects
Game::~Game()
{
	for (auto player = players.begin(); player != players.end(); ++player)
	{
		delete (*player);
	}
	players.clear();
}

// Prints the name of the current game
void Game::printGameName()
{
	std::cout << "[[[" << gameName << "]]]" << std::endl;
}

// Prints status messages of all surviving players
void Game::printAlivePlayers()
{
	for (auto player = players.begin(); player != players.end(); ++player)
	{
		(*player)->aliveMessage();
	}
}


// Target distance required to survive the game
const unsigned int RedLightGreenLight::distance = 1000;

// Probability of falling down while moving
const float RedLightGreenLight::fallDownRate = 0.1f;


// Adds a player to Red Light Green Light game
// Player is wrapped as PlayerRLGL
void RedLightGreenLight::join(Player* player)
{
	players.push_back(new PlayerRLGL(*player));
}

// Executes the Red Light Green Light game
// Players move for a fixed number of turns.
// After all turns, players who have not escaped are eliminated.
void RedLightGreenLight::play()
{
	printGameName();

    initial_count = players.size();
	std::cout << initial_count << " players joined the game." << std::endl;

	// Each turn, only players who are still playing can act.
	// Players who fall down or fail to advance stop playing immediately.

	for (int t = 0; t < turn; ++t) // 10이 아닐때까지
	{

		for(auto player = players.begin(); player != players.end(); ++player){

			if ((*player) -> isPlaying()){

				(*player) -> act();
			}
		}

	}

	std::cout << "[Game Over]" << std::endl;

	auto player = players.begin();
	while (player != players.end())
	{
		if ((*player)->isPlaying())
		{
			(*player)->dyingMessage();
			delete (*player);
			player = players.erase(player);
		}
		else
		{
			++player;
		}
	}

	survivor_count = players.size();
	death_count = initial_count - survivor_count;

	printAlivePlayers();

	std::cout << "\n[Game Statistics]" << std::endl;
	std::cout << "Total players: " << initial_count << std::endl;
	std::cout << "Survivors: " << survivor_count << std::endl;
	std::cout << "Deaths: " << death_count << std::endl << std::endl;
}




// Adds a player to Rock Paper Scissors game
void RPS::join(Player* player)
{
	players.push_back(new PlayerRPS(*player));
}

// Executes Rock Paper Scissors game
// Players are eliminated immediately based on act() result
void RPS::play()
{
	printGameName();

    initial_count = players.size();
	std::cout << initial_count << " players joined the game." << std::endl;

	if (players.size() < 2)
	{
		std::cout << "There are not sufficient players." << std::endl;
		return;
	}

	// In RPS, each player performs exactly one action.
	// Players who lose are immediately eliminated in the same round.

	auto player = players.begin();

	while (player != players.end())
	{
		if ((*player)->act())
		{
			++player;
		}
		else
		{
			(*player)->dyingMessage();
			delete (*player);
			player = players.erase(player);
		}
	}

	survivor_count = players.size();
	death_count = initial_count - survivor_count;

	printAlivePlayers();

	std::cout << "\n[Game Statistics]" << std::endl;
	std::cout << "Total players: " << initial_count << std::endl;
	std::cout << "Survivors: " << survivor_count << std::endl;
	std::cout << "Deaths: " << death_count << std::endl << std::endl;
}

// Executes Tug of War game
// Players are split into two teams.
// The team with lower total power is eliminated.
void TugOfWar::join(Player* player)
{
	players.push_back(new PlayerTOW(*player));
}

void TugOfWar::play(){

	printGameName();

    initial_count = players.size();
	std::cout << initial_count << " players joined the game." << std::endl;

	if (players.size() < 2) {
		std::cout << "There are not sufficient players." << std::endl;
		return;
	}


	// Players are alternately assigned to two teams.
	// Team assignment does not depend on player attributes.
	std::list<Player*> team1, team2;

	int count = 0;
	for(auto player : players)
	{
		if (count % 2 == 0)
			team1.push_back(player);
		else 
			team2.push_back(player);

		count ++;
			

	}

	// Team power is calculated as the sum of (agility + fearlessness).
	// The team with lower total power is completely eliminated.
	int team1_power = 0;
	int team2_power = 0;

	for (auto player : team1){
		team1_power += player->getPower();
	}

	for (auto player : team2){
		team2_power += player->getPower();
	}

	std::list<Player*>* losingTeam;
	if (team1_power > team2_power){
		std::cout << "Team 2 lost" << std::endl;
		losingTeam = &team2;
	}
	else if (team2_power > team1_power){
		std::cout << "Team 1 lost" << std::endl;
		losingTeam = &team1;
	}
	else{
		std::cout << "It's a tie ! Both teams survive." << std::endl;
		losingTeam = nullptr;
	}// If both teams have equal power, no players are eliminated.

	if (losingTeam != nullptr){
		auto player = players.begin();
		while(player != players.end()){

			auto condi = std::find(losingTeam->begin(), losingTeam->end(), *player);

			if (condi != losingTeam->end()){
				(*player) -> dyingMessage();
				delete (*player);
				player = players.erase(player);
			
			}else{
				++player;
			}

		}
	}

	survivor_count = players.size();
	death_count = initial_count - survivor_count;

	std::cout << "Team 1 power: " << team1_power << std::endl;
	std::cout << "Team 2 power: " << team2_power << std::endl;

	printAlivePlayers();

	std::cout << "\n[Game Statistics]" << std::endl;
	std::cout << "Total players: " << initial_count << std::endl;
	std::cout << "Survivors: " << survivor_count << std::endl;
	std::cout << "Deaths: " << death_count << std::endl << std::endl;
}


// Executes Glass Bridge game
// Each step has one safe glass panel chosen randomly.
// Players are eliminated when stepping on unsafe glass.
void GlassBridge::join(Player *player){
	players.push_back(new PlayerGlassBridge(*player));

}

void GlassBridge::play(){

	printGameName();
    initial_count = players.size();

	std::cout << initial_count << " players joined the game." << std::endl;

	if (players.size() < 1) {
		std::cout << "There are not sufficient players." << std::endl;
		return;
	}

	// safeGlass[i] == true means the glass at step i is safe
	bool safeGlass[totalSteps];
	for (int i = 0; i < totalSteps; ++i) {
		safeGlass[i] = (Player::getRandomProbability() < 0.5f);
	}

	int currentStep = 0;
	auto player = players.begin();

	// The bridge has a single safe path that is shared by all players.
	// Once a player falls, the safe path up to that point is revealed.
	// Next players continue from the same step,
	// making the game easier as more players attempt it.
	while (player != players.end() && currentStep < totalSteps) {
		bool chooseCorrect = (Player::getRandomProbability() < 0.5f) == safeGlass[currentStep];

		if (chooseCorrect) {
			std::cout << "Player #" << (*player)->getAgility() << " stepped on safe glass at step " << (currentStep + 1) << std::endl;
			currentStep++;

			if (currentStep >= totalSteps) {
				std::cout << "Bridge completed! Remaining players survive." << std::endl;
				break;
			}
		} else {
			std::cout << "Player #" << (*player)->getAgility() << " fell at step " << (currentStep + 1) << "!" << std::endl;
			(*player)->dyingMessage();
			delete (*player);
			player = players.erase(player);

		}
	}

	survivor_count = players.size();
	death_count = initial_count - survivor_count;

	printAlivePlayers();

	std::cout << "\n[Game Statistics]" << std::endl;
	std::cout << "Total players: " << initial_count << std::endl;
	std::cout << "Survivors: " << survivor_count << std::endl;
	std::cout << "Deaths: " << death_count << std::endl;
	std::cout << "Steps completed: " << currentStep << "/" << totalSteps << std::endl << std::endl;
}


// Executes Marbles game
// Players are paired and play odd/even guessing.
// Winners advance to next round.
void Marbles::join(Player *player){
	players.push_back(new PlayerMarble(*player));

}


// If the number of players is odd, one player gets a bye
void Marbles::play(){

	printGameName();

    initial_count = players.size();

	std::cout << initial_count << " players joined the game." << std::endl;

	if (players.size() < 2) {
		std::cout << "There are not sufficient players." << std::endl;
		return;
	}

	std::list<Player*> winners;
	std::list<Player*> losers;

	
	if (players.size() % 2 == 1) {
		auto lastPlayer = players.end();
		--lastPlayer;
		std::cout << "Player #" << (*lastPlayer)->getNumber() << " gets a bye." << std::endl;
		winners.push_back(*lastPlayer);
		players.erase(lastPlayer);
	}

	// Players compete in pairs.
	// Each match produces exactly one winner and one loser.

	// Winners advance to the next round.
	// Losers are eliminated immediately after the round.

	// This structure guarantees that approximately half the players survive each round.

	int numMatches = players.size() / 2;

	for (int i = 0; i < numMatches; ++i) {
		auto player1 = players.begin();
		auto player2 = players.begin();
		++player2;

		int marbles2 = static_cast<int>(Player::getRandomProbability() * 10) + 1;

		bool isOdd = (marbles2 % 2 == 1);
		bool guessOdd = (Player::getRandomProbability() < 0.5f);

		std::cout << "Match: Player #" << (*player1)->getNumber()
		          << " vs Player #" << (*player2)->getNumber() << std::endl;

		Player* winner;
		Player* loser;
		if (guessOdd == isOdd) {
			winner = *player1;
			loser = *player2;
		} else {
			winner = *player2;
			loser = *player1;
		}

		winners.push_back(winner);
		losers.push_back(loser);

		players.erase(player1);
		players.erase(player2);
	}

	for (auto loser : losers) {
		loser->dyingMessage();
		delete loser;
	}


	players = winners;

	survivor_count = players.size();
	death_count = initial_count - survivor_count;

	printAlivePlayers();

	std::cout << "\n[Game Statistics]" << std::endl;
	std::cout << "Total players: " << initial_count << std::endl;
	std::cout << "Survivors: " << survivor_count << std::endl;
	std::cout << "Deaths: " << death_count << std::endl << std::endl;
}



void Ddakji::join(Player *player){
	players.push_back(new PlayerDdakji(*player));

}


// Executes Ddakji game
// Players compete based on power (agility + fearlessness).
// In case of tie, the winner is chosen randomly.
void Ddakji::play(){

	printGameName();

    initial_count = players.size();

	std::cout << initial_count << " players joined the game." << std::endl;

	if (players.size() < 2) {
		std::cout << "There are not sufficient players." << std::endl;
		return;
	}

	std::list<Player*> winners;
	std::list<Player*> losers;

	if (players.size() % 2 == 1) {
		auto lastPlayer = players.end();
		--lastPlayer;
		std::cout << "Player #" << (*lastPlayer)->getNumber() << " gets a bye." << std::endl;
		winners.push_back(*lastPlayer);
		players.erase(lastPlayer);
	}

	int numMatches = players.size() / 2;

	// The winner is primarily determined by player power.
	// Randomness is applied only when both players have equal power.

	for (int i = 0; i < numMatches; ++i) {
		auto player1 = players.begin();
		auto player2 = players.begin();
		++player2;

		int power1 = (*player1)->getPower();
		int power2 = (*player2)->getPower();

		std::cout << "Match: Player #" << (*player1)->getNumber()
		          << " vs Player #" << (*player2)->getNumber() << std::endl;

		Player* winner;
		Player* loser;

		if (power1 > power2) {
			winner = *player1;
			loser = *player2;
		} else if (power2 > power1) {
			winner = *player2;
			loser = *player1;
		} else {
			if (Player::getRandomProbability() < 0.5f) {
				winner = *player1;
				loser = *player2;
			} else {
				winner = *player2;
				loser = *player1;
			}
		}

		winners.push_back(winner);
		losers.push_back(loser);

		players.erase(player1);
		players.erase(player2);
	}

	
	for (auto loser : losers) {
		loser->dyingMessage();
		delete loser;
	}

	players = winners;

	survivor_count = players.size();
	death_count = initial_count - survivor_count;

	printAlivePlayers();

	std::cout << "\n[Game Statistics]" << std::endl;
	std::cout << "Total players: " << initial_count << std::endl;
	std::cout << "Survivors: " << survivor_count << std::endl;
	std::cout << "Deaths: " << death_count << std::endl << std::endl;

}

void Pysical_Asia_ship::join(Player * player){
	
	players.push_back(new PlayerShip (* player));
}

// Executes Physical Asia Ship game (custom game)
// Players perform tasks and are ranked by task completion time.
// In each round, only the fastest half survive.
// The game continues until only two players remain.
void Pysical_Asia_ship::play() {

    printGameName();

    initial_count = players.size();
    std::cout << initial_count << " players joined the game." << std::endl;

    if (players.size() < 3) {
        survivor_count = players.size();
        death_count = initial_count - survivor_count;
        return;
    }

    int round = 1;

	// A minimum of two survivors is enforced to guarantee
	// that the final Squid Game can always be played.
    while (players.size() > 2) {

        std::cout << "\n[Task Round " << round << "]" << std::endl;

        // (player, taskTime)
        std::vector<std::pair<Player*, float>> results;

	// Each player performs a solo task and produces a completion time.
	// This simulates a time-based physical challenge.
        for (auto p : players) {
            float t = static_cast<PlayerShip*>(p)->doTask();
            results.push_back({p, t});
        }

        // Players are ranked by task completion time.
		// Faster players are considered more capable in this game.
        std::sort(results.begin(), results.end(),
            [](const auto& a, const auto& b) {
                return a.second < b.second;
            });

		// Only the fastest half of the players survive each round.
		// This ensures deterministic convergence toward two final players.
        int surviveCount = results.size() / 2;  
        if (surviveCount < 2) surviveCount = 2; 

        std::list<Player*> nextPlayers;

        for (int i = 0; i < results.size(); ++i) {
            if (i < surviveCount) {
                nextPlayers.push_back(results[i].first);
            } else {
                results[i].first->dyingMessage();
                delete results[i].first;
            }
        }

        players = nextPlayers;

        std::cout << "Survivors: " << players.size() << std::endl;
        round++;
    }

    survivor_count = players.size();
    death_count = initial_count - survivor_count;

    std::cout << "\n[Final Survivors]" << std::endl;
    printAlivePlayers();
}

// Executes Squid Game (final game)
// Players fight until only one survivor remains.
// The final survivor is recorded as the winner.
void SquidGame::join(Player * player){
	
	players.push_back(new PlayerSquidGame (* player));
}

void SquidGame::play(){

	// This is the final game.
	// Players repeatedly fight until only one survivor remains.
	printGameName();

    initial_count = players.size();

	std::cout << initial_count << " players joined the game." << std::endl;

	// Squid Game requires at least two players to proceed.
	if (players.size() < 2) {
		std::cout << "There are not sufficient players." << std::endl;
		return;
	}

	// The game proceeds in repeated rounds.
	// In each round, every remaining player performs an action.
	// Players who fail their action are immediately eliminated.
	while(players.size() > 1){
		
		std::cout << "\n[FINAL Round]" << std::endl;
		auto player = players.begin();

		// Iterate through all remaining players in the current round
		while (player != players.end()){

			// act() returns true if the player survives this confrontation
			if ((*player) -> act()){
				++player;
			} else{
				(*player) -> dyingMessage();
				delete (*player);
				player = players.erase(player);
			}	// If act() fails, the player is eliminated and removed immediately
		}

		// After one full round, report the number of survivors
		std::cout << "Survivors: " << players.size() << std::endl;

	}
	// At this point, either one or zero players remain
	survivor_count = players.size();
	death_count = initial_count - survivor_count;

	std:: cout << "\n[Final Survivor]" <<std::endl;

	// If exactly one player remains, that player is recorded as the winner
	if (survivor_count == 1)
		winner = players.front();
	else
		winner = nullptr;
		
	printAlivePlayers();
}


// Prints summary of the game including statistics and winner
// Death rate is calculated as (deaths / total players) * 100
void Game::printSummary() const
{
    float deathRate = 0.0f;
    float survivalRate = 0.0f;

    if (initial_count > 0) {
        deathRate = static_cast<float>(death_count) / initial_count * 100.0f;
        survivalRate = static_cast<float>(survivor_count) / initial_count * 100.0f;
    }

    std::cout << "| " << gameName
              << " | " << initial_count
              << " | " << survivor_count
              << " | " << death_count
              << " | " << deathRate << "% "
              << " | ";

    if (winner) {
        winner->printStatus();  
    } else {
        std::cout << "N/A";
    }

    std::cout << " |" << std::endl;
}
