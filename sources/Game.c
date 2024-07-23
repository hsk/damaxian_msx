// Game.c : ゲーム画面
#include "bios.h"
#include "System.h"
#include "Math.h"
#include "App.h"
#include "Game.h"
#include "Back.h"
#include "Ship.h"
#include "Shot.h"
#include "Enemy.h"
#include "Bullet.h"
// 変数の定義
u8 gameFlag;   // フラグ
static u8 gameCount;   // カウント
static u8 gameBackAngle;   // 撃ち返し
static i16 gameBackCos;
static i16 gameBackSin;
static void GameInitialize(void);
static void GameLoad(void);
static void GameStart(void);
static void GamePlay(void);
static void GameTimeUp(void);
static void GameOver(void);
static void GameUnload(void);
static void GameEnd(void);
void GameUpdate(void) { // ゲームを更新する
    if (appState == GAME_STATE_INITIALIZE)   GameInitialize();// 初期化
    else if (appState == GAME_STATE_LOAD)    GameLoad();      // ロード
    else if (appState == GAME_STATE_START)   GameStart();     // 開始
    else if (appState == GAME_STATE_PLAY)    GamePlay();      // プレイ
    else if (appState == GAME_STATE_TIMEUP)  GameTimeUp();    // タイムアップ
    else if (appState == GAME_STATE_OVER)    GameOver();      // オーバー
    else if (appState == GAME_STATE_UNLOAD)  GameUnload();    // アンロード
    else                                     GameEnd();       // 終了
    // 一時停止
    if (gameFlag & (1 << GAME_FLAG_PAUSE)) return;
    BackUpdate();   // 背景の更新
    ShipUpdate();   // 自機の更新
    ShotUpdate();   // ショットの更新
    EnemyUpdate();  // 敵の更新
    BulletUpdate(); // 弾の更新
    // ステータスの更新
    if (gameFlag & (1 << GAME_FLAG_STATUS)) BackTransferStatus();
}
static void GameInitialize(void) { // ゲームを初期化する
    // スプライトのクリア
    SystemClearSprite();
    // タイマの初期化
    appTimer[0] = 0x03;
    appTimer[1] = 0;
    appTimer[2] = 0;
    appTimer[3] = 0;
    ShipInitialize();   // 自機の初期化
    ShotInitialize();   // ショットの初期化
    EnemyInitialize();  // 敵の初期化
    BulletInitialize(); // 弾の初期化
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
        BackStoreMessage(BACK_MESSAGE_START);// メッセージのロード
        // フラグの設定
        gameFlag &= ~((1<<GAME_FLAG_PLAYABLE)|(1<<GAME_FLAG_PAUSE)|(1<<GAME_FLAG_STATUS));
        appPhase++; // 状態の更新
    }
    if (appPhase++ != 60*3) return;
    BackRestoreMessage(); // メッセージのアンロード
    appState = GAME_STATE_PLAY; // 状態の更新
    appPhase = APP_PHASE_NULL;
}
static void GameCheckShotEnemy(void);
static void GameCheckShipBullet(void);
static void GameCheckShipEnemy(void);
static void GamePlay2(void) {
    // タイマの更新
    if (appTimer[0]|appTimer[1]|appTimer[2]|appTimer[3]) {
        appTimer[3]--;
        if (appTimer[3] == 255) {
            appTimer[3] = 9;
            appTimer[2]--;
            if (appTimer[2] == 255) {
                appTimer[2] = 9;
                appTimer[1]--;
                if (appTimer[1] == 255) {
                    appTimer[1] = 9;
                    appTimer[0]--;
                    if (appTimer[0] == 255) {
                        appTimer[0] = 0;
                    }
                }
            }
        }
    }
    // 倒した数の設定
    GameCheckShotEnemy();  // ショットと敵のヒットチェック
    GameCheckShipBullet(); // 自機と弾のヒットチェック
    GameCheckShipEnemy();  // 自機と敵のヒットチェック
}
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
    GamePlay2();
    {
        // タイムアップ
        if (appTimer[0]|appTimer[1]|appTimer[2]|appTimer[3]) return;
        // 状態の更新
        appState = GAME_STATE_TIMEUP;
        appPhase = APP_PHASE_NULL;
    }
}
static void GameTimeUp(void) { // ゲームがタイムアップする
    if (appPhase == 0) {
        BackStoreMessage(BACK_MESSAGE_TIMEUP); // メッセージのロード
        gameCount = 180; // カウントの設定
        // フラグの設定
        gameFlag &= ~((1<<GAME_FLAG_PLAYABLE)|(1<<GAME_FLAG_PAUSE)|(1<<GAME_FLAG_STATUS));
        appPhase++; // 状態の更新
    }
    // カウントの更新
    gameCount--;
    if (gameCount) return;
    // 更新
    // 状態の更新
    appState = GAME_STATE_OVER;
    appPhase = APP_PHASE_NULL;
}
static void GameOver(void) { // ゲームオーバーになる
    if (appPhase == 0) {// 初期化
        BackStoreMessage(BACK_MESSAGE_GAMEOVER); // メッセージのロード
        gameCount = 180;// カウントの設定
        // フラグの設定
        gameFlag &= ~((1<<GAME_FLAG_PLAYABLE)|(1<<GAME_FLAG_PAUSE)|(1<<GAME_FLAG_STATUS));
        appPhase++;// 状態の更新
    }
    // カウントの更新
    gameCount--;
    if (gameCount==0) {
        // メッセージのアンロード
        BackRestoreMessage();
        // 状態の更新
        appState = GAME_STATE_UNLOAD;
        appPhase = APP_PHASE_NULL;
    }
}
static void GameUnload(void) { // ゲームをアンロードする
    if (appPhase == 0) {// 初期化
        gameFlag = 0;   // フラグの設定
        appPhase++;     // 状態の更新
    }
    appState = GAME_STATE_END; // 状態の更新
}
static void GameEnd(void) { // ゲームを終了する
    appMode = APP_MODE_TITLE; // モードの更新
    appState = 0; // 状態の更新
}
static void GameShootBack(ENEMY* ix);
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
            GameShootBack(ix);// 敵の撃ち返し
            iy->state = SHOT_STATE_NULL;// ショットの状態の更新
        }
    }
}
static void GameCheckShipBullet(void) { // 自機と弾のヒットチェックを行う
    if (ship.nodamage) return;// 自機の存在
    BULLET* ix = bullets;
    for (u8 b = BULLET_SIZE; b; ix++, b--) {// 弾の走査
        if (ix->state == BULLET_STATE_NULL) continue;// 弾の存在
        // ヒットチェック
        i16 a = (i8)ix->xi - (i8)ship.x;
        if (a < 0) a = -a;
        if (a >= 6) continue;
        a = (i8)ix->yi - (i8)ship.y;
        if (a < 0) a = -a;
        if (a >= 6) continue;
        ix->state = BULLET_STATE_NULL; // 弾の状態の更新
        ship.state = SHIP_STATE_BOMB;// 自機の状態の更新
        ship.phase = APP_PHASE_NULL;
    }
}
static void GameCheckShipEnemy(void) { // 自機と敵のヒットチェックを行う
    if (ship.nodamage)return;// 自機の存在
    ENEMY* ix = enemies;
    for (u8 b = ENEMIES_SIZE; b; ix++,--b) {// 敵の走査
        if (ix->nodamage) continue;// 敵の存在
        // ヒットチェック X
        i16 a = ix->xi-ship.x;
        if (a < 0) a = -a;
        if (a >= 8) continue;
        // ヒットチェック Y
        a = ix->yi-ship.y;
        if (a < 0) a = -a;
        if (a >= 8) continue;
        // 更新
        ix->nodamage = 0x80; // 敵のノーダメージの更新
        ix->state = ENEMY_STATE_BOMB;// 敵の状態の更新
        ix->phase = APP_PHASE_NULL;
        ship.state = SHIP_STATE_BOMB;// 自機の状態の更新
        ship.phase = APP_PHASE_NULL;
    }
}

