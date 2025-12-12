#include "Player.h"
#include "Game.h"
// Initialize random engine with current time for non-deterministic behavior
std::default_random_engine Player::random_engine(time(nullptr));
// Ability values are uniformly distributed between 0 and 100
std::uniform_int_distribution<unsigned int> Player::ability_range(0, 100);
// Probability generator for all random decisions (0.0 ~ 1.0)
std::uniform_real_distribution<float> Player::possibility(0.f, 1.f);

float PlayerRLGL::fallDownRate = 0.1f;


// Constructs a player with random agility and fearlessness
// Each player receives independent random abilities
Player::Player(int number)
{
	this->number = number;

	agility = ability_range(random_engine);
	fearlessness = ability_range(random_engine);
}

// Constructs a player with fixed abilities (used for testing or final winner display)
Player::Player(int number, int agility, int fearlessness)
{
	this->number = number;
	this->agility = agility;
	this->fearlessness = fearlessness;
}





bool PlayerRLGL::act()
{
	// Calculate movement distance based on agility, player number,
	// and a bonus influenced by fearlessness
	int moving_distance = agility + number + agility * (fearlessness * 0.01);
	current_distance += moving_distance;
	
	// If the player reaches the target distance, they escape successfully
	if (current_distance >= RedLightGreenLight::distance)
	{
		playing = false;  // Player escapes and is no longer active in this game
		std::cout << "Player #" << number << " escaped! (distance: " << current_distance << ")" << std::endl;
		return true;
	}
	
	// Even if not escaped, the player may fall down with a fixed probability
	if (possibility(random_engine) < fallDownRate)
	{
		playing = false; // Player falls and is eliminated
		return false;
	}
	
	// Otherwise, the player survives and continues to the next turn
	return true; 
}



void PlayerRLGL::dyingMessage()
{ 
	// If the player is still marked as playing,
	// they failed to escape before the game ended
	if (isPlaying())
	{
		printStatus();
		std::cout << " is still on the ground and died." << std::endl;
	}
		
	// Otherwise, the player fell down during movement
	else
	{
		printStatus();
		std::cout << " fell down and died." << std::endl;
	}
};





bool PlayerRPS::act()
{
	// Enumeration for Rock-Paper-Scissors choices
	enum rpsType { Rock, Paper, Scissors };

	// Randomly generates one of the three choices with equal probability
	auto rps = [] {
		float p = possibility(random_engine);

		if (p < float(1.f / 3.f))
			return rpsType::Rock;
		else if (p < float(2.f / 3.f))
			return rpsType::Paper;
		else
			return  rpsType::Scissors;
	};

	// Repeat until the result is not a tie
	rpsType myRPS, yourRPS;
	do {
		myRPS = rps();
		yourRPS = rps();
	}
	while(myRPS == yourRPS);
	
	// Standard Rock-Paper-Scissors rules
	if (myRPS == Rock)
	{
		if (yourRPS == Paper)
			return false;
		else
			return true;
	}
	else if (myRPS == Paper)
	{
		if (yourRPS == Scissors)
			return false;
		else
			return true;
	}
	else
	{
		if (yourRPS == Rock)
			return false;
		else
			return true;
	}
}


void PlayerRPS::dyingMessage()
{
	printStatus();
	std::cout << " died." << std::endl;
};



// In Tug of War, individual actions do not affect the outcome.
// Team result is determined at the game level.
bool PlayerTOW::act()
{
	return true;
}

void PlayerTOW::dyingMessage()
{
	printStatus();
	std::cout << " fell into the water and died." << std::endl;
}
// Glass Bridge elimination is handled entirely in Game::play()
bool PlayerGlassBridge::act(){

	return true;
}

void PlayerGlassBridge::dyingMessage(){

	printStatus();
	std::cout << " fell into the water and died." << std::endl;
}

// Marble game logic is resolved by pair matching in Game::play()
bool PlayerMarble::act(){

	return true;
}

void PlayerMarble::dyingMessage(){

	printStatus();
	std::cout << " lost at marbles and died." << std::endl;
}

// Ddakji outcome is decided based on power comparison in Game::play()
bool PlayerDdakji::act(){
	return true;
}

void PlayerDdakji::dyingMessage(){
	printStatus();
	std::cout << " lose at Ddakji." << std::endl;
}


float PlayerShip::doTask() {

    // Base task completion time
    float baseTime = 8.0f;

    // Ability factor reduces time based on agility and fearlessness
    float abilityFactor =
        (getAgility() * 0.6f + getFearlessness() * 0.4f) / 150.0f;

    // Random noise to prevent deterministic outcomes
    float randomFactor = Player::getRandomProbability();

    // Final task time calculation
    // Faster players (high ability) tend to finish earlier,
    // but randomness still plays a role
    float taskTime =
        baseTime
        * (1.1f - abilityFactor)
        * (0.85f + randomFactor * 0.3f);

    std::cout << "Player #" << getNumber()
              << " task time: " << taskTime << "s" << std::endl;

    return taskTime;
}



void PlayerShip::dyingMessage(){
	printStatus();
	std::cout << " lose at Ship." << std::endl;
}

bool PlayerSquidGame :: act(){

	// Randomly choose between attacking or defending
	bool isAttack = (Player :: getRandomProbability() < 0.5f);

	// Base success probability
	float baseProb = 0.4f;

	// Ability bonus increases success chance
	float abilityBonus = (getAgility() * 0.5f + getFearlessness() * 0.5f) / 200.0f;

	float successProb = baseProb + abilityBonus;

	// Attacking is riskier, defending is slightly safer
	if (isAttack) successProb -= 0.1f;

	else successProb += 0.05f;

	// Final random roll to determine survival
	float roll = Player::getRandomProbability();

	std::cout << "Player #" << getNumber()
			<< (isAttack ? " attacks" : " defends")
			<< " (success prob : " << successProb 
			<< ", roll : " << roll << ")" ;

	if (roll < successProb){
		std:: cout << " -> SURVIVED" << std::endl;
		return true;
	}else {
		std::cout << " -> FAILED" <<std::endl;
		return false;
	}
}

void PlayerSquidGame::dyingMessage()
{
    printStatus();
    std::cout << " was eliminated in the Squid Game." << std::endl;
}