# 새로운 게임 추가하기 - 실습 가이드

## 목표: Tug of War (줄다리기) 게임 구현하기

### 게임 규칙
- 2팀으로 나누어 줄다리기 진행
- 각 팀의 힘 = 모든 플레이어의 (agility + fearlessness) 합계
- 진 팀의 모든 플레이어는 탈락

---

## Step 1: Game.h에 새 게임 클래스 선언

**파일**: `source_codes/Game.h`

**추가 위치**: `RPS` 클래스 뒤 (약 56번째 줄 이후)

```cpp
class TugOfWar : public Game
{
	friend class PlayerTOW;  // PlayerTOW와 friend 관계

public:
	TugOfWar() : Game("Tug of War") {};
	~TugOfWar() {};
	void join(Player* player);
	void play();
};
```

---

## Step 2: Player.h에 새 플레이어 클래스 선언

**파일**: `source_codes/Player.h`

**추가 위치**: `PlayerRPS` 클래스 뒤 (약 44번째 줄 이후)

```cpp
class PlayerTOW : public Player
{
public:
	PlayerTOW(const Player& player) : Player(player) { };
	bool act();  // 줄다리기 결과 판정
	void dyingMessage();
};
```

---

## Step 3: Game.cpp에 게임 로직 구현

**파일**: `source_codes/Game.cpp`

**추가 위치**: `RPS::play()` 함수 뒤 (약 121번째 줄 이후)

```cpp
void TugOfWar::join(Player* player)
{
	// TODO: PlayerTOW 객체를 생성하여 players 리스트에 추가
	// 힌트: players.push_back(new PlayerTOW(*player));
}

void TugOfWar::play()
{
	printGameName();

	// TODO 1: 플레이어가 2명 미만이면 게임 불가
	if (/* 조건 작성 */)
	{
		std::cout << "There are not sufficient players." << std::endl;
		return;
	}

	// TODO 2: 플레이어를 두 팀으로 나누기
	std::list<Player*> team1, team2;
	int count = 0;
	for (auto player : players)
	{
		if (/* 짝수/홀수로 팀 나누기 */)
			team1.push_back(player);
		else
			team2.push_back(player);
		count++;
	}

	// TODO 3: 각 팀의 총 힘 계산
	int team1_power = 0;
	int team2_power = 0;

	// team1의 힘 계산
	for (auto player : team1)
	{
		// 힌트: agility + fearlessness를 더하기
		// player->??? 로 접근 불가 - Player 클래스의 멤버가 protected
		// 해결: PlayerTOW::act()에서 처리하거나, 다른 방법 고민
	}

	// team2의 힘 계산
	for (auto player : team2)
	{
		// 힌트: 위와 동일
	}

	// TODO 4: 승부 판정 및 탈락 처리
	std::list<Player*>* losingTeam;
	if (team1_power > team2_power)
	{
		std::cout << "Team 2 lost!" << std::endl;
		losingTeam = &team2;
	}
	else if (team2_power > team1_power)
	{
		std::cout << "Team 1 lost!" << std::endl;
		losingTeam = &team1;
	}
	else
	{
		std::cout << "It's a tie! Both teams survive." << std::endl;
		losingTeam = nullptr;
	}

	// TODO 5: 진 팀 플레이어 제거
	if (losingTeam != nullptr)
	{
		auto player = players.begin();
		while (player != players.end())
		{
			// 힌트: losingTeam에 있는 플레이어를 찾아서 제거
			// std::find(losingTeam->begin(), losingTeam->end(), *player)

			if (/* losingTeam에 포함되어 있으면 */)
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
	}

	printAlivePlayers();
	std::cout << players.size() << " players are alive." << std::endl << std::endl;
}
```

---

## Step 4: Player.cpp에 플레이어 동작 구현

**파일**: `source_codes/Player.cpp`

**추가 위치**: `PlayerRPS::dyingMessage()` 함수 뒤 (약 106번째 줄 이후)

