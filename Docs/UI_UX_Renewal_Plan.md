# UI/UX 리뉴얼 분석 및 적용 가이드

## 1. 기존 UI 재검토 결과

| 대상 | 현재 상태 | 기능 관점 문제 | 디자인 관점 문제 |
| --- | --- | --- | --- |
| `WBP_HUD` | `CanvasPanel`과 단일 `TextBlock`만 존재 | 점수·웨이브 타이머·레벨·체력 연결이 없음 | 정보 우선순위, 정렬, 게이지, 일관된 색 체계가 없음 |
| `WBP_MainMenu` | 에셋 없음 | 시작·종료 버튼 및 레벨 이동 연결이 없음 | 메뉴 레이아웃과 상태별 버튼 스타일이 없음 |
| `HP` 위젯 | 별도 에셋 없음 | 현재 체력만 정수 반환, 최대 체력/비율 UI API가 없음 | 숫자와 게이지를 함께 보여주는 체력 표현이 없음 |
| PlayerController UI | HUD를 한 번 생성해 화면에 추가 | 위젯 참조 저장, 메뉴 전환, 입력 모드/마우스 전환이 없음 | HUD와 메뉴의 표시 우선순위를 관리하지 않음 |
| GameState UI 데이터 | 점수/웨이브 데이터는 보유 | 남은 시간을 UI에서 읽을 함수가 없음 | HUD 표시 형식이 정의되지 않음 |

`WBP_HUD.uasset`는 바이너리 에셋이므로 이 브랜치에서는 C++ 기본 UI를 실제 동작 가능한 fallback으로 제공하고, Unreal Editor에서 같은 이름의 위젯을 배치하면 자동 연결되는 구조로 개선했다.

## 2. 반영한 개선 기획

### HUD 정보 구조

한 화면에서 다음 정보를 즉시 확인할 수 있게 한다.

- 왼쪽: 점수, 현재 레벨/전체 레벨, 현재 웨이브/전체 웨이브
- 중앙: 남은 시간 `MM:SS`
- 오른쪽: 현재/최대 체력 숫자와 체력 게이지
- 남은 시간 10초 이하: 타이머를 경고색으로 전환
- 체력 30% 이하: 체력 바를 경고색으로 전환

권장 계층 구조:

```text
CanvasPanel (HUDRoot)
└─ Border (StatusBackground)
   └─ HorizontalBox (StatusRow)
      ├─ VerticalBox (ProgressGroup)
      │  ├─ ScoreText
      │  ├─ LevelText
      │  └─ WaveText
      ├─ TimerText
      └─ VerticalBox (HealthGroup)
         ├─ HealthText
         └─ SizeBox
            └─ HealthBar
```

### 메뉴 정보 구조

메인 메뉴:

- 제목: `SURVIVAL ARCADE`
- 설명
- `StartButton`: 게임 시작
- `QuitButton`: 게임 종료

게임 오버 메뉴:

- 제목: `GAME OVER`
- 설명
- `RestartButton`: 게임 데이터를 초기화하고 첫 레벨부터 재시작
- `MainMenuButton`: 설정한 메인 메뉴 레벨로 이동

권장 계층 구조:

```text
CanvasPanel (MenuRoot)
├─ BackgroundBlur
├─ Border (DimLayer)
└─ Border (MenuCard)
   └─ VerticalBox (MenuColumn)
      ├─ TitleText
      ├─ SubtitleText
      ├─ StartButton
      ├─ RestartButton
      ├─ MainMenuButton
      └─ QuitButton
```

## 3. 디자인 토큰

| 항목 | 값/방향 |
| --- | --- |
| 기본 배경 | 짙은 네이비, 불투명도 86~96% |
| 강조색 | Cyan `#26D9FF` |
| 기본 글자 | Blue White `#EBF5FF` |
| 경고색 | Red Orange `#FF381F` |
| 버튼 Normal | `#0A1F3D` |
| 버튼 Hover | `#0D8CB8` |
| 버튼 Pressed | `#055075` |
| 제목 크기 | 42 |
| HUD 타이머 | 32 |
| 버튼 텍스트 | 22 |
| 본문/보조 정보 | 17~24 |

C++ fallback 메뉴는 `FButtonStyle`의 Normal/Hovered/Pressed 색을 모두 정의하고, 눌림 상태에서 padding을 바꿔 클릭 피드백을 제공한다. 메뉴 뒤에는 `BackgroundBlur`와 반투명 dim layer를 적용한다.

## 4. 기능 연결표

| UI 요소 | C++ 호출 | 결과 |
| --- | --- | --- |
| `StartButton` | `AJupiterPlayerController::StartNewGame` | GameInstance 초기화 후 `FirstGameLevelName` 이동 |
| `RestartButton` | `AJupiterPlayerController::RestartGame` | 첫 레벨부터 재시작 |
| `MainMenuButton` | `AJupiterPlayerController::ReturnToMainMenu` | `MainMenuLevelName` 이동, 이름이 비어 있으면 현재 레벨 위에 메뉴 표시 |
| `QuitButton` | `AJupiterPlayerController::QuitGame` | 게임 종료 요청 |
| 캐릭터 사망 | `AJupiterGameState::OnGameOver` | 게임 오버 메뉴 표시 및 UI 입력 모드 전환 |
| HUD | `UJupiterHUDWidget::RefreshHUD` | 점수·시간·레벨·웨이브·체력을 0.1초 간격 갱신 |

## 5. Unreal Editor 적용 순서

C++ fallback UI는 에셋을 지정하지 않아도 표시된다. 디자이너 에셋으로 외형을 추가 조정할 때는 아래 순서로 적용한다.

1. 기존 `WBP_HUD`의 부모 클래스를 `JupiterHUDWidget`으로 Reparent한다.
2. 위젯 이름을 정확히 `ScoreText`, `TimerText`, `LevelText`, `WaveText`, `HealthText`, `HealthBar`로 지정하고 **Is Variable**을 켠다.
3. `WBP_MainMenu`, `WBP_GameOver`를 만들고 부모 클래스를 `JupiterMenuWidget`으로 지정한다.
4. 메뉴 위젯의 버튼/텍스트 이름을 `TitleText`, `SubtitleText`, `StartButton`, `RestartButton`, `MainMenuButton`, `QuitButton`과 정확히 맞춘다.
5. PlayerController 블루프린트의 `HUDWidgetClass`, `MainMenuWidgetClass`, `GameOverWidgetClass`에 각각 위젯을 지정한다.
6. 메뉴 전용 레벨에서는 `bShowMainMenuOnBeginPlay`를 켠다. 게임 레벨에서는 끈다.
7. `FirstGameLevelName`은 첫 게임 맵 이름으로, `MainMenuLevelName`은 실제 메인 메뉴 맵 이름으로 설정한다.
8. 패키징 빌드에서 `QuitButton` 종료와 모든 레벨 이름을 최종 확인한다.

위젯 이름이 다르거나 부모 클래스가 맞지 않으면 PlayerController는 경고 로그를 남기고, 화면이 비는 대신 C++ fallback HUD/메뉴를 사용한다.
