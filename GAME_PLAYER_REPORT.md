# Game.h와 Player.h 클래스 구조 보고서

## 목차
1. [개요](#개요)
2. [Player 클래스 계층 구조](#player-클래스-계층-구조)
3. [Game 클래스 계층 구조](#game-클래스-계층-구조)
4. [게임별 상세 설명](#게임별-상세-설명)
5. [설계 패턴 및 특징](#설계-패턴-및-특징)

---

## 개요

이 프로젝트는 오징어 게임을 모티브로 한 게임 시뮬레이션 시스템입니다. 플레이어들이 여러 게임을 순차적으로 진행하며, 각 게임에서 생존자만 다음 게임으로 진행하는 구조입니다.

### 핵심 설계 원칙
- **상속과 다형성**: 기본 클래스(Game, Player)를 상속받아 각 게임별 특화된 기능 구현
- **캡슐화**: 게임별 로직을 독립적으로 관리
- **동적 메모리 관리**: 플레이어 객체의 동적 생성 및 제거

---

## Player 클래스 계층 구조

### 기본 Player 클래스

```cpp
class Player
{
protected:
    unsigned int number;           // 플레이어 번호
    unsigned int agility;          // 민첩성 (0~100)
    unsigned int fearlessness;     // 대담함 (0~100)
    bool playing = true;           // 게임 진행 상태

public:
    Player(int number);
    Player(int number, int agility, int fearlessness);

    // 가상 함수들
    virtual bool act() { return true; };
    virtual bool isPlaying() { return playing; };
    virtual void dyingMessage();
    virtual void printStatus();

    // Getter 함수들
    int getNumber() const;
    int getAgility() const;
    int getFearlessness() const;
    int getPower() const;          // agility + fearlessness

    // 정적 랜덤 함수
    static float getRandomProbability();
};
```

### Player 파생 클래스

| 클래스 | 게임 | 주요 메서드 | 특징 |
|--------|------|------------|------|
| **PlayerRLGL** | 무궁화 꽃이 피었습니다 | `bool act()` | 이동 거리 계산, 넘어질 확률 체크 |
| **PlayerRPS** | 가위바위보 | `bool act()` | 랜덤 가위바위보 선택 및 승부 결정 |
| **PlayerTOW** | 줄다리기 | `bool act()` | 팀 기반 게임, 개별 행동 없음 |
| **PlayerGlassBridge** | 유리다리 | `bool act()` | 게임 로직에서 처리 |
| **PlayerMarble** | 구슬치기 | `bool act()` | 게임 로직에서 처리 |
| **PlayerDdakji** | 딱지치기 | `bool act()` | 게임 로직에서 처리 |
| **PlayerShip** | 신체 아시아 | `float doTask()` | 작업 완료 시간 계산 |
| **PlayerSquidGame** | 오징어 게임 | `bool act()` | 공격/방어 선택 및 생존 확률 계산 |

---

## Game 클래스 계층 구조

### 기본 Game 클래스

```cpp
class Game
{
protected:
    std::string gameName;
    std::list<Player*> players;

    unsigned int initial_count = 0;    // 초기 참가자 수
    unsigned int survivor_count = 0;   // 생존자 수
    unsigned int death_count = 0;      // 사망자 수
    Player* winner = nullptr;          // 최종 우승자 (SquidGame용)

public:
    Game(std::string name);
    virtual ~Game();

    // 순수 가상 함수 (반드시 구현 필요)
    virtual void join(Player* player) = 0;
    virtual void play() = 0;

    // 공통 함수
    void printGameName();
    void printAlivePlayers();
    void printSummary() const;
    std::list<Player*> getAlivePlayers();
};
```

### Game 파생 클래스

| 게임 클래스 | 게임명 | 최소 인원 | 승부 방식 |
|------------|--------|----------|----------|
| **RedLightGreenLight** | 무궁화 꽃이 피었습니다 | 1명 | 거리 1000 이상 도달, 넘어지지 않기 |
| **RPS** | 가위바위보 | 2명 | 가위바위보 승부 |
| **TugOfWar** | 줄다리기 | 2명 | 팀 파워 합산 대결 |
| **GlassBridge** | 유리다리 | 1명 | 18개 칸 안전한 유리 선택 |
| **Marbles** | 구슬치기 | 2명 | 홀짝 맞추기 1:1 대결 |
| **Ddakji** | 딱지치기 | 2명 | 파워(agility+fearlessness) 비교 |
| **Pysical_Asia_ship** | 신체 아시아 | 3명 | 작업 완료 시간 경쟁 |
| **SquidGame** | 오징어 게임 (최종전) | 2명 | 라운드제 생존 게임 |

---

## 게임별 상세 설명

### 1. RedLightGreenLight (무궁화 꽃이 피었습니다)

**게임 규칙**:
- 목표 거리: 1000
- 턴 수: 20턴 (생성자로 변경 가능)
- 넘어질 확률: 10%

**생존 조건**:
- 총 이동 거리가 1000 이상 도달
- 넘어지지 않기

**이동 거리 계산**:
```cpp
int moving_distance = agility + number + agility * (fearlessness * 0.01);
```

**특징**:
- 플레이어 번호가 낮을수록 불리 (이동 거리에 번호 포함)
- 높은 민첩성과 대담함이 유리

---

### 2. RPS (가위바위보)

**게임 규칙**:
- 각 플레이어가 랜덤하게 가위/바위/보 선택
- 승부가 날 때까지 반복
- 패배 시 즉시 탈락

**특징**:
- 능력치 무관, 완전 랜덤
- 빠른 인원 감소 게임

---

### 3. TugOfWar (줄다리기)

**게임 규칙**:
- 플레이어를 2개 팀으로 교대 배치 (짝수 번호/홀수 번호)
- 각 팀의 파워 합산 비교
- 낮은 팀 전원 탈락

**팀 파워 계산**:
```cpp
team_power = Σ(agility + fearlessness)
```

**특징**:
- 동점일 경우 양팀 모두 생존
- 운보다 능력치가 중요

---

### 4. GlassBridge (유리다리)

**게임 규칙**:
- 총 18개의 칸
- 각 칸마다 안전한 유리 위치가 랜덤으로 결정 (50% 확률)
- 플레이어는 순서대로 유리 선택 (50% 확률)
- 틀린 유리를 밟으면 탈락

**특징**:
- 앞 플레이어가 안전한 경로를 밝혀줌
- 순서가 뒤일수록 유리
- 능력치 무관

---

### 5. Marbles (구슬치기)

**게임 규칙**:
- 플레이어를 2명씩 페어링
- 홀수 명이면 마지막 한 명 부전승
- 각 페어에서 홀/짝 맞추기
- 틀린 플레이어 탈락

**승부 결정**:
```cpp
int marbles = random(1~10);
bool isOdd = (marbles % 2 == 1);
bool guessOdd = random(50%);
```

**특징**:
- 능력치 무관, 완전 랜덤
- 정확히 절반 생존 (부전승 제외)

---

### 6. Ddakji (딱지치기)

**게임 규칙**:
- 플레이어를 2명씩 페어링
- 홀수 명이면 마지막 한 명 부전승
- 파워(agility + fearlessness) 비교
- 낮은 플레이어 탈락

**승부 결정**:
```cpp
int power1 = player1->getPower();
int power2 = player2->getPower();

if (power1 > power2) → player1 승리
else if (power2 > power1) → player2 승리
else → 랜덤 (50%)
```

**특징**:
- 능력치가 높을수록 유리
- 동점일 경우에만 랜덤

---

### 7. Pysical_Asia_ship (신체 아시아)

**게임 규칙**:
- 최소 3명 필요
- 각 플레이어가 작업 수행 (시간 측정)
- 빠른 순서대로 절반 생존
- 2명 남을 때까지 반복

**작업 시간 계산**:
```cpp
float baseTime = 8.0f;
float abilityFactor = (agility * 0.6 + fearlessness * 0.4) / 150.0f;
float randomFactor = random(0~1);
float taskTime = baseTime * (1.1 - abilityFactor) * (0.85 + randomFactor * 0.3);
```

**특징**:
- 능력치가 높을수록 작업 시간 짧음
- 랜덤 요소로 변동성 있음
- 라운드제 진행

---

### 8. SquidGame (오징어 게임 - 최종전)

**게임 규칙**:
- 최소 2명 필요
- 라운드마다 모든 플레이어가 행동
- 실패한 플레이어 즉시 탈락
- 1명 남을 때까지 진행

**행동 결정**:
```cpp
bool isAttack = random(50%);  // 공격/방어 선택
float baseProb = 0.4;
float abilityBonus = (agility * 0.5 + fearlessness * 0.5) / 200.0;
float successProb = baseProb + abilityBonus;

if (isAttack) successProb -= 0.1;  // 공격은 위험
else successProb += 0.05;          // 방어는 안전
```

**특징**:
- 능력치가 높을수록 생존 확률 증가
- 공격/방어 선택에 따라 확률 변동
- 최종 생존자를 `winner`로 기록

---

## 설계 패턴 및 특징

### 1. 상속을 통한 다형성

```cpp
// Game 클래스 계층
Game (추상 클래스)
├── RedLightGreenLight
├── RPS
├── TugOfWar
├── GlassBridge
├── Marbles
├── Ddakji
├── Pysical_Asia_ship
└── SquidGame

// Player 클래스 계층
Player (기본 클래스)
├── PlayerRLGL
├── PlayerRPS
├── PlayerTOW
├── PlayerGlassBridge
├── PlayerMarble
├── PlayerDdakji
├── PlayerShip
└── PlayerSquidGame
```

### 2. Friend 클래스

각 Game 클래스는 해당하는 Player 클래스를 `friend`로 선언하여 protected 멤버에 접근 가능:

```cpp
class RedLightGreenLight : public Game {
    friend class PlayerRLGL;
    static const unsigned int distance;
    static const float fallDownRate;
};
```

### 3. 정적 멤버 활용

**Player 클래스의 랜덤 생성기**:
```cpp
static std::default_random_engine random_engine;
static std::uniform_int_distribution<unsigned int> ability_range;  // 0~100
static std::uniform_real_distribution<float> possibility;          // 0.0~1.0
```

**Game 클래스의 상수**:
```cpp
static const unsigned int distance = 1000;         // RedLightGreenLight
static const float fallDownRate = 0.1f;            // RedLightGreenLight
static const int totalSteps = 18;                  // GlassBridge
```

### 4. 메모리 관리

**소멸자에서 동적 메모리 해제**:
```cpp
Game::~Game() {
    for (auto player = players.begin(); player != players.end(); ++player) {
        delete (*player);
    }
    players.clear();
}
```

**게임 중 탈락자 처리**:
```cpp
(*player)->dyingMessage();
delete (*player);
player = players.erase(player);
```

### 5. 플레이어 복사 생성

**각 게임별로 플레이어를 복사하여 생성**:
```cpp
void RedLightGreenLight::join(Player* player) {
    players.push_back(new PlayerRLGL(*player));
}
```

이는 원본 플레이어 객체를 보존하면서 게임별 특화된 플레이어를 생성하기 위함입니다.

### 6. 통계 및 요약 기능

**Game 클래스의 통계 추적**:
```cpp
unsigned int initial_count = 0;    // 초기 참가자 수
unsigned int survivor_count = 0;   // 생존자 수
unsigned int death_count = 0;      // 사망자 수
Player* winner = nullptr;          // 최종 우승자
```

**요약 출력**:
```cpp
void Game::printSummary() const {
    float deathRate = (death_count / initial_count) * 100.0f;
    float survivalRate = (survivor_count / initial_count) * 100.0f;
    // 게임명, 참가자, 생존자, 사망자, 사망률, 우승자 출력
}
```

---

## 능력치 시스템

### Player 능력치

| 능력치 | 범위 | 영향을 주는 게임 |
|--------|------|-----------------|
| **agility (민첩성)** | 0~100 | RedLightGreenLight, Ddakji, TugOfWar, Pysical_Asia_ship, SquidGame |
| **fearlessness (대담함)** | 0~100 | RedLightGreenLight, Ddakji, TugOfWar, Pysical_Asia_ship, SquidGame |
| **power (파워)** | 0~200 | agility + fearlessness의 합 |

### 능력치 생성

**랜덤 생성**:
```cpp
Player::Player(int number) {
    this->number = number;
    agility = ability_range(random_engine);      // 0~100
    fearlessness = ability_range(random_engine); // 0~100
}
```

**고정값 생성**:
```cpp
Player::Player(int number, int agility, int fearlessness) {
    this->number = number;
    this->agility = agility;
    this->fearlessness = fearlessness;
}
```

---

## 게임 난이도 분석

| 게임 | 난이도 결정 요소 | 생존율 예상 |
|------|-----------------|-----------|
| RedLightGreenLight | 능력치 + 운 | 중간 (40~60%) |
| RPS | 순수 운 | 중간 (50%) |
| TugOfWar | 능력치 + 팀 배치 | 중간 (50%) |
| GlassBridge | 순수 운 + 순서 | 낮음 (10~30%) |
| Marbles | 순수 운 | 중간 (50%) |
| Ddakji | 능력치 + 약간의 운 | 중간 (50%) |
| Pysical_Asia_ship | 능력치 + 운 | 고정 (50%, 라운드당) |
| SquidGame | 능력치 + 운 | 점진적 감소 |

---

## 코드 품질 특징

### 장점
1. **명확한 클래스 계층 구조**: 상속과 다형성을 효과적으로 활용
2. **캡슐화**: 게임별 로직이 독립적으로 구현됨
3. **재사용성**: 공통 기능을 기본 클래스에 구현
4. **확장성**: 새로운 게임 추가가 용이
5. **메모리 관리**: 소멸자에서 동적 메모리 해제

### 개선 가능한 부분
1. **스마트 포인터 사용**: `std::unique_ptr` 또는 `std::shared_ptr` 사용 고려
2. **예외 처리**: 예외 상황에 대한 처리 추가
3. **const 정확성**: const 멤버 함수 추가 활용
4. **인터페이스 분리**: 일부 게임은 공통 인터페이스로 추상화 가능

---

## 참고 파일

- [Game.h](source_codes/Game.h) - 게임 클래스 선언
- [Game.cpp](source_codes/Game.cpp) - 게임 클래스 구현
- [Player.h](source_codes/Player.h) - 플레이어 클래스 선언
- [Player.cpp](source_codes/Player.cpp) - 플레이어 클래스 구현
- [Project.cpp](source_codes/Project.cpp) - 메인 프로그램

---

## 결론

이 프로젝트는 객체지향 프로그래밍의 핵심 개념들을 효과적으로 활용하여 게임 시뮬레이션 시스템을 구현했습니다. 상속, 다형성, 캡슐화를 통해 코드의 재사용성과 확장성을 확보했으며, 각 게임별로 독립적인 로직을 구현하여 유지보수가 용이한 구조를 갖추고 있습니다.