```cpp
bool PlayerTOW::act()
{
	// 줄다리기에서는 개별 행동이 필요 없음
	// 게임 로직에서 처리
	return true;
}

void PlayerTOW::dyingMessage()
{
	printStatus();
	std::cout << " fell into the water and died." << std::endl;
}
```

---

## Step 5: Project.cpp에 게임 추가

**파일**: `source_codes/Project.cpp`

**수정 위치**: 약 13-16번째 줄

```cpp
std::vector<Game*> games;
games.push_back(new RedLightGreenLight(20));
games.push_back(new RPS());
games.push_back(new TugOfWar());  // 새 게임 추가!
games.push_back(new RedLightGreenLight(10));
```

---

## 컴파일 & 실행

```cmd
cd source_codes
cl.exe /EHsc Project.cpp Game.cpp Player.cpp && Project.exe
```

---

## 도전 과제

### 1. 기본 구현 완성
- [ ] `TugOfWar::join()` 구현
- [ ] 플레이어 2명 미만 체크
- [ ] 두 팀으로 나누기
- [ ] 각 팀의 힘 계산 (문제: agility, fearlessness는 protected!)
- [ ] 승부 판정
- [ ] 진 팀 제거

### 2. 문제 해결: protected 멤버 접근
**문제**: Player 클래스의 agility, fearlessness가 protected라서 접근 불가

**해결 방법 3가지**:

#### 방법 1: Getter 함수 추가 (권장)
```cpp
// Player.h에 추가
int getAgility() const { return agility; }
int getFearlessness() const { return fearlessness; }
```

#### 방법 2: PlayerTOW에서 계산 함수 제공
```cpp
// Player.h - PlayerTOW 클래스에 추가
int getPower() const { return agility + fearlessness; }
```

#### 방법 3: friend 관계 활용
```cpp
// Player.h에서
friend class TugOfWar;  // TugOfWar가 직접 접근 가능
```

### 3. 개선 아이디어
- [ ] 팀을 능력치 기반으로 균등하게 나누기
- [ ] 각 팀원 목록 출력
- [ ] 각 팀의 힘 차이 출력
- [ ] 무승부 처리 방식 변경 (재경기 등)

---

## 다른 게임 아이디어

### Glass Bridge (유리다리)
```cpp
class GlassBridge : public Game
{
	static const int totalSteps = 18;  // 총 18칸
	// 각 칸마다 50% 확률로 강화유리 또는 일반유리
	// 일반유리 밟으면 추락
};
```

### Marbles (구슬치기)
```cpp
class Marbles : public Game
{
	// 2명씩 짝지어 구슬 게임
	// 각자 10개의 구슬 보유
	// 홀/짝 맞추기 게임으로 승부
};
```

### Ddakji (딱지치기)
```cpp
class Ddakji : public Game
{
	// agility가 높을수록 승리 확률 높음
	// 3판 2선승제
};
```

---

## 디버깅 팁

1. **컴파일 에러 시**:
   - 헤더 파일에 클래스 선언했는지 확인
   - cpp 파일에 구현했는지 확인
   - 세미콜론(;) 빠뜨리지 않았는지 확인

2. **실행 에러 시**:
   - 포인터 nullptr 체크
   - players.size() 확인
   - 메모리 누수 없는지 확인 (delete 호출)

3. **로직 테스트**:
   - 작은 숫자로 테스트 (456명 → 10명)
   - 출력 메시지 추가해서 디버깅

---

## 체크리스트

완성 후 확인:
- [ ] 컴파일 성공
- [ ] 게임이 정상 실행됨
- [ ] 게임 이름이 출력됨
- [ ] 탈락자가 제대로 처리됨
- [ ] 생존자 수가 올바름
- [ ] 메모리 누수 없음 (delete 호출)

---

**화이팅! 직접 구현해보세요!** 🎮

막히는 부분이 있으면 언제든지 질문하세요!
