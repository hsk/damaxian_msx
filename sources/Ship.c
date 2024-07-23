// Ship.c : ゲーム画面／自機
#include "bios.h"
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Ship.h"
// 定数の定義
static const u8 const shipSpriteTable[] = { // 自機データ
    0xf8, 0xf8, 0x00, 0x0f,
    0xf8, 0xf8, 0x04, 0x0f,
};
// 変数の定義
SHIP ship; // パラメータ
void ShipInitialize(void) { // 自機を初期化する
    ship.state = SHIP_STATE_PLAY; // 状態の設定
    ship.phase = 0;
}
static void ShipNull(void);
static void ShipPlay(void);
void ShipUpdate(void) { // 自機を更新する
    u8 a = ship.state;
    if      (a == SHIP_STATE_NULL) ShipNull(); // 自機はなし
    else if (a == SHIP_STATE_PLAY) ShipPlay(); // 操作
}
static void ShipNull(void) { // 自機はなし
    sprite[GAME_SPRITE_SHIP+0x00] = 0xc0;// 描画の開始
    sprite[GAME_SPRITE_SHIP+0x01] = 0xc0;
    sprite[GAME_SPRITE_SHIP+0x02] = 0xc0;
    sprite[GAME_SPRITE_SHIP+0x03] = 0xc0;
}
static void ShipPlay(void) { // 自機を操作する
    if (ship.phase==0) {// 初期化
        ship.x = 0x60;// 位置の設定
        ship.y = 0xb1;
        ship.phase++;// 状態の更新
    }// 待機処理
    // 自機を定位置に移動
    if (gameFlag&(1<<GAME_FLAG_PLAYABLE)) {// 操作可能かどうか
        // Ｘ方向の移動
        if (input[INPUT_KEY_LEFT]) {// ←が押された
            if (ship.x>=0x8+2) ship.x-=2;
        } else if(input[INPUT_KEY_RIGHT]) {// →が押された
            if (ship.x<0xb8) ship.x+=2;
        }
    }
    // 描画の開始
    SystemSetSprite(
        &shipSpriteTable[0],
        &sprite[GAME_SPRITE_SHIP], ship.y,ship.x);
}
