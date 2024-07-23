// Enemy.c : ゲーム画面／敵
#include "bios.h"
#include "System.h"
#include "Math.h"
#include "App.h"
#include "Game.h"
#include "Ship.h"
#include "Enemy.h"
// 定数の定義
// 敵データ
extern const u8 const enemyPointTable[];
static const u8 const enemyStayTable[] = {
    0x7f, 0x00,
};
static const u8 const enemySpriteTable[] = {
    0xf8, 0xf8, 0x80, 0x0a,      // 0:→
    0xf8, 0xf8, 0x84, 0x09,
    0xf8, 0xf8, 0x88, 0x0a,      // 0:↓
    0xf8, 0xf8, 0x8c, 0x09,
    0xf8, 0xf8, 0x90, 0x0a,      // 0:←
    0xf8, 0xf8, 0x94, 0x09,
    0xf8, 0xf8, 0x98, 0x0a,      // 0:↑
    0xf8, 0xf8, 0x9c, 0x09,
    0xf8, 0xf8, 0xa0, 0x06,      // 1:→
    0xf8, 0xf8, 0xa4, 0x06,
    0xf8, 0xf8, 0xa8, 0x06,      // 1:↓
    0xf8, 0xf8, 0xac, 0x06,
    0xf8, 0xf8, 0xb0, 0x06,      // 1:←
    0xf8, 0xf8, 0xb4, 0x06,
    0xf8, 0xf8, 0xb8, 0x06,      // 1:↑
    0xf8, 0xf8, 0xbc, 0x06,
    0xf8, 0xf8, 0xc0, 0x0d,      // 2:→
    0xf8, 0xf8, 0xc4, 0x0d,
    0xf8, 0xf8, 0xc8, 0x0d,      // 2:↓
    0xf8, 0xf8, 0xcc, 0x0d,
    0xf8, 0xf8, 0xd0, 0x0d,      // 2:←
    0xf8, 0xf8, 0xd4, 0x0d,
    0xf8, 0xf8, 0xd8, 0x0d,      // 2:↑
    0xf8, 0xf8, 0xdc, 0x0d,
    0xf8, 0xf8, 0xe0, 0x04,      // 3:→
    0xf8, 0xf8, 0xe4, 0x04,
    0xf8, 0xf8, 0xe8, 0x04,      // 3:↓
    0xf8, 0xf8, 0xec, 0x04,
    0xf8, 0xf8, 0xf0, 0x04,      // 3:←
    0xf8, 0xf8, 0xf4, 0x04,
    0xf8, 0xf8, 0xf8, 0x04,      // 3:↑
    0xf8, 0xf8, 0xfc, 0x04,
};
static const u8* const enemySpritePatternTable[] = {
    &enemySpriteTable[0x0000],
    &enemySpriteTable[0x0000],
    &enemySpriteTable[0x0020],
    &enemySpriteTable[0x0060],
    &enemySpriteTable[0x0060],
    &enemySpriteTable[0x0020],
    &enemySpriteTable[0x0040],
    &enemySpriteTable[0x0060],
    &enemySpriteTable[0x0040],
};
static const u8 const bombSpriteTable[] = {// 爆発データ
    0xf8, 0xf8, 0x10, 0x06,
    0xf8, 0xf8, 0x14, 0x0a,
    0xf8, 0xf8, 0x18, 0x06,
    0xf8, 0xf8, 0x1c, 0x0f,
};
// 変数の定義
ENEMY enemies[ENEMIES_SIZE];   // パラメータ
static u8 spriteOffset[2];                                      // スプライトオフセット
void EnemyInitialize(void) { // 敵を初期化する
    // 敵の走査
    ENEMY* ix = enemies;
    for (u8 c=0;c<ENEMIES_SIZE;ix++,c++) {
        ix->ss = *(i16*)&enemyPointTable[c<<1];// 位置の設定
        // スプライトの設定
        ix->sprite_src = (i16)enemySpritePatternTable[c];
        ix->sprite_offset = c << 2;
        ix->state = ENEMY_STATE_IN;// 状態の設定
        ix->phase = 0;
    }
    // スプライトオフセットの初期化
    spriteOffset[0] = 0;
    spriteOffset[1] = GAME_SPRITE_ENEMY;
}
static void EnemyNull(ENEMY* ix);
static void EnemyIn(ENEMY* ix);
static void EnemyStay(ENEMY* ix);
static void EnemyTurn(ENEMY* ix);
static void EnemyApproach(ENEMY* ix);
static void EnemyBomb(ENEMY* ix);
static void EnemyDraw(ENEMY* ix);
void EnemyUpdate(void) { // 敵を更新する
    ENEMY* ix = enemies;// 敵の走査
    for (u8 b = ENEMIES_SIZE;b;ix++,--b) {
        u8 a = ix->state;
        if      (a == ENEMY_STATE_NULL)     EnemyNull(ix);     // なし
        else if (a == ENEMY_STATE_IN)       EnemyIn(ix);       // イン
        else if (a == ENEMY_STATE_STAY)     EnemyStay(ix);     // 待機
        else if (a == ENEMY_STATE_TURN)     EnemyTurn(ix);     // ターン
        else if (a == ENEMY_STATE_APPROACH) EnemyApproach(ix); // アプローチ
        else                                EnemyBomb(ix);     // 爆発
    }
    // スプライトオフセットの更新
    u8 a = spriteOffset[0]+4;
    if (a >= ENEMIES_SIZE * 0x04) a = 0;
    spriteOffset[0] = a;
    spriteOffset[1] = GAME_SPRITE_ENEMY_OFFSET - spriteOffset[1];
}
static void EnemyNull(ENEMY* ix) { // 敵はなし
    if (ix->phase == 0) {// 初期化
        ix->nodamage = 0x80;// ノーダメージの設定
        ix->phase++;// 状態の更新
    }// 待機の処理
    // 描画の開始
    u8 a = spriteOffset[0] + ix->sprite_offset;
    if (a >= (ENEMIES_SIZE * 0x04)) a -= (ENEMIES_SIZE * 0x04);
    u8* hl = sprite + a + spriteOffset[1];
    *hl++ = 0xc0;
    *hl++ = 0xc0;
    *hl++ = 0xc0;
    *hl = 0xc0;
}
static void EnemyIn(ENEMY* ix) { // 敵がインする
    if (ix->phase==0) {// 初期化
        // 位置の設定
        ix->xi = ix->xs;
        ix->yi = 0xf4;
        ix->xd = 0;
        ix->yd = 0;
        ix->angle = 0x40;// 方向の設定
        ix->nodamage = 0;// ノーダメージの設定
        ix->count0 = 0x40 - ix->ys;// カウントの設定
        ix->animation = 0;// アニメーションの設定
        ix->phase++;// 状態の更新
    } // インの処理
    // カウントの更新
    if (ix->count0 == 0) ix->count0 -= 2; // 位置の更新
    else if (ix->yi != ix->ys) ix->yi += 2; // 方向の更新
    else if (ix->angle != 0xc0) ix->angle -= 8;
    else {
        ix->state = ENEMY_STATE_STAY;// 状態の設定
        ix->phase = 0;
    } // 処理の完了
    EnemyDraw(ix); // 敵の描画
}
static void EnemyStay(ENEMY* ix) { // 敵が待機する
    if (ix->phase==0) {
        // カウントの設定
        u8* hl = (void*)&enemyStayTable[0];
        u8 a = SystemGetRandom() & (*hl++);
        a += *hl;
        ix->count0 = a;
        ix->count1 = 0x40;
        ix->phase++;// 状態の更新
    } // 待機の処理
    if (gameFlag & (1<<GAME_FLAG_PLAYABLE)) {// 操作可能かどうか
        if(ix->count0)      ix->count0--;// カウント0更新
        else if(ix->count1) ix->count1--;// カウント1更新
        else {
            ix->state = ENEMY_STATE_TURN;// 状態の設定
            ix->phase = 0;
        }
    }
    EnemyDraw(ix);// 敵の描画
}
static void EnemyTurn(ENEMY* ix) { // 敵がターンする
    if (ix->phase == 0) {
        ix->turn = 0x02;// 回転の設定
        if ((SystemGetRandom()&0b00010000) == 0) ix->turn = 0xfe;
        ix->phase++;// 状態の更新
    }
    ix->angle += ix->turn;// 方向の更新
    if (ix->angle==0x40) ix->state = ENEMY_STATE_APPROACH;// 状態の更新
    // 位置の更新
    i16 hl = SystemGetCos(ix->angle);
    ix->x += hl + (hl>>1);// X 方向は x1.5 の移動
    ix->y += (SystemGetSin(ix->angle)<<1);// Y 方向は x2.0 の移動
    EnemyDraw(ix);
}
static void EnemyApproach(ENEMY* ix) { // 敵がアプローチする
    {
        // 方向の更新
        u8 a = ix->xi;
        u8 b = ship.x;
        if (a >= 0xe0) a = 0xfe;
        else if (a == b) a = 0;
        else if (a < b) a = 0xfe;
        else a = 0x02;
        a += ix->angle;
        if (a < 0x22) a = 0x22;
        else if (a > 0x5e) a = 0x5e;
        ix->angle = a;
    }
    {
        ix->x += (SystemGetCos(ix->angle) << 1);// 位置の更新
        ix->y += (SystemGetSin(ix->angle) << 1);
    }
    if (ix->yi >= 0xc8) {
        ix->state = ENEMY_STATE_IN;
        ix->phase = 0;
    }
    EnemyDraw(ix);// 敵の描画
}
static void EnemyBomb(ENEMY* ix) { // 敵が爆発する
    if (ix->phase==0) {// 初期化処理
        ix->nodamage = 0x80;// ノーダメージの設定
        ix->animation = 0;// アニメーションの設定
        ix->phase++;// 状態の更新
    }
    // 爆発の処理
    // アニメーションの更新
    ix->animation++;
    // 状態の更新
    if (ix->animation == 0x1f) { 
        ix->state = ENEMY_STATE_IN;
        ix->phase = 0;
    } // 処理の完了
    // 描画の開始
    u8 a = spriteOffset[0] + ix->sprite_offset;
    if (a >= (ENEMIES_SIZE * 0x04)) a -= (ENEMIES_SIZE * 0x04);
    a += spriteOffset[1];
    SystemSetSprite(
        bombSpriteTable + ((ix->animation & 0b00011000)>>1),
        sprite + a, ix->yi, ix->xi);
}
static void EnemyDraw(ENEMY* ix) { // 敵を描画する
    ix->animation++;// アニメーションの更新
    // 描画の開始
    u8 a = spriteOffset[0] + ix->sprite_offset;
    if (a >= (ENEMIES_SIZE * 0x04)) a -= (ENEMIES_SIZE * 0x04);
    a += spriteOffset[1];
    u8* de = &sprite[a];
    u8* hl = (u8*)ix->sprite_src;
    hl += (((ix->angle + 0x20) & 0b11000000) >> 3);
    hl += ((ix->animation & 0b00001000) >> 1);
    u8 b = ix->xi;
    if (0xe0 <= b && b < 0xf8) {
        *de++ = 0xc0;
        *de++ = 0xc0;
        *de++ = 0xc0;
        *de = 0xc0;
        return;
    }
    SystemSetSprite(hl,de,ix->yi,b);
}
