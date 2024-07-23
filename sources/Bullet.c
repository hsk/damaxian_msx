// Bullet.s : ゲーム画面／弾
#include "bios.h"
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Bullet.h"
// 定数の定義
static const u8 const bulletSpriteTable[] = {// 弾データ
    0xf8, 0xf8, 0x40, 0x06,
    0xf8, 0xf8, 0x40, 0x04,
};
// 変数の定義
BULLET bullets[BULLET_SIZE]; // パラメータ
BULLET bulletEntry;             // エントリ
static u8 spriteOffset[2];                              // スプライトオフセット

void BulletInitialize(void) { // 弾を初期化する
    BULLET* ix = bullets;// 弾の走査
    for (u8 c = 0;c<BULLET_SIZE*4;c+=4,ix++) {
        ix->sprite_offset = c; // スプライトの設定
        ix->state = BULLET_STATE_NULL;// 状態の設定
    }
    spriteOffset[0] = 0;// スプライトオフセットの初期化
    spriteOffset[1] = GAME_SPRITE_BULLET;
}
static void BulletNull(BULLET* ix);
static void BulletMove(BULLET* ix);
void BulletUpdate(void) { // 弾を更新する
    BULLET* ix = bullets;// 弾の走査
    for (u8 b = BULLET_SIZE;b;--b,ix++) {
        if (ix->state == BULLET_STATE_NULL)BulletNull(ix);// なし
        else BulletMove(ix); // 移動
    } // 更新の終了
    // スプライトオフセットの更新
    spriteOffset[0] += 4;
    if (spriteOffset[0] >= (BULLET_SIZE * 0x04)) spriteOffset[0] = 0;
    spriteOffset[1] = GAME_SPRITE_BULLET_OFFSET - spriteOffset[1];
}
static void BulletNull(BULLET* ix) { // 弾はなし
    // 待機の処理
    // 描画の開始
    u8* hl = spriteOffset;
    u8 a = (*hl++)+ix->sprite_offset;
    if (a>=BULLET_SIZE * 0x04) a-=BULLET_SIZE * 0x04;
    a += *hl;
    hl = sprite+a;
    *hl++ = 0xc0;
    *hl++ = 0xc0;
    *hl++ = 0xc0;
    *hl = 0xc0;
}
static void BulletMove(BULLET* ix) { // 弾が移動する
    ix->x += ix->spx;// 移動
    ix->y += ix->spy;
    if (ix->xi > 0xc4 || ix->yi > 0xc4) ix->state = BULLET_STATE_NULL;// 状態の更新
    // 描画の開始
    u8* hl = spriteOffset;
    u8 a = (*hl++) + ix->sprite_offset;
    if (a >= (BULLET_SIZE * 0x04)) a -= (BULLET_SIZE * 0x04);
    a += *hl;
    hl = sprite + a;
    a = ix->xi;
    if (0xe0 <= a && a < 0xf8) {
        *hl++ = 0xc0;
        *hl++ = 0xc0;
        *hl++ = 0xc0;
        *hl = 0xc0;
        return;
    }
    SystemSetSprite((u8*)ix->sprite_src, hl, ix->yi, ix->xi);
}
void BulletEntry(void) {// 弾をエントリする
    // 弾の走査
    BULLET* ix = bullets;    
    for (u8 b = BULLET_SIZE;b;ix++, --b) {
        if (ix->state != BULLET_STATE_NULL) continue;
        ix->x = bulletEntry.x;// 位置の設定
        ix->y = bulletEntry.y;
        ix->spx = bulletEntry.spx;// 速度の設定
        ix->spy = bulletEntry.spy;
        ix->sprite_src = // スプライトの設定
            (i16)&bulletSpriteTable[bulletEntry.sprite_src_l << 2];
        ix->state = BULLET_STATE_MOVE;// 状態の更新
        break;
    }
}
