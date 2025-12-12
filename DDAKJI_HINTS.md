# 딱지치기(Ddakji) 게임 구현 힌트

## 게임 개요
딱지치기는 두 명의 플레이어가 1:1로 대결하는 게임입니다. 각 플레이어는 자신의 능력치를 바탕으로 딱지를 뒤집으려고 시도합니다.

---

## 현재 코드 상태

### 완성된 부분
- `Ddakji::join()` - 플레이어를 게임에 참가시키는 함수 (완성)
- `PlayerDdakji` 클래스 정의 (완성)

### 미완성 부분
- `Ddakji::play()` - 게임 로직 구현 필요 ([Game.cpp:394-408](source_codes/Game.cpp#L394-L408))

---

## 구현 힌트

### 1. 기본 게임 구조
다른 게임들(Marbles, RPS 등)을 참고하여 다음 흐름을 구현하세요:

```
1. 플레이어 수 체크 (2명 이상 필요)
2. 플레이어들을 1:1로 매칭
3. 각 매치에서 승자와 패자 결정
4. 패자 제거
5. 생존자 통계 출력
```

### 2. 플레이어 매칭 방법

#### 참고: Marbles 게임의 매칭 방식 ([Game.cpp:320-363](source_codes/Game.cpp#L320-L363))
```cpp
// 홀수 명이면 마지막 한 명은 자동 부전승
if (players.size() % 2 == 1) {
    auto lastPlayer = players.end();
    --lastPlayer;
    // 부전승 처리
    winners.push_back(*lastPlayer);
    players.erase(lastPlayer);
}

// 2명씩 매칭
int numMatches = players.size() / 2;
for (int i = 0; i < numMatches; ++i) {
    auto player1 = players.begin();
    auto player2 = players.begin();
    ++player2;

    // 승부 로직
    // ...
}
```

### 3. 승부 결정 방법

딱지치기의 승부는 **능력치 대결**입니다:

#### 방법 A: 단순 능력치 합산
```cpp
int power1 = (*player1)->getPower();  // agility + fearlessness
int power2 = (*player2)->getPower();

if (power1 > power2) {
    // player1 승리
} else if (power2 > power1) {
    // player2 승리
} else {
    // 동점인 경우 처리 (랜덤 또는 둘 다 생존)
}
```

#### 방법 B: 능력치 + 랜덤 요소
```cpp
// 민첩성(agility)이 높을수록 성공 확률 증가
float successRate1 = (*player1)->getAgility() / 100.0f;
float successRate2 = (*player2)->getAgility() / 100.0f;

bool flip1 = (Player::getRandomProbability() < successRate1);
bool flip2 = (Player::getRandomProbability() < successRate2);

if (flip1 && !flip2) {
    // player1 승리
} else if (flip2 && !flip1) {
    // player2 승리
} else {
    // 둘 다 성공 또는 둘 다 실패 시 능력치로 결정
}
```

#### 방법 C: 대담함(fearlessness) 활용
```cpp
// 대담함이 높을수록 강하게 던짐
int throwPower1 = (*player1)->getAgility() + (*player1)->getFearlessness() / 2;
int throwPower2 = (*player2)->getAgility() + (*player2)->getFearlessness() / 2;

// 난수를 더해서 변동성 추가
throwPower1 += static_cast<int>(Player::getRandomProbability() * 20);
throwPower2 += static_cast<int>(Player::getRandomProbability() * 20);
```

### 4. 승자/패자 처리

#### Marbles 게임 참고 ([Game.cpp:358-372](source_codes/Game.cpp#L358-L372))
```cpp
std::list<Player*> winners;
std::list<Player*> losers;

// 매치마다 승자와 패자를 리스트에 추가
winners.push_back(winner);
losers.push_back(loser);

// 플레이어 리스트에서 제거
players.erase(player1);
players.erase(player2);

// 패자들 제거
for (auto loser : losers) {
    loser->dyingMessage();
    delete loser;
}

// 승자들을 players로
players = winners;
```

### 5. 통계 출력

모든 게임에서 공통으로 사용하는 패턴:
```cpp
unsigned int survivor_count = players.size();
unsigned int death_count = initial_count - survivor_count;

printAlivePlayers();

std::cout << "\n[Game Statistics]" << std::endl;
std::cout << "Total players: " << initial_count << std::endl;
std::cout << "Survivors: " << survivor_count << std::endl;
std::cout << "Deaths: " << death_count << std::endl << std::endl;
```

---

## 구현 체크리스트

- [ ] 플레이어 수가 2명 미만이면 게임 중단
- [ ] 홀수 명일 경우 부전승 처리
- [ ] 플레이어를 2명씩 페어링
- [ ] 각 페어에서 승부 결정 로직 구현
- [ ] 승자 리스트에 추가
- [ ] 패자는 dyingMessage() 호출 후 메모리 해제
- [ ] 최종 생존자 통계 출력

---

## 추천 구현 순서

1. **Marbles 게임의 `play()` 함수를 복사**하여 시작
2. **승부 결정 로직만 수정**
   - 구슬 게임 로직 대신 딱지치기 로직으로 변경
3. **출력 메시지 커스터마이징**
   - "Match: Player #X vs Player #Y" 등
4. **테스트 및 디버깅**

---

## 참고 파일

- [Game.cpp:394-408](source_codes/Game.cpp#L394-L408) - Ddakji::play() 미완성 부분
- [Game.cpp:303-383](source_codes/Game.cpp#L303-L383) - Marbles::play() 참고용
- [Player.h:81-88](source_codes/Player.h#L81-L88) - PlayerDdakji 클래스
- [Player.cpp:162-171](source_codes/Player.cpp#L162-L171) - PlayerDdakji 구현

---

## 예시 출력 형식

```
[[[Ddakji]]]
10 players joined the game.
Player #1 gets a bye.
Match: Player #2 vs Player #3
Player #3 won the match.
Match: Player #4 vs Player #5
Player #4 won the match.
...
Player #3(45,67) lose at Ddakji.
Player #5(32,88) lose at Ddakji.
...
Player #1(56,78) is alive.
Player #2(89,45) is alive.
Player #4(67,90) is alive.

[Game Statistics]
Total players: 10
Survivors: 5
Deaths: 5
```

---

## 추가 팁

### 능력치 활용 방법
- **agility (민첩성)**: 딱지를 정확하게 던지는 능력
- **fearlessness (대담함)**: 강하게 던지는 능력
- **getPower()**: agility + fearlessness 합계

### 랜덤 함수 사용
```cpp
// 0.0 ~ 1.0 사이의 난수
float randomValue = Player::getRandomProbability();

// 특정 범위의 정수 난수 (예: 1~10)
int randomInt = static_cast<int>(Player::getRandomProbability() * 10) + 1;
```

### 메모리 관리 주의사항
- 패자는 반드시 `delete`로 메모리 해제
- `erase()`로 리스트에서 제거
- 승자는 `winners` 리스트로 이동 후 `players`에 재할당

---

## 구현 완료 후 확인사항

1. 메모리 누수가 없는가?
2. 홀수 명일 때 부전승이 정상 작동하는가?
3. 모든 플레이어가 정확히 한 번씩만 매칭되는가?
4. 통계가 정확하게 출력되는가?
5. dyingMessage()가 모든 패자에게 호출되는가?

---

**화이팅! 다른 게임 구현을 참고하면 충분히 완성할 수 있습니다!**
