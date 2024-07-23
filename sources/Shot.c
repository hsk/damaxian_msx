// Shot.c : ゲーム画面／ショット
#include "bios.h"
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Ship.h"
#include "Shot.h"

// 定数の定義
static const u8 const shotSpriteTable[] = { // ショットデータ
    0xfc, 0xf8, 0x08, 0x0f
};
// 変数の定義
SHOT shot[SHOT_SIZE]; // パラメータ
void ShotInitialize(void) {// ショットを初期化する
    SHOT* ix = shot;
    for (u8 c=0;c<SHOT_SIZE;ix++,++c) {// ショットの走査
        ix->index = c; // インデックスの設定
        ix->state = SHOT_STATE_NULL;// 状態の設定
    }
}
static void ShotNull(SHOT* ix);
static void ShotMove(SHOT* ix);
void ShotUpdate(void) {// ショットを更新する
    SHOT* ix = shot;// ショットの走査
    for(u8 b=SHOT_SIZE;b;ix++,--b) {
        if (ix->state == SHOT_STATE_NULL) ShotNull(ix);// なし
        else                              ShotMove(ix); // 移動
    }
}
static void ShotNull(SHOT* ix) {// ショットはなし
    // 待機の処理
    // 描画の開始
    u8* hl = &sprite[GAME_SPRITE_SHOT+(ix->index<<2)];
    *hl++ = 0xc0;
    *hl++ = 0xc0;
    *hl++ = 0xc0;
    *hl = 0xc0;
}
static void ShotMove(SHOT* ix) { // ショットが移動する
    if (ix->phase == 0) {// 初期化
        ix->x = ship.x;// 位置の初期化
        ix->y = ship.y;
        ix->phase++;// 状態の更新
    }
    u8 a = ix->y = ix->y-4;// 移動
    if (0xf0 <= a && a < 0xf9) ix->state = SHOT_STATE_NULL; // 状態の更新
    // 描画の開始
    SystemSetSprite(shotSpriteTable,
        sprite+GAME_SPRITE_SHOT + (ix->index<<2),ix->y,ix->x);
}
void ShotEntry(void) { // ショットをエントリする
    // ショットの走査
    SHOT* ix = shot;
    for (u8 b = SHOT_SIZE;b; ix++,--b) {
        if (ix->state != SHOT_STATE_NULL) continue;// 状態の更新
        ix->state = SHOT_STATE_MOVE;
        ix->phase = 0;
        break;
    }
}