static void getvec(ENEMY* ix) {
    // ベクトルの取得
    i8 x = ship.x - ix->xi;
    u8 y = ship.y - ix->yi;
    // 必ず下向きに撃ち返す
    if (y & (1<<7)) {
        y >>= 1;
        x >>= 1;
    }
    i16 hl = ((i16)x<<8)|y;
    // 方向の取得
    gameBackAngle = SystemGetAtan2(hl);
    // 弾の位置の設定
    bulletEntry.x = ix->x;
    bulletEntry.y = ix->y;
}
static void b(void) {
    static const u8 const backTypeTable[] = {0x00, 0x01, 0x01, 0x00, 0x00}; // 撃ち返しデータ
    static const u8 const backAngleTable[] = {0x00, 0x0c, 0xf4, 0x18, 0xe8};
    static const u8 const backSpeedTable[] = {
        //0x03, 0x00, 0x02, 0x01, 0x02, 0x01, 0x03, 0x00, 0x03, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0x02, 0x01, 0x02, 0x00, 0x02, 0x00, 0x02, 0x01, 0x02, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0x02, 0x00, 0x01, 0x01, 0x01, 0x01, 0x02, 0x00, 0x02, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0x01, 0x01, 0x01, 0x00, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0x01, 0x01, 0x01, 0x00, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    };
    for(u8 b = 5,c = 0;b;c++,--b) {// 弾のエントリ
        // メインの処理
        u8* hl = (void*)(backTypeTable+c);
        bulletEntry.sprite_src_l = *hl;
        hl = (void*)(backAngleTable+c);
        u8 a = gameBackAngle + *hl;
        gameBackCos = SystemGetCos(a);
        gameBackSin = SystemGetSin(a);
        bulletEntry.spx = 0;
        bulletEntry.spy = 0;
        hl = (void*)(backSpeedTable+c*2+(appTimer[0] << 4));
        for (u8 d = *hl++;d;d--) {
            bulletEntry.spx += gameBackCos;
            bulletEntry.spy += gameBackSin;
        }
        u8 e = *hl;
        if(e) {
            gameBackCos = gameBackCos>>1;
            gameBackSin = gameBackSin>>1;
            for(;e;e--) {
                bulletEntry.spx += gameBackCos;
                bulletEntry.spy += gameBackSin;
            }
        }
        BulletEntry();
    }
}
static void GameShootBack(ENEMY* ix) { // 敵が弾を打ち返す
    // 敵が自機に近い場合は撃ち返さない
    if (ship.y-0x20 < ix->yi) {
        u8 a = ship.x-0x18;
        if (a < ix->xi && ix->xi <= a+0x30) return;
    }
    getvec(ix);
    b();
}
