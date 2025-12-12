#include <string>
#include <list>

class Player;

class Game
{
protected:
	std::string gameName;
	std::list<Player*> players;

    unsigned int initial_count = 0;
    unsigned int survivor_count = 0;
    unsigned int death_count = 0;
    Player* winner = nullptr;

	virtual void printGameName();
public:
	Game(std::string name) :gameName(name) {};
	~Game();
	virtual void join(Player* player) = 0;
	virtual void play() = 0;
	void printAlivePlayers();
	std::list<Player*> getAlivePlayers() { return players; };
    void printSummary() const;
};


class RedLightGreenLight : public Game
{
	friend class PlayerRLGL;

	static const unsigned int distance;
	static const float fallDownRate;

	const unsigned int turn = 20;
public:
	RedLightGreenLight() : Game("Red Light Green Light") {};
	RedLightGreenLight(int t) : Game("Red Light Green Light"), turn(t) {};
	~RedLightGreenLight() {};
	void join(Player* player);
	void play();
};





class RPS : public Game
{
	friend class PlayerRPS;

public:
	RPS() : Game("Rock Paper Scissors") {};
	~RPS() {};
	void join(Player* player);
	void play();
};


class TugOfWar : public Game{

	friend class PlayerTOW;

public:
	TugOfWar() : Game("Tug of War") {};
	~TugOfWar() {};
	void join(Player* player);
	void play();
};


class GlassBridge : public Game 
{
	static const int totalSteps= 18;

	friend class PlayerGlassBridge;

	public :
		GlassBridge() : Game("Glass Bridge") {};
		~GlassBridge() {};
		void join(Player * player);
		void play();

};


class Marbles : public Game {

	public : 
		Marbles() : Game("Marbles") {};
		~Marbles() {};
		void join(Player * player);
		void play();

};


class Ddakji : public Game {

	public : 
		Ddakji() : Game("Ddakji") {} ;
		~Ddakji() {};
		void join(Player *player);
		void play();
};

class Pysical_Asia_ship : public Game{

	public : 
		Pysical_Asia_ship() : Game("Pysical Asia Ship") {};
		~Pysical_Asia_ship() {};

		void join(Player *player);
		void play();
};


class SquidGame : public Game{

	public : 
		SquidGame() : Game("SquidGame") {};
		~SquidGame() {};

		void join(Player *player);
		void play();
};