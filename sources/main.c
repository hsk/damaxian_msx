// main.c : メインプログラム
#include "bios.h"
#include "System.h"
#include "App.h"
// 変数定義
static u8 h_timiRoutine[5];     // タイマ割り込み
static void H_timiEntry(void);
void main(void) {// メインプログラム
    // 初期化
    SystemInitialize();// システムの初期化
    AppInitialize();// アプリケーションの初期化
    DI();// 割り込みの禁止
    // タイマ割り込み処理の保存
    for(u8 i=0,*hl = (u8*)H_TIMI, *de = h_timiRoutine;i<5;i++) *de++ = *hl++;
    // タイマ割り込み処理の書き換え
    ((u8*)H_TIMI)[0] = 0xc3;
    *(i16*)&((u8*)H_TIMI)[1] = (i16)H_timiEntry;
    EI();// 割り込み禁止の解除
    while((flag & (1<<FLAG_CANCEL)) == 0);// キャンセル待ち
    // 終了
    DI();// 割り込みの禁止
    // タイマ割り込み処理の復帰
    for(u8 i=0,*hl = h_timiRoutine, *de = (u8*)H_TIMI;i<5;i++) *de++ = *hl++;
    EI();// 割り込み禁止の解除
}
static void H_timiEntry(void) {// タイマ割り込みのエントリ
    EI();// 割り込み禁止の解除
    if (!(flag & (1<<FLAG_H_TIMI))) {// すでに割り込み済みかどうか
        // 処理の開始
        flag |= (1<<FLAG_H_TIMI); // 割り込みの完了
        if (request&(1<<REQUEST_VIDEO_REGISTER)) SystemTransferVideoRegister(); // ビデオレジスタの転送
        if (request & (1 << REQUEST_VRAM)) SystemTransferVram(); // VRAM の転送
        SystemTransferSprite(); // スプライトの転送
        SystemUpdateInput();// キー入力の更新
        if (input[INPUT_BUTTON_STOP]==1) flag |= (1 << FLAG_CANCEL);// STOP キーによるキャンセル
        // 処理の完了
        AppUpdate(); // アプリケーションの更新
        flag &= ~(1<<FLAG_H_TIMI); // 割り込みの完了
    }
    __asm;
        jp      _h_timiRoutine// 保存されたタイマ割り込みルーチンの実行
    __endasm;
}
