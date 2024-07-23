// Game.c : ゲーム画面
#include "bios.h"
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Ship.h"
#include "Shot.h"
#include "Enemy.h"
// 変数の定義
u8 gameFlag;   // フラグ
static void GameInitialize(void);
static void GameLoad(void);
static void GameStart(void);
static void GamePlay(void);
void GameUpdate(void) { // ゲームを更新する
    if (appState == GAME_STATE_INITIALIZE)   GameInitialize();// 初期化
    else if (appState == GAME_STATE_LOAD)    GameLoad();      // ロード
    else if (appState == GAME_STATE_START)   GameStart();     // 開始
    else if (appState == GAME_STATE_PLAY)    GamePlay();      // プレイ
    // 一時停止
    if (gameFlag & (1 << GAME_FLAG_PAUSE)) return;
    ShipUpdate();   // 自機の更新
    ShotUpdate();   // ショットの更新
    EnemyUpdate();  // 敵の更新
}
static void GameInitialize(void) { // ゲームを初期化する
    // スプライトのクリア
    SystemClearSprite();
    ShipInitialize();   // 自機の初期化
    ShotInitialize();   // ショットの初期化
    EnemyInitialize();  // 敵の初期化
    gameFlag = 0; // フラグの初期化
    // 状態の更新
    appState = GAME_STATE_LOAD;
    appPhase = APP_PHASE_NULL;
}
static void GameLoad(void) { // ゲームをロードする
    if (appPhase == 0) { // フェーズ0
        gameFlag |= 1 << GAME_FLAG_STATUS; // フラグの設定
        appPhase++;                        // 状態の更新
    } else {             // フェーズ1 ロードの処理
        appState = GAME_STATE_START; // 状態の更新
        appPhase = APP_PHASE_NULL;
    }
}
static void GameStart(void) { // ゲームを開始する
    if (appPhase == 0) {// 初期化
        // フラグの設定
        gameFlag &= ~((1<<GAME_FLAG_PLAYABLE)|(1<<GAME_FLAG_PAUSE)|(1<<GAME_FLAG_STATUS));
        appPhase++; // 状態の更新
    }
    appState = GAME_STATE_PLAY; // 状態の更新
    appPhase = APP_PHASE_NULL;
}
static void GameCheckShotEnemy(void);
static void GamePlay(void) { // ゲームをプレイする
    {
        if (appPhase == 0) {// 初期化
            // フラグの設定
            gameFlag &= ~(1<<GAME_FLAG_PAUSE);
            gameFlag |= (1<<GAME_FLAG_PLAYABLE)|(1<<GAME_FLAG_STATUS);
            appPhase++; // 状態の更新
        }
        // プレイの処理
        if (input[INPUT_BUTTON_ESC]==0x01) { // START ボタンが押された
            gameFlag ^= (1 << GAME_FLAG_PAUSE); // ポーズフラグをフリップ
        }
    }
    if (gameFlag & (1<<GAME_FLAG_PAUSE)) return; // 一時停止
    GameCheckShotEnemy();
}
static void GameCheckShotEnemy(void) { // ショットと敵のヒットチェックを行う
    SHOT* iy = (SHOT*)shot;
    for (u8 c=0;c<SHOT_SIZE;c++,iy++) {// ショットの走査
        if (iy->state == SHOT_STATE_NULL) continue; // ショットの存在
        ENEMY* ix = enemies;
        for (u8 b=0;b<ENEMIES_SIZE;ix++, b++) {// 敵の走査
            if (ix->nodamage) continue;// 敵の存在
            // ヒットチェック
            i8 a = ix->xi-iy->x;
            if (!((i8)0xf9 <= a && a < 8)) continue;
            a = ix->yi-iy->y;
            if (!((i8)0xf6 <= a && a < 0x0b)) continue;
            // あたり
            ix->nodamage = 0x80;// 敵のノーダメージの更新
            ix->state = ENEMY_STATE_BOMB;// 敵の状態の更新
            ix->phase = APP_PHASE_NULL;
            iy->state = SHOT_STATE_NULL;// ショットの状態の更新
        }
    }
}
