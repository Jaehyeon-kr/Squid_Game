#include <iostream>
#include <random>
#include <time.h>

class Player
{
public:
	Player(int number);
	Player(int number, int agility, int fearlessness);
	~Player() {};
	virtual bool act() { return true; };
	virtual bool isPlaying() { return playing; };
	void aliveMessage() { printStatus(); std::cout << " is alive." << std::endl; };
	virtual void dyingMessage() { printStatus(); std::cout << " died." << std::endl; };
	virtual void printStatus() { std::cout << "Player #" << number << "(" << agility << "," << fearlessness << ")"; };

	int getNumber() const { return number; }
	int getAgility() const { return agility; }
	int getFearlessness() const { return fearlessness; }
	int getPower() const { return agility + fearlessness; }

	static float getRandomProbability() { return possibility(random_engine); }
	
protected:
	unsigned int number;
	unsigned int agility;
	unsigned int fearlessness;
	bool playing = true;
	static std::default_random_engine random_engine;
	static std::uniform_int_distribution<unsigned int> ability_range;
	static std::uniform_real_distribution<float> possibility;
};



class PlayerRLGL : public Player
{
	static float fallDownRate;
	unsigned int current_distance = 0;
public:
	PlayerRLGL(const Player& player) : Player(player) { playing = true; };
	bool act();
	void dyingMessage();
};

class PlayerRPS : public Player
{
public:
	PlayerRPS(const Player& player) : Player(player) {  };
	bool act();
	void dyingMessage();
};

class PlayerTOW : public Player{

	public : 
		PlayerTOW(const Player & player) : Player(player) {} ;
		bool act();
		void dyingMessage();
};

class PlayerGlassBridge : public Player{

	public :
		PlayerGlassBridge(const Player & player) : Player(player) {} ;
		bool act();
		void dyingMessage();
};


class PlayerMarble : public Player{

	public :
		PlayerMarble(const Player & player) : Player(player) {} ;
		bool act();
		void dyingMessage();
};


class PlayerDdakji : public Player{

	public :
		PlayerDdakji(const Player & player) : Player(player) {} ;
		bool act();
		void dyingMessage();

};

class PlayerShip : public Player{

	public : 
		PlayerShip(const Player & player) : Player(player) {} ;
		float doTask();  
		void dyingMessage();
};

class PlayerSquidGame  : public Player{

	public :
		PlayerSquidGame(const Player & player) : Player(player) {} ;
		bool act();
		void dyingMessage();
};